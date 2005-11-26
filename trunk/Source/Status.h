/*
 *	Status class - zmiana statusu
 *	v. 0.1.2.2
 *
 *	Wykorzystywanie tylko za zgod¹ twórcy,
 *	Twórca za nic nie odpowiada.
 *
 *  (C)2005 Winthux
 *
 *  $Id: $
 */

#pragma once

#define GG_STATUS_LENGTH 70
#define TLEN_STATUS_LENGTH 255
#define JABBER_STATUS_LENGTH 255
#define DEFAULT_STATUS_LENGTH 255

#include <list>

/*
 *	TODO:
 *	- niech zapamiêtuje status i opis (czy warto status?), ¿eby póŸniej przywróciæ
 *
 */

namespace kAway {
	struct itemNet {
		bool use;
		int net;
	};

	struct itemInfo {
    std::string info;
		int net;
	};

	struct StatInfo {
    std::string info;
		int st;

		bool operator == (const StatInfo a) const {
			return (a.st == this->st && !a.info.compare(this->info)) ? true : false;
		}

		bool operator != (const StatInfo a) const {
			return (a.st != this->st || a.info.compare(this->info)) ? true : false;
		}

		bool operator == (const int _st) const {
			return (this->st == _st) ? true : false;
		}

		bool operator != (const int _st) const {
			return (this->st != _st) ? true : false;
		}

    bool operator == (std::string _info) const {
			return (!_info.compare(this->info)) ? true : false;
		}

    bool operator != (std::string _info) const {
			return (_info.compare(this->info)) ? true : false;
		}
	};

	class Status {
	  public:
		  Status(const int cfgCol);
		  ~Status();

	  public:
		  // Inicjuje listê sieci
		  void Init();
		  // Zmienia status, txt - opis, st - id statusu
		  void ChangeStatus(const char *txt, int st = -1);
		  // Zmienia status na wybranej sieci
		  void ChangeStatus(int net, const char *txt, int st = -1);
		  // Ustawia wartoœæ use dla sieci net
		  bool SetNet(int net, bool use);
		  // Zapamiêtuje aktualny opis na ka¿dej sieci
		  void RememberInfo();
		  // Zapamiêtuje aktualny opis na wybranej sieci
		  void RememberInfo(int net);
		  // Przywraca opis na ka¿dej sieci
		  void BackInfo();
		  // Przywraca opis na wybranej sieci
		  void BackInfo(int net);
		  // Przywraca zapamiêtane opisy
		  void BackRemeberedInfo();

		  // lista z sieciami
      std::list<itemNet> nets;

      std::string LimitChars(std::string Status, int net, int s = 0);

	  protected:
      virtual std::string Format(std::string txt, int net) = 0;

	  private:
		  /* Sieci */
		  // Szuka sieci w konnekcie
		  int GetNets(bool mask = true);
		  // Odczytuje z konfigruacji jakie sieci sa w konnekcie
		  int LoadNets();
		  // zapisuje sieci do konfiguracji
		  void SaveNets();
		  // zwraca czy obslugujemy siec net
		  bool NetState(int net);
		  // zwraca czy siec jest ignorowana
		  bool IsIgnored(int net);

	  public:
		  /* Opisy */
		  // Dodaje info do listy
		  void AddInfo(char *info, int net);
		  // Zwraca opis sieci net
		  std::string GetInfo(int net);

	  private:
		  // Id kolumny zapisuj¹cej, na których sieciach zmieniac status
		  int cfgCol;
		  // Czy ma zmieniaæ opis przy Ukrytym?
		  bool onHidden;
		  // lista z opisami na sieciach
		  std::list<itemInfo> info;
		  // poprzedni opis
		  StatInfo prevStat;
	};

	class CtrlStatus : public kAway::Status {
	  public:
		  CtrlStatus(int _cfgCol) : Status(_cfgCol) {}
	  protected:
      std::string Format(std::string txt, int net);
	};
}