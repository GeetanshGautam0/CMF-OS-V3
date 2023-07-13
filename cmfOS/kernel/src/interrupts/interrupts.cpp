#include "interrupts.h"


__attribute__((interrupt)) void PageFault_Handler(interrupt_frame* frame){
    Panic("Page fault detected", true, true);
}

__attribute__((interrupt)) void DoubleFault_Handler(interrupt_frame* frame) {
    Panic("Double fault detected", true, true);
}

__attribute__((interrupt)) void GPFault_Handler(interrupt_frame* frame) {
    Panic("General Protection fault detected", true, true);
}

__attribute__((interrupt)) void DivisionByZero_Handler(interrupt_frame* frame) {
    Panic("Division By Zero error detected", true, true);
}

__attribute__((interrupt)) void BoundRangeExceeded_Handler(interrupt_frame* frame) {
    Panic("Array-like object: Bound range exceeded", true, true);
}

__attribute__((interrupt)) void InvalidOpcode_Handler(interrupt_frame* frame) {
    Panic("Invalid Opcode", true, true);
}

__attribute__((interrupt)) void DeviceNotAvailable_Handler(interrupt_frame* frame) {
    Panic("Requested device is not available", true, true);
}

__attribute__((interrupt)) void SegmentNotPresent_Handler(interrupt_frame* frame) {
    Panic("Segment not present", true, true);
}

__attribute__((interrupt)) void StackSegmentFault_Handler(interrupt_frame* frame) {
    Panic("Stack segment fault detected", true, true);
}

__attribute__((interrupt)) void SystemError_Handler(interrupt_frame* frame) {
    Panic("<System Error>", true, true);
}

__attribute__((interrupt)) void KeyboardInt_Handler(interrupt_frame* frame) {
    uint8_t scanCode = inb(0x60);
    HandleKeyboard(scanCode);
    PIC_EndMaster();
}

__attribute__((interrupt)) void MouseInt_Handler(interrupt_frame* frame) {
    uint8_t mouseData = inb(0x60);
    HandlePS2Mouse(mouseData);
    PIC_EndSlave();
}

__attribute__((interrupt)) void PITInt_Handler(interrupt_frame* frame) {
    PIT::Tick();
    PIC_EndMaster();
}

void PIC_EndMaster() {
    outb(PIC1_COMMAND, PIC_EOI);
}

void PIC_EndSlave() {
    outb(PIC2_COMMAND, PIC_EOI);
    outb(PIC1_COMMAND, PIC_EOI);
}

void RemapPIC() {
    uint8_t a1, a2;

    a1 = inb(PIC1_DATA);
    io_wait();
    a2 = inb(PIC2_DATA);
    io_wait();

    outb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);
    io_wait();
    outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);
    io_wait();

    outb(PIC1_DATA, 0x20);
    io_wait();
    outb(PIC2_DATA, 0x28);
    io_wait();

    outb(PIC1_DATA, 4);
    io_wait();
    outb(PIC2_DATA, 2);
    io_wait();

    outb(PIC1_DATA, ICW4_8086);
    io_wait();
    outb(PIC2_DATA, ICW1_ICW4);
    io_wait();

    outb(PIC1_DATA, a1);
    io_wait();
    outb(PIC2_DATA, a2);
    io_wait();
}

