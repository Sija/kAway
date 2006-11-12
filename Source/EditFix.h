/**
 *  Fix for text controls
 *
 *  Licensed under The GNU Lesser General Public License
 *  Redistributions of files must retain the above copyright notice.
 *
 *  @filesource
 *  @copyright    Copyright (c) 2005-2006 Sijawusz Pur Rahnama
 *  @copyright    Copyright (c) 2003-2005 Kamil 'Olórin' Figiela
 *  @link         svn://konnekt.info/kaway2/ kAway2 plugin SVN Repo
 *  @version      $Revision$
 *  @modifiedby   $LastChangedBy$
 *  @lastmodified $Date$
 *  @license      http://creativecommons.org/licenses/LGPL/2.1/
 */

#pragma once

#ifndef __EDITFIX_H__
#define __EDITFIX_H__

#include <windows.h>
#include <string>

LRESULT CALLBACK EditFix(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam);

#endif // __EDITFIX_H__