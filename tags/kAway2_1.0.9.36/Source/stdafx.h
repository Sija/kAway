/*
 *  kAway2 stdafx
 *
 *  include file for standard system include files,
 *  or project specific include files that are used frequently, but
 *  are changed infrequently
 *
 *  Please READ /License.txt FIRST!
 *
 *  Copyright (C)2005-2006 Sijawusz Pur Rahnama
 *
 *  $Id$
 */

#pragma once

#define WIN32_LEAN_AND_MEAN // Exclude rarely-used stuff from Windows headers
#define _WIN32_WINNT 0x0501

#include <windows.h>
#include <process.h>
#include <commctrl.h>
#include <map>
#include <list>
#include <deque>
#include <stdstring.h>
#include <string>
// #include <sstream>

#pragma comment(lib, "comctl32.lib")

#ifdef _DEBUG
  #pragma comment(lib, "stamina_d.lib")
#else
  #pragma comment(lib, "stamina.lib")
#endif

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

#include "../Project/resource.h"
#include "plugsNET.h"