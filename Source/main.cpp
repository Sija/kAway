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
#include <konnekt/lib.h>

#include "main.h"
#include "plugsNET.h"
#include "functions.h"

#include <Stamina/Time64.h>
#include <Stamina/Time64.cpp>

#include "Status.h"
#include "Status.cpp"
#include "Control.h"
#include "Control.cpp"

kAway2::Control *pCtrl = NULL;

int __stdcall DllMain(void * hinstDLL, unsigned long fdwReason, void * lpvReserved) {
  return(true);
}

using namespace kAway2;

namespace kAway2 {
  int IStart() {
    return(1);
  }

  int IEnd() {
    delete pCtrl;
    pCtrl = NULL;

    return(1);
  }

  int ISetCols() {
    Ctrl->SetColumn(DTCFG, kAway2::cfg::netChange, DT_CT_STR, "", "kAway2/netChange");

    return(1);
  }

  int IPrepare() {
    pCtrl = new kAway2::Control();

    /* Configuration */
    UIGroupInsert(IMIG_CFG_PLUGS, kAway2::ui::cfgGroup, -1, ACTR_SAVE, "kAway2", kAway2::ico::logoSmall);
    UIGroupInsert(kAway2::ui::cfgGroup, kAway2::ui::replyCfgGroup, -1, ACTR_SAVE, "Autoresponder", 0x50000010);

    /* Plugin info box */
    char header[400];
    sprintf(header, "<span class='note'>Powered by: <b>%s</b></span><br/>"
      "<span class='note'>Skompilowano: <b>%s</b> [<b>%s</b>]</span><br/>"
      "Informacje o wtyczce i Ÿród³a na stronie projektu "
      "<b>KPlugins</b> (http://kplugins.net/)<br/><br/>"
      "Copyright © 2004-2005 <b>Sijawusz Pur Rahnama</b><br/>"
      "Copyright © 2004-2005 <b>KPlugins Team</b>",
      kAway2::poweredBy, __DATE__, __TIME__);

    UIActionCfgAddPluginInfoBox2(kAway2::ui::cfgGroup, "Implementacja IRCowej funkcjonalnoœci <b>/away [...]</b>", header, shared::Icon32(kAway2::ico::logoBig).c_str());

    /* Main tab */
    /*  General settings group */
    UIActionCfgAdd(kAway2::ui::cfgGroup, 0, ACTT_GROUP, "Ustawienia");
    UIActionCfgAdd(kAway2::ui::cfgGroup, 0, ACTT_CHECK, "Synchronizuj z trybem auto-away", kAway2::cfg::autoAwaySync);
    UIActionCfgAdd(kAway2::ui::cfgGroup, 0, ACTT_CHECK, "Wy³¹czaj w momencie wys³ania wiadomoœci", kAway2::cfg::disableOnMsgSend);
    UIActionCfgAdd(kAway2::ui::cfgGroup, 0, ACTT_CHECK, "Zapisuj wiadomoœci w osobnej historii", kAway2::cfg::saveToHistory);
    UIActionCfgAdd(kAway2::ui::cfgGroup, 0, ACTT_CHECK, "Komendy /away [...] i /back z okna rozmów", kAway2::cfg::ircCmds);
    UIActionCfgAdd(kAway2::ui::cfgGroup, 0, ACTT_GROUPEND);

    /*  Interface group */
    UIActionCfgAdd(kAway2::ui::cfgGroup, 0, ACTT_GROUP, "Interfejs");
    UIActionCfgAdd(kAway2::ui::cfgGroup, 0, ACTT_CHECK, "Pokazuj przycisk w menu tray'a", kAway2::cfg::btnInTrayMenu);
    UIActionCfgAdd(kAway2::ui::cfgGroup, 0, ACTT_CHECK, "Pokazuj przycisk w g³ównym oknie", kAway2::cfg::btnInMainWindow);
    UIActionCfgAdd(kAway2::ui::cfgGroup, 0, ACTT_CHECK, "Pokazuj przycisk w oknie rozmowy", kAway2::cfg::btnInCntWindow);
    UIActionCfgAdd(kAway2::ui::cfgGroup, 0, ACTT_GROUPEND);

    /* Net selection */
    UIActionCfgAdd(kAway2::ui::cfgGroup, 0, ACTT_GROUP, "Wybierz sieci, na których ma dzia³aæ wtyczka:");
    pCtrl->netListInit();
    UIActionCfgAdd(kAway2::ui::cfgGroup, 0, ACTT_GROUPEND);

    /*  Interface group */
    UIActionCfgAdd(kAway2::ui::cfgGroup, 0, ACTT_GROUP, "Informowanie o trybie away");
    // UIActionCfgAdd(kAway2::ui::cfgGroup, 0, ACTT_CHECK, "Wysy³aj powiadomienia przy w³¹czonym statusie 'ukryty'", kAway2::cfg::notifyWhenInvisible);
    UIActionCfgAdd(kAway2::ui::cfgGroup, 0, ACTT_CHECK, "Informuj za pomoc¹ K.Notify", kAway2::cfg::useKNotify);
    UIActionCfgAdd(kAway2::ui::cfgGroup, 0, ACTT_SEPARATOR);
    UIActionCfgAdd(kAway2::ui::cfgGroup, 0, ACTT_CHECK, "Odpowiadaj na wiadomoœci", kAway2::cfg::notifyOnReply);
    UIActionCfgAdd(kAway2::ui::cfgGroup, 0, ACTT_CHECK, "Wysy³aj komunikat informuj¹cy o w³¹czeniu trybu away", kAway2::cfg::notifyOnEnable);
    UIActionCfgAdd(kAway2::ui::cfgGroup, 0, ACTT_CHECK, "Wysy³aj komunikat informuj¹cy o wy³¹czeniu trybu away", kAway2::cfg::notifyOnDisable);
    UIActionCfgAdd(kAway2::ui::cfgGroup, 0, ACTT_GROUPEND);

    /* Autoresponder tab */
    /*  Settings group */
    UIActionCfgAdd(kAway2::ui::replyCfgGroup, 0, ACTT_GROUP, "Ustawienia");
    UIActionCfgAdd(kAway2::ui::replyCfgGroup, 0, ACTT_CHECK, "Formatuj kod HTML" AP_TIP "Znaki specjalne nale¿y zamieniaæ na ich HTML-owe odpowiedniki (np.: (< na &lt;), (> na &gt;))", kAway2::cfg::replyUseHtml);
    UIActionCfgAdd(kAway2::ui::replyCfgGroup, 0, ACTT_SEPARATOR);
    UIActionCfgAdd(kAway2::ui::replyCfgGroup, 0, ACTT_COMMENT, "Minimalny interwa³ pomiêdzy wysy³anymi odpowiedziami [s]:");
    UIActionCfgAdd(kAway2::ui::replyCfgGroup, 0, ACTT_SLIDER, "Ma³y\nDu¿y" AP_STEP "60" AP_MINIMUM "0" AP_MAXIMUM "3600" AP_TIP "0 = odpowiedŸ bêdzie wysy³ana tylko jeden raz\n3600 = 1h ;>", kAway2::cfg::replyMinInterval);
    UIActionCfgAdd(kAway2::ui::replyCfgGroup, 0, ACTT_GROUPEND);

    /*  Settings group */
    UIActionCfgAdd(kAway2::ui::replyCfgGroup, 0, ACTT_GROUP, "Szablony odpowiedzi");
    UIActionCfgAdd(kAway2::ui::replyCfgGroup, 0, ACTT_COMMENT, "OdpowiedŸ:");
    UIActionCfgAdd(kAway2::ui::replyCfgGroup, 0, ACTT_TEXT, "", kAway2::cfg::tpl::reply);

    UIActionCfgAdd(kAway2::ui::replyCfgGroup, 0, ACTT_COMMENT, "W³¹czenie trybu away:");
    UIActionCfgAdd(kAway2::ui::replyCfgGroup, 0, ACTT_TEXT, "", kAway2::cfg::tpl::enable);

    UIActionCfgAdd(kAway2::ui::replyCfgGroup, 0, ACTT_COMMENT, "Wy³¹czenie trybu away:");
    UIActionCfgAdd(kAway2::ui::replyCfgGroup, 0, ACTT_TEXT, "", kAway2::cfg::tpl::disable);
    UIActionCfgAdd(kAway2::ui::replyCfgGroup, 0, ACTT_GROUPEND);

    /*
    UIActionCfgAdd(kAway2::ui::replyCfgGroup, 0, ACTT_GROUP, "Wyœlij powiadomienie jeœli kontakt przyœle 'has³o':");
    UIActionCfgAdd(kAway2::ui::replyCfgGroup, 0, ACTT_EDIT | ACTSC_INLINE, "", kAway2::cfg::replyMagicKey, 0, 0, 125);
    UIActionCfgAdd(kAway2::ui::replyCfgGroup, 0, ACTT_COMMENT | ACTSC_INLINE, "przez");
    UIActionCfgAdd(kAway2::ui::replyCfgGroup, 0, ACTT_COMBO | ACTSCOMBO_LIST, "nie powiadamiaj" AP_ICO "0x00000074" AP_VALUE "" "\nSMS" AP_ICO "0x50000090" AP_VALUE "sms" "\nEMail" AP_ICO "0x50000080" AP_VALUE "email", kAway2::cfg::replyMagicKeyNotifyMethod, 0, 0, 125);
    UIActionCfgAdd(kAway2::ui::replyCfgGroup, 0, ACTT_GROUPEND);
    */

    return(1);
  }

  ActionCfgProc(sUIActionNotify_base * anBase) {
    sUIActionNotify_2params * an = (anBase->s_size >= sizeof(sUIActionNotify_2params)) ? static_cast<sUIActionNotify_2params*>(anBase) : 0;

    switch (anBase->act.id & ~IMIB_CFG) {
      // ...
    }

    return(0);
  }

  ActionProc(sUIActionNotify_base * anBase) {
    sUIActionNotify_2params * an = (anBase->s_size >= sizeof(sUIActionNotify_2params)) ? static_cast<sUIActionNotify_2params*>(anBase) : 0;
    if ((anBase->act.id & IMIB_) == IMIB_CFG) return(kAway2::ActionCfgProc(anBase));
    bool isMe = (GETCNTI(anBase->act.cnt, CNT_NET) == kAway2::net);

    return(0);
  }
}

int __stdcall IMessageProc(sIMessage_base * msgBase) {
  sIMessage_2params * msg = (msgBase->s_size >= sizeof(sIMessage_2params)) ? static_cast<sIMessage_2params*>(msgBase) : 0;

  switch (msgBase->id) {
    case IM_PLUG_NET:        return(kAway2::net);
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
    case IM_SETCOLS:         return(kAway2::ISetCols());
    case IM_UI_PREPARE:      return(kAway2::IPrepare());
    case IM_START:           return(kAway2::IStart());
    case IM_END:             return(kAway2::IEnd());
    case IM_UIACTION:        return(kAway2::ActionProc((sUIActionNotify_base*)msg->p1));

    default:
      if (Ctrl) Ctrl->setError(IMERROR_NORESULT);
      return(0);
  }
  return(0);
}