/*
 *  kAway2 Core
 *
 *  Please READ /License.txt FIRST!
 *
 *  Copyright (C)2005 Sijawusz Pur Rahnama
 *
 *  $Id$
 */

#include "stdafx.h"
#include "main.h"

#include "Helpers.h"

#include "NetList.h"
#include "Format.h"
#include "Status.h"
#include "AwayWnd.h"
#include "Message.h"
#include "Control.h"

#include "NetList.cpp"
#include "Format.cpp"
#include "Status.cpp"
#include "AwayWnd.cpp"
#include "Message.cpp"
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
    if (pCtrl->isEnabled()) {
      if (pCtrl->isMuteSwitched()) {
        SETINT(kSound::Cfg::mute, 0);
      }
      pCtrl->disable("", true);
    }

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
    Ctrl->SetColumn(DTCFG, cfg::mruSize, DT_CT_INT, 20, "kAway2/mruSize");
    Ctrl->SetColumn(DTCFG, cfg::muteOnEnable, DT_CT_INT, 0, "kAway2/muteOnEnable");
    Ctrl->SetColumn(DTCFG, cfg::disableConfirmation, DT_CT_INT, 1, "kAway2/disableConfirmation");

    Ctrl->SetColumn(DTCFG, cfg::btnInMainWindow, DT_CT_INT, 1, "kAway2/btnInMainWindow");
    Ctrl->SetColumn(DTCFG, cfg::btnInCntWindow, DT_CT_INT, 1, "kAway2/btnInCntWindow");
    Ctrl->SetColumn(DTCFG, cfg::btnInTrayMenu, DT_CT_INT, 1, "kAway2/btnInTrayMenu");

    Ctrl->SetColumn(DTCFG, cfg::dateFormat, DT_CT_STR, "%d/%m/%Y", "kAway2/dateFormat");
    Ctrl->SetColumn(DTCFG, cfg::timeFormat, DT_CT_STR, "%H:%M", "kAway2/timeFormat");

    Ctrl->SetColumn(DTCFG, cfg::tpl::enable, DT_CT_STR, "brb/afk {[msg]}", "kAway2/tpl/enable");
    Ctrl->SetColumn(DTCFG, cfg::tpl::disable, DT_CT_STR, "i'm back {[msg] }:>", "kAway2/tpl/disable");
    Ctrl->SetColumn(DTCFG, cfg::tpl::reply, DT_CT_STR, 
      "Hello <b>{display}</b>, i'm away from {date} @ {time} {[msg]}.\r\n"
      "Leave a message after the beep. Byeee.", "kAway2/tpl/reply");
    // Ctrl->SetColumn(DTCFG, cfg::tpl::forward, DT_CT_STR, "", "kAway2/tpl/forward");
    // Ctrl->SetColumn(DTCFG, cfg::tpl::sms, DT_CT_STR, "", "kAway2/tpl/sms");
    // Ctrl->SetColumn(DTCFG, cfg::tpl::email, DT_CT_STR, "", "kAway2/tpl/email");
    Ctrl->SetColumn(DTCFG, cfg::tpl::status, DT_CT_STR, "{status |} away {[msg]}", "kAway2/tpl/status");
    Ctrl->SetColumn(DTCFG, cfg::tpl::autoAway, DT_CT_STR, "autoAway is on, so... i'm off :>", "kAway2/tpl/autoAway");

    Ctrl->SetColumn(DTCFG, cfg::reply::onEnable, DT_CT_INT, 0, "kAway2/reply/onEnable");
    Ctrl->SetColumn(DTCFG, cfg::reply::onDisable, DT_CT_INT, 0, "kAway2/reply/onDisable");
    Ctrl->SetColumn(DTCFG, cfg::reply::onMsg, DT_CT_INT, 1, "kAway2/reply/onMsg");
    Ctrl->SetColumn(DTCFG, cfg::reply::whenInvisible, DT_CT_INT, 1, "kAway2/reply/whenInvisible");
    Ctrl->SetColumn(DTCFG, cfg::reply::showInWnd, DT_CT_INT, 1, "kAway2/reply/showInWnd");
    Ctrl->SetColumn(DTCFG, cfg::reply::minInterval, DT_CT_INT, 900, "kAway2/reply/minInterval");
    Ctrl->SetColumn(DTCFG, cfg::reply::useHtml, DT_CT_INT, 1, "kAway2/reply/useHtml");
    Ctrl->SetColumn(DTCFG, cfg::reply::netChange, DT_CT_STR, "", "kAway2/reply/netChange");

    Ctrl->SetColumn(DTCFG, cfg::status::onEnableSt, DT_CT_INT, ST_NA, "kAway2/status/onEnableSt");
    Ctrl->SetColumn(DTCFG, cfg::status::whenInvisible, DT_CT_INT, 1, "kAway2/status/whenInvisible");
    Ctrl->SetColumn(DTCFG, cfg::status::changeOnEnable, DT_CT_INT, 1, "kAway2/status/changeOnEnable");
    Ctrl->SetColumn(DTCFG, cfg::status::changeInfoOnEnable, DT_CT_INT, 1, "kAway2/status/changeInfoOnEnable");
    Ctrl->SetColumn(DTCFG, cfg::status::netChange, DT_CT_STR, "", "kAway2/status/netChange");

    Ctrl->SetColumn(DTCNT, cfg::tpl::enable, DT_CT_STR, "", "kAway2/tpl/enable");
    Ctrl->SetColumn(DTCNT, cfg::tpl::disable, DT_CT_STR, "", "kAway2/tpl/disable");
    Ctrl->SetColumn(DTCNT, cfg::tpl::reply, DT_CT_STR, "", "kAway2/tpl/reply");

    Ctrl->SetColumn(DTCNT, cfg::saveToHistory, DT_CT_INT, 0, "kAway2/saveToHistory");
    Ctrl->SetColumn(DTCNT, cfg::disableOnMsgSend, DT_CT_INT, 0, "kAway2/disableOnMsgSend");
    Ctrl->SetColumn(DTCNT, cfg::ircCmds, DT_CT_INT, 0, "kAway2/ircCmds");

    Ctrl->SetColumn(DTCNT, cfg::reply::onEnable, DT_CT_INT, 0, "kAway2/reply/onEnable");
    Ctrl->SetColumn(DTCNT, cfg::reply::onDisable, DT_CT_INT, 0, "kAway2/reply/onDisable");
    Ctrl->SetColumn(DTCNT, cfg::reply::onMsg, DT_CT_INT, 0, "kAway2/reply/onMsg");
    Ctrl->SetColumn(DTCNT, cfg::reply::whenInvisible, DT_CT_INT, 0, "kAway2/reply/whenInvisible");
    Ctrl->SetColumn(DTCNT, cfg::reply::showInWnd, DT_CT_INT, 0, "kAway2/reply/showInWnd");
    Ctrl->SetColumn(DTCNT, cfg::reply::minInterval, DT_CT_INT, -1, "kAway2/reply/minInterval");
    Ctrl->SetColumn(DTCNT, cfg::reply::useHtml, DT_CT_INT, 0, "kAway2/reply/useHtml");

    Ctrl->SetColumn(DTCFG, cfg::sms::interval, DT_CT_INT, 150, "kAway2/sms/interval");
    Ctrl->SetColumn(DTCFG, cfg::sms::gate, DT_CT_STR, "", "kAway2/sms/gate");
    Ctrl->SetColumn(DTCFG, cfg::sms::number, DT_CT_INT, 0, "kAway2/sms/number");

    return(1);
  }

  int IPrepare() {
    pCtrl = new Control();
    wCtrl = new AwayWnd("kAway2AwayWnd", "kA2_awayMsg", cfg::mruSize);

    lCtrl::status = new NetList(cfg::status::netChange, ui::statusCfgGroup, dynAct::status, 
      act::cfgGroupCheckCreate, act::cfgGroupCheckDestroy);
    lCtrl::status->loadNets();

    lCtrl::reply = new NetList(cfg::reply::netChange, ui::replyCfgGroup, dynAct::reply, 
      act::replyCfgGroupCheckCreate, act::replyCfgGroupCheckDestroy);
    lCtrl::reply->loadNets();

    sCtrl = new Status(lCtrl::status, cfg::status::whenInvisible, "status");
    pCtrl->setStatusCtrl(sCtrl);

    pCtrl->Log("net = %i, Ctrl = %i, pCtrl = %i, sCtrl = %i, wCtrl = %i", 
      net, Ctrl, pCtrl, sCtrl, wCtrl);

    /* Defining help variables */
    tHelpVars stVars, rVars;

    stVars.push_back( helpVar( "status", "Aktualny opis statusu" ) );
    stVars.push_back( helpVar( "msg", "Przyczyna nieobecnoœci" ) );
    stVars.push_back( helpVar( "date", "Data w³¹czenia trybu away" ) );
    stVars.push_back( helpVar( "time", "Czas w³¹czenia trybu away" ) );

    rVars.push_back( helpVar( "display", "Nazwa wyœwietlania kontaktu" ) );
    rVars.push_back( helpVar( "name", "Imiê przypisane do kontaktu" ) );
    rVars.push_back( helpVar( "nick", "Ksywka przypisana do kontaktu" ) );
    rVars.push_back( helpVar( "surname", "Nazwisko przypisane do kontaktu" ) );
    rVars.push_back( helpVar( "date", "Data w³¹czenia trybu away" ) );
    rVars.push_back( helpVar( "time", "Czas w³¹czenia trybu away" ) );
    rVars.push_back( helpVar( "msg", "Przyczyna nieobecnoœci/powrotu" ) );

    /* Registering icons */
    IconRegister(IML_32, ico::logoBig, Ctrl->hDll(), IDI_LOGO);
    IconRegister(IML_16, ico::logoSmall, Ctrl->hDll(), IDI_ENABLE);
    // IconRegister(IML_16, ico::logoSmall, Ctrl->hDll(), IDI_LOGO);

    IconRegister(IML_16, ico::enable, Ctrl->hDll(), IDI_ENABLE);
    IconRegister(IML_16, ico::disable, Ctrl->hDll(), IDI_DISABLE);

    IconRegister(IML_16, ico::ignore, Ctrl->hDll(), IDI_IGNORE);
    IconRegister(IML_16, ico::unIgnore, Ctrl->hDll(), IDI_UNIGNORE);

    IconRegister(IML_16, ico::status, Ctrl->hDll(), IDI_STATUS);
    IconRegister(IML_16, ico::reply, Ctrl->hDll(), IDI_REPLY);

    IconRegister(IML_16, ico::accept, Ctrl->hDll(), IDI_ACCEPT);
    IconRegister(IML_16, ico::cancel, Ctrl->hDll(), IDI_CANCEL);

    IconRegister(IML_16, ico::help, Ctrl->hDll(), IDI_HELP);
    IconRegister(IML_16, ico::trash, Ctrl->hDll(), IDI_TRASH);

    /* Adding configuration tabs */
    UIGroupInsert(IMIG_NFO, ui::cntCfgGroup, -1, ACTR_SAVE, "kAway2", ico::logoSmall);
    UIGroupInsert(IMIG_CFG_PLUGS, ui::cfgGroup, -1, ACTR_SAVE, "kAway2", ico::logoSmall);
    UIGroupInsert(ui::cfgGroup, ui::statusCfgGroup, -1, ACTR_SAVE, "Status", ico::status);
    UIGroupInsert(ui::cfgGroup, ui::replyCfgGroup, -1, ACTR_SAVE, "Autoresponder", ico::reply);

    /* Plugin info box */
    char header[400];
    sprintf(header, "<span class='note'>Powered by: <b>%s</b></span><br/>"
      "<span class='note'>Skompilowano: <b>%s</b> [<b>%s</b>]</span><br/>"
      "Informacje o wtyczce na stronie projektu "
      "<b>KPlugins</b> (http://kplugins.net/)<br/><br/>"
      "Copyright © 2004-2005 <b>Sijawusz Pur Rahnama</b><br/>"
      "Copyright © 2004-2005 <b>KPlugins Team</b>",
      poweredBy, __DATE__, __TIME__);

    char desc[300] = 
      "Wtyczka ma za zadanie zaj¹æ siê kwesti¹ Twojej nieobecnoœci przy komputerze :)<br/>"
      "Osoby pisz¹ce do Ciebie zostan¹ powiadomione (albo i nie ;)) o tym od kiedy i dlaczego Cie nie ma, "
      "ty za to dostaniesz informacjê o tym ile osób nawiedzi³o Ciê podczas Twoich godzin-bez-komputera ;>";

    char dateFormat[1024] = AP_TIPRICH
      "%<b>a</b> - Abbreviated weekday name<br/>"
      "%<b>A</b> - Full weekday name<br/>"
      "%<b>b</b> - Abbreviated month name<br/>"
      "%<b>B</b> - Full month name<br/>"
      "%<b>d</b> - Day of month as decimal number (01 – 31)<br/>"
      "%<b>j</b> - Day of year as decimal number (001 – 366)<br/>"
      "%<b>m</b> - Month as decimal number (01 – 12)<br/>"
      "%<b>M</b> - Minute as decimal number (00 – 59)<br/>"
      "%<b>U</b> - Week of year as decimal number, with Sunday as first day of week (00 – 53)<br/>"
      "%<b>w</b> - Weekday as decimal number (0 – 6; Sunday is 0)<br/>"
      "%<b>W</b> - Week of year as decimal number, with Monday as first day of week (00 – 53)<br/>"
      "%<b>x</b> - Date representation for current locale<br/>"
      "%<b>y</b> - Year without century, as decimal number (00 – 99)<br/>"
      "%<b>Y</b> - Year with century, as decimal number<br/>"
      "%<b>z</b>, %<b>Z</b> - Either the time-zone name or time zone abbreviation, depending on registry settings; "
      "no characters if time zone is unknown<br/>"
      "%<b>%</b> - Percent sign"
      AP_TIP_WIDTH "300";

    char timeFormat[400] = AP_TIPRICH
      "%<b>H</b> - Hour in 24-hour format (00 – 23)<br/>"
      "%<b>I</b> - Hour in 12-hour format (01 – 12)<br/>"
      "%<b>p</b> - Current locale's A.M./P.M. indicator for 12-hour clock<br/>"
      "%<b>S</b> - Second as decimal number (00 – 59)<br/>"
      "%<b>X</b> - Time representation for current locale<br/>"
      "%<b>%</b> - Percent sign"
      AP_TIP_WIDTH "285";

    UIActionCfgAddPluginInfoBox2(ui::cfgGroup, desc, header, Helpers::icon32(ico::logoBig).c_str(), -4);
    UIActionCfgAddPluginInfoBox2(ui::cntCfgGroup, desc, header, Helpers::icon32(ico::logoBig).c_str(), -5);

    /* Main tab */
    /* |-> General settings group */
    UIActionCfgAdd(ui::cfgGroup, 0, ACTT_GROUP, "Ustawienia");
    UIActionCfgAdd(ui::cfgGroup, 0, ACTT_CHECK, "Synchronizuj z trybem auto-away", cfg::autoAwaySync);
    UIActionCfgAdd(ui::cfgGroup, 0, ACTT_CHECK, "Wycisz przy w³¹czeniu trybu away", cfg::muteOnEnable);
    UIActionCfgAdd(ui::cfgGroup, 0, ACTT_CHECK, "Wy³¹czaj w momencie wys³ania wiadomoœci", cfg::disableOnMsgSend);
    UIActionCfgAdd(ui::cfgGroup, 0, ACTT_CHECK, "Zapisuj wiadomoœci w osobnej historii", cfg::saveToHistory);
    UIActionCfgAdd(ui::cfgGroup, 0, ACTT_CHECK, "Komendy z okna rozmów (a'la IRC)" AP_TIPRICH 
      "/<b>away</b> [...] - w³¹cza tryb away<br/>"
      "/<b>brb</b> [...] - j.w. + nie wysy³a powiadomieñ o w³.<br/>"
      "/<b>back</b> [...] - wy³¹cza tryb away<br/>"
      "/<b>re</b> [...] - j.w. + nie wysy³a powiadomieñ o wy³." AP_TIP_WIDTH "240", cfg::ircCmds);
    UIActionCfgAdd(ui::cfgGroup, 0, ACTT_CHECK, "Pokazuj powiadomienia K.Notify", cfg::useKNotify);
    UIActionCfgAdd(ui::cfgGroup, 0, ACTT_CHECK, "Pytaj przed wyjœciem z trybu away", cfg::disableConfirmation);
    UIActionCfgAdd(ui::cfgGroup, 0, ACTT_GROUPEND);

    UIActionCfgAdd(ui::cfgGroup, 0, ACTT_GROUP, "Formatowanie czasu");
    UIActionCfgAdd(ui::cfgGroup, 0, ACTT_EDIT | ACTSC_INLINE, dateFormat, cfg::dateFormat);
    UIActionCfgAdd(ui::cfgGroup, 0, ACTT_COMMENT, "Format daty");
    UIActionCfgAdd(ui::cfgGroup, 0, ACTT_EDIT | ACTSC_INLINE, timeFormat, cfg::timeFormat);
    UIActionCfgAdd(ui::cfgGroup, 0, ACTT_COMMENT, "Format godziny");
    UIActionCfgAdd(ui::cfgGroup, 0, ACTT_GROUPEND);

    UIActionCfgAdd(ui::cfgGroup, 0, ACTT_GROUP, "Powód nieobecnoœci dla auto-away'a");
    UIActionCfgAdd(ui::cfgGroup, 0, ACTT_TEXT, 0, cfg::tpl::autoAway);
    UIActionCfgAdd(ui::cfgGroup, 0, ACTT_GROUPEND);

    /* |-> Interface group */
    UIActionCfgAdd(ui::cfgGroup, 0, ACTT_GROUP, "Interfejs");
    UIActionCfgAdd(ui::cfgGroup, 0, ACTT_CHECK | ACTSC_NEEDRESTART, "Pokazuj przycisk w g³ównym oknie", cfg::btnInMainWindow);
    UIActionCfgAdd(ui::cfgGroup, 0, ACTT_CHECK | ACTSC_NEEDRESTART, "Pokazuj przycisk w oknie rozmowy", cfg::btnInCntWindow);
    UIActionCfgAdd(ui::cfgGroup, 0, ACTT_CHECK | ACTSC_NEEDRESTART, "Pokazuj przycisk w menu tray'a", cfg::btnInTrayMenu);
    UIActionCfgAdd(ui::cfgGroup, 0, ACTT_GROUPEND);

    UIActionCfgAdd(ui::cfgGroup, 0, ACTT_GROUP, "Historia powodów nieobecnoœci");
    UIActionCfgAdd(ui::cfgGroup, 0, ACTT_COMMENT, "Iloœæ pozycji w historii:");
    UIActionCfgAdd(ui::cfgGroup, 0, ACTT_SLIDER, "Ma³o\nDu¿o" AP_MINIMUM "1" AP_MAXIMUM "25", cfg::mruSize);
    UIActionCfgAdd(ui::cfgGroup, 0, ACTT_SEPARATOR);
    UIActionCfgAdd(ui::cfgGroup, act::clearMru, ACTT_BUTTON, "wyczyœæ" AP_ICO "667112", 0, 0, 0, 80, 30);
    UIActionCfgAdd(ui::cfgGroup, 0, ACTT_GROUPEND);

    /* Status tab */
    /* |-> Settings group */
    UIActionCfgAdd(ui::statusCfgGroup, 0, ACTT_GROUP, "Ustawienia");
    /*
    UIActionCfgAdd(ui::statusCfgGroup, 0, ACTT_COMBO | ACTSCOMBO_LIST | ACTSC_INLINE, 
      "Zaraz wracam" AP_ICO "#40000410" AP_VALUE "65\n"
      "Nieosi¹galny" AP_ICO "#40000210" AP_VALUE "33\n"
      "Nie przeszkadzaæ" AP_ICO "#40000220" AP_VALUE "34\n"
      "Ukryty" AP_ICO "#40000420" AP_VALUE "66", 
      cfg::status::onEnableSt);
    UIActionCfgAdd(ui::statusCfgGroup, 0, ACTT_COMMENT, "Domyœlnie ustawiony status");
    */
    UIActionCfgAdd(ui::statusCfgGroup, 0, ACTT_CHECK, "Zmieniaj status przy w³¹czonym statusie 'ukryty'", cfg::status::whenInvisible);
    UIActionCfgAdd(ui::statusCfgGroup, 0, ACTT_SEPARATOR);
    UIActionCfgAdd(ui::statusCfgGroup, 0, ACTT_CHECK, "Zmieniaj status przy w³¹czeniu trybu away", cfg::status::changeOnEnable);
    UIActionCfgAdd(ui::statusCfgGroup, 0, ACTT_CHECK, "Zmieniaj opis statusu przy w³¹czeniu trybu away", cfg::status::changeInfoOnEnable);
    UIActionCfgAdd(ui::statusCfgGroup, 0, ACTT_GROUPEND);

    UIActionCfgAdd(ui::statusCfgGroup, 0, ACTT_GROUP, "Domyœlnie zmieniaj status na:");
    UIActionCfgAdd(ui::statusCfgGroup, 0, ACTT_IMAGE | ACTSC_INLINE, Helpers::icon16(UIIcon(IT_STATUS, 0, ST_AWAY, 0)).c_str());
    UIActionCfgAdd(ui::statusCfgGroup, 0, ACTT_RADIO | ACTSC_INLINE, "Zaraz wracam" AP_VALUE "65", cfg::status::onEnableSt);
    UIActionCfgAdd(ui::statusCfgGroup, 0, ACTT_IMAGE | ACTSC_INLINE, Helpers::icon16(UIIcon(IT_STATUS, 0, ST_NA, 0)).c_str());
    UIActionCfgAdd(ui::statusCfgGroup, 0, ACTT_RADIO | ACTSC_INLINE, "Nieosi¹galny" AP_VALUE "33", cfg::status::onEnableSt);
    UIActionCfgAdd(ui::statusCfgGroup, 0, ACTT_IMAGE | ACTSC_INLINE, Helpers::icon16(UIIcon(IT_STATUS, 0, ST_DND, 0)).c_str());
    UIActionCfgAdd(ui::statusCfgGroup, 0, ACTT_RADIO | ACTSC_INLINE, "Nie przeszkadzaæ" AP_VALUE "34", cfg::status::onEnableSt);
    UIActionCfgAdd(ui::statusCfgGroup, 0, ACTT_IMAGE | ACTSC_INLINE, Helpers::icon16(UIIcon(IT_STATUS, 0, ST_HIDDEN, 0)).c_str());
    UIActionCfgAdd(ui::statusCfgGroup, 0, ACTT_RADIO | ACTSRADIO_LAST, "Ukryty" AP_VALUE "66", cfg::status::onEnableSt);
    UIActionCfgAdd(ui::statusCfgGroup, 0, ACTT_GROUPEND);

    UIActionCfgAdd(ui::statusCfgGroup, 0, ACTT_GROUP, "Szablon statusu");
    sCtrl->fCtrl->UIDrawHelpBtn(stVars, ui::statusCfgGroup, ico::help);
    UIActionCfgAdd(ui::statusCfgGroup, 0, ACTT_TEXT, 0, cfg::tpl::status);
    UIActionCfgAdd(ui::statusCfgGroup, 0, ACTT_GROUPEND);

    /* |-> Net selection group */
    UIActionCfgAdd(ui::statusCfgGroup, 0, ACTT_GROUP, "Wybierz sieci, na których chcesz zmieniaæ status:");
    lCtrl::status->UIDraw();
    UIActionCfgAdd(ui::statusCfgGroup, 0, ACTT_GROUPEND);

    /* Autoresponder tab */
    /* |-> Minimal reply interval group */
    UIActionCfgAdd(ui::replyCfgGroup, 0, ACTT_GROUP, "Minimalny interwa³ pomiêdzy wysy³anymi odpowiedziami [s]:");
    UIActionCfgAdd(ui::replyCfgGroup, 0, ACTT_SLIDER, "Ma³y\nDu¿y" AP_STEP "60" AP_MINIMUM "0" AP_MAXIMUM "3600" AP_TIPRICH 
      "<b>0</b> - odpowiedŸ bêdzie wys³ana tylko jeden raz<br/><b>3600</b> - odpowiedŸ nie zostanie ponownie "
      "wys³ana przez min. <b>1h</b> :>", cfg::reply::minInterval);
    UIActionCfgAdd(ui::replyCfgGroup, 0, ACTT_GROUPEND);

    /* |-> Net selection group */
    UIActionCfgAdd(ui::replyCfgGroup, 0, ACTT_GROUP, "Wybierz sieci, na których maj¹ dzia³aæ powiadomienia:");
    lCtrl::reply->UIDraw();
    UIActionCfgAdd(ui::replyCfgGroup, 0, ACTT_GROUPEND);

    UIActionCfgAdd(ui::replyCfgGroup, 0, ACTT_GROUP, "Wysy³anie powiadomieñ");
    UIActionCfgAdd(ui::replyCfgGroup, 0, ACTT_CHECK, "Pokazuj wys³ane powiadomienia w okienku rozmowy", cfg::reply::showInWnd);
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

    /* |-> Templates group */
    UIActionCfgAdd(ui::replyCfgGroup, 0, ACTT_GROUP, "Szablony powiadomieñ");
    UIActionCfgAdd(ui::replyCfgGroup, 0, ACTT_CHECK, "Formatuj kod HTML" AP_TIP 
      "Jeœli w³¹czone, znaki specjalne nale¿y zamieniaæ na ich HTML-owe odpowiedniki (np.: (< na &lt;), (> na &gt;))", cfg::reply::useHtml);
    UIActionCfgAdd(ui::replyCfgGroup, 0, ACTT_SEPARATOR);

    UIActionCfgAdd(ui::replyCfgGroup, 0, ACTT_COMMENT, "OdpowiedŸ:");
    sCtrl->fCtrl->UIDrawHelpBtn(rVars, ui::replyCfgGroup, ico::help);
    UIActionCfgAdd(ui::replyCfgGroup, 0, ACTT_TEXT, 0, cfg::tpl::reply);

    UIActionCfgAdd(ui::replyCfgGroup, 0, ACTT_COMMENT, "W³¹czenie trybu away:");
    sCtrl->fCtrl->UIDrawHelpBtn(rVars, ui::replyCfgGroup, ico::help);
    UIActionCfgAdd(ui::replyCfgGroup, 0, ACTT_TEXT, 0, cfg::tpl::enable);

    UIActionCfgAdd(ui::replyCfgGroup, 0, ACTT_COMMENT, "Wy³¹czenie trybu away:");
    sCtrl->fCtrl->UIDrawHelpBtn(rVars, ui::replyCfgGroup, ico::help);
    UIActionCfgAdd(ui::replyCfgGroup, 0, ACTT_TEXT, 0, cfg::tpl::disable);
    UIActionCfgAdd(ui::replyCfgGroup, 0, ACTT_GROUPEND);

    /* Main tab in contact window */
    /* |-> Minimal reply interval group */
    UIActionCfgAdd(ui::cntCfgGroup, 0, ACTT_GROUP, "Minimalny interwa³ pomiêdzy wysy³anymi odpowiedziami [s]:");
    UIActionCfgAdd(ui::cntCfgGroup, 0, ACTT_SLIDER, "Ma³y\nDu¿y" AP_STEP "60" AP_MINIMUM "-1" AP_MAXIMUM "3600" AP_TIPRICH 
      "<b>-1</b> - pobierze wartoœæ domyœln¹ z konfiguracji<br/><b>0</b> - odpowiedŸ bêdzie wys³ana tylko jeden raz<br/>"
      "<b>3600</b> - odpowiedŸ nie zostanie ponownie wys³ana przez min. <b>1h</b> :>", cfg::reply::minInterval);
    UIActionCfgAdd(ui::cntCfgGroup, 0, ACTT_GROUPEND);

    UIActionCfgAdd(ui::cntCfgGroup, 0, ACTT_GROUP, "Ustawienia");
    UIActionCfgAdd(ui::cntCfgGroup, 0, ACTT_COMBO | ACTSCOMBO_LIST | ACTSCOMBO_BYPOS | ACTSC_INLINE, 
      "Domyœlnie" CFGICO "#1B\nTak" CFGICO "#A2DE5\nNie" CFGICO "#A2DE6", cfg::reply::showInWnd);
    UIActionCfgAdd(ui::cntCfgGroup, 0, ACTT_COMMENT, "Pokazuj wys³ane powiadomienia w okienku rozmowy");

    UIActionCfgAdd(ui::cntCfgGroup, 0, ACTT_COMBO | ACTSCOMBO_LIST | ACTSCOMBO_BYPOS | ACTSC_INLINE, 
      "Domyœlnie" CFGICO "#1B\nTak" CFGICO "#A2DE5\nNie" CFGICO "#A2DE6", cfg::saveToHistory);
    UIActionCfgAdd(ui::cntCfgGroup, 0, ACTT_COMMENT, "Zapisuj wiadomoœci w osobnej historii");

    UIActionCfgAdd(ui::cntCfgGroup, 0, ACTT_COMBO | ACTSCOMBO_LIST | ACTSCOMBO_BYPOS | ACTSC_INLINE, 
      "Domyœlnie" CFGICO "#1B\nTak" CFGICO "#A2DE5\nNie" CFGICO "#A2DE6", cfg::disableOnMsgSend);
    UIActionCfgAdd(ui::cntCfgGroup, 0, ACTT_COMMENT, "Wy³¹czaj w momencie wys³ania wiadomoœci");

    UIActionCfgAdd(ui::cntCfgGroup, 0, ACTT_COMBO | ACTSCOMBO_LIST | ACTSCOMBO_BYPOS | ACTSC_INLINE, 
      "Domyœlnie" CFGICO "#1B\nTak" CFGICO "#A2DE5\nNie" CFGICO "#A2DE6", cfg::ircCmds);
    UIActionCfgAdd(ui::cntCfgGroup, 0, ACTT_COMMENT, "Komendy z okna rozmów (a'la IRC)");
    UIActionCfgAdd(ui::cntCfgGroup, 0, ACTT_GROUPEND);

    UIActionCfgAdd(ui::cntCfgGroup, 0, ACTT_GROUP, "Wysy³anie powiadomieñ");
    UIActionCfgAdd(ui::cntCfgGroup, 0, ACTT_COMBO | ACTSCOMBO_LIST | ACTSCOMBO_BYPOS | ACTSC_INLINE, 
      "Domyœlnie" CFGICO "#1B\nTak" CFGICO "#A2DE5\nNie" CFGICO "#A2DE6", cfg::reply::whenInvisible);
    UIActionCfgAdd(ui::cntCfgGroup, 0, ACTT_COMMENT, "Wysy³aj powiadomienia przy w³¹czonym statusie 'ukryty'");

    UIActionCfgAdd(ui::cntCfgGroup, 0, ACTT_SEPARATOR);
    UIActionCfgAdd(ui::cntCfgGroup, 0, ACTT_COMBO | ACTSCOMBO_LIST | ACTSCOMBO_BYPOS | ACTSC_INLINE, 
      "Domyœlnie" CFGICO "#1B\nTak" CFGICO "#A2DE5\nNie" CFGICO "#A2DE6", cfg::reply::onMsg);
    UIActionCfgAdd(ui::cntCfgGroup, 0, ACTT_COMMENT, "Odpowiadaj na odebrane wiadomoœci");

    UIActionCfgAdd(ui::cntCfgGroup, 0, ACTT_COMBO | ACTSCOMBO_LIST | ACTSCOMBO_BYPOS | ACTSC_INLINE, 
      "Domyœlnie" CFGICO "#1B\nTak" CFGICO "#A2DE5\nNie" CFGICO "#A2DE6", cfg::reply::onEnable);
    UIActionCfgAdd(ui::cntCfgGroup, 0, ACTT_COMMENT, "Wysy³aj powiadomienie o w³¹czeniu trybu away");

    UIActionCfgAdd(ui::cntCfgGroup, 0, ACTT_COMBO | ACTSCOMBO_LIST | ACTSCOMBO_BYPOS | ACTSC_INLINE, 
      "Domyœlnie" CFGICO "#1B\nTak" CFGICO "#A2DE5\nNie" CFGICO "#A2DE6", cfg::reply::onDisable);
    UIActionCfgAdd(ui::cntCfgGroup, 0, ACTT_COMMENT, "Wysy³aj powiadomienie o wy³¹czeniu trybu away");
    UIActionCfgAdd(ui::cntCfgGroup, 0, ACTT_GROUPEND);

    /* |-> Templates group */
    UIActionCfgAdd(ui::cntCfgGroup, 0, ACTT_GROUP, "Szablony powiadomieñ");
    UIActionCfgAdd(ui::cntCfgGroup, 0, ACTT_COMBO | ACTSCOMBO_LIST | ACTSCOMBO_BYPOS | ACTSC_INLINE, 
      "Domyœlnie" CFGICO "#1B\nTak" CFGICO "#A2DE5\nNie" CFGICO "#A2DE6", cfg::reply::useHtml);
    UIActionCfgAdd(ui::cntCfgGroup, 0, ACTT_COMMENT, "Formatuj kod HTML" AP_TIP 
      "Jeœli w³¹czone, znaki specjalne nale¿y zamieniaæ na ich HTML-owe odpowiedniki (np.: (< na &lt;), (> na &gt;))");
    UIActionCfgAdd(ui::cntCfgGroup, 0, ACTT_SEPARATOR);

    UIActionCfgAdd(ui::cntCfgGroup, 0, ACTT_COMMENT, "OdpowiedŸ:");
    sCtrl->fCtrl->UIDrawHelpBtn(rVars, ui::cntCfgGroup, ico::help);
    UIActionCfgAdd(ui::cntCfgGroup, 0, ACTT_TEXT, 0, cfg::tpl::reply);

    UIActionCfgAdd(ui::cntCfgGroup, 0, ACTT_COMMENT, "W³¹czenie trybu away:");
    sCtrl->fCtrl->UIDrawHelpBtn(rVars, ui::cntCfgGroup, ico::help);
    UIActionCfgAdd(ui::cntCfgGroup, 0, ACTT_TEXT, 0, cfg::tpl::enable);

    UIActionCfgAdd(ui::cntCfgGroup, 0, ACTT_COMMENT, "Wy³¹czenie trybu away:");
    sCtrl->fCtrl->UIDrawHelpBtn(rVars, ui::cntCfgGroup, ico::help);
    UIActionCfgAdd(ui::cntCfgGroup, 0, ACTT_TEXT, 0, cfg::tpl::disable);
    UIActionCfgAdd(ui::cntCfgGroup, 0, ACTT_GROUPEND);

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

  ActionProc(sUIActionNotify_base *anBase) {
    sUIActionNotify_2params *an = static_cast<sUIActionNotify_2params*>(anBase);

    pCtrl->Debug("[ActionProc()]: anBase->act.id = %i, an->code = %i", anBase->act.id, an->code);

    lCtrl::status->actionHandle(anBase->act.id, an->code);
    lCtrl::reply->actionHandle(anBase->act.id, an->code);

    switch (anBase->act.id) {
      case ui::powerInMainWnd:
      case ui::powerInCntWnd:
      case ui::powerInTrayMenu:
        if (an->code != ACTN_ACTION) break;
        if (pCtrl->isEnabled()) {
          if (!GETINT(cfg::disableConfirmation) || Ctrl->ICMessage(IMI_CONFIRM, (int) "Na pewno chcesz wy³¹czyæ tryb away?")) 
            pCtrl->disable();
        } else {
          wCtrl->show("Podaj powód nieobecnoœci", "Podaj przyczynê swojej nieobecnoœci");
        }
        break;
      case act::clearMru:
        if (an->code != ACTN_ACTION) break;
        Helpers::clearMru("kA2_awayMsg");
        break;
      case act::resetCntSettings:
        if (an->code != ACTN_ACTION) break;

        int count = IMessage(IMC_CNT_COUNT);
        for (int i = 0; i < count; i++) {
          SETCNTI(i, cfg::reply::onEnable, 0);
          SETCNTI(i, cfg::reply::onDisable, 0);
          SETCNTI(i, cfg::reply::onMsg, 0);
          SETCNTI(i, cfg::reply::whenInvisible, 0);
          SETCNTI(i, cfg::reply::minInterval, -1);
          SETCNTI(i, cfg::reply::useHtml, 0);
          SETCNTI(i, cfg::reply::showInWnd, 0);

          SETCNTC(i, cfg::tpl::enable, "");
          SETCNTC(i, cfg::tpl::disable, "");
          SETCNTC(i, cfg::tpl::reply, "");
        }
        break;
    }
    return(0);
  }
}

int __stdcall IMessageProc(sIMessage_base *msgBase) {
  sIMessage_2params *msg = static_cast<sIMessage_2params*>(msgBase);

  switch (msgBase->id) {
    case IM_PLUG_NET:        return(net);
    case IM_PLUG_TYPE:       return(IMT_UI | IMT_CONFIG | IMT_ALLMESSAGES);
    case IM_PLUG_VERSION:    return(0);
    case IM_PLUG_SDKVERSION: return(KONNEKT_SDK_V);
    case IM_PLUG_SIG:        return((int)"KAWAY2");
    case IM_PLUG_CORE_V:     return((int)"W98");
    case IM_PLUG_UI_V:       return(0);
    case IM_PLUG_NAME:       return((int)"kAway2");
    case IM_PLUG_NETNAME:    return(0);
    case IM_PLUG_PRIORITY:   return(PLUGP_HIGHEST);
    case IM_PLUG_INIT:       Plug_Init(msg->p1, msg->p2);   return(1);
    case IM_PLUG_DEINIT:     Plug_Deinit(msg->p1, msg->p2); return(1);
    case IM_SETCOLS:         return(ISetCols());
    case IM_UI_PREPARE:      return(IPrepare());
    case IM_START:           return(IStart());
    case IM_END:             return(IEnd());
    case IM_UIACTION:        return(ActionProc((sUIActionNotify_base*)msg->p1));

    case IM_STATUSCHANGE: {
      if (pCtrl->isEnabled() && !pCtrl->isAutoAway()) sCtrl->actionHandle(msgBase);
    }

    case IM_ALLPLUGINSINITIALIZED: {
      if (true) {
        int oldId = Ctrl->ICMessage(IMC_FINDPLUG, plugsNET::kaway, IMT_ALL);

        if (oldId) {
          Ctrl->IMessage(&sIMessage_plugOut(oldId,  "kAway2 jest nastêpc¹ wtyczki K.Away :)",
            sIMessage_plugOut::erNo, sIMessage_plugOut::euNowAndOnNextStart));
          return(-1);
        }
      }
      return(1);
    }

    case IM_AWAY: {
      if (!pCtrl->isEnabled() && GETINT(cfg::autoAwaySync)) {
        pCtrl->setAutoAway(true);
        pCtrl->enable(GETSTRA(cfg::tpl::autoAway), -1, true);
      }
      return(0);
    }

    case IM_BACK: {
      if (pCtrl->isEnabled() && GETINT(cfg::autoAwaySync) && pCtrl->isAutoAway()) {
        pCtrl->setAutoAway(false);
        pCtrl->disable("", true);
      }
      return(0);
    }

    case IM_MSG_RCV: {
      cMessage *m = (cMessage*) msg->p1;

      if (m->type != MT_MESSAGE)
        return(0);

      // we're searchin' for contact id
      int cnt = Ctrl->ICMessage(IMC_CNT_FIND, m->net, (int) (strlen(m->fromUid) ? m->fromUid : m->toUid));

      if (strlen(m->fromUid) && pCtrl->isEnabled()) {
        bool firstTime = pCtrl->msgRcvTimes.find(cnt) == pCtrl->msgRcvTimes.end();

        if (pCtrl->isIgnoredUid(cnt))
          return(0);

        if (Helpers::altCfgVal(cnt, cfg::reply::onMsg)) {
          int interval = Helpers::altCfgVal(cnt, cfg::reply::minInterval, false);

          if ((!interval && firstTime) || interval && ((interval + pCtrl->msgRcvTimes[cnt]) < m->time)) {
            pCtrl->msgRcvTimes[cnt] = m->time;
            pCtrl->sendMsgTpl(cnt, cfg::tpl::reply);
          }
        }

        if (Helpers::altCfgVal(cnt, cfg::saveToHistory)) {
          std::string fileName = pCtrl->getAwayTime()->strftime("%d %B %Y, %A");
          sHISTORYADD item;

          item.m = m;
          item.dir = "kAway2";
          item.name = fileName.c_str();
          item.session = firstTime ? 0 : 1;

          Ctrl->ICMessage(IMI_HISTORY_ADD, (int) &item);
        }
      } else if (!strlen(m->fromUid) && Helpers::altCfgVal(cnt, cfg::ircCmds) &&
        !GetExtParam(m->ext, "kA2AutoMsg").length() && m->body[0] == '/') {

        if (strlen(m->body) == 1)
          return(0);

        Stamina::tStringVector params;
        std::string body(m->body); 
        body = body.substr(1, body.length());

        Stamina::split(body, " ", params);
        std::string cmd = params[0];
        std::string msg = (params.size() > 1) ? body.substr(params[0].length() + 1, body.length()) : "";
        bool del = false;

        if (cmd == "away") {
          del = true; pCtrl->enable(msg);
        } else if (cmd == "brb") {
          del = true; pCtrl->enable(msg, -1, true);
        } else if (cmd == "back") {
          del = true; pCtrl->disable(msg);
        } else if (cmd == "re") {
          del = true; pCtrl->disable(msg, true);
        }

        if (del) {
          pCtrl->Debug("{IM_MSG_RCV}: cmd = %s, msg = %s, params = %i",
            cmd.c_str(), (msg.length() ? msg.c_str() : "(none)"), params.size());

          return(IM_MSG_delete);
        }
      } else if (!strlen(m->fromUid) && Helpers::altCfgVal(cnt, cfg::disableOnMsgSend) && pCtrl->isEnabled()) {
        if (!GetExtParam(m->ext, "kA2AutoMsg").length()) pCtrl->disable();
      }
      return(0);
    }

    default:
      if (Ctrl) Ctrl->setError(IMERROR_NORESULT);
      return(0);
  }
  return(0);
}