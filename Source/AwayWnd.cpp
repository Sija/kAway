/*
 *  Status Window class
 *
 *  Please READ /License.txt FIRST! 
 *
 *  Copyright (C)2005 Sijawusz Pur Rahnama
 *  Copyright (C)2004 Kuba Niegowski
 *  Copyright (C)2003-2004 Olórin
 *
 *  $Id$
 */

#include "editFix.h"

// dlg ctrl ID
#define STATUS_OK           0x2000
#define STATUS_EDIT         0x2001
#define STATUS_INFO         0x2002
#define STATUS_CHANGE_STAT  0x2003
#define STATUS_CHANGE_INFO  0x2004
#define STATUS_EDIT_INFO    0x2005

namespace kAway2 {
  LRESULT CALLBACK AwayWndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam) {
    switch(iMsg) {
      case WM_CREATE: {
        SendMessage(hWnd, WM_SETICON, (WPARAM) ICON_BIG, (LPARAM) ICMessage(IMI_ICONGET, 0x1f7, IML_16));
        SendMessage(hWnd, WM_SETICON, (WPARAM) ICON_SMALL, (LPARAM) ICMessage(IMI_ICONGET, 0x1f7, IML_16));

        LPCREATESTRUCT pCreate = (LPCREATESTRUCT) lParam;
        int net = (int) pCreate->lpCreateParams;
        wndData *data = new wndData;
        SetWindowLong(hWnd, GWL_USERDATA, (LONG) data);
        data->net = net;

        wCtrl->addInstance(net, hWnd);

        HFONT font = CreateFont(-11, 0, 0, 0, 0, 0, 0, 0, EASTEUROPE_CHARSET, OUT_DEFAULT_PRECIS, 
          CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Tahoma");
        SendMessage(hWnd, WM_SETFONT, (WPARAM) font, true);

        HWND hwndTip = CreateWindowEx(NULL, TOOLTIPS_CLASS, NULL,
          WS_POPUP | TTS_NOPREFIX | TTS_ALWAYSTIP,
          CW_USEDEFAULT, CW_USEDEFAULT,
          CW_USEDEFAULT, CW_USEDEFAULT,
          hWnd, NULL, Ctrl->hInst(), NULL);
        SetWindowPos(hwndTip, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);

        RECT rect;
        GetClientRect(hWnd, &rect);
        TOOLINFO ti;
        ZeroMemory(&ti, sizeof(TOOLINFO));
        ti.cbSize = sizeof(TOOLINFO);
        ti.uFlags = TTF_SUBCLASS;       
        ti.hinst = Ctrl->hInst();
        ti.uId = 0;
        ti.rect.left = rect.left;
        ti.rect.top = rect.top;
        ti.rect.right = rect.right;
        ti.rect.bottom = rect.bottom;

        tStats stats;
        // stats.push_back( tStat( ST_ONLINE, "Dostêpny", data->hImlOnline ) );
        // stats.push_back( tStat( ST_CHAT, "Pogadam", data->hImlChat ) );
        stats.push_back( tStat( ST_AWAY, "Zaraz wracam", data->hImlAway ) );
        stats.push_back( tStat( ST_NA, "Nieosi¹galny", data->hImlNa ) );
        stats.push_back( tStat( ST_DND, "Nie przeszkadzaæ", data->hImlDnd ) );
        stats.push_back( tStat( ST_HIDDEN, "Ukryty", data->hImlInv ) );
        // stats.push_back( tStat( ST_OFFLINE, "Niedostêpny", data->hImlOffline ) );

        int count = stats.size();
        int width = count * 40;
        int x = 5 + int(double(300 - width) / 2);

        for (tStats::iterator it = stats.begin(); it != stats.end(); it++) {
          HWND hWndTmp = CreateWindow("button", "", WS_TABSTOP | WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON | BS_ICON,
            x, 30, 40, 20, hWnd, (HMENU) it->id, Ctrl->hInst(), NULL);
          x += 40;
          ti.hwnd = hWndTmp;
          ti.lpszText = (LPSTR) it->name.c_str();
          SendMessage(hwndTip, TTM_ADDTOOL, 0, (LPARAM) &ti);
          wCtrl->prepareButtonImage(it->img, hWnd, net, it->id);
        }

        // pole combo - wybór opisu
        HWND hWndCombo = CreateWindow("combobox", "", WS_TABSTOP | WS_CHILD | WS_VISIBLE | CBS_AUTOHSCROLL | WS_EX_CONTROLPARENT | 
          WS_EX_NOPARENTNOTIFY | CBS_DROPDOWNLIST, 13, 58, 274, 100, hWnd, (HMENU) STATUS_EDIT, Ctrl->hInst(), NULL);
        SendMessage (hWndCombo, WM_SETFONT, (WPARAM) font, true);
        
        // oldEditFix = (WNDPROC) SetWindowLongPtr(GetDlgItem(hWndCombo, 0x3e9), GWLP_WNDPROC, (LONG_PTR) EditFix);

        // przycisk OK
        HWND hWndTmp = CreateWindowEx(WS_EX_CONTROLPARENT, "button", "OK", BS_DEFPUSHBUTTON | BS_TEXT | WS_TABSTOP | WS_CHILD | WS_VISIBLE, 
          212, 190, 75, 25, hWnd, (HMENU) STATUS_OK, Ctrl->hInst(), NULL);
        SendMessage(hWndTmp, WM_SETFONT, (WPARAM) font, true);

        // napis
        hWndTmp = CreateWindow("static", wCtrl->getWndDesc().c_str(), WS_CHILD | WS_VISIBLE | SS_CENTER, 
          13, 8, 274, 15, hWnd, (HMENU) STATUS_INFO, Ctrl->hInst(), NULL);
        SendMessage(hWndTmp, WM_SETFONT, (WPARAM) font, true);

        // checkbox - zmieñ status
        hWndTmp = CreateWindow("button", "status", WS_TABSTOP | WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX,
          13, 193, 50, 20, hWnd, (HMENU) STATUS_CHANGE_STAT, Ctrl->hInst(), NULL);      
        SendMessage(hWndTmp, WM_SETFONT, (WPARAM) font, true);

        ti.hwnd = hWndTmp;
        ti.lpszText = "Zmieñ status";
        SendMessage(hwndTip, TTM_ADDTOOL, 0, (LPARAM) &ti);
        CheckDlgButton(hWnd, STATUS_CHANGE_STAT, GETINT(cfg::status::changeOnEnable) ? BST_CHECKED : 0);

        // checkbox - zmieñ opis
        hWndTmp = CreateWindow("button", "opis", WS_TABSTOP | WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX,
          73, 193, 50, 20, hWnd, (HMENU) STATUS_CHANGE_INFO, Ctrl->hInst(), NULL);      
        SendMessage(hWndTmp, WM_SETFONT, (WPARAM) font, true);

        ti.hwnd = hWndTmp;
        ti.lpszText = "Zmieñ opis";
        SendMessage(hwndTip, TTM_ADDTOOL, 0, (LPARAM) &ti);
        CheckDlgButton(hWnd, STATUS_CHANGE_INFO, GETINT(cfg::status::changeInfoOnEnable) ? BST_CHECKED : 0);

        // odczytujemy liste mru
        sMRU list;
        std::string name = wCtrl->getMruName();

        list.name = name.c_str();
        list.count = 20;
        list.flags = MRU_GET_USETEMP | MRU_SET_LOADFIRST;
        sIMessage_MRU mru(IMC_MRU_GET, &list);
        IMessage(&mru);

        // wype³niamy combobox
        for (int i = 0; i < mru.MRU->count; i++) {
          SendMessage(hWndCombo, CB_ADDSTRING, 0, (LPARAM) (char*) mru.MRU->values[i]);
        }

        HWND edit = CreateWindowEx(WS_EX_CLIENTEDGE, "edit", mru.MRU->values[0], WS_TABSTOP | WS_VISIBLE | WS_CHILD | WS_VSCROLL | 
          ES_MULTILINE | ES_WANTRETURN, 13, 88, 274, 100, hWnd, (HMENU) STATUS_EDIT_INFO, Ctrl->hInst(), NULL);
        SendMessage(edit, WM_SETFONT, (WPARAM) font, true);

        oldEditFix = (WNDPROC) SetWindowLongPtr(edit, GWLP_WNDPROC, (LONG_PTR) EditFix);

        // int chkSt = IMessage(IM_GET_STATUS, net);
        int chkSt = ST_NA;
        CheckDlgButton(hWnd, chkSt, BST_CHECKED);
        SendMessage(hWndCombo, CB_SETCURSEL, 0, 0);
        break;
      }

    case WM_DESTROY: {
      wndData *data = (wndData *) GetWindowLong(hWnd, GWL_USERDATA);
      wCtrl->removeInstance(data->net);

      delete data;
      return(0);
    }

    case WM_COMMAND:
      switch(LOWORD(wParam)) {
        case 2:
        case SC_CLOSE:
        DestroyWindow(hWnd);
        break;

        case 1:
        case STATUS_OK: {
          wndData *data = (wndData *) GetWindowLong(hWnd, GWL_USERDATA);
          int len = SendMessage(GetDlgItem(hWnd, STATUS_EDIT_INFO), WM_GETTEXTLENGTH, 0, 0) + 1;
          char * msg = new char[len];
          
          GetWindowText(GetDlgItem(hWnd, STATUS_EDIT_INFO), msg, len);
          sMRU list;
          std::string name = wCtrl->getMruName();

          list.name = name.c_str();
          list.flags = MRU_SET_LOADFIRST | MRU_GET_USETEMP;
          list.current = msg;
          list.count = 20;
          IMessage(&sIMessage_MRU(IMC_MRU_SET, &list));

          int status = -1;
          if (IsDlgButtonChecked(hWnd, STATUS_CHANGE_STAT) == BST_CHECKED) {
            if (IsDlgButtonChecked(hWnd, ST_ONLINE)) status = ST_ONLINE;
            if (IsDlgButtonChecked(hWnd, ST_CHAT)) status = ST_CHAT;
            if (IsDlgButtonChecked(hWnd, ST_AWAY)) status = ST_AWAY;
            if (IsDlgButtonChecked(hWnd, ST_NA)) status = ST_NA;
            if (IsDlgButtonChecked(hWnd, ST_DND)) status = ST_DND;
            if (IsDlgButtonChecked(hWnd, ST_HIDDEN)) status = ST_HIDDEN;
            if (IsDlgButtonChecked(hWnd, ST_OFFLINE)) status = ST_OFFLINE;
          }

          SETINT(cfg::status::changeOnEnable, (IsDlgButtonChecked(hWnd, STATUS_CHANGE_STAT) == BST_CHECKED) ? 1 : 0);
          SETINT(cfg::status::changeInfoOnEnable, (IsDlgButtonChecked(hWnd, STATUS_CHANGE_INFO) == BST_CHECKED) ? 1 : 0);

          pCtrl->enable(msg, status);

          delete [] msg;
          DestroyWindow(hWnd);
          break;
        }

        default: {
          if (HIWORD(wParam) == CBN_SELCHANGE) {
            int len = SendMessage(GetDlgItem(hWnd, STATUS_EDIT), WM_GETTEXTLENGTH, 0, 0) + 1;
            char * msg = new char[len];

            GetWindowText(GetDlgItem(hWnd, STATUS_EDIT), msg, len);
            SetWindowText(GetDlgItem(hWnd, STATUS_EDIT_INFO), msg);
            delete [] msg;

            return(1);
          }
        }
      }
    }
    return(DefWindowProc(hWnd, iMsg, wParam, lParam));
  }

  AwayWnd::AwayWnd(std::string className, std::string mruName) {
    this->className = className;
    this->mruName = mruName;

    this->classRegister();
  }

  AwayWnd::~AwayWnd() {
    this->classUnRegister();
  }

  void AwayWnd::classRegister() {
    WNDCLASSEX awayWnd;
    ZeroMemory(&awayWnd, sizeof(WNDCLASSEX));
    awayWnd.cbSize = sizeof(awayWnd);
    awayWnd.style = CS_HREDRAW | CS_VREDRAW;
    awayWnd.lpfnWndProc = AwayWndProc;
    awayWnd.cbClsExtra = 0;
    awayWnd.cbWndExtra = 0;
    awayWnd.hInstance = Ctrl->hInst();
    awayWnd.hCursor = LoadCursor(NULL, IDC_ARROW);
    awayWnd.lpszMenuName = NULL;
    awayWnd.lpszClassName = this->className.c_str();
    awayWnd.hbrBackground = GetSysColorBrush(COLOR_BTNFACE);
    awayWnd.hIcon = LoadIcon(Ctrl->hDll(), MAKEINTRESOURCE(179));
    awayWnd.hIconSm = LoadIcon(Ctrl->hDll(), MAKEINTRESOURCE(179));
    RegisterClassEx(&awayWnd);
  }

  void AwayWnd::classUnRegister() {
    UnregisterClass(this->className.c_str(), Ctrl->hInst());
  }

  void AwayWnd::show(std::string title, std::string desc) {
    if (this->haveInstance(net)) {
      SetActiveWindow((HWND) this->getInstance(net));
    } else {
      this->wndDesc = desc;
      HWND hWnd = CreateWindowEx(NULL, this->className.c_str(), title.c_str(), WS_VISIBLE | WS_CAPTION | WS_SYSMENU,
        (GetSystemMetrics(SM_CXSCREEN) / 2) - ((300 - (GetSystemMetrics(SM_CXFIXEDFRAME) * 2)) / 2),
        (GetSystemMetrics(SM_CYSCREEN) / 2) - ((100 - (GetSystemMetrics(SM_CYFIXEDFRAME) * 2) + GetSystemMetrics(SM_CYCAPTION)) / 2),
        300 + GetSystemMetrics(SM_CXFIXEDFRAME) * 2,220 + GetSystemMetrics(SM_CYFIXEDFRAME) * 2 + GetSystemMetrics(SM_CYCAPTION),
        NULL, NULL, Ctrl->hInst(), (void*) net);
    }
  }

  void AwayWnd::prepareButtonImage(HIMAGELIST &hIml, HWND hWnd, int net, int status) {
    hWnd = GetDlgItem(hWnd, status);

    if (ICMessage(IMC_ISWINXP)) {
      hIml = ImageList_Create(16, 16, ILC_COLOR32 | ILC_MASK, 7, 0);
      HICON hIco = (HICON) IMessage(IMI_ICONGET, 0, 0, UIIcon(IT_STATUS, net, status, 0), IML_ICO2);

      if (ImageList_ReplaceIcon(hIml, -1, hIco) == -1) { // normal
        hIco = (HICON) IMessage(IMI_ICONGET, 0, 0, UIIcon(IT_STATUS, 0, status, 0), IML_16);
        ImageList_ReplaceIcon(hIml, -1, hIco);
      }

      ImageList_ReplaceIcon(hIml, -1, hIco); // hover
      ImageList_ReplaceIcon(hIml, -1, hIco); // pressed
      ImageList_ReplaceIcon(hIml, -1, hIco); // disabled
      ImageList_ReplaceIcon(hIml, -1, hIco); // focused
      ImageList_ReplaceIcon(hIml, -1, hIco); // focus + hover
      ImageList_ReplaceIcon(hIml, -1, hIco); // pressed + ??
      // DestroyIcon(hIco); // wedlug Konnekt SDK to jest kopia do usuniecia - naprawde nie jest to kopia

      BUTTON_IMAGELIST bil;
      ZeroMemory(&bil, sizeof(BUTTON_IMAGELIST));
      bil.himl = hIml;
      bil.margin.left = 0;
      bil.margin.top = 0;
      bil.margin.right = 0;
      bil.margin.bottom = 0;
      bil.uAlign = BUTTON_IMAGELIST_ALIGN_LEFT;
      Button_SetImageList(hWnd, &bil);
    } else {
      SendMessage(hWnd, BM_SETIMAGE, IMAGE_ICON, (LPARAM) IMessage(IMI_ICONGET, 0, 0, UIIcon(IT_STATUS, net, status, 0), IML_ICO2));
    }
  }
}