#pragma once
#include "../renderer/panic.h"

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
