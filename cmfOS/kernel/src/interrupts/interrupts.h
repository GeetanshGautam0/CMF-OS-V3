#pragma once
#include "../renderer/panic.h"
#include "../renderer/KernelRenderer.h"
#include "../IO.h"
#include "../std/ktypedef.h"
#include "../input/KB.h"

#define PIC1_COMMAND    0x20
#define PIC1_DATA       0x21
#define PIC2_COMMAND    0xA0 
#define PIC2_DATA       0xA1
#define PIC_EOI         0x20

#define ICW1_INIT       0x10
#define ICW1_ICW4       0x01
#define ICW4_8086       0x01

struct interrupt_frame;

__attribute__((interrupt)) void PageFault_Handler(struct interrupt_frame* frame);
__attribute__((interrupt)) void DoubleFault_Handler(struct interrupt_frame* frame);
__attribute__((interrupt)) void GPFault_Handler(struct interrupt_frame* frame);
__attribute__((interrupt)) void DivisionByZero_Handler(struct interrupt_frame* frame);
__attribute__((interrupt)) void BoundRangeExceeded_Handler(struct interrupt_frame* frame);
__attribute__((interrupt)) void InvalidOpcode_Handler(struct interrupt_frame* frame);
__attribute__((interrupt)) void DeviceNotAvailable_Handler(struct interrupt_frame* frame);
__attribute__((interrupt)) void SegmentNotPresent_Handler(struct interrupt_frame* frame);
__attribute__((interrupt)) void StackSegmentFault_Handler(struct interrupt_frame* frame);
__attribute__((interrupt)) void SystemError_Handler(struct interrupt_frame* frame);
__attribute__((interrupt)) void KeyboardInt_Handler(struct interrupt_frame* frame);

void RemapPIC();
void PIC_EndMaster();
void PIC_EndSlave();
