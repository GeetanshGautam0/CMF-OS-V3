#pragma once
#include "../../std/ctypedef.h"

namespace PIT {
    enum Blinker {KERNEL, MENU, FOOTER};

    extern bool KBlinkEnb;

    extern double TimeSinceBoot;
    const uint64_t BaseFrequency = 1193182;

    void Sleepd(double seconds);
    void Sleep(uint64_t milliseconds);

    void SetDivisor(uint16_t divisor);
    uint64_t GetFrequency();
    void SetFrequency(uint64_t frequency);
    void Tick();

    void EnableKBlink();
    uint16_t GetDivisor();
}