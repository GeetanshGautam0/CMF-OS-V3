#pragma once
#include "../std/ctypedef.h"
#include "../std/ktypedef.h"

void Panic(const char* panicMessage, bool lockRenderers = true, bool halt = true);
