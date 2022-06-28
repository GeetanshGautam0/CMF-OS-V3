#include "interrupts.h"


__attribute__((interrupt)) void PageFault_Handler(struct interrupt_frame* frame){
    Panic("Page fault detected", true, true);
}

__attribute__((interrupt)) void DoubleFault_Handler(struct interrupt_frame* frame) {
    Panic("Double fault detected", true, true);
}

__attribute__((interrupt)) void GPFault_Handler(struct interrupt_frame* frame) {
    Panic("General Protection fault detected", true, true);
}

__attribute__((interrupt)) void DivisionByZero_Handler(struct interrupt_frame* frame) {
    Panic("Division By Zero error detected", true, true);
}

__attribute__((interrupt)) void BoundRangeExceeded_Handler(struct interrupt_frame* frame) {
    Panic("Array-like object: Bound range exceeded", true, true);
}

__attribute__((interrupt)) void InvalidOpcode_Handler(struct interrupt_frame* frame) {
    Panic("Invalid Opcode", true, true);
}

__attribute__((interrupt)) void DeviceNotAvailable_Handler(struct interrupt_frame* frame) {
    Panic("Requested device is not available", true, true);
}

__attribute__((interrupt)) void SegmentNotPresent_Handler(struct interrupt_frame* frame) {
    Panic("Segment not present", true, true);
}

__attribute__((interrupt)) void StackSegmentFault_Handler(struct interrupt_frame* frame) {
    Panic("Stack segment fault detected", true, true);
}

__attribute__((interrupt)) void SystemError_Handler(struct interrupt_frame* frame) {
    Panic("<System Error>", true, true);
}

