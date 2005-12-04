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

#include "netList.h"
#include "Format.h"
#include "Status.h"
#include "Control.h"

#include "netList.cpp"
#include "Format.cpp"
#include "Status.cpp"
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
    delete pCtrl, sCtrl;
    pCtrl, sCtrl = NULL;

    delete lCtrl::status, lCtrl::reply;
    lCtrl::status, lCtrl::reply = NULL;

    return(1);
  }

  int ISetCols() {
    Ctrl->SetColumn(DTCFG, cfg::status::netChange, DT_CT_STR, "", "kAway2/status/netChange");
    Ctrl->SetColumn(DTCFG, cfg::status::whenInvisible, DT_CT_INT, 1, "kAway2/status/whenInvisible");

    Ctrl->SetColumn(DTCFG, cfg::reply::netChange, DT_CT_STR, "", "kAway2/reply/netChange");
    Ctrl->SetColumn(DTCFG, cfg::reply::whenInvisible, DT_CT_INT, 1, "kAway2/reply/whenInvisible");

    return(1);
  }

  int IPrepare() {
    pCtrl = new Control();

    lCtrl::status = new netList(cfg::status::netChange, ui::cfgGroup, dynAct::status, 
      act::cfgGroupCheckCreate, act::cfgGroupCheckDestroy);
    lCtrl::status->loadNets();

    lCtrl::reply = new netList(cfg::reply::netChange, ui::replyCfgGroup, dynAct::reply, 
      act::replyCfgGroupCheckCreate, act::replyCfgGroupCheckDestroy);
    lCtrl::reply->loadNets();

    sCtrl = new Status(lCtrl::status, cfg::status::whenInvisible);
    pCtrl->setStatusCtrl(sCtrl);

    pCtrl->Enable("test");
    sCtrl->ChangeStatus("ssaj mego bena dzifko!");

    pCtrl->Debug("net = %i, Ctrl = %i, pCtrl = %i, sCtrl = %i", net, Ctrl, pCtrl, sCtrl);

    /* Registering icons */
    // IconRegister(IML_32, ico::logoBig, Ctrl->hDll(), IDI_LOGO32);
    // IconRegister(IML_16, ico::logoSmall, Ctrl->hDll(), IDI_LOGO16);
    IconRegister(IML_16, ico::logoSmall, Ctrl->hDll(), IDI_ENABLE); // temp, will be deleted
    IconRegister(IML_16, ico::enable, Ctrl->hDll(), IDI_ENABLE);
    IconRegister(IML_16, ico::disable, Ctrl->hDll(), IDI_DISABLE);

    /* Adding configuration tabs */
    UIGroupInsert(IMIG_CFG_PLUGS, ui::cfgGroup, -1, ACTR_SAVE, "kAway2", ico::logoSmall);
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

    UIActionCfgAddPluginInfoBox2(ui::cfgGroup, "Implementacja IRCowej funkcjonalnoœci <b>/away [...]</b>", header,
      shared::Icon32(ico::logoBig).c_str());

    /* Main tab */
    /* |-> General settings group */
    UIActionCfgAdd(ui::cfgGroup, 0, ACTT_GROUP, "Ustawienia");
    UIActionCfgAdd(ui::cfgGroup, 0, ACTT_CHECK, "Synchronizuj z trybem auto-away", cfg::autoAwaySync);
    UIActionCfgAdd(ui::cfgGroup, 0, ACTT_CHECK, "Wy³¹czaj w momencie wys³ania wiadomoœci", cfg::disableOnMsgSend);
    UIActionCfgAdd(ui::cfgGroup, 0, ACTT_CHECK, "Zapisuj wiadomoœci w osobnej historii", cfg::saveToHistory);
    UIActionCfgAdd(ui::cfgGroup, 0, ACTT_CHECK, "Komendy '/away' i '/back' z okna rozmów" AP_TIP 
      "/away [...] w³¹cza tryb away, a /back [...] go wy³¹cza; mo¿na podaæ te¿ powód w³¹czenia/wy³¹czenia trybu", cfg::ircCmds);
    UIActionCfgAdd(ui::cfgGroup, 0, ACTT_CHECK, "Pokazuj powiadomienia K.Notify", cfg::useKNotify);
    UIActionCfgAdd(ui::cfgGroup, 0, ACTT_CHECK, "Zmieniaj status przy w³¹czonym statusie 'ukryty'", cfg::status::whenInvisible);
    UIActionCfgAdd(ui::cfgGroup, 0, ACTT_GROUPEND);

    /* |-> Net selection group */
    UIActionCfgAdd(ui::cfgGroup, 0, ACTT_GROUP, "Wybierz sieci, na których chcesz zmieniaæ status:");
    lCtrl::status->UIDraw();
    UIActionCfgAdd(ui::cfgGroup, 0, ACTT_GROUPEND);

    /* |-> Interface group */
    UIActionCfgAdd(ui::cfgGroup, 0, ACTT_GROUP, "Interfejs");
    UIActionCfgAdd(ui::cfgGroup, 0, ACTT_CHECK, "Pokazuj przycisk w menu tray'a", cfg::btnInTrayMenu);
    UIActionCfgAdd(ui::cfgGroup, 0, ACTT_CHECK, "Pokazuj przycisk w g³ównym oknie", cfg::btnInMainWindow);
    UIActionCfgAdd(ui::cfgGroup, 0, ACTT_CHECK, "Pokazuj przycisk w oknie rozmowy", cfg::btnInCntWindow);
    UIActionCfgAdd(ui::cfgGroup, 0, ACTT_GROUPEND);

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
    UIActionCfgAdd(ui::replyCfgGroup, 0, ACTT_SEPARATOR);
    UIActionCfgAdd(ui::replyCfgGroup, 0, ACTT_CHECK, "Wyœlij powiadomienie jeœli kontakt przyœle 'has³o'", cfg::reply::magicKey);
    UIActionCfgAdd(ui::replyCfgGroup, 0, ACTT_COMBO | ACTSC_INLINE | ACTSCOMBO_LIST, 
      "SMS" AP_ICO "0x50000090" AP_VALUE "sms\n" 
      "EMail" AP_ICO "0x50000080" AP_VALUE "email\n" 
      "Przekierowanie" AP_ICO "0x50000080" AP_VALUE "forward", // bad icon for now
      cfg::reply::magicKeyNotifyMethod);
    UIActionCfgAdd(ui::replyCfgGroup, 0, ACTT_COMMENT, "Sposób wysy³ki");
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

    return(1);
  }

  ActionProc(sUIActionNotify_base * anBase) {
    sUIActionNotify_2params * an = static_cast<sUIActionNotify_2params*>(anBase);

    // bool isMe = (GETCNTI(anBase->act.cnt, CNT_NET) == net);

    pCtrl->Debug("[ActionProc()]: anBase->act.id = %i, an->code = %i", anBase->act.id, an->code);

    lCtrl::status->actionHandle(anBase->act.id, an->code);
    lCtrl::reply->actionHandle(anBase->act.id, an->code);

    switch (anBase->act.id) {
      //
    }
    return(0);
  }
}

int __stdcall IMessageProc(sIMessage_base * msgBase) {
  sIMessage_2params * msg = (msgBase->s_size >= sizeof(sIMessage_2params)) ? static_cast<sIMessage_2params*>(msgBase) : 0;

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