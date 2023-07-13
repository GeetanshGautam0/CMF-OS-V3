#pragma once
#include "../renderer/panic.h"
#include "../IO.h"
#include "../std/ktypedef.h"
#include "../input/KB.h"
#include "../input/mouse.h"
#include "../scheduling/pit/pit.h"

#define PIC1_COMMAND                0x20
#define PIC1_DATA                   0x21
#define PIC2_COMMAND                0xA0 
#define PIC2_DATA                   0xA1
#define PIC_EOI                     0x20

#define ICW1_INIT                   0x10
#define ICW1_ICW4                   0x01
#define ICW4_8086                   0x01

#define DivByZero_Offset            0x0
#define BoundRangeExceeded_Offset   0x5
#define InvalidOpcode_Offset        0x6
#define DeviceNotAvailable_Offset   0x7
#define DoubleFault_Offset          0x8
#define InvTTS_Offset               0xA
#define SegmentNotPresent_Offset    0xB
#define StackSegmentFault_Offset    0xC
#define GPFault_Offset              0xD
#define PageFault_Offset            0xE
#define SecurityException_Offset    0x1E
#define _Offset                     0x

struct interrupt_frame;

__attribute__((interrupt)) void PageFault_Handler           (interrupt_frame* frame);
__attribute__((interrupt)) void DoubleFault_Handler         (interrupt_frame* frame);
__attribute__((interrupt)) void GPFault_Handler             (interrupt_frame* frame);
__attribute__((interrupt)) void DivisionByZero_Handler      (interrupt_frame* frame);
__attribute__((interrupt)) void BoundRangeExceeded_Handler  (interrupt_frame* frame);
__attribute__((interrupt)) void InvalidOpcode_Handler       (interrupt_frame* frame);
__attribute__((interrupt)) void DeviceNotAvailable_Handler  (interrupt_frame* frame);
__attribute__((interrupt)) void SegmentNotPresent_Handler   (interrupt_frame* frame);
__attribute__((interrupt)) void StackSegmentFault_Handler   (interrupt_frame* frame);
__attribute__((interrupt)) void SystemError_Handler         (interrupt_frame* frame);
__attribute__((interrupt)) void KeyboardInt_Handler         (interrupt_frame* frame);
__attribute__((interrupt)) void MouseInt_Handler            (interrupt_frame* frame);
__attribute__((interrupt)) void PITInt_Handler              (interrupt_frame* frame);

void RemapPIC();
void PIC_EndMaster();
void PIC_EndSlave();
