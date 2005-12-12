/*
 *  kAway2
 *
 *  Please READ /License.txt FIRST! 
 *
 *  Copyright (C)2005 Sijawusz Pur Rahnama
 *
 *  $Id$
 */

#include "stdafx.h"

#include "NetList.h"
#include "Format.h"
#include "Status.h"
#include "AwayWnd.h"
#include "Control.h"

#include "NetList.cpp"
#include "Format.cpp"
#include "Status.cpp"
#include "AwayWnd.cpp"
#include "Control.cpp"

int __stdcall DllMain(void * hinstDLL, unsigned long fdwReason, void * lpvReserved) {
  return(true);
}

using namespace kAway2;

namespace kAway2 {
  int IStart() {

    return(1);
  }

  int IEnd() {
    delete pCtrl, sCtrl, wCtrl;
    pCtrl, sCtrl, wCtrl = NULL;

    delete lCtrl::status, lCtrl::reply;
    lCtrl::status, lCtrl::reply = NULL;

    return(1);
  }

  int ISetCols() {
    Ctrl->SetColumn(DTCFG, cfg::autoAwaySync, DT_CT_INT, 0, "kAway2/autoAwaySync");
    Ctrl->SetColumn(DTCFG, cfg::useKNotify, DT_CT_INT, 1, "kAway2/useKNotify");
    Ctrl->SetColumn(DTCFG, cfg::ircCmds, DT_CT_INT, 1, "kAway2/ircCmds");
    Ctrl->SetColumn(DTCFG, cfg::disableOnMsgSend, DT_CT_INT, 0, "kAway2/disableOnMsgSend");
    Ctrl->SetColumn(DTCFG, cfg::saveToHistory, DT_CT_INT, 1, "kAway2/saveToHistory");

    Ctrl->SetColumn(DTCFG, cfg::btnInMainWindow, DT_CT_INT, 1, "kAway2/btnInMainWindow");
    Ctrl->SetColumn(DTCFG, cfg::btnInCntWindow, DT_CT_INT, 1, "kAway2/btnInCntWindow");
    Ctrl->SetColumn(DTCFG, cfg::btnInTrayMenu, DT_CT_INT, 1, "kAway2/btnInTrayMenu");

    Ctrl->SetColumn(DTCFG, cfg::tpl::enable, DT_CT_STR, "", "kAway2/tpl/enable");
    Ctrl->SetColumn(DTCFG, cfg::tpl::disable, DT_CT_STR, "", "kAway2/tpl/disable");
    Ctrl->SetColumn(DTCFG, cfg::tpl::reply, DT_CT_STR, "", "kAway2/tpl/reply");
    Ctrl->SetColumn(DTCFG, cfg::tpl::forward, DT_CT_STR, "", "kAway2/tpl/forward");
    Ctrl->SetColumn(DTCFG, cfg::tpl::sms, DT_CT_STR, "", "kAway2/tpl/sms");
    Ctrl->SetColumn(DTCFG, cfg::tpl::email, DT_CT_STR, "", "kAway2/tpl/email");
    Ctrl->SetColumn(DTCFG, cfg::tpl::status, DT_CT_STR, "", "kAway2/tpl/status");

    Ctrl->SetColumn(DTCFG, cfg::reply::onEnable, DT_CT_INT, 0, "kAway2/reply/onEnable");
    Ctrl->SetColumn(DTCFG, cfg::reply::onDisable, DT_CT_INT, 0, "kAway2/reply/onDisable");
    Ctrl->SetColumn(DTCFG, cfg::reply::onMsg, DT_CT_INT, 1, "kAway2/reply/onMsg");
    Ctrl->SetColumn(DTCFG, cfg::reply::whenInvisible, DT_CT_INT, 1, "kAway2/reply/whenInvisible");
    Ctrl->SetColumn(DTCFG, cfg::reply::minInterval, DT_CT_INT, 0, "kAway2/reply/minInterval");
    Ctrl->SetColumn(DTCFG, cfg::reply::useHtml, DT_CT_INT, 0, "kAway2/reply/useHtml");
    Ctrl->SetColumn(DTCFG, cfg::reply::netChange, DT_CT_STR, "", "kAway2/reply/netChange");

    Ctrl->SetColumn(DTCFG, cfg::sms::interval, DT_CT_INT, 150, "kAway2/sms/interval");
    Ctrl->SetColumn(DTCFG, cfg::sms::gate, DT_CT_STR, "", "kAway2/sms/gate");
    Ctrl->SetColumn(DTCFG, cfg::sms::number, DT_CT_INT, 0, "kAway2/sms/number");

    Ctrl->SetColumn(DTCFG, cfg::status::whenInvisible, DT_CT_INT, 1, "kAway2/status/whenInvisible");
    Ctrl->SetColumn(DTCFG, cfg::status::netChange, DT_CT_STR, "", "kAway2/status/netChange");
    Ctrl->SetColumn(DTCFG, cfg::status::changeOnEnable, DT_CT_INT, 1, "kAway2/status/changeOnEnable");
    Ctrl->SetColumn(DTCFG, cfg::status::changeInfoOnEnable, DT_CT_INT, 1, "kAway2/status/changeInfoOnEnable");

    return(1);
  }

  int IPrepare() {
    pCtrl = new Control();
    wCtrl = new AwayWnd("kAway2AwayWnd", "kA2_awayMsg");

    lCtrl::status = new NetList(cfg::status::netChange, ui::statusCfgGroup, dynAct::status, 
      act::cfgGroupCheckCreate, act::cfgGroupCheckDestroy);
    lCtrl::status->loadNets();

    lCtrl::reply = new NetList(cfg::reply::netChange, ui::replyCfgGroup, dynAct::reply, 
      act::replyCfgGroupCheckCreate, act::replyCfgGroupCheckDestroy);
    lCtrl::reply->loadNets();

    sCtrl = new Status(lCtrl::status, cfg::status::whenInvisible, "status");
    pCtrl->setStatusCtrl(sCtrl);

    pCtrl->Log("net = %i, Ctrl = %i, pCtrl = %i, sCtrl = %i, wCtrl = %i", net, Ctrl, pCtrl, sCtrl, wCtrl);

    /* Defining help variables */
    tHelpVars stVars, rVars;

    stVars.push_back( helpVar( "status", "Aktualny status" ) );
    stVars.push_back( helpVar( "date", "Data w³¹czenia trybu away" ) );
    stVars.push_back( helpVar( "time", "Czas w³¹czenia trybu away" ) );
    stVars.push_back( helpVar( "msg", "Przyczyna nieobecnoœci" ) );

    rVars.push_back( helpVar( "display", "Nazwa wyœwietlania przypisana do kontaktu" ) );
    rVars.push_back( helpVar( "name", "Imiê przypisane do kontaktu" ) );
    rVars.push_back( helpVar( "nick", "Ksywka przypisana do kontaktu" ) );
    rVars.push_back( helpVar( "surname", "Nazwisko przypisane do kontaktu" ) );
    rVars.push_back( helpVar( "date", "Data w³¹czenia trybu away" ) );
    rVars.push_back( helpVar( "time", "Czas w³¹czenia trybu away" ) );
    rVars.push_back( helpVar( "msg", "Przyczyna nieobecnoœci" ) );

    /* Registering icons */
    // IconRegister(IML_32, ico::logoBig, Ctrl->hDll(), IDI_LOGO32);
    // IconRegister(IML_16, ico::logoSmall, Ctrl->hDll(), IDI_LOGO16);
    IconRegister(IML_16, ico::logoSmall, Ctrl->hDll(), IDI_ENABLE); // temp, will be deleted
    IconRegister(IML_16, ico::enable, Ctrl->hDll(), IDI_ENABLE);
    IconRegister(IML_16, ico::disable, Ctrl->hDll(), IDI_DISABLE);

    /* Adding configuration tabs */
    UIGroupInsert(IMIG_CFG_PLUGS, ui::cfgGroup, -1, ACTR_SAVE, "kAway2", ico::logoSmall);
    UIGroupInsert(ui::cfgGroup, ui::statusCfgGroup, -1, ACTR_SAVE, "Status", 0x00000020);
    UIGroupInsert(ui::cfgGroup, ui::replyCfgGroup, -1, ACTR_SAVE, "Autoresponder", 0x50000010);

    /* Plugin info box */
    char header[400];
    sprintf(header, "<span class='note'>Powered by: <b>%s</b></span><br/>"
      "<span class='note'>Skompilowano: <b>%s</b> [<b>%s</b>]</span><br/>"
      "Informacje o wtyczce i Ÿród³a na stronie projektu "
      "<b>KPlugins</b> (http://kplugins.net/)<br/><br/>"
      "Copyright © 2004-2005 <b>Sijawusz Pur Rahnama</b><br/>"
      "Copyright © 2004-2005 <b>KPlugins Team</b>",
      poweredBy, __DATE__, __TIME__);

    UIActionCfgAddPluginInfoBox2(ui::cfgGroup, "Implementacja IRCowej funkcjonalnoœci <b>/away [...]</b>", 
      header, shared::Icon32(ico::logoBig).c_str());

    /* Main tab */
    /* |-> General settings group */
    UIActionCfgAdd(ui::cfgGroup, 0, ACTT_GROUP, "Ustawienia");
    UIActionCfgAdd(ui::cfgGroup, 0, ACTT_CHECK, "Synchronizuj z trybem auto-away", cfg::autoAwaySync);
    UIActionCfgAdd(ui::cfgGroup, 0, ACTT_CHECK, "Wy³¹czaj w momencie wys³ania wiadomoœci", cfg::disableOnMsgSend);
    UIActionCfgAdd(ui::cfgGroup, 0, ACTT_CHECK, "Zapisuj wiadomoœci w osobnej historii", cfg::saveToHistory);
    UIActionCfgAdd(ui::cfgGroup, 0, ACTT_CHECK, "Komendy '/away' i '/back' z okna rozmów" AP_TIP 
      "/away [...] w³¹cza tryb away, a /back [...] go wy³¹cza; mo¿na podaæ te¿ powód w³¹czenia/wy³¹czenia trybu", cfg::ircCmds);
    UIActionCfgAdd(ui::cfgGroup, 0, ACTT_CHECK, "Pokazuj powiadomienia K.Notify", cfg::useKNotify);
    UIActionCfgAdd(ui::cfgGroup, 0, ACTT_GROUPEND);

    /* |-> Interface group */
    UIActionCfgAdd(ui::cfgGroup, 0, ACTT_GROUP, "Interfejs");
    UIActionCfgAdd(ui::cfgGroup, 0, ACTT_CHECK | ACTSC_NEEDRESTART, "Pokazuj przycisk w menu tray'a", cfg::btnInTrayMenu);
    UIActionCfgAdd(ui::cfgGroup, 0, ACTT_CHECK | ACTSC_NEEDRESTART, "Pokazuj przycisk w g³ównym oknie", cfg::btnInMainWindow);
    UIActionCfgAdd(ui::cfgGroup, 0, ACTT_CHECK | ACTSC_NEEDRESTART, "Pokazuj przycisk w oknie rozmowy", cfg::btnInCntWindow);
    UIActionCfgAdd(ui::cfgGroup, 0, ACTT_GROUPEND);

    /* Status tab */
    /* |-> Settings group */
    UIActionCfgAdd(ui::statusCfgGroup, 0, ACTT_GROUP, "Ustawienia");
    UIActionCfgAdd(ui::statusCfgGroup, 0, ACTT_CHECK, "Zmieniaj status przy w³¹czonym statusie 'ukryty'", cfg::status::whenInvisible);
    UIActionCfgAdd(ui::statusCfgGroup, 0, ACTT_SEPARATOR);
    UIActionCfgAdd(ui::statusCfgGroup, 0, ACTT_CHECK, "Zmieniaj status przy w³¹czeniu trybu away", cfg::status::changeOnEnable);
    UIActionCfgAdd(ui::statusCfgGroup, 0, ACTT_CHECK, "Zmieniaj opis statusu przy w³¹czeniu trybu away", cfg::status::changeInfoOnEnable);
    UIActionCfgAdd(ui::statusCfgGroup, 0, ACTT_GROUPEND);

    UIActionCfgAdd(ui::statusCfgGroup, 0, ACTT_GROUP, "Szablon statusu");
    UIActionCfgAdd(ui::statusCfgGroup, 0, ACTT_TIPBUTTON | ACTSBUTTON_ALIGNRIGHT | ACTSC_INLINE, 
      sCtrl->fCtrl->buildHtmlHelp(stVars).c_str());
    UIActionCfgAdd(ui::statusCfgGroup, 0, ACTT_TEXT, 0, cfg::tpl::status);
    UIActionCfgAdd(ui::statusCfgGroup, 0, ACTT_GROUPEND);

    /* |-> Net selection group */
    UIActionCfgAdd(ui::statusCfgGroup, 0, ACTT_GROUP, "Wybierz sieci, na których chcesz zmieniaæ status:");
    lCtrl::status->UIDraw();
    UIActionCfgAdd(ui::statusCfgGroup, 0, ACTT_GROUPEND);

    /* Autoresponder tab */
    /* |-> Minimal reply interval group */
    UIActionCfgAdd(ui::replyCfgGroup, 0, ACTT_GROUP, "Minimalny interwa³ pomiêdzy wysy³anymi odpowiedziami [s]:");
    UIActionCfgAdd(ui::replyCfgGroup, 0, ACTT_SLIDER, "Ma³y\nDu¿y" AP_STEP "60" AP_MINIMUM "0" AP_MAXIMUM "3600" AP_TIP 
      "0 = odpowiedŸ bêdzie wysy³ana tylko jeden raz\n3600 = 1h ;>", cfg::reply::minInterval);
    UIActionCfgAdd(ui::replyCfgGroup, 0, ACTT_GROUPEND);

    /* |-> Net selection group */
    UIActionCfgAdd(ui::replyCfgGroup, 0, ACTT_GROUP, "Wybierz sieci, na których maj¹ dzia³aæ powiadomienia:");
    lCtrl::reply->UIDraw();
    UIActionCfgAdd(ui::replyCfgGroup, 0, ACTT_GROUPEND);

    UIActionCfgAdd(ui::replyCfgGroup, 0, ACTT_GROUP, "Wysy³anie powiadomieñ");
    UIActionCfgAdd(ui::replyCfgGroup, 0, ACTT_CHECK, "Wysy³aj powiadomienia przy w³¹czonym statusie 'ukryty'", cfg::reply::whenInvisible);
    UIActionCfgAdd(ui::replyCfgGroup, 0, ACTT_SEPARATOR);
    UIActionCfgAdd(ui::replyCfgGroup, 0, ACTT_CHECK, "Odpowiadaj na odebrane wiadomoœci", cfg::reply::onMsg);
    UIActionCfgAdd(ui::replyCfgGroup, 0, ACTT_CHECK, "Wysy³aj powiadomienie o w³¹czeniu trybu away", cfg::reply::onEnable);
    UIActionCfgAdd(ui::replyCfgGroup, 0, ACTT_CHECK, "Wysy³aj powiadomienie o wy³¹czeniu trybu away", cfg::reply::onDisable);
    /*
    UIActionCfgAdd(ui::replyCfgGroup, 0, ACTT_SEPARATOR);
    UIActionCfgAdd(ui::replyCfgGroup, 0, ACTT_CHECK, "Wyœlij powiadomienie jeœli kontakt przyœle 'has³o'", cfg::reply::magicKey);
    UIActionCfgAdd(ui::replyCfgGroup, 0, ACTT_COMBO | ACTSC_INLINE | ACTSCOMBO_LIST, 
      "SMS" AP_ICO "0x50000090" AP_VALUE "sms\n" 
      "EMail" AP_ICO "0x50000080" AP_VALUE "email\n" 
      "Przekierowanie" AP_ICO "0x50000080" AP_VALUE "forward", // bad icon for now
      cfg::reply::magicKeyNotifyMethod);
    UIActionCfgAdd(ui::replyCfgGroup, 0, ACTT_COMMENT, "Sposób wysy³ki");
    */
    UIActionCfgAdd(ui::replyCfgGroup, 0, ACTT_GROUPEND);

    /* |-> Settings group */
    UIActionCfgAdd(ui::replyCfgGroup, 0, ACTT_GROUP, "Szablony powiadomieñ");
    UIActionCfgAdd(ui::replyCfgGroup, 0, ACTT_CHECK, "Formatuj kod HTML" AP_TIP 
      "Jeœli w³¹czone, znaki specjalne nale¿y zamieniaæ na ich HTML-owe odpowiedniki (np.: (< na &lt;), (> na &gt;))", cfg::reply::useHtml);
    UIActionCfgAdd(ui::replyCfgGroup, 0, ACTT_SEPARATOR);

    UIActionCfgAdd(ui::replyCfgGroup, 0, ACTT_COMMENT, "OdpowiedŸ:");
    UIActionCfgAdd(ui::replyCfgGroup, 0, ACTT_TEXT, 0, cfg::tpl::reply);

    UIActionCfgAdd(ui::replyCfgGroup, 0, ACTT_COMMENT, "W³¹czenie trybu away:");
    UIActionCfgAdd(ui::replyCfgGroup, 0, ACTT_TEXT, 0, cfg::tpl::enable);

    UIActionCfgAdd(ui::replyCfgGroup, 0, ACTT_COMMENT, "Wy³¹czenie trybu away:");
    UIActionCfgAdd(ui::replyCfgGroup, 0, ACTT_TEXT, 0, cfg::tpl::disable);
    UIActionCfgAdd(ui::replyCfgGroup, 0, ACTT_GROUPEND);

    /* Buttons */
    if (GETINT(cfg::btnInMainWindow))
      UIActionAdd(ICMessage(IMI_GETPLUGINSGROUP), ui::powerInMainWnd, 0, "W³¹cz tryb away", ico::enable);
    if (GETINT(cfg::btnInCntWindow))
      UIActionAdd(IMIG_MSGTB, ui::powerInCntWnd, 0, "W³¹cz tryb away", ico::enable);
    if (GETINT(cfg::btnInTrayMenu)) {
      UIActionInsert(IMIG_TRAY, 0, 0, ACTT_SEP);
      UIActionInsert(IMIG_TRAY, ui::powerInTrayMenu, 0, 0, "W³¹cz tryb away", ico::enable);
    }

    return(1);
  }

  ActionProc(sUIActionNotify_base * anBase) {
    sUIActionNotify_2params * an = static_cast<sUIActionNotify_2params*>(anBase);

    pCtrl->Debug("[ActionProc()]: anBase->act.id = %i, an->code = %i", anBase->act.id, an->code);

    lCtrl::status->actionHandle(anBase->act.id, an->code);
    lCtrl::reply->actionHandle(anBase->act.id, an->code);

    switch (anBase->act.id) {
      case ui::powerInMainWnd:
      case ui::powerInCntWnd:
      case ui::powerInTrayMenu:
        if (pCtrl->isEnabled()) {
          if (ICMessage(IMI_CONFIRM, (int) "Na pewno chcesz wy³¹czyæ tryb away?")) 
            pCtrl->disable();
        } else {
          wCtrl->show("Podaj powód nieobecnoœci", "Podaj przyczynê swojej nieobecnoœci");
        }
        break;
    }
    return(0);
  }
}

int __stdcall IMessageProc(sIMessage_base * msgBase) {
  sIMessage_2params * msg = static_cast<sIMessage_2params*>(msgBase);

  switch (msgBase->id) {
    case IM_PLUG_NET:        return(net);
    case IM_PLUG_TYPE:       return(IMT_UI | IMT_CONFIG | IMT_ALLMESSAGES);
    case IM_PLUG_VERSION:    return(0);
    case IM_PLUG_SDKVERSION: return(KONNEKT_SDK_V);
    case IM_PLUG_SIG:        return((int)"kA2");
    case IM_PLUG_CORE_V:     return((int)"W98");
    case IM_PLUG_UI_V:       return(0);
    case IM_PLUG_NAME:       return((int)"kAway2");
    case IM_PLUG_NETNAME:    return(0);
    case IM_PLUG_PRIORITY:   return(PLUGP_HIGH);
    case IM_PLUG_INIT:       Plug_Init(msg->p1, msg->p2);   return(1);
    case IM_PLUG_DEINIT:     Plug_Deinit(msg->p1, msg->p2); return(1);
    case IM_SETCOLS:         return(ISetCols());
    case IM_UI_PREPARE:      return(IPrepare());
    case IM_START:           return(IStart());
    case IM_END:             return(IEnd());
    case IM_UIACTION:        return(ActionProc((sUIActionNotify_base*)msg->p1));

    default:
      if (Ctrl) Ctrl->setError(IMERROR_NORESULT);
      return(0);
  }
  return(0);
}