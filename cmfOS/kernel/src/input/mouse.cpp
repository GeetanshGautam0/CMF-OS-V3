#include "mouse.h"

#define RS_BUFFER   10
#define YRS_BUFFER  RS_BUFFER
#define AYRS_BUFFER RS_BUFFER * 4
#define CPR         2

uint8_t MousePointer[] = {
    (uint8_t)0b11111111, (uint8_t)0b111000000,
    (uint8_t)0b11111111, (uint8_t)0b10000000,
    (uint8_t)0b11111110, (uint8_t)0b00000000,
    (uint8_t)0b11111100, (uint8_t)0b00000000,
    (uint8_t)0b11111000, (uint8_t)0b00000000,
    (uint8_t)0b11110000, (uint8_t)0b00000000,
    (uint8_t)0b11100000, (uint8_t)0b00000000,
    (uint8_t)0b11000000, (uint8_t)0b00000000,
    (uint8_t)0b11000000, (uint8_t)0b00000000,
    (uint8_t)0b10000000, (uint8_t)0b00000000,
    (uint8_t)0b10000000, (uint8_t)0b00000000,
    (uint8_t)0b00000000, (uint8_t)0b00000000,
    (uint8_t)0b00000000, (uint8_t)0b00000000,
    (uint8_t)0b00000000, (uint8_t)0b00000000,
    (uint8_t)0b00000000, (uint8_t)0b00000000,
    (uint8_t)0b00000000, (uint8_t)0b00000000
};

void MouseWait(){
    uint64_t timeout = 100000;
    while (timeout--){
        if ((inb(0x64) & 0b10) == 0){
            return;
        }
    }
}

void MouseWaitInput(){
    uint64_t timeout = 100000;
    while (timeout--){
        if (inb(0x64) & 0b1){
            return;
        }
    }
}

void MouseWrite(uint8_t value){
    MouseWait();
    outb(0x64, 0xD4);
    MouseWait();
    outb(0x60, value);
}

uint8_t MouseRead(){
    MouseWaitInput();
    return inb(0x60);
}

uint8_t MouseCycle = 0;
uint8_t MousePacket[4];
bool MousePacketReady = false;
bool setup = false;
CMath::Point MousePosition;
CMath::Point MousePositionOld;

void HandlePS2Mouse(uint8_t data) {
    ProcessMousePacket();

    static bool skip = true;
    if (skip) {skip = false; return;}

    switch (MouseCycle) {
        case 0:
            if (MousePacketReady) break;
            if (data & 0b0000100 == 0) break;   // out of sync
            MousePacket[0] = data;
            MouseCycle ++;
            break;
        case 1:
            if (MousePacketReady) break;
            MousePacket[1] = data;
            MouseCycle ++;
            break;
        case 2:
            if (MousePacketReady) break;
            MousePacket[2] = data;
            MousePacketReady = true;
            MouseCycle = 0;
            break;
    }
}

void ProcessMousePacket() {
    if (!MousePacketReady) return;

    bool xNegative, yNegative, xOverflow, yOverflow;
    // Region regionSize = KernelRenderer.GetBounds();

    if (!setup) {
        if (MousePacket[0] & PS2XSign){
            xNegative = true;
        }else xNegative = false;

        if (MousePacket[0] & PS2YSign){
            yNegative = true;
        }else yNegative = false;

        if (MousePacket[0] & PS2XOverflow){
            xOverflow = true;
        }else xOverflow = false;

        if (MousePacket[0] & PS2YOverflow){
            yOverflow = true;
        }else yOverflow = false;

        if (!xNegative){
            MousePosition.x += MousePacket[1];
            if (xOverflow){
                MousePosition.x += 255;
            }
        } else
        {
            MousePacket[1] = 256 - MousePacket[1];
            MousePosition.x -= MousePacket[1];
            if (xOverflow){
                MousePosition.x -= 255;
            }
        }

        if (!yNegative){
            MousePosition.y -= MousePacket[2];
            if (yOverflow){
                MousePosition.y -= 255;
            }
        } else
        {
            MousePacket[2] = 256 - MousePacket[2];
            MousePosition.y += MousePacket[2];
            if (yOverflow){
                MousePosition.y += 255;
            }
        }

        // if (MousePosition.x < regionSize.x_start + RS_BUFFER) MousePosition.x = regionSize.x_start + RS_BUFFER;
        // if (MousePosition.x > regionSize.x_end - RS_BUFFER) MousePosition.x = regionSize.x_end - RS_BUFFER;
// 
        // if (MousePosition.y < regionSize.y_start + YRS_BUFFER) MousePosition.y = regionSize.y_start + YRS_BUFFER;
        // if (MousePosition.y > regionSize.y_end - AYRS_BUFFER) MousePosition.y = regionSize.y_end - AYRS_BUFFER;
    }

    else {
        // MousePosition = {regionSize.x_start, regionSize.y_start};
        setup = true;
    }

    // KernelRenderer.clearMouseCursor(MousePointer, MousePositionOld);
    // KernelRenderer.drawMouseCursor(MousePointer, MousePosition);

    if (MousePacket[0] & PS2Left){
        // KernelRenderer.drawFShape({RECT, {(unsigned int)(MousePosition.x - CPR), (unsigned int)(MousePosition.x + CPR), (unsigned int)(MousePosition.y - CPR), (unsigned int)(MousePosition.y + CPR)}, 0}, Color::BLACK, Color::BLACK);
    }
    if (MousePacket[0] & PS2Middle){
        // KernelRenderer.drawFShape({RECT, {(unsigned int)(MousePosition.x - CPR), (unsigned int)(MousePosition.x + CPR), (unsigned int)(MousePosition.y - CPR), (unsigned int)(MousePosition.y + CPR)}, 0}, Color::GREEN, Color::GREEN);
    }
    if (MousePacket[0] & PS2Right){
        // KernelRenderer.drawFShape({RECT, {(unsigned int)(MousePosition.x - CPR), (unsigned int)(MousePosition.x + CPR), (unsigned int)(MousePosition.y - CPR), (unsigned int)(MousePosition.y + CPR)}, 0}, Color::RED, Color::RED);
    }

    MousePacketReady = false;
    MousePositionOld = MousePosition;
}

void InitPS2Mouse() {

    outb(0x64, 0xA8); //enabling the auxiliary device - mouse

    MouseWait();
    outb(0x64, 0x20); //tells the keyboard controller that we want to send a command to the mouse
    MouseWaitInput();
    uint8_t status = inb(0x60);
    status |= 0b10;
    MouseWait();
    outb(0x64, 0x60);
    MouseWait();
    outb(0x60, status); // setting the correct bit is the "compaq" status byte

    MouseWrite(0xF6);
    MouseRead();

    MouseWrite(0xF4);
    MouseRead();

}
