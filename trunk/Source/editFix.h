/*
 *  Fix for text controls
 *
 *  Please READ /License.txt FIRST!
 *
 *  Copyright (C)2005-2006 Sijawusz Pur Rahnama
 *  Copyright (C)2004-2005 Olórin
 *
 *  $Id$
 */

#pragma once

/* usage:
   include it & put it after CreateWindow("edit", ...)
   SetProp(hwnd, "oldWndProc", (HANDLE) SetWindowLongPtr(hwnd, GWLP_WNDPROC, (LONG_PTR) editFix));
 */

LRESULT CALLBACK editFix(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam) {
  switch (iMsg) {
    case WM_CHAR: {
      HWND parent = GetParent(hWnd);
 
      short flag = HIBYTE(GetKeyState(VK_CONTROL)) ? MOD_CONTROL : 0;
      flag |= HIBYTE(GetKeyState(VK_MENU)) ? MOD_ALT : 0;
      flag |= HIBYTE(GetKeyState(VK_SHIFT)) ? MOD_SHIFT : 0;

      if ((flag & MOD_CONTROL) && HIBYTE(GetKeyState(VK_RETURN))) {
        SendMessage(parent, WM_COMMAND, (WPARAM) 1, 0);
      } else if (!(flag & MOD_ALT) && (flag & MOD_CONTROL) && HIBYTE(GetKeyState(VkKeyScan('a')))) {
        SendMessage(hWnd, EM_SETSEL, (WPARAM)0, (LPARAM)-1);
        return(0);
      } else if (!(flag & MOD_ALT) && (flag & MOD_CONTROL) && HIBYTE(GetKeyState(VK_BACK))) {
        int length = SendMessage(hWnd, WM_GETTEXTLENGTH, 0, 0) + 1;
        char * body_tmp = new char[length];

        GetWindowText(hWnd, body_tmp, length);
        std::string temp = body_tmp, rest;

        int selectionBeg, selectionEnd;
        SendMessage(hWnd, EM_GETSEL, (WPARAM) &selectionBeg, (LPARAM) &selectionEnd);

        if (selectionBeg != selectionEnd) {
          SendMessage(hWnd, EM_REPLACESEL, true, (LPARAM) "");
          return(0);
        }

        rest = temp.substr(selectionBeg);
        temp = temp.substr(0, selectionBeg);

        int pos = temp.find_last_of(" ", temp.length() - 2);
        int pos2 = temp.find_last_of("\r\n", temp.length() - 1); 
        int pos3 = temp.find_last_of(".", temp.length() - 2);
        // int pos4 = temp.find_last_of(",", temp.length() - 2);

        if (pos < pos2) pos = pos2;
        if (pos < pos3) pos = pos3;
        // if(pos < pos4) pos = pos4;

        if (pos == -1) pos = 0;
        selectionBeg = selectionBeg - (temp.length() - pos) + 1;

        temp = temp.substr(0, pos + ((pos == pos2) ? -1 : ((pos == 0) ? 0 : 1)));
        temp = temp + rest;
        SetWindowText(hWnd, (char*) temp.c_str());

        SendMessage(hWnd, EM_SETSEL, (WPARAM) selectionBeg, (LPARAM) selectionBeg);
        SendMessage(hWnd, EM_SCROLLCARET, (WPARAM) 0, (LPARAM) 0);
        delete [] body_tmp;
        return(0);
      }
      break;
    }
  }
  return(CallWindowProc((WNDPROC) GetProp(hWnd, "oldWndProc"), hWnd, iMsg, wParam, lParam));
}