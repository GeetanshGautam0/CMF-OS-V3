#pragma once
#include "../IO.h"
#include "../std/math.h"
#include "../std/cstr.h"
#include "../std/ktypedef.h"

#define PS2Left         0b00000001
#define PS2Right        0b00000010
#define PS2Middle       0b00000100

#define PS2XSign        0b00010000
#define PS2YSign        0b00100000
#define PS2XOverflow    0b01000000
#define PS2YOverflow    0b10000000

void InitPS2Mouse();
void HandlePS2Mouse(uint8_t data);
void ProcessMousePacket();

extern uint8_t MousePointer[];

extern CMath::Point MousePosition;
