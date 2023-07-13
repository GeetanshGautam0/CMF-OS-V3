#pragma once
#include "../std/ctypedef.h"
#include "../std/math.h"
#include "../std/SimpleFont.h"
#include "../std/cstr.h"
#include "../std/ktypedef.h"
#include "KBScanCodes.h"


enum KB_DEBUG_MODE {ALL, UNKNOWN, NONE};
extern KB_DEBUG_MODE KeyboardDebugMode;

void HandleKeyboard(uint8_t scancode);
