/*
 *  kAway
 *  
 *  Please READ /License.txt FIRST! 
 * 
 *  Copyright (C)2005 Sijawusz Pur Rahnama
 *
 *  $Id: $
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

kAway::Control *pCtrl = NULL;

int __stdcall DllMain(void * hinstDLL, unsigned long fdwReason, void * lpvReserved) {
  return(true);
}

using namespace kAway;

namespace kAway {
  int IStart() {
    return(1);
  }

  int IEnd() {
	  delete pCtrl;
	  pCtrl = NULL;

    return(1);
  }

  int ISetCols() {
	  Ctrl->SetColumn(DTCFG, kAway::cfg::netChange, DT_CT_STR, "", "kAway2/netChange");

    return(1);
  }

  int IPrepare() {
	  pCtrl = new kAway::Control();

	  /* Configuration */
	  UIGroupInsert(IMIG_CFG_PLUGS, kAway::ui::cfgGroup, -1, ACTR_SAVE, "kAway", kAway::ico::logoSmall);
	  UIGroupInsert(kAway::ui::cfgGroup, kAway::ui::replyCfgGroup, -1, ACTR_SAVE, "Autoresponder", 0x50000010);

	  /* Plugin info box */
    char header[400];
    sprintf(header, "<span class='note'>Powered by: <b>%s</b></span><br/>"
      "<span class='note'>Skompilowano: <b>%s</b> [<b>%s</b>]</span><br/>"
      "Informacje o wtyczce i Ÿród³a na stronie projektu "
      "<b>KPlugins</b> (http://kplugins.net/)<br/><br/>"
      "Copyright © 2004-2005 <b>Sijawusz Pur Rahnama</b><br/>"
      "Copyright © 2004-2005 <b>KPlugins Team</b>",
      kAway::poweredBy, __DATE__, __TIME__);

    UIActionCfgAddPluginInfoBox2(kAway::ui::cfgGroup, "Implementacja IRCowej funkcjonalnoœci <b>/away [...]</b>", header, shared::Icon32(kAway::ico::logoBig).c_str());

    /* Main tab */
	  /*  General settings group */
	  UIActionCfgAdd(kAway::ui::cfgGroup, 0, ACTT_GROUP, "Ustawienia");
    UIActionCfgAdd(kAway::ui::cfgGroup, 0, ACTT_CHECK, "Synchronizuj z trybem auto-away", kAway::cfg::autoAwaySync);
    UIActionCfgAdd(kAway::ui::cfgGroup, 0, ACTT_CHECK, "Wy³¹czaj w momencie wys³ania wiadomoœci", kAway::cfg::disableOnMsgSend);
    UIActionCfgAdd(kAway::ui::cfgGroup, 0, ACTT_CHECK, "Zapisuj wiadomoœci w osobnej historii", kAway::cfg::saveToHistory);
    UIActionCfgAdd(kAway::ui::cfgGroup, 0, ACTT_CHECK, "Komendy /away [...] i /back z okna rozmów", kAway::cfg::ircCmds);
	  UIActionCfgAdd(kAway::ui::cfgGroup, 0, ACTT_GROUPEND);

	  /*  Interface group */
	  UIActionCfgAdd(kAway::ui::cfgGroup, 0, ACTT_GROUP, "Interfejs");
    UIActionCfgAdd(kAway::ui::cfgGroup, 0, ACTT_CHECK, "Pokazuj przycisk w menu tray'a", kAway::cfg::btnInTrayMenu);
    UIActionCfgAdd(kAway::ui::cfgGroup, 0, ACTT_CHECK, "Pokazuj przycisk w g³ównym oknie", kAway::cfg::btnInMainWindow);
    UIActionCfgAdd(kAway::ui::cfgGroup, 0, ACTT_CHECK, "Pokazuj przycisk w oknie rozmowy", kAway::cfg::btnInCntWindow);
	  UIActionCfgAdd(kAway::ui::cfgGroup, 0, ACTT_GROUPEND);

	  /* Net selection */
	  UIActionCfgAdd(kAway::ui::cfgGroup, 0, ACTT_GROUP, "Wybierz sieci, na których ma dzia³aæ wtyczka:");
	  pCtrl->netListInit();
	  UIActionCfgAdd(kAway::ui::cfgGroup, 0, ACTT_GROUPEND);

	  /*  Interface group */
	  UIActionCfgAdd(kAway::ui::cfgGroup, 0, ACTT_GROUP, "Informowanie o trybie away");
    // UIActionCfgAdd(kAway::ui::cfgGroup, 0, ACTT_CHECK, "Wysy³aj powiadomienia przy w³¹czonym statusie 'ukryty'", kAway::cfg::notifyWhenInvisible);
    UIActionCfgAdd(kAway::ui::cfgGroup, 0, ACTT_CHECK, "Informuj za pomoc¹ K.Notify", kAway::cfg::useKNotify);
	  UIActionCfgAdd(kAway::ui::cfgGroup, 0, ACTT_SEPARATOR);
    UIActionCfgAdd(kAway::ui::cfgGroup, 0, ACTT_CHECK, "Odpowiadaj na wiadomoœci", kAway::cfg::notifyOnReply);
    UIActionCfgAdd(kAway::ui::cfgGroup, 0, ACTT_CHECK, "Wysy³aj komunikat informuj¹cy o w³¹czeniu trybu away", kAway::cfg::notifyOnEnable);
    UIActionCfgAdd(kAway::ui::cfgGroup, 0, ACTT_CHECK, "Wysy³aj komunikat informuj¹cy o wy³¹czeniu trybu away", kAway::cfg::notifyOnDisable);
	  UIActionCfgAdd(kAway::ui::cfgGroup, 0, ACTT_GROUPEND);

	  /* Autoresponder tab */
	  /*  Settings group */
	  UIActionCfgAdd(kAway::ui::replyCfgGroup, 0, ACTT_GROUP, "Ustawienia");
    UIActionCfgAdd(kAway::ui::replyCfgGroup, 0, ACTT_CHECK, "Formatuj kod HTML" AP_TIP "Znaki specjalne nale¿y zamieniaæ na ich HTML-owe odpowiedniki (np.: (< na &lt;), (> na &gt;))", kAway::cfg::replyUseHtml);
	  UIActionCfgAdd(kAway::ui::replyCfgGroup, 0, ACTT_SEPARATOR);
	  UIActionCfgAdd(kAway::ui::replyCfgGroup, 0, ACTT_COMMENT, "Minimalny interwa³ pomiêdzy wysy³anymi odpowiedziami [s]:");
    UIActionCfgAdd(kAway::ui::replyCfgGroup, 0, ACTT_SLIDER, "Ma³y\nDu¿y" AP_STEP "60" AP_MINIMUM "0" AP_MAXIMUM "3600" AP_TIP "0 = odpowiedŸ bêdzie wysy³ana tylko jeden raz\n3600 = 1h ;>", kAway::cfg::replyMinInterval);
	  UIActionCfgAdd(kAway::ui::replyCfgGroup, 0, ACTT_GROUPEND);

	  /*  Settings group */
    UIActionCfgAdd(kAway::ui::replyCfgGroup, 0, ACTT_GROUP, "Szablony odpowiedzi");
    UIActionCfgAdd(kAway::ui::replyCfgGroup, 0, ACTT_COMMENT, "OdpowiedŸ:");
    UIActionCfgAdd(kAway::ui::replyCfgGroup, 0, ACTT_TEXT, "", kAway::cfg::tpl::reply);

    UIActionCfgAdd(kAway::ui::replyCfgGroup, 0, ACTT_COMMENT, "W³¹czenie trybu away:");
    UIActionCfgAdd(kAway::ui::replyCfgGroup, 0, ACTT_TEXT, "", kAway::cfg::tpl::enable);

    UIActionCfgAdd(kAway::ui::replyCfgGroup, 0, ACTT_COMMENT, "Wy³¹czenie trybu away:");
    UIActionCfgAdd(kAway::ui::replyCfgGroup, 0, ACTT_TEXT, "", kAway::cfg::tpl::disable);
	  UIActionCfgAdd(kAway::ui::replyCfgGroup, 0, ACTT_GROUPEND);

    /*
    UIActionCfgAdd(kAway::ui::replyCfgGroup, 0, ACTT_GROUP, "Wyœlij powiadomienie jeœli kontakt przyœle 'has³o':");
    UIActionCfgAdd(kAway::ui::replyCfgGroup, 0, ACTT_EDIT | ACTSC_INLINE, "", kAway::cfg::replyMagicKey, 0, 0, 125);
	  UIActionCfgAdd(kAway::ui::replyCfgGroup, 0, ACTT_COMMENT | ACTSC_INLINE, "przez");
    UIActionCfgAdd(kAway::ui::replyCfgGroup, 0, ACTT_COMBO | ACTSCOMBO_LIST, "nie powiadamiaj" AP_ICO "0x00000074" AP_VALUE "" "\nSMS" AP_ICO "0x50000090" AP_VALUE "sms" "\nEMail" AP_ICO "0x50000080" AP_VALUE "email", kAway::cfg::replyMagicKeyNotifyMethod, 0, 0, 125);
	  UIActionCfgAdd(kAway::ui::replyCfgGroup, 0, ACTT_GROUPEND);
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
    if ((anBase->act.id & IMIB_) == IMIB_CFG) return(kAway::ActionCfgProc(anBase));
    bool isMe = (GETCNTI(anBase->act.cnt, CNT_NET) == kAway::net);

    return(0);
  }
}

int __stdcall IMessageProc(sIMessage_base * msgBase) {
  sIMessage_2params * msg = (msgBase->s_size >= sizeof(sIMessage_2params)) ? static_cast<sIMessage_2params*>(msgBase) : 0;

  switch (msgBase->id) {
    case IM_PLUG_NET:        return(kAway::net);
    case IM_PLUG_TYPE:       return(IMT_UI | IMT_CONFIG | IMT_ALLMESSAGES);
    case IM_PLUG_VERSION:    return(0);
    case IM_PLUG_SDKVERSION: return(KONNEKT_SDK_V);
    case IM_PLUG_SIG:        return((int)"KAWAY");
    case IM_PLUG_CORE_V:     return((int)"W98");
    case IM_PLUG_UI_V:       return(0);
    case IM_PLUG_NAME:       return((int)"kAway");
    case IM_PLUG_NETNAME:    return(0);
    case IM_PLUG_PRIORITY:   return(PLUGP_HIGH);

    case IM_PLUG_INIT:       Plug_Init(msg->p1, msg->p2);   return(1);
    case IM_PLUG_DEINIT:     Plug_Deinit(msg->p1, msg->p2); return(1);
    case IM_SETCOLS:         return(kAway::ISetCols());
    case IM_UI_PREPARE:      return(kAway::IPrepare());
    case IM_START:           return(kAway::IStart());
    case IM_END:             return(kAway::IEnd());
    case IM_UIACTION:        return(kAway::ActionProc((sUIActionNotify_base*)msg->p1));

    default:
      if (Ctrl) Ctrl->setError(IMERROR_NORESULT);
      return(0);
  }
  return(0);
}