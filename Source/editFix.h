/*
 *  Fix for text controls
 *
 *  Please READ /License.txt FIRST!
 *
 *  Copyright (C)2005 Sijawusz Pur Rahnama
 *  Copyright (C)2004 Kuba Niegowski
 *  Copyright (C)2003-2004 Olórin
 *
 *  $Id$
 */

#pragma once

/* usage:
   include it & put it after CreateWindow("edit", ...)
   editProcOld = (WNDPROC) SetWindowLongPtr(my_edit, GWLP_WNDPROC, (LONG_PTR) editProc);
 */

WNDPROC oldEditFix = NULL;

LRESULT CALLBACK EditFix(HWND hWnd,UINT iMsg,WPARAM wParam, LPARAM lParam){
  switch (iMsg) {
    case WM_CHAR: {
      HWND parent = GetParent(hWnd);

      short tmp = GetKeyState(VK_CONTROL);     
      short flag = HIBYTE(tmp) ? MOD_CONTROL : 0;

      tmp = GetKeyState(VK_MENU);
      flag |= HIBYTE(tmp) ? MOD_ALT : 0;

      if (!(flag & MOD_ALT) && (flag & MOD_CONTROL) && HIBYTE(GetKeyState(VK_BACK))) {
        int length = SendMessage(hWnd, WM_GETTEXTLENGTH, 0, 0) + 1;
        char * body_tmp = new char[length];

        GetWindowText(hWnd, body_tmp, length);
        std::string temp = body_tmp, rest;

        int selectionBeg, selectionEnd;
        SendMessage(hWnd, EM_GETSEL, (WPARAM) &selectionBeg, (LPARAM) &selectionEnd);

        if (selectionBeg != selectionEnd) goto ret;
          
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
        delete [] body_tmp;
        return(0);
      }
      break;
    }
  }

ret:
  return(CallWindowProc(oldEditFix, hWnd, iMsg, wParam, lParam));
}