/*
 *  kAway2 stdafx
 *
 *  include file for standard system include files,
 *  or project specific include files that are used frequently, but
 *  are changed infrequently
 *
 *  Please READ /License.txt FIRST!
 *
 *  Copyright (C)2005 Sijawusz Pur Rahnama
 *
 *  $Id: stdafx.h 26 2006-01-04 04:46:47Z sija $
 */

#pragma once

#define WIN32_LEAN_AND_MEAN // Exclude rarely-used stuff from Windows headers
#define _WIN32_WINNT 0x0501

// Windows Header Files:
#include <windows.h>

#include <process.h>
#include <commctrl.h>
#include <map>
#include <list>
#include <string>
#include <deque>
#include <stdstring.h>

#pragma comment(lib, "comctl32.lib")
#pragma comment(lib, "stamina.lib")

#define yes (true)
#define no (false)
#define ever (;;)

/*
#include <stamina/threadrun.h>
#include <stamina/thread.h>
#include <stamina/threadinvoke.h>
#include <stamina/string.h>
#include <stamina/timer.h>
*/

#include <stamina/helpers.h>
#include <stamina/regex.h>
#include <stamina/time64.h>

#include <konnekt/plug_export.h>
#include <konnekt/ui.h>
#include <konnekt/plug_func.h>
#include <konnekt/knotify.h>
#include <konnekt/tabletka.h>
#include <konnekt/ksound.h>
#include <konnekt/ui_message_controls.h>
#include <konnekt/lib.h>

#include "../res/resource.h"
#include "plugsNET.h"