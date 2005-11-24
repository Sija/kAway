// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently

#pragma once

#define WIN32_LEAN_AND_MEAN // Exclude rarely-used stuff from Windows headers
#define _WIN32_WINNT 0x500

// Windows Header Files:
#include <windows.h>

#include <process.h>
#include <map>
#include <stdstring.h>

#include <stamina/timer.h>
#include <stamina/threadrun.h>
#include <stamina/thread.h>
#include <stamina/threadinvoke.h>
#include <stamina/helpers.h>
#include <stamina/regex.h>

#include <konnekt/plug_export.h>
#include <konnekt/ui.h>
#include <konnekt/plug_func.h>
#include <konnekt/knotify.h>
#include <konnekt/ksound.h>

#include "functions.h"