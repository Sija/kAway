/**
 *  kAway2 stdafx
 *
 *  Include file for standard system include files,
 *  or project specific include files that are used frequently,
 *  but are changed infrequently.
 *
 *  Licensed under The GNU Lesser General Public License
 *  Redistributions of files must retain the above copyright notice.
 *
 *  @filesource
 *  @copyright    Copyright (c) 2005-2006 Sijawusz Pur Rahnama
 *  @link         svn://konnekt.info/kaway2/ kAway2 plugin SVN Repo
 *  @version      $Revision$
 *  @modifiedby   $LastChangedBy$
 *  @lastmodified $Date$
 *  @license      http://creativecommons.org/licenses/LGPL/2.1/
 */

#pragma once

// Modify the following defines if you have to target a platform prior to the ones specified below.
// Refer to MSDN for the latest info on corresponding values for different platforms.
#ifndef WINVER				// Allow use of features specific to Windows XP or later.
  #define WINVER 0x0501		// Change this to the appropriate value to target other versions of Windows.
#endif

#ifndef _WIN32_WINNT		// Allow use of features specific to Windows XP or later.                   
  #define _WIN32_WINNT 0x0501	// Change this to the appropriate value to target other versions of Windows.
#endif						

#ifndef _WIN32_WINDOWS		// Allow use of features specific to Windows 98 or later.
  #define _WIN32_WINDOWS 0x0410 // Change this to the appropriate value to target Windows Me or later.
#endif

#ifndef _WIN32_IE			// Allow use of features specific to IE 6.0 or later.
  #define _WIN32_IE 0x0600	// Change this to the appropriate value to target other versions of IE.
#endif

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers

// Windows Header Files:
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

/*
#include <stamina/threadrun.h>
#include <stamina/thread.h>
#include <stamina/threadinvoke.h>
#include <stamina/timer.h>
*/

#include <stamina/helpers.h>
#include <stamina/regex.h>
#include <stamina/time64.h>
#include <stamina/object.h>
#include <stamina/string.h>

using namespace Stamina;
using namespace std;

#include <konnekt/plug_export.h>
#include <konnekt/ui.h>
#include <konnekt/plug_func.h>
#include <konnekt/knotify.h>
#include <konnekt/ksound.h>
#include <konnekt/sms.h>
#include <konnekt/tabletka.h>
#include <konnekt/ui_message_controls.h>
#include <konnekt/lib.h>

#include "../Project/resource.h"
#include "plugsNET.h"