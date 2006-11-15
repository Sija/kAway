/**
  *  kAway2 Controller class
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

#include "stdafx.h"
#include "Controller.h"

//#include "Forwarders/CntForwarder.h"
//#include "Forwarders/SMSForwarder.h"

namespace kAway2 {
  Controller::Controller() : isOn(false), muteStateSwitched(false), autoAway(typeDisabled), pluginsGroup(0) {
    /* Static values like net, type or version */
    this->addStaticValue(IM_PLUG_TYPE, IMT_UI | IMT_CONFIG | IMT_ALLMESSAGES);
    this->addStaticValue(IM_PLUG_PRIORITY, PLUGP_HIGHEST);
    this->addStaticValue(IM_PLUG_NAME, (int) "kAway2");
    this->addStaticValue(IM_PLUG_SIG, (int) sig);
    this->addStaticValue(IM_PLUG_NET, net);

    /* Callbacks */
    this->registerObserver(IM_UI_PREPARE, bind(resolve_cast0(&Controller::onPrepare), this));
    this->registerObserver(IM_UIACTION, bind(resolve_cast0(&Controller::onAction), this));
    this->registerObserver(IM_MSG_RCV, bind(resolve_cast0(&Controller::onMsgRcv), this));
    this->registerObserver(IM_BEFOREEND, bind(resolve_cast0(&Controller::onEnd), this));
    this->registerObserver(IM_STATUSCHANGE, bind(resolve_cast0(&Controller::onStatusChange), this));
    this->registerObserver(IM_ALLPLUGINSINITIALIZED, bind(resolve_cast0(&Controller::onPluginsLoaded), this));
    this->registerObserver(IM_AWAY, bind(resolve_cast0(&Controller::onAutoAway), this));
    this->registerObserver(IM_BACK, bind(resolve_cast0(&Controller::onBack), this));

    /* API callbacks */
    this->registerObserver(api::isEnabled, bind(resolve_cast0(&Controller::apiEnabled), this));
    this->registerObserver(api::enable, bind(resolve_cast0(&Controller::apiEnable), this));
    this->registerObserver(api::disable, bind(resolve_cast0(&Controller::apiDisable), this));
    this->registerObserver(api::isIgnored, bind(resolve_cast0(&Controller::apiIgnored), this));
    this->registerObserver(api::isAutoAway, bind(resolve_cast0(&Controller::apiAutoAway), this));
    this->registerObserver(api::ignore, bind(resolve_cast0(&Controller::apiIgnore), this));
    this->registerObserver(api::showAwayWnd, bind(resolve_cast0(&Controller::apiShowAwayWnd), this));

    /* Configuration columns */
    this->setColumn(DTCFG, cfg::autoAwaySync, DT_CT_INT, syncExtended, "kAway2/autoAwaySync");
    this->setColumn(DTCFG, cfg::useKNotify, DT_CT_INT, 1, "kAway2/useKNotify");
    this->setColumn(DTCFG, cfg::ircCmds, DT_CT_INT, 1, "kAway2/ircCmds");
    this->setColumn(DTCFG, cfg::disableOnMsgSend, DT_CT_INT, 0, "kAway2/disableOnMsgSend");
    this->setColumn(DTCFG, cfg::saveToHistory, DT_CT_INT, 1, "kAway2/saveToHistory");
    this->setColumn(DTCFG, cfg::mruSize, DT_CT_INT, 20, "kAway2/mruSize");
    this->setColumn(DTCFG, cfg::muteOnEnable, DT_CT_INT, 0, "kAway2/muteOnEnable");
    this->setColumn(DTCFG, cfg::disableConfirmation, DT_CT_INT, 1, "kAway2/disableConfirmation");

    this->setColumn(DTCFG, cfg::btnInMainWindow, DT_CT_INT, 1, "kAway2/btnInMainWindow");
    this->setColumn(DTCFG, cfg::btnInCntWindow, DT_CT_INT, 1, "kAway2/btnInCntWindow");
    this->setColumn(DTCFG, cfg::btnInTrayMenu, DT_CT_INT, 1, "kAway2/btnInTrayMenu");

    this->setColumn(DTCFG, cfg::dateFormat, DT_CT_STR, "%d/%m/%Y", "kAway2/dateFormat");
    this->setColumn(DTCFG, cfg::timeFormat, DT_CT_STR, "%H:%M", "kAway2/timeFormat");

    this->setColumn(DTCFG, cfg::wnd::changeInfoOnEnable, DT_CT_INT, 1, "kAway2/wnd/changeInfoOnEnable");
    this->setColumn(DTCFG, cfg::wnd::changeOnEnable, DT_CT_INT, 1, "kAway2/wnd/changeOnEnable");
    this->setColumn(DTCFG, cfg::wnd::muteOnEnable, DT_CT_INT, 0, "kAway2/wnd/muteOnEnable");
    this->setColumn(DTCFG, cfg::wnd::onEnableSt, DT_CT_INT, ST_NA, "kAway2/wnd/onEnableSt");

    this->setColumn(DTCFG, cfg::summary::interval, DT_CT_INT, 60, "kAway2/summary/interval");
    this->setColumn(DTCFG, cfg::summary::inAutoAway, DT_CT_INT, 0, "kAway2/summary/inAutoAway");
    this->setColumn(DTCFG, cfg::summary::minMsgCount, DT_CT_INT, 0, "kAway2/summary/minMsgCount");

    this->setColumn(DTCFG, cfg::fwd::inAutoAway, DT_CT_INT, 0, "kAway2/fwd/inAutoAway");

    this->setColumn(DTCFG, cfg::tpl::enable, DT_CT_STR, "brb/afk {[msg]}", "kAway2/tpl/enable");
    this->setColumn(DTCFG, cfg::tpl::disable, DT_CT_STR, "i'm back {[msg] }:>", "kAway2/tpl/disable");
    this->setColumn(DTCFG, cfg::tpl::reply, DT_CT_STR, 
      "Hello <b>{display|uid}</b>, i'm away from {date, }<b>{time}</b> {[msg]}.\r\n"
      "Leave a message after the beep. Byeee.", "kAway2/tpl/reply");
    this->setColumn(DTCFG, cfg::tpl::status, DT_CT_STR, "{status |} away {[msg|time]}", "kAway2/tpl/status");
    this->setColumn(DTCFG, cfg::tpl::autoAway, DT_CT_STR, "autoAway is on, so... i'm off :>", "kAway2/tpl/autoAway");

    this->setColumn(DTCFG, cfg::reply::onEnable, DT_CT_INT, 0, "kAway2/reply/onEnable");
    this->setColumn(DTCFG, cfg::reply::onDisable, DT_CT_INT, 0, "kAway2/reply/onDisable");
    this->setColumn(DTCFG, cfg::reply::onMsg, DT_CT_INT, 1, "kAway2/reply/onMsg");
    this->setColumn(DTCFG, cfg::reply::whenInvisible, DT_CT_INT, 0, "kAway2/reply/whenInvisible");
    this->setColumn(DTCFG, cfg::reply::showInWnd, DT_CT_INT, 1, "kAway2/reply/showInWnd");
    this->setColumn(DTCFG, cfg::reply::minInterval, DT_CT_INT, 900, "kAway2/reply/minInterval");
    this->setColumn(DTCFG, cfg::reply::minIntervalType, DT_CT_INT, intervalTypeBoth, "kAway2/reply/minIntervalType");
    this->setColumn(DTCFG, cfg::reply::useHtml, DT_CT_INT, 1, "kAway2/reply/useHtml");
    this->setColumn(DTCFG, cfg::reply::netChange, DT_CT_STR, "", "kAway2/reply/netChange");

    this->setColumn(DTCFG, cfg::status::onEnableSt, DT_CT_INT, ST_NA, "kAway2/status/onEnableSt");
    this->setColumn(DTCFG, cfg::status::onAutoAwaySt, DT_CT_INT, ST_AWAY, "kAway2/status/onAutoAwaySt");
    this->setColumn(DTCFG, cfg::status::whenInvisible, DT_CT_INT, 0, "kAway2/status/whenInvisible");
    this->setColumn(DTCFG, cfg::status::changeOnEnable, DT_CT_INT, 1, "kAway2/status/changeOnEnable");
    this->setColumn(DTCFG, cfg::status::changeInfoOnEnable, DT_CT_INT, 1, "kAway2/status/changeInfoOnEnable");
    this->setColumn(DTCFG, cfg::status::netChange, DT_CT_STR, "", "kAway2/status/netChange");
    this->setColumn(DTCFG, cfg::status::dotsAppend, DT_CT_INT, 1, "kAway2/status/dotsAppend");
    this->setColumn(DTCFG, cfg::status::chgOnlyIfOnline, DT_CT_INT, 1, "kAway2/status/chgOnlyIfOnline");

    this->setColumn(DTCFG, cfg::extAutoAway::status, DT_CT_INT, ST_NA, "kAway2/extAutoAway/status");
    this->setColumn(DTCFG, cfg::extAutoAway::time, DT_CT_INT, 1800, "kAway2/extAutoAway/time");

    this->setColumn(DTCNT, cfg::tpl::enable, DT_CT_STR, "", "kAway2/tpl/enable");
    this->setColumn(DTCNT, cfg::tpl::disable, DT_CT_STR, "", "kAway2/tpl/disable");
    this->setColumn(DTCNT, cfg::tpl::reply, DT_CT_STR, "", "kAway2/tpl/reply");

    this->setColumn(DTCNT, cfg::saveToHistory, DT_CT_INT, 0, "kAway2/saveToHistory");
    this->setColumn(DTCNT, cfg::disableOnMsgSend, DT_CT_INT, 0, "kAway2/disableOnMsgSend");
    this->setColumn(DTCNT, cfg::ircCmds, DT_CT_INT, 0, "kAway2/ircCmds");

    this->setColumn(DTCNT, cfg::reply::onEnable, DT_CT_INT, 0, "kAway2/reply/onEnable");
    this->setColumn(DTCNT, cfg::reply::onDisable, DT_CT_INT, 0, "kAway2/reply/onDisable");
    this->setColumn(DTCNT, cfg::reply::onMsg, DT_CT_INT, 0, "kAway2/reply/onMsg");
    this->setColumn(DTCNT, cfg::reply::whenInvisible, DT_CT_INT, 0, "kAway2/reply/whenInvisible");
    this->setColumn(DTCNT, cfg::reply::showInWnd, DT_CT_INT, 0, "kAway2/reply/showInWnd");
    this->setColumn(DTCNT, cfg::reply::minInterval, DT_CT_INT, -1, "kAway2/reply/minInterval");
    this->setColumn(DTCNT, cfg::reply::minIntervalType, DT_CT_INT, -1, "kAway2/reply/minIntervalType");
    this->setColumn(DTCNT, cfg::reply::useHtml, DT_CT_INT, 0, "kAway2/reply/useHtml");

    // binding method for extended away
    this->extAutoAwayTimer.reset(timerTmplCreate(bind(&Controller::onExtAutoAway, this)));
  }

  /* IMessage callback methods */

  void Controller::onPrepare() {
    this->pluginsGroup = Helpers::getPluginsGroup();

    this->mruList = new MRU(cfg::mruName, cfg::mruSize, true);
    this->wnd = new AwayWnd();

    this->statusList = new NetList(cfg::status::netChange, ui::statusCfgGroup, dynAct::status, 
      act::cfgGroupCheckCreate, act::cfgGroupCheckDestroy);
    statusList->addIgnored(plugsNET::konnferencja);
    statusList->addIgnored(plugsNET::klan);
    statusList->addIgnored(plugsNET::checky);
    statusList->addIgnored(plugsNET::actio);
    statusList->loadNets();

    this->autoReplyList = new NetList(cfg::reply::netChange, ui::replyCfgGroup, dynAct::reply, 
      act::replyCfgGroupCheckCreate, act::replyCfgGroupCheckDestroy);
    autoReplyList->addIgnored(plugsNET::klan);
    autoReplyList->addIgnored(plugsNET::checky);
    autoReplyList->addIgnored(plugsNET::actio);
    autoReplyList->loadNets();

    this->statusCtrl = new FormattedStatus(statusList, cfg::status::whenInvisible, cfg::status::dotsAppend, "status");
    statusCtrl->addReplacementSt(plugsNET::gg, ST_CHAT, ST_ONLINE);
    statusCtrl->addReplacementSt(plugsNET::gg, ST_DND, ST_AWAY);
    statusCtrl->addReplacementSt(plugsNET::gg, ST_NA, ST_AWAY);

    log("[Controller<%i>::onPrepare()]: Ctrl = %i, sCtrl = %i, wnd = %i", 
      this, Ctrl, statusCtrl, wnd);

    /* Defining help variables */
    Format::tHelpVars stVars, rVars;

    stVars.push_back(Format::sHelpVar("status", "Aktualny opis statusu"));
    stVars.push_back(Format::sHelpVar("msg", "Przyczyna nieobecno�ci"));
    stVars.push_back(Format::sHelpVar("date", "Data w��czenia trybu away"));
    stVars.push_back(Format::sHelpVar("time", "Czas w��czenia trybu away"));

    rVars.push_back(Format::sHelpVar("uid", "Identyfikator kontaktu w sieci"));
    rVars.push_back(Format::sHelpVar("display", "Nazwa wy�wietlania kontaktu"));
    rVars.push_back(Format::sHelpVar("name", "Imi� przypisane do kontaktu"));
    rVars.push_back(Format::sHelpVar("nick", "Ksywka przypisana do kontaktu"));
    rVars.push_back(Format::sHelpVar("surname", "Nazwisko przypisane do kontaktu"));
    rVars.push_back(Format::sHelpVar("date", "Data w��czenia trybu away"));
    rVars.push_back(Format::sHelpVar("time", "Czas w��czenia trybu away"));
    rVars.push_back(Format::sHelpVar("msg", "Przyczyna nieobecno�ci/powrotu"));

    /* Registering icons */
    IconRegister(IML_32, ico::logoBig, Ctrl->hDll(), IDI_LOGO);
    // IconRegister(IML_16, ico::logoSmall, Ctrl->hDll(), IDI_LOGO);
    IconRegister(IML_16, ico::logoSmall, Ctrl->hDll(), IDI_ENABLE);

    IconRegister(IML_16, ico::enable, Ctrl->hDll(), IDI_ENABLE);
    IconRegister(IML_16, ico::disable, Ctrl->hDll(), IDI_DISABLE);

    IconRegister(IML_16, ico::ignore, Ctrl->hDll(), IDI_IGNORE);
    IconRegister(IML_16, ico::unIgnore, Ctrl->hDll(), IDI_UNIGNORE);

    IconRegister(IML_16, ico::status, Ctrl->hDll(), IDI_STATUS);
    IconRegister(IML_16, ico::reply, Ctrl->hDll(), IDI_REPLY);

    IconRegister(IML_16, ico::accept, Ctrl->hDll(), IDI_ACCEPT);
    IconRegister(IML_16, ico::cancel, Ctrl->hDll(), IDI_CANCEL);

    // IconRegister(IML_16, ico::help, Ctrl->hDll(), IDI_HELP);
    IconRegister(IML_16, ico::trash, Ctrl->hDll(), IDI_TRASH);
    IconRegister(IML_16, ico::msg, Ctrl->hDll(), IDI_MSG);

    /* Adding configuration tabs */
    UIGroupAdd(IMIG_NFO, ui::cntCfgGroup, ACTR_SAVE | ACTR_INIT, "kAway2", ico::logoSmall);
    UIGroupAdd(IMIG_CFG_PLUGS, ui::cfgGroup, ACTR_SAVE, "kAway2", ico::logoSmall);
    UIGroupAdd(ui::cfgGroup, ui::replyCfgGroup, ACTR_SAVE, "Autoresponder", ico::reply);
    UIGroupAdd(ui::cfgGroup, ui::statusCfgGroup, ACTR_SAVE, "Opcje statusu", ico::status);

    /* Plugin info box */
    char header[400];
    sprintf(header, "<span class='note'>Powered by: <b>%s</b></span><br/>"
      "<span class='note'>Skompilowano: <b>%s</b> [<b>%s</b>]</span><br/>"
      "Informacje o wtyczce na stronie projektu "
      "<b>KPlugins</b> (http://kplugins.net/)<br/><br/>"
      "Copyright � 2004-2006 <b>Sijawusz Pur Rahnama</b><br/>"
      "Copyright � 2004-2006 <b>KPlugins Team</b>",
      poweredBy, __DATE__, __TIME__);

    char desc[] = 
      "Wtyczka ma za zadanie zaj�� si� kwesti� Twojej nieobecno�ci przy komputerze :)<br/>"
      "Osoby pisz�ce do Ciebie zostan� powiadomione (albo i nie ;)) o tym od kiedy i dlaczego Cie nie ma, "
      "ty za to dostaniesz informacj� o tym ile os�b nawiedzi�o Ci� podczas Twoich godzin-bez-komputera ;>";

    char dateFormat[] = AP_TIPRICH
      "%<b>a</b> - Abbreviated weekday name<br/>"
      "%<b>A</b> - Full weekday name<br/>"
      "%<b>b</b> - Abbreviated month name<br/>"
      "%<b>B</b> - Full month name<br/>"
      "%<b>d</b> - Day of month as decimal number (01 � 31)<br/>"
      "%<b>j</b> - Day of year as decimal number (001 � 366)<br/>"
      "%<b>m</b> - Month as decimal number (01 � 12)<br/>"
      "%<b>U</b> - Week of year as decimal number, with Sunday as first day of week (00 � 53)<br/>"
      "%<b>w</b> - Weekday as decimal number (0 � 6; Sunday is 0)<br/>"
      "%<b>W</b> - Week of year as decimal number, with Monday as first day of week (00 � 53)<br/>"
      "%<b>x</b> - Date representation for current locale<br/>"
      "%<b>y</b> - Year without century, as decimal number (00 � 99)<br/>"
      "%<b>Y</b> - Year with century, as decimal number<br/>"
      "%<b>z</b>, %<b>Z</b> - Either the time-zone name or time zone abbreviation, depending on registry settings; "
      "no characters if time zone is unknown<br/>"
      "%<b>%</b> - Percent sign"
      AP_TIP_WIDTH "300";

    char timeFormat[] = AP_TIPRICH
      "%<b>H</b> - Hour in 24-hour format (00 � 23)<br/>"
      "%<b>I</b> - Hour in 12-hour format (01 � 12)<br/>"
      "%<b>M</b> - Minute as decimal number (00 � 59)<br/>"
      "%<b>p</b> - Current locale's A.M./P.M. indicator for 12-hour clock<br/>"
      "%<b>S</b> - Second as decimal number (00 � 59)<br/>"
      "%<b>X</b> - Time representation for current locale<br/>"
      "%<b>%</b> - Percent sign"
      AP_TIP_WIDTH "285";

    UIActionCfgAddPluginInfoBox2(ui::cfgGroup, desc, header, Helpers::icon16(ico::logoSmall).a_str(), -4);
    UIActionCfgAddPluginInfoBox2(ui::cntCfgGroup, desc, header, Helpers::icon16(ico::logoSmall).a_str(), -4);

    /* User tab */
    ifINT {
      UIActionCfgAdd(43, 0, ACTT_GROUP, "Tryb auto-away (podstawowy)");
      UIActionCfgAdd(43, 0, ACTT_SEPARATOR, "Zmieniaj status na:");
      UIActionCfgAdd(43, 0, ACTT_CHECK, "Tylko przy statusach 'dost�pny' i 'pogadam'", cfg::status::chgOnlyIfOnline);
      UIActionCfgAdd(43, 0, ACTT_IMAGE | ACTSC_INLINE, Helpers::icon16(UIIcon(IT_STATUS, 0, ST_AWAY, 0)).a_str());
      UIActionCfgAdd(43, 0, ACTT_RADIO | ACTSC_INLINE, "Zaraz wracam" AP_VALUE "65", cfg::status::onAutoAwaySt);
      UIActionCfgAdd(43, 0, ACTT_IMAGE | ACTSC_INLINE, Helpers::icon16(UIIcon(IT_STATUS, 0, ST_NA, 0)).a_str());
      UIActionCfgAdd(43, 0, ACTT_RADIO | ACTSC_INLINE, "Nieosi�galny" AP_VALUE "33", cfg::status::onAutoAwaySt);
      UIActionCfgAdd(43, 0, ACTT_IMAGE | ACTSC_INLINE, Helpers::icon16(UIIcon(IT_STATUS, 0, ST_DND, 0)).a_str());
      UIActionCfgAdd(43, 0, ACTT_RADIO | ACTSC_INLINE, "Nie przeszkadza�" AP_VALUE "34", cfg::status::onAutoAwaySt);
      UIActionCfgAdd(43, 0, ACTT_IMAGE | ACTSC_INLINE, Helpers::icon16(UIIcon(IT_STATUS, 0, ST_HIDDEN, 0)).a_str());
      UIActionCfgAdd(43, 0, ACTT_RADIO | ACTSRADIO_LAST, "Ukryty" AP_VALUE "66", cfg::status::onAutoAwaySt);
      UIActionCfgAdd(43, 0, ACTT_GROUPEND);

      UIActionCfgAdd(43, 0, ACTT_GROUP, "Tryb auto-away (rozszerzony)");
      UIActionCfgAdd(43, 0, ACTT_SEPARATOR, "Po jakim czasie zmienia� status:");
      UIActionCfgAdd(43, 0, ACTT_SLIDER, "15m, 1s\n45m" AP_STEP "60" AP_MINIMUM "901" AP_MAXIMUM "2700"
        AP_TIP "Czas w sekundach po jakim zostanie ustawiony wybrany poni�ej status", cfg::extAutoAway::time);
      UIActionCfgAdd(43, 0, ACTT_SEPARATOR, "Zmieniaj status na:");
      UIActionCfgAdd(43, 0, ACTT_IMAGE | ACTSC_INLINE, Helpers::icon16(UIIcon(IT_STATUS, 0, ST_AWAY, 0)).a_str());
      UIActionCfgAdd(43, 0, ACTT_RADIO | ACTSC_INLINE, "Zaraz wracam" AP_VALUE "65", cfg::extAutoAway::status);
      UIActionCfgAdd(43, 0, ACTT_IMAGE | ACTSC_INLINE, Helpers::icon16(UIIcon(IT_STATUS, 0, ST_NA, 0)).a_str());
      UIActionCfgAdd(43, 0, ACTT_RADIO | ACTSC_INLINE, "Nieosi�galny" AP_VALUE "33", cfg::extAutoAway::status);
      UIActionCfgAdd(43, 0, ACTT_IMAGE | ACTSC_INLINE, Helpers::icon16(UIIcon(IT_STATUS, 0, ST_DND, 0)).a_str());
      UIActionCfgAdd(43, 0, ACTT_RADIO | ACTSC_INLINE, "Nie przeszkadza�" AP_VALUE "34", cfg::extAutoAway::status);
      UIActionCfgAdd(43, 0, ACTT_IMAGE | ACTSC_INLINE, Helpers::icon16(UIIcon(IT_STATUS, 0, ST_HIDDEN, 0)).a_str());
      UIActionCfgAdd(43, 0, ACTT_RADIO, "Ukryty" AP_VALUE "66", cfg::extAutoAway::status);
      UIActionCfgAdd(43, 0, ACTT_IMAGE | ACTSC_INLINE, Helpers::icon16(46).a_str());
      UIActionCfgAdd(43, 0, ACTT_RADIO | ACTSC_BOLD | ACTSRADIO_LAST, "Nie zmieniaj" AP_VALUE "-1", cfg::extAutoAway::status);
      UIActionCfgAdd(43, 0, ACTT_GROUPEND);
    }

    /* Main tab */
    /* |-> General settings group */
    ifINT {
      UIActionCfgAdd(ui::cfgGroup, 0, ACTT_GROUP, "Ustawienia");
      UIActionCfgAdd(ui::cfgGroup, 0, ACTT_CHECK, "Wycisz przy w��czeniu trybu away", cfg::muteOnEnable);
      ifADV UIActionCfgAdd(ui::cfgGroup, 0, ACTT_CHECK, "Wy��czaj w momencie wys�ania wiadomo�ci", cfg::disableOnMsgSend);
      ifADV UIActionCfgAdd(ui::cfgGroup, 0, ACTT_CHECK, "Zapisuj wiadomo�ci w osobnej historii", cfg::saveToHistory);
      UIActionCfgAdd(ui::cfgGroup, 0, ACTT_CHECK, "Komendy z okna rozm�w (a'la IRC)" AP_TIPRICH 
        "/<b>away</b> [...] - w��cza tryb away<br/>"
        "/<b>brb</b> [...] - j.w. + nie wysy�a powiadomie� o w�.<br/>"
        "/<b>back</b> [...] - wy��cza tryb away<br/>"
        "/<b>re</b> [...] - j.w. + nie wysy�a powiadomie� o wy�." AP_TIP_WIDTH "240", cfg::ircCmds);
      if (Helpers::pluginExists(plugsNET::knotify))
        UIActionCfgAdd(ui::cfgGroup, 0, ACTT_CHECK, "Pokazuj powiadomienia K.Notify", cfg::useKNotify);
      ifADV UIActionCfgAdd(ui::cfgGroup, 0, ACTT_CHECK, "Pytaj przed wyj�ciem z trybu away", cfg::disableConfirmation);
      UIActionCfgAdd(ui::cfgGroup, 0, ACTT_GROUPEND);
    }

    ifADV {
      UIActionCfgAdd(ui::cfgGroup, 0, ACTT_GROUP, "Formatowanie czasu");
      UIActionCfgAdd(ui::cfgGroup, 0, ACTT_EDIT | ACTSC_INLINE, dateFormat, cfg::dateFormat);
      UIActionCfgAdd(ui::cfgGroup, 0, ACTT_COMMENT, "Format daty");
      UIActionCfgAdd(ui::cfgGroup, 0, ACTT_EDIT | ACTSC_INLINE, timeFormat, cfg::timeFormat);
      UIActionCfgAdd(ui::cfgGroup, 0, ACTT_COMMENT, "Format godziny");
      UIActionCfgAdd(ui::cfgGroup, 0, ACTT_GROUPEND);
    }

    ifINT {
      UIActionCfgAdd(ui::cfgGroup, 0, ACTT_GROUP, "Ustawienia auto-away'a");
      UIActionCfgAdd(ui::cfgGroup, 0, ACTT_SEPARATOR, "Synchronizuj z trybem auto-away:");
      UIActionCfgAdd(ui::cfgGroup, 0, ACTT_RADIO | ACTSC_INLINE, "podstawowym" AP_VALUE "1", cfg::autoAwaySync);
      UIActionCfgAdd(ui::cfgGroup, 0, ACTT_RADIO, "rozszerzonym" AP_VALUE "2", cfg::autoAwaySync);
      UIActionCfgAdd(ui::cfgGroup, 0, ACTT_RADIO | ACTSC_BOLD | ACTSRADIO_LAST, "nie synchronizuj" AP_VALUE "0", cfg::autoAwaySync);
      UIActionCfgAdd(ui::cfgGroup, 0, ACTT_SEPARATOR, "Pow�d nieobecno�ci:");
      UIActionCfgAdd(ui::cfgGroup, 0, ACTT_TEXT, 0, cfg::tpl::autoAway);
      UIActionCfgAdd(ui::cfgGroup, 0, ACTT_GROUPEND);
    }

    /* |-> Interface group */
    UIActionCfgAdd(ui::cfgGroup, 0, ACTT_GROUP, "Interfejs");
    UIActionCfgAdd(ui::cfgGroup, 0, ACTT_CHECK | ACTSC_NEEDRESTART, "Pokazuj przycisk w g��wnym oknie", cfg::btnInMainWindow);
    UIActionCfgAdd(ui::cfgGroup, 0, ACTT_CHECK | ACTSC_NEEDRESTART, "Pokazuj przycisk w oknie rozmowy", cfg::btnInCntWindow);
    UIActionCfgAdd(ui::cfgGroup, 0, ACTT_CHECK | ACTSC_NEEDRESTART, "Pokazuj przycisk w menu tray'a", cfg::btnInTrayMenu);
    UIActionCfgAdd(ui::cfgGroup, 0, ACTT_GROUPEND);

    UIActionCfgAdd(ui::cfgGroup, 0, ACTT_GROUP, "Historia powod�w nieobecno�ci");
    UIActionCfgAdd(ui::cfgGroup, 0, ACTT_SEPARATOR, "Ilo�� pozycji w historii:");
    UIActionCfgAdd(ui::cfgGroup, 0, ACTT_SLIDER, "Ma�o\nDu�o" AP_MINIMUM "1" AP_MAXIMUM "30", cfg::mruSize);
    UIActionCfgAdd(ui::cfgGroup, 0, ACTT_SEPARATOR);
    UIActionCfgAdd(ui::cfgGroup, act::clearMru, ACTT_BUTTON, "wyczy��" AP_ICO "667112", 0, 0, 0, 80, 30);
    UIActionCfgAdd(ui::cfgGroup, 0, ACTT_GROUPEND);

    /*
    ifPRO {
      UIActionCfgAdd(ui::cfgGroup, 0, ACTT_GROUP, "Ustawienia raportowania");
      UIActionCfgAdd(ui::cfgGroup, 0, ACTT_SEPARATOR, "Aktywne metody raportu:");

      //fCtrl->UIDrawActiveSum();

      UIActionCfgAdd(ui::cfgGroup, 0, ACTT_SEPARATOR, "Odst�p czasu pomi�dzy wysy�anymi raportami [min]:");
      UIActionCfgAdd(ui::cfgGroup, 0, ACTT_SLIDER, "Nie wysy�aj\n1 dzie�" AP_MINIMUM "0" AP_MAXIMUM "1440" AP_STEP "60", 
        cfg::summary::interval);
      UIActionCfgAdd(ui::cfgGroup, 0, ACTT_SEPARATOR);
      UIActionCfgAdd(ui::cfgGroup, 0, ACTT_CHECK, "Wysy�aj w trybie auto-away", cfg::summary::inAutoAway);
      UIActionCfgAdd(ui::cfgGroup, 0, ACTT_SPINNER | ACTSC_INLINE, AP_MINIMUM "0" AP_MAXIMUM "1000" 
        AP_TIP "0 = bez limitu", cfg::summary::minMsgCount, 0, 0, 120);
      UIActionCfgAdd(ui::cfgGroup, 0, ACTT_COMMENT, "Minimalna ilo�� wiadomo�ci");
      UIActionCfgAdd(ui::cfgGroup, 0, ACTT_GROUPEND);

      UIActionCfgAdd(ui::cfgGroup, 0, ACTT_GROUP, "Ustawienia przekierowania");
      UIActionCfgAdd(ui::cfgGroup, 0, ACTT_SEPARATOR, "Aktywne przekierowania:");

      //fCtrl->UIDrawActiveFwd();

      UIActionCfgAdd(ui::cfgGroup, 0, ACTT_SEPARATOR);
      UIActionCfgAdd(ui::cfgGroup, 0, ACTT_CHECK, "Wysy�aj w trybie auto-away", cfg::fwd::inAutoAway);
      UIActionCfgAdd(ui::cfgGroup, 0, ACTT_GROUPEND);
    }
    */

    /* Status tab */
    /* |-> Settings group */
    UIActionCfgAdd(ui::statusCfgGroup, 0, ACTT_GROUP, "Ustawienia");
    /*
    UIActionCfgAdd(ui::statusCfgGroup, 0, ACTT_COMBO | ACTSCOMBO_LIST | ACTSC_INLINE, 
      "Zaraz wracam" AP_ICO "#40000410" AP_VALUE "65\n"
      "Nieosi�galny" AP_ICO "#40000210" AP_VALUE "33\n"
      "Nie przeszkadza�" AP_ICO "#40000220" AP_VALUE "34\n"
      "Ukryty" AP_ICO "#40000420" AP_VALUE "66", 
      cfg::status::onEnableSt);
    UIActionCfgAdd(ui::statusCfgGroup, 0, ACTT_COMMENT, "Domy�lnie ustawiony status");
    */
    ifINT {
      ifNORM UIActionCfgAdd(ui::statusCfgGroup, 0, ACTT_CHECK, "Dodawaj '...' przy skracaniu d�ugich opis�w", cfg::status::dotsAppend);
      UIActionCfgAdd(ui::statusCfgGroup, 0, ACTT_CHECK, "Zmieniaj status przy w��czonym statusie 'ukryty'", cfg::status::whenInvisible);
      UIActionCfgAdd(ui::statusCfgGroup, 0, ACTT_SEPARATOR);
    }
    UIActionCfgAdd(ui::statusCfgGroup, 0, ACTT_CHECK, "Zmieniaj status przy w��czeniu trybu away", cfg::status::changeOnEnable);
    UIActionCfgAdd(ui::statusCfgGroup, 0, ACTT_CHECK, "Zmieniaj opis statusu przy w��czeniu trybu away", cfg::status::changeInfoOnEnable);
    UIActionCfgAdd(ui::statusCfgGroup, 0, ACTT_GROUPEND);

    UIActionCfgAdd(ui::statusCfgGroup, 0, ACTT_GROUP, "Domy�lnie zmieniaj status na:");
    UIActionCfgAdd(ui::statusCfgGroup, 0, ACTT_IMAGE | ACTSC_INLINE, Helpers::icon16(UIIcon(IT_STATUS, 0, ST_AWAY, 0)).a_str());
    UIActionCfgAdd(ui::statusCfgGroup, 0, ACTT_RADIO | ACTSC_INLINE, "Zaraz wracam" AP_VALUE "65", cfg::status::onEnableSt);
    UIActionCfgAdd(ui::statusCfgGroup, 0, ACTT_IMAGE | ACTSC_INLINE, Helpers::icon16(UIIcon(IT_STATUS, 0, ST_NA, 0)).a_str());
    UIActionCfgAdd(ui::statusCfgGroup, 0, ACTT_RADIO | ACTSC_INLINE, "Nieosi�galny" AP_VALUE "33", cfg::status::onEnableSt);
    UIActionCfgAdd(ui::statusCfgGroup, 0, ACTT_IMAGE | ACTSC_INLINE, Helpers::icon16(UIIcon(IT_STATUS, 0, ST_DND, 0)).a_str());
    UIActionCfgAdd(ui::statusCfgGroup, 0, ACTT_RADIO | ACTSC_INLINE, "Nie przeszkadza�" AP_VALUE "34", cfg::status::onEnableSt);
    UIActionCfgAdd(ui::statusCfgGroup, 0, ACTT_IMAGE | ACTSC_INLINE, Helpers::icon16(UIIcon(IT_STATUS, 0, ST_HIDDEN, 0)).a_str());
    UIActionCfgAdd(ui::statusCfgGroup, 0, ACTT_RADIO | ACTSRADIO_LAST, "Ukryty" AP_VALUE "66", cfg::status::onEnableSt);
    UIActionCfgAdd(ui::statusCfgGroup, 0, ACTT_GROUPEND);

    UIActionCfgAdd(ui::statusCfgGroup, 0, ACTT_GROUP, "Szablon statusu");
    statusCtrl->stringFormatter->UIDrawHelpBtn(stVars, ui::statusCfgGroup);
    UIActionCfgAdd(ui::statusCfgGroup, 0, ACTT_TEXT, 0, cfg::tpl::status);
    UIActionCfgAdd(ui::statusCfgGroup, 0, ACTT_GROUPEND);

    /* |-> Net selection group */
    statusList->UIDraw(3, "Wybierz sieci, na kt�rych chcesz zmienia� status:");

    /* Autoresponder tab */
    /* |-> Minimal reply interval group */
    ifPRO {
      UIActionCfgAdd(ui::replyCfgGroup, 0, ACTT_GROUP, "Minimalny interwa� pomi�dzy wysy�anymi odpowiedziami");
      UIActionCfgAdd(ui::replyCfgGroup, 0, ACTT_SEPARATOR, "Czas interwa�u:");
      UIActionCfgAdd(ui::replyCfgGroup, 0, ACTT_SLIDER, "Ma�y\nDu�y" AP_STEP "60" AP_MINIMUM "0" AP_MAXIMUM "3600" AP_TIPRICH 
        "<b>0</b> - odpowied� b�dzie wys�ana tylko jeden raz<br/><b>3600</b> - odpowied� nie zostanie ponownie "
        "wys�ana przez min. <b>1h</b> :>", cfg::reply::minInterval);
      UIActionCfgAdd(ui::replyCfgGroup, 0, ACTT_SEPARATOR, "Interwa� b�dzie liczony od ostatniej ...");
      UIActionCfgAdd(ui::replyCfgGroup, 0, ACTT_IMAGE | ACTSC_INLINE, Helpers::icon16(ico::msg).a_str());
      UIActionCfgAdd(ui::replyCfgGroup, 0, ACTT_RADIO | ACTSC_INLINE, "otrzymanej wiadomo�ci" AP_VALUE "0", cfg::reply::minIntervalType);
      UIActionCfgAdd(ui::replyCfgGroup, 0, ACTT_IMAGE | ACTSC_INLINE, Helpers::icon16(ico::reply).a_str());
      UIActionCfgAdd(ui::replyCfgGroup, 0, ACTT_RADIO, "wys�anej wiadomo�ci" AP_VALUE "1"
        AP_TIP "(w tym powiadomie�)", cfg::reply::minIntervalType);
      UIActionCfgAdd(ui::replyCfgGroup, 0, ACTT_IMAGE | ACTSC_INLINE, Helpers::icon16(ico::msg).a_str());
      UIActionCfgAdd(ui::replyCfgGroup, 0, ACTT_RADIO | ACTSRADIO_LAST, "wys�anej i otrzymanej wiadomo�ci" 
        AP_VALUE "2", cfg::reply::minIntervalType);
      UIActionCfgAdd(ui::replyCfgGroup, 0, ACTT_GROUPEND);
    }

    /* |-> Net selection group */
    autoReplyList->UIDraw(3, "Wybierz sieci, na kt�rych maj� dzia�a� powiadomienia:");

    UIActionCfgAdd(ui::replyCfgGroup, 0, ACTT_GROUP, "Wysy�anie powiadomie�");
    ifINT {
      // ifADV UIActionCfgAdd(ui::replyCfgGroup, 0, ACTT_CHECK, "Pokazuj wys�ane powiadomienia w okienku rozmowy", cfg::reply::showInWnd);
      UIActionCfgAdd(ui::replyCfgGroup, 0, ACTT_CHECK, "Wysy�aj powiadomienia przy w��czonym statusie 'ukryty'", cfg::reply::whenInvisible);
      UIActionCfgAdd(ui::replyCfgGroup, 0, ACTT_SEPARATOR);
    }
    UIActionCfgAdd(ui::replyCfgGroup, 0, ACTT_CHECK, "Odpowiadaj na odebrane wiadomo�ci", cfg::reply::onMsg);
    UIActionCfgAdd(ui::replyCfgGroup, 0, ACTT_CHECK, "Wysy�aj powiadomienie o w��czeniu trybu away", cfg::reply::onEnable);
    UIActionCfgAdd(ui::replyCfgGroup, 0, ACTT_CHECK, "Wysy�aj powiadomienie o wy��czeniu trybu away", cfg::reply::onDisable);
    /*
    UIActionCfgAdd(ui::replyCfgGroup, 0, ACTT_SEPARATOR);
    UIActionCfgAdd(ui::replyCfgGroup, 0, ACTT_CHECK, "Wy�lij powiadomienie je�li kontakt przy�le 'has�o'", cfg::reply::magicKey);
    UIActionCfgAdd(ui::replyCfgGroup, 0, ACTT_COMBO | ACTSC_INLINE | ACTSCOMBO_LIST, 
      "SMS" AP_ICO "0x50000090" AP_VALUE "sms\n" 
      "EMail" AP_ICO "0x50000080" AP_VALUE "email\n" 
      "Przekierowanie" AP_ICO "0x50000080" AP_VALUE "forward", // bad icon for now
      cfg::reply::magicKeyNotifyMethod);
    UIActionCfgAdd(ui::replyCfgGroup, 0, ACTT_COMMENT, "Spos�b wysy�ki");
    */
    UIActionCfgAdd(ui::replyCfgGroup, 0, ACTT_GROUPEND);

    /* |-> Templates group */
    UIActionCfgAdd(ui::replyCfgGroup, 0, ACTT_GROUP, "Szablony powiadomie�");
    ifINT UIActionCfgAdd(ui::replyCfgGroup, 0, ACTT_CHECK, "Formatuj kod HTML" AP_TIP 
      "Je�li w��czone, znaki specjalne nale�y zamienia� na ich HTML-owe odpowiedniki (np.: (< na &lt;), (> na &gt;))", cfg::reply::useHtml);

    UIActionCfgAdd(ui::replyCfgGroup, 0, ACTT_SEPARATOR, "Odpowied�:");
    statusCtrl->stringFormatter->UIDrawHelpBtn(rVars, ui::replyCfgGroup);
    UIActionCfgAdd(ui::replyCfgGroup, 0, ACTT_TEXT, 0, cfg::tpl::reply);

    UIActionCfgAdd(ui::replyCfgGroup, 0, ACTT_SEPARATOR, "W��czenie trybu away:");
    statusCtrl->stringFormatter->UIDrawHelpBtn(rVars, ui::replyCfgGroup);
    UIActionCfgAdd(ui::replyCfgGroup, 0, ACTT_TEXT, 0, cfg::tpl::enable);

    UIActionCfgAdd(ui::replyCfgGroup, 0, ACTT_SEPARATOR, "Wy��czenie trybu away:");
    statusCtrl->stringFormatter->UIDrawHelpBtn(rVars, ui::replyCfgGroup);
    UIActionCfgAdd(ui::replyCfgGroup, 0, ACTT_TEXT, 0, cfg::tpl::disable);
    UIActionCfgAdd(ui::replyCfgGroup, 0, ACTT_GROUPEND);

    /* Main tab in contact window */
    /* |-> Minimal reply interval group */
    ifPRO {
      UIActionCfgAdd(ui::cntCfgGroup, 0, ACTT_GROUP, "Minimalny interwa� pomi�dzy wysy�anymi odpowiedziami");
      UIActionCfgAdd(ui::cntCfgGroup, 0, ACTT_SEPARATOR, "Czas interwa�u:");
      UIActionCfgAdd(ui::cntCfgGroup, 0, ACTT_SLIDER, "Ma�y\nDu�y" AP_STEP "60" AP_MINIMUM "-1" AP_MAXIMUM "3600" AP_TIPRICH 
        "<b>-1</b> - pobierze warto�� domy�ln� z konfiguracji<br/><b>0</b> - odpowied� b�dzie wys�ana tylko jeden raz<br/>"
        "<b>3600</b> - odpowied� nie zostanie ponownie wys�ana przez min. <b>1h</b> :>", cfg::reply::minInterval);
      UIActionCfgAdd(ui::cntCfgGroup, 0, ACTT_SEPARATOR, "Interwa� b�dzie liczony od ostatniej ...");
      UIActionCfgAdd(ui::cntCfgGroup, 0, ACTT_IMAGE | ACTSC_INLINE, Helpers::icon16(ico::msg).a_str());
      UIActionCfgAdd(ui::cntCfgGroup, 0, ACTT_RADIO | ACTSC_INLINE, "otrzymanej wiadomo�ci" AP_VALUE "0", cfg::reply::minIntervalType);
      UIActionCfgAdd(ui::cntCfgGroup, 0, ACTT_IMAGE | ACTSC_INLINE, Helpers::icon16(ico::reply).a_str());
      UIActionCfgAdd(ui::cntCfgGroup, 0, ACTT_RADIO, "wys�anej wiadomo�ci" AP_VALUE "1"
        AP_TIP "(w tym powiadomie�)", cfg::reply::minIntervalType);
      UIActionCfgAdd(ui::cntCfgGroup, 0, ACTT_IMAGE | ACTSC_INLINE, Helpers::icon16(ico::msg).a_str());
      UIActionCfgAdd(ui::cntCfgGroup, 0, ACTT_RADIO | ACTSC_INLINE, "wys�anej i otrzymanej wiadomo�ci" 
        AP_VALUE "2", cfg::reply::minIntervalType);
      UIActionCfgAdd(ui::cntCfgGroup, 0, ACTT_IMAGE | ACTSC_INLINE, Helpers::icon16(27).a_str());
      UIActionCfgAdd(ui::cntCfgGroup, 0, ACTT_RADIO | ACTSC_BOLD | ACTSRADIO_LAST, "domy�lnie" AP_VALUE "-1", cfg::reply::minIntervalType);
      UIActionCfgAdd(ui::cntCfgGroup, 0, ACTT_GROUPEND);
    }

    UIActionCfgAdd(ui::cntCfgGroup, 0, ACTT_GROUP, "Ustawienia");
    ifADV {
      /* UIActionCfgAdd(ui::cntCfgGroup, 0, ACTT_COMBO | ACTSCOMBO_LIST | ACTSCOMBO_BYPOS | ACTSC_INLINE, 
        "Domy�lnie" CFGICO "#1B\nTak" CFGICO "#A2DE5\nNie" CFGICO "#A2DE6", cfg::reply::showInWnd);
      UIActionCfgAdd(ui::cntCfgGroup, 0, ACTT_COMMENT, "Pokazuj wys�ane powiadomienia w okienku rozmowy"); */

      UIActionCfgAdd(ui::cntCfgGroup, 0, ACTT_COMBO | ACTSCOMBO_LIST | ACTSCOMBO_BYPOS | ACTSC_INLINE, 
        "Domy�lnie" CFGICO "#1B\nTak" CFGICO "#A2DE5\nNie" CFGICO "#A2DE6", cfg::saveToHistory);
      UIActionCfgAdd(ui::cntCfgGroup, 0, ACTT_COMMENT, "Zapisuj wiadomo�ci w osobnej historii");

      UIActionCfgAdd(ui::cntCfgGroup, 0, ACTT_COMBO | ACTSCOMBO_LIST | ACTSCOMBO_BYPOS | ACTSC_INLINE, 
        "Domy�lnie" CFGICO "#1B\nTak" CFGICO "#A2DE5\nNie" CFGICO "#A2DE6", cfg::disableOnMsgSend);
      UIActionCfgAdd(ui::cntCfgGroup, 0, ACTT_COMMENT, "Wy��czaj w momencie wys�ania wiadomo�ci");
    }

    ifINT {
      UIActionCfgAdd(ui::cntCfgGroup, 0, ACTT_COMBO | ACTSCOMBO_LIST | ACTSCOMBO_BYPOS | ACTSC_INLINE, 
        "Domy�lnie" CFGICO "#1B\nTak" CFGICO "#A2DE5\nNie" CFGICO "#A2DE6", cfg::ircCmds);
      UIActionCfgAdd(ui::cntCfgGroup, 0, ACTT_COMMENT, "Komendy z okna rozm�w (a'la IRC)");
      UIActionCfgAdd(ui::cntCfgGroup, 0, ACTT_GROUPEND);

      UIActionCfgAdd(ui::cntCfgGroup, 0, ACTT_GROUP, "Wysy�anie powiadomie�");
      UIActionCfgAdd(ui::cntCfgGroup, 0, ACTT_COMBO | ACTSCOMBO_LIST | ACTSCOMBO_BYPOS | ACTSC_INLINE, 
        "Domy�lnie" CFGICO "#1B\nTak" CFGICO "#A2DE5\nNie" CFGICO "#A2DE6", cfg::reply::whenInvisible);
      UIActionCfgAdd(ui::cntCfgGroup, 0, ACTT_COMMENT, "Wysy�aj powiadomienia przy w��czonym statusie 'ukryty'");
      UIActionCfgAdd(ui::cntCfgGroup, 0, ACTT_SEPARATOR);
    }

    UIActionCfgAdd(ui::cntCfgGroup, 0, ACTT_COMBO | ACTSCOMBO_LIST | ACTSCOMBO_BYPOS | ACTSC_INLINE, 
      "Domy�lnie" CFGICO "#1B\nTak" CFGICO "#A2DE5\nNie" CFGICO "#A2DE6", cfg::reply::onMsg);
    UIActionCfgAdd(ui::cntCfgGroup, 0, ACTT_COMMENT, "Odpowiadaj na odebrane wiadomo�ci");

    UIActionCfgAdd(ui::cntCfgGroup, 0, ACTT_COMBO | ACTSCOMBO_LIST | ACTSCOMBO_BYPOS | ACTSC_INLINE, 
      "Domy�lnie" CFGICO "#1B\nTak" CFGICO "#A2DE5\nNie" CFGICO "#A2DE6", cfg::reply::onEnable);
    UIActionCfgAdd(ui::cntCfgGroup, 0, ACTT_COMMENT, "Wysy�aj powiadomienie o w��czeniu trybu away");

    UIActionCfgAdd(ui::cntCfgGroup, 0, ACTT_COMBO | ACTSCOMBO_LIST | ACTSCOMBO_BYPOS | ACTSC_INLINE, 
      "Domy�lnie" CFGICO "#1B\nTak" CFGICO "#A2DE5\nNie" CFGICO "#A2DE6", cfg::reply::onDisable);
    UIActionCfgAdd(ui::cntCfgGroup, 0, ACTT_COMMENT, "Wysy�aj powiadomienie o wy��czeniu trybu away");
    UIActionCfgAdd(ui::cntCfgGroup, 0, ACTT_GROUPEND);

    /* |-> Templates group */
    UIActionCfgAdd(ui::cntCfgGroup, 0, ACTT_GROUP, "Szablony powiadomie�");
    ifINT {
      UIActionCfgAdd(ui::cntCfgGroup, 0, ACTT_COMBO | ACTSCOMBO_LIST | ACTSCOMBO_BYPOS | ACTSC_INLINE, 
        "Domy�lnie" CFGICO "#1B\nTak" CFGICO "#A2DE5\nNie" CFGICO "#A2DE6", cfg::reply::useHtml);
      UIActionCfgAdd(ui::cntCfgGroup, 0, ACTT_COMMENT, "Formatuj kod HTML" AP_TIP 
        "Je�li w��czone, znaki specjalne nale�y zamienia� na ich HTML-owe odpowiedniki (np.: (< na &lt;), (> na &gt;), etc.)");
    }

    UIActionCfgAdd(ui::cntCfgGroup, 0, ACTT_SEPARATOR, "Odpowied�:");
    statusCtrl->stringFormatter->UIDrawHelpBtn(rVars, ui::cntCfgGroup);
    UIActionCfgAdd(ui::cntCfgGroup, 0, ACTT_TEXT, 0, cfg::tpl::reply);

    UIActionCfgAdd(ui::cntCfgGroup, 0, ACTT_SEPARATOR, "W��czenie trybu away:");
    statusCtrl->stringFormatter->UIDrawHelpBtn(rVars, ui::cntCfgGroup);
    UIActionCfgAdd(ui::cntCfgGroup, 0, ACTT_TEXT, 0, cfg::tpl::enable);

    UIActionCfgAdd(ui::cntCfgGroup, 0, ACTT_SEPARATOR, "Wy��czenie trybu away:");
    statusCtrl->stringFormatter->UIDrawHelpBtn(rVars, ui::cntCfgGroup);
    UIActionCfgAdd(ui::cntCfgGroup, 0, ACTT_TEXT, 0, cfg::tpl::disable);
    UIActionCfgAdd(ui::cntCfgGroup, 0, ACTT_GROUPEND);

    /* Buttons */
    if (GETINT(cfg::btnInMainWindow)) {
      UIActionAdd(this->pluginsGroup, ui::powerInMainWnd, 0, "W��cz tryb away", ico::enable);
    }
    if (GETINT(cfg::btnInCntWindow)) {
      UIGroupAdd(IMIG_MSGTB, ui::msgTbGrp, ACTR_INIT, "kAway2", ico::logoSmall);
      UIActionAdd(ui::msgTbGrp, ui::powerInCntWnd, ACTSC_BOLD, "W��cz tryb away", ico::enable);
      UIActionAdd(ui::msgTbGrp, ui::ignoreBtn, ACTS_DISABLED, "W��cz ignorowanie", ico::ignore);
    }
    if (GETINT(cfg::btnInTrayMenu)) {
      UIActionInsert(IMIG_TRAY, 0, 0, ACTT_SEP);
      UIActionInsert(IMIG_TRAY, ui::powerInTrayMenu, 0, 0, "W��cz tryb away", ico::enable);
    }

    this->setSuccess();
  }

  void Controller::onEnd() {
    if (this->isEnabled()) {
      this->disable("", true);
    }
    this->extAutoAwayTimer.reset();
    this->setSuccess();
  }

  void Controller::onMsgRcv() {
    cMessage *m = (cMessage*) this->getIM()->p1;

    // hmmm, i have to remove it some sunny day...
    if ((m->type != MT_MESSAGE) || (m->flag & MF_AUTOMATED)) {
      return this->setReturnCode(0);
    }

    // we're searchin' for contact id
    int cnt = Ctrl->ICMessage(IMC_CNT_FIND, m->net, (int) (strlen(m->fromUid) ? m->fromUid : m->toUid));
    bool userMsg = !strlen(m->fromUid) && !GetExtParam(m->ext, cfg::extParamName).length() && (m->type == MT_MESSAGE);

    if (userMsg && (m->body[0] == '/' && strlen(m->body) > 1) && Helpers::altCfgVal(cnt, cfg::ircCmds)) {
      tStringVector params;
      std::string body(m->body); 
      body = body.substr(1, body.length());

      split(body, " ", params);
      String cmd = params[0];
      String awayMsg = (params.size() > 1) ? body.substr(params[0].length() + 1, body.length()) : "";

      bool del = false, set = false; // irc-type command like
      int st = 0; // default status

      RegEx reg;
      reg.match("/^(away|brb)\\[(.+)\\]$/", cmd.a_str());

      if (reg.isMatched()) {
        cmd = reg[1];

        if (reg[2] == "away") st = ST_AWAY;
        else if (reg[2] == "xa") st = ST_NA;
        else if (reg[2] == "dnd") st = ST_DND;
        else if (reg[2] == "hidden") st = ST_HIDDEN;
        else if (reg[2] == "-") st = -1;
      }

      if (cmd == "away") {
        del = true; set = this->enable(awayMsg, st);
      } else if (cmd == "brb") {
        del = true; set = this->enable(awayMsg, st, true);
      } else if (cmd == "back") {
        del = true; set = this->disable(awayMsg);
      } else if (cmd == "re") {
        del = true; set = this->disable(awayMsg, true);
      }

      if (del) {
        logDebug("{IM_MSG_RCV}: cmd = %s, reason = %s, params = %i",
          cmd.c_str(), nullChk(awayMsg), params.size());

        if (!set) {
          return this->setReturnCode(IM_MSG_delete);
        }

        String body, ext(m->ext);
        body += "<u>";
        body += (cmd == "away" || cmd == "brb") ? "W��czono" : "Wy��czono";
        body += "</u> tryb away";
        body += awayMsg.length() ? " (<b>" + awayMsg + "</b>)" : (String) "";

        ext = SetExtParam(ext, MEX_ADDINFO, "kAway2");
        ext = SetExtParam(ext, MEX_NOSOUND, "1");

        char * newBody = (char*) Ctrl->GetTempBuffer(body.length() + 1 + ext.length() + 1);
        strcpy(newBody, body.a_str());
        char * newExt = newBody + body.length() + 2;
        strcpy(newExt, ext.a_str());

        m->body = newBody;
        m->ext = newExt;
        m->flag |= (m->flag & MF_HTML) ? 0 : MF_HTML;
        m->flag |= MF_DONTADDTOHISTORY;

        return this->setReturnCode(IM_MSG_update | IM_MSG_delete);
      }
    }

    if (this->isEnabled() && !this->cntProp(cnt)->ignored) {
      if (userMsg && Helpers::altCfgVal(cnt, cfg::disableOnMsgSend)) {
        this->disable(); return this->setReturnCode(0);
      }

      if (Helpers::altCfgVal(cnt, cfg::saveToHistory)) {
        Helpers::addItemToHistory(m, cnt, cfg::historyFolder, "", this->cntProp(cnt)->historySess);
        this->cntProp(cnt)->historySess = 1;
      }

      // this->addMsg2CntQueue(cnt, m);
      // fCtrl->onNewMsg(cnt, m);

      if (Helpers::altCfgVal(cnt, cfg::reply::onMsg)) {
        int intType = Helpers::altCfgVal(cnt, cfg::reply::minIntervalType, false);

        if (strlen(m->fromUid)) {
          int lastMsgTime = this->cntProp(cnt)->lastMsgTime;
          int interval = Helpers::altCfgVal(cnt, cfg::reply::minInterval, false);

          if ((!interval && !lastMsgTime) || (interval && ((interval + lastMsgTime) < m->time))) {
            this->sendMsgTpl(cnt, Controller::tplReply);
          }
          if ((intType == intervalTypeRcvTime) || (intType == intervalTypeBoth)) {
            this->cntProp(cnt)->lastMsgTime = m->time;
          }
        } else {
          if ((intType == intervalTypeSendTime) || (intType == intervalTypeBoth)) {
            this->cntProp(cnt)->lastMsgTime = m->time;
          }
        }
      }
    }
  }

  void Controller::onAction() {
    sUIActionNotify_2params* an = this->getAN();
    int cnt = an->act.cnt;

    logDebug("[Controller<%i>::onAction()]: an->act.id = %i, an->act.cnt = %i, an->code = %i", 
      this, an->act.id, an->act.cnt, an->code);

    statusList->actionHandle(an->act.id, an->code);
    autoReplyList->actionHandle(an->act.id, an->code);

    switch (an->act.id) {
      case ui::msgTbGrp:
      case ui::powerInMainWnd:
      case ui::powerInCntWnd:
      case ui::powerInTrayMenu: {
        if ((an->act.id == ui::msgTbGrp) && (an->code == ACTN_CREATEGROUP)) {
          bool isIgnored = this->cntProp(cnt)->ignored;
          bool isEnabled = this->isEnabled();

          Helpers::chgBtn(ui::msgTbGrp, ui::ignoreBtn, AC_CURRENT, 
            (isIgnored) ? "Wy��cz ignorowanie" : "W��cz ignorowanie", (isIgnored) ? ico::unIgnore : ico::ignore,
            (isEnabled) ? 0 : ACTS_DISABLED);
          Helpers::chgBtn(ui::msgTbGrp, ui::powerInCntWnd, AC_CURRENT, 
            (isEnabled) ? "Wy��cz tryb away" : "W��cz tryb away", (isEnabled) ? ico::disable : ico::enable);
          break;
        } else if (an->code != ACTN_ACTION) break;

        if (this->isEnabled()) {
          if (!GETINT(cfg::disableConfirmation) || Ctrl->ICMessage(IMI_CONFIRM, (int) "Na pewno chcesz wy��czy� tryb away?")) 
            this->disable();
        } else {
          wnd->show();
        }
        break;
      }

      case ui::cntCfgGroup: {
        if ((an->code == ACTN_CREATE)) {
          UIActionSetStatus(an->act, !Ctrl->DTgetPos(DTCNT, cnt) ? -1 : 0, ACTS_HIDDEN);
        }
        break;
      }

      case ui::ignoreBtn: {
        if ((an->code == ACTN_ACTION) && this->isEnabled()) {
          this->cntProp(cnt)->ignored = !this->cntProp(cnt)->ignored;
        }
        break;
      }

      case act::clearMru: {
        if (an->code == ACTN_ACTION) {
          MRU::clear(cfg::mruName);
        }
        break;
      }

      case act::resetCntSettings: {
        if (an->code != ACTN_ACTION) break;

        int count = Ctrl->IMessage(IMC_CNT_COUNT);
        for (int i = 0; i < count; i++) {
          SETCNTI(i, cfg::saveToHistory, 0);
          SETCNTI(i, cfg::disableOnMsgSend, 0);
          SETCNTI(i, cfg::ircCmds, 0);

          SETCNTI(i, cfg::reply::onEnable, 0);
          SETCNTI(i, cfg::reply::onDisable, 0);
          SETCNTI(i, cfg::reply::onMsg, 0);
          SETCNTI(i, cfg::reply::whenInvisible, 0);
          SETCNTI(i, cfg::reply::minInterval, -1);
          SETCNTI(i, cfg::reply::minIntervalType, -1);
          SETCNTI(i, cfg::reply::useHtml, 0);
          SETCNTI(i, cfg::reply::showInWnd, 0);

          SETCNTC(i, cfg::tpl::enable, "");
          SETCNTC(i, cfg::tpl::disable, "");
          SETCNTC(i, cfg::tpl::reply, "");
        }
        break;
      }
    }
  }

  void Controller::onStatusChange() {
    if (this->isEnabled() && !this->isAutoAway()) {
      statusCtrl->actionHandle(this->getIM());
    }
  }

  void Controller::onPluginsLoaded() {
    if (int oldId = Helpers::pluginExists(plugsNET::kaway)) {
      Ctrl->IMessage(&sIMessage_plugOut(oldId, "kAway2 jest nast�pc� wtyczki K.Away :)",
        sIMessage_plugOut::erNo, sIMessage_plugOut::euNowAndOnNextStart));
      return this->setFailure();
    }
    if (int ggCrypt = Helpers::pluginExists(plugsNET::ggcrypt)) {
      Ctrl->IMessage(&sIMessage_plugOut(ggCrypt, "Wtyczka GG Crypt jest przestarza�a, przy czym\n"
        "nie pozwala na poprawne dzia�anie wtyczki kAway2.",
        sIMessage_plugOut::erNo, sIMessage_plugOut::euNowAndOnNextStart));
      return this->setFailure();
    }
    this->setSuccess();
  }

  void Controller::onExtAutoAway() {
    this->setAutoAway(typeExtended);

    if (GETINT(cfg::autoAwaySync) == syncExtended && !this->isEnabled()) {
      this->enable(GETSTRA(cfg::tpl::autoAway), GETINT(cfg::extAutoAway::status), true);
    } else {
      this->changeStatus(GETINT(cfg::extAutoAway::status));
    }
    this->extAutoAwayTimer->stop();
  }

  void Controller::onAutoAway() {
    if (this->isEnabled()) return;

    int status = GETINT(cfg::status::onAutoAwaySt);
    int syncType = GETINT(cfg::autoAwaySync);

    statusCtrl->rememberInfo();
    this->setAutoAway(typeBasic);

    if (syncType == syncExtended || GETINT(cfg::extAutoAway::status) != -1) {
      this->extAutoAwayTimer->start((GETINT(cfg::extAutoAway::time) - GETINT(CFG_AUTOAWAY)) * 1000);
    }
    if (syncType == syncBasic) {
      this->enable(GETSTRA(cfg::tpl::autoAway), status, true);
    } else {
      this->changeStatus(status);
    }
  }

  void Controller::onBack() {
    if (!this->isAutoAway()) return;

    if (this->isEnabled()) {
      this->extAutoAwayTimer->stop();
      this->disable("", true);
    } else {
      statusCtrl->restoreInfo();
    }
    this->setAutoAway(typeDisabled);
  }

  /* API callback methods */

  void Controller::apiEnabled() {
    this->setReturnCode(this->isEnabled());
  }

  void Controller::apiEnable() {
    sIMessage_2params* msg = this->getIM();

    logDebug("Remote API Call [enable]: from = %s, msg = %s, status = %i", 
      Helpers::getPlugName(msg->sender), nullChk((char*)msg->p1), msg->p2);
    this->setReturnCode(this->enable((char*)msg->p1, msg->p2));
  }

  void Controller::apiDisable() {
    sIMessage_2params* msg = this->getIM();

    logDebug("Remote API Call [disable]: from = %s, msg = %s", 
      Helpers::getPlugName(msg->sender), nullChk((char*)msg->p1));
    this->setReturnCode(this->disable((char*)msg->p1));
  }

  void Controller::apiIgnored() {
    this->setReturnCode(this->cntProp(this->getIM()->p1)->ignored);
  }

  void Controller::apiAutoAway() {
    this->setReturnCode(this->isAutoAway());
  }

  void Controller::apiIgnore() {
    sIMessage_2params* msg = this->getIM();

    logDebug("Remote API Call [ignore]: from = %s, cnt = %i, ignore = %s", 
      Helpers::getPlugName(msg->sender), msg->p1, btoa((bool)msg->p2));
    if (this->isEnabled()) {
      this->cntProp(msg->p1)->ignored = (bool) msg->p2;
    }
  }

  void Controller::apiShowAwayWnd() {
    logDebug("Remote API Call [showAwayWnd]: from = %s",
      Helpers::getPlugName(this->getIM()->sender));
    wnd->show();
  }

  /* strictly Controller methods */

  bool Controller::enable(const StringRef& msg, int status, bool silent) {
    if (this->isOn) return false;

    this->awayMsg = msg;
    this->awayTime.now();
    this->isOn = true;

    if (this->autoAway) {
      this->awayTime = this->awayTime.getTime64() - GETINT(CFG_AUTOAWAY);
    }

    bool chgStatus = GETINT(this->isFromWnd ? cfg::wnd::changeOnEnable : cfg::status::changeOnEnable);
    bool chgInfo = GETINT(this->isFromWnd ? cfg::wnd::changeInfoOnEnable : cfg::status::changeInfoOnEnable);

    if (chgInfo) {
      statusCtrl->stringFormatter->addVar("date", this->awayTime.strftime(GETSTRA(cfg::dateFormat)));
      statusCtrl->stringFormatter->addVar("time", this->awayTime.strftime(GETSTRA(cfg::timeFormat)));
      statusCtrl->stringFormatter->addVar("msg", msg);
    }

    if (chgStatus || chgInfo) {
      int defStatus = GETINT(this->isFromWnd ? cfg::wnd::onEnableSt : cfg::status::onEnableSt);
      int tplCol = !chgInfo ? 0 : cfg::tpl::status;
      status = !chgStatus ? (this->autoAway ? status : -1) : (status ? status : defStatus);

      statusCtrl->rememberInfo();
      this->changeStatus(status, tplCol);
    }

    if (GETINT(this->isFromWnd ? cfg::wnd::muteOnEnable : cfg::muteOnEnable) && !GETINT(kSound::Cfg::mute)) {
      Helpers::UIActionCall(this->pluginsGroup, kSound::action::mute);
      this->muteStateSwitched = true;
    }

    this->switchBtns(true);
    this->fromWnd(false);

    int count = Ctrl->IMessage(IMC_CNT_COUNT);
    for (int i = 1; i < count; i++) {
      if (Helpers::isMsgWndOpen(i)) {
        if (Helpers::altCfgVal(i, cfg::reply::onEnable) && !silent && !this->cntProp(i)->ignored) {
          this->sendMsgTpl(i, tplEnable);
        }
      }
    }

    this->showKNotify("Tryb away zosta� <b>w��czony<b>", ico::enable);
    log("[Controller<%i>::enable()]: msg = %s, silent = %s", this, nullChk(msg), btoa(silent));
    Ctrl->IMessage(api::isAway, NET_BROADCAST, -1, (int) msg.c_str(), status);

    return true;
  }

  bool Controller::disable(const StringRef& msg, bool silent) {
    if (!this->isOn) return false;

    statusCtrl->stringFormatter->clearVars();
    statusCtrl->restoreInfo();

    if (this->muteStateSwitched && GETINT(kSound::Cfg::mute)) {
      Helpers::UIActionCall(this->pluginsGroup, kSound::action::mute);
    }

    this->switchBtns(false);

    int count = Ctrl->IMessage(IMC_CNT_COUNT);
    for (int i = 1; i < count; i++) {
      if (Helpers::isMsgWndOpen(i)) {
        if (Helpers::altCfgVal(i, cfg::reply::onDisable) && !silent && !this->cntProp(i)->ignored) {
          this->sendMsgTpl(i, tplDisable, msg);
        }
      }
    }

    for (tCnts::iterator it = this->cntProps.begin(); it != this->cntProps.end(); it++) {
      logDebug("[Controller::disable().saved]: cnt = %i, ignored = %s", it->first, btoa(it->second.ignored));
      for (tMsgQueue::iterator it2 = it->second.msgQueue.begin(); it2 != it->second.msgQueue.end(); it2++) {
        logDebug("[Controller::disable().saved.item]: msg = %s", (*it2)->body);
        messageFree((*it2), true);
      }
    }

    this->awayMsg = "";
    this->awayTime.clear();
    this->cntProps.clear();

    this->isOn = false;
    this->muteStateSwitched = false;

    this->showKNotify("Tryb away zosta� <b>wy��czony<b>", ico::disable);
    log("[Controller<%i>::disable()]: msg = %s, silent = %s", this, nullChk(msg), btoa(silent));
    Ctrl->IMessage(api::isBack, NET_BROADCAST, -1, (int) msg.c_str());

    return true;
  }

  void Controller::showKNotify(const char * text, int ico) {
    if (!GETINT(cfg::useKNotify) || !Helpers::pluginExists(plugsNET::knotify)) return;
    Helpers::showKNotify((char*) text, ico);
  }

  void Controller::switchBtns(bool state) {
    int ico = (state) ? ico::disable : ico::enable;
    char * name = (state) ? "Wy��cz tryb away" : "W��cz tryb away";

    Helpers::chgBtn(IMIG_TRAY, ui::powerInTrayMenu, name, ico);
    Helpers::chgBtn(this->pluginsGroup, ui::powerInMainWnd, name, ico, 
      (state && (Helpers::findParentAction(IMIG_MAINWND, this->pluginsGroup) != IMIG_MAINTB)) ? ACTS_CHECKED : 0);
    Helpers::chgBtn(IMIG_MSGTB, ui::msgTbGrp, "kAway2", ico::logoSmall, ACTR_INIT | ACTS_GROUP | (state ? ACTS_CHECKED : 0));

    int count = Ctrl->IMessage(IMC_CNT_COUNT);
    for (int i = 1; i < count; i++) {
      if (Helpers::isMsgWndOpen(i)) {
        Helpers::chgBtn(IMIG_MSGTB, ui::msgTbGrp, i, "kAway2", ico::logoSmall, ACTR_INIT | ACTS_GROUP | (state ? ACTS_CHECKED : 0));
      }
    }
    delete [] name;
  }

  void Controller::changeStatus(int status, int tplCol) {
    int actSt, chgOnlyIfOnline = (this->autoAway == typeBasic) && GETINT(cfg::status::chgOnlyIfOnline);
    NetList::tNets nets = statusList->getNets();

    for (NetList::tNets::iterator it = nets.begin(); it != nets.end(); it++) {
      actSt = statusCtrl->getActualStatus(it->net);
      if (!statusCtrl->isNetValid(it->net) || (chgOnlyIfOnline && (actSt != ST_ONLINE && actSt != ST_CHAT))) {
        continue;
      }
      if (tplCol) {
        statusCtrl->changeStatus(it->net, GETSTRA(tplCol), status);
      } else if (status != -1) {
        statusCtrl->changeStatus(it->net, status);
      }
    }
  }

  void Controller::sendMsgTpl(int cnt, enAutoMsgTpl tpl, const StringRef& msgVal) {
    int session, net = GETCNTI(cnt, CNT_NET);

    if (((statusCtrl->getActualStatus(net) == ST_HIDDEN) && !Helpers::altCfgVal(cnt, cfg::reply::whenInvisible)) || 
      !autoReplyList->getNetState(net) || !autoReplyList->isConnected(net))
      return;

    String ext, uid(GETCNTC(cnt, CNT_UID));
    ext = SetExtParam(ext, cfg::extParamName, itos(tpl));
    ext = SetExtParam(ext, MEX_ADDINFO, "kAway2");
    ext = SetExtParam(ext, MEX_NOSOUND, "1");

    Format format;
    format.addVar("uid", uid);
    format.addVar("display", GETCNTC(cnt, CNT_DISPLAY));
    format.addVar("name", GETCNTC(cnt, CNT_NAME));
    format.addVar("nick", GETCNTC(cnt, CNT_NICK));
    format.addVar("surname", GETCNTC(cnt, CNT_SURNAME));
    format.addVar("date", Helpers::isToday(this->awayTime) ? "" : this->awayTime.strftime(GETSTRA(cfg::dateFormat)));
    format.addVar("time", this->awayTime.strftime(GETSTRA(cfg::timeFormat)));
    format.addVar("msg", (tpl == tplDisable) ? msgVal : this->awayMsg);

    /*
    String body = Helpers::trim(format.parse(Helpers::altCfgStrVal(cnt, tpl)));
    cMessage msg = Message::prepare(uid, "", net, body, MT_MESSAGE, ext, 
      MF_SEND | (Helpers::altCfgVal(cnt, cfg::reply::useHtml) ? MF_HTML : 0));

    HWND hwndMsg = (HWND) UIGroupHandle(sUIAction(0, IMIG_MSGWND, cnt));
    if (!(session = (int) GetProp(hwndMsg, "MsgSession"))) {
      SetProp(hwndMsg, "MsgSession", (HANDLE) 1);
    }

    Helpers::addItemToHistory(&msg, cnt, "messages", "", session);
    Message::send(&msg);

    if (Helpers::altCfgVal(cnt, cfg::reply::showInWnd)) {
      Message::inject(&msg, cnt);
    }
    */
    Message::send(cnt, Helpers::trim(format.parse(Helpers::altCfgStrVal(cnt, tpl))), 
      MT_MESSAGE, ext, Helpers::altCfgVal(cnt, cfg::reply::useHtml));

    logDebug("[Controller::sendMsgTpl()]: msg.tpl = %i", tpl);
  }
}