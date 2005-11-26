/*
 *  kAway
 *  
 *  Please READ /License.txt FIRST! 
 * 
 *  Copyright (C)2005 Sijawusz Pur Rahnama
 *
 *  $Id: $
 */

#pragma once

int ignoredNets[] = 
{
	plugsNET::konnferencja,
	plugsNET::checky,
  plugsNET::klan
};

kAway::Status::Status(const int _cfgCol)
{
	this->cfgCol = _cfgCol;
}

kAway::Status::~Status()
{
	this->SaveNets();
	this->info.clear();
	this->nets.clear();
}

void kAway::Status::ChangeStatus( const char *txt, int st )
{
	std::string buff, status(txt);
	itemNet item;

	// Je¿eli nic nie ma w txt to nie zmieniamy statusu
	if (status.length() == 0)
		return;

	for (std::list<itemNet>::iterator it = this->nets.begin(); it != this->nets.end(); it++)
	{
		item = *it;

		if ( item.net == plugsNET::klan && ( ST_OFFLINE != IMessage( IM_GET_STATUS, item.net ) )  )
		{
			if ( item.use )
			{
				buff = this->Format( status, item.net );
				buff = LimitChars( buff, item.net );

				if( this->prevStat != st || this->prevStat != buff )
				{
					this->prevStat.st = st;
					this->prevStat.info = buff;

					if (this->onHidden)
						IMessage( IM_CHANGESTATUS , item.net , IMT_PROTOCOL , st , (int)buff.c_str());
					else if ( ST_HIDDEN != IMessage( IM_GET_STATUS, item.net ))
						IMessage( IM_CHANGESTATUS , item.net , IMT_PROTOCOL , st , (int)buff.c_str());
				}
			}
		}
		else if (IMessage( IM_ISCONNECTED , item.net , IMT_PROTOCOL ))
		{
			if ( item.use )
			{
        //IMLOG("[%i]: %s", status.c_str(), st);
				buff = this->Format( status, item.net );

				if (this->onHidden)
					IMessage( IM_CHANGESTATUS , item.net , IMT_PROTOCOL , st , (int)buff.c_str());
				else if ( ST_HIDDEN != IMessage( IM_GET_STATUS, item.net ))
					IMessage( IM_CHANGESTATUS , item.net , IMT_PROTOCOL , st , (int)buff.c_str());
			}
		}
	}
}

void kAway::Status::ChangeStatus(int net, const char *txt, int st )
{
	std::string buff, status(txt);

	// Je¿eli nic nie ma w txt to nie zmieniamy statusu
	if (status.length() == 0)
		return;

	if ( net == plugsNET::klan && ( ST_OFFLINE != IMessage( IM_GET_STATUS, net ) )  )
	{
		if (this->onHidden)
			IMessage( IM_CHANGESTATUS , net , IMT_PROTOCOL , st , (int)buff.c_str());
		else if ( ST_HIDDEN != IMessage( IM_GET_STATUS, net ))
			IMessage( IM_CHANGESTATUS , net , IMT_PROTOCOL , st , (int)buff.c_str());
	}
	else if (IMessage( IM_ISCONNECTED , net , IMT_PROTOCOL ))
	{
		if (this->onHidden)
			IMessage( IM_CHANGESTATUS , net , IMT_PROTOCOL , st , (int)buff.c_str());
		else if ( ST_HIDDEN != IMessage( IM_GET_STATUS, net ))
			IMessage( IM_CHANGESTATUS , net , IMT_PROTOCOL , st , (int)buff.c_str());
	}
}

std::string kAway::Status::LimitChars( std::string Status, int net, int s )
{
	UINT iLimit;

	switch (net)
	{
	case plugsNET::gg:
			iLimit = GG_STATUS_LENGTH;
			break;
	case plugsNET::dwutlenek:
			iLimit = TLEN_STATUS_LENGTH;
			break;
	case plugsNET::kjabber:
	case plugsNET::kjabber1:
	case plugsNET::kjabber2:
	case plugsNET::kjabber3:
	case plugsNET::kjabber4:
	case plugsNET::kjabber5:
	case plugsNET::kjabber6:
	case plugsNET::kjabber7:
	case plugsNET::kjabber8:
	case plugsNET::kjabber9:
	case plugsNET::kjabber10:
			iLimit = JABBER_STATUS_LENGTH;
			break;
	default:
			iLimit = DEFAULT_STATUS_LENGTH;
			break;
	}

  iLimit = iLimit - s;
	if ( Status.length() > iLimit )
	{
		Status.resize( iLimit );
		Status.replace( Status.length()-3, 3, "...");
	}
	return Status;
}

void kAway::Status::Init()
{
	this->GetNets();
}

int kAway::Status::GetNets(bool mask)
{
	int id, type, net, a=0;
	char *txt;
	int plugs = IMessage( IMC_PLUG_COUNT );
	itemNet cfg;
	int k=0;

	for (int i = 1; i < plugs ; i++ )
	{
		id = IMessage( IMC_PLUG_ID, 0, 0, i );
		type = IMessageDirect( IM_PLUG_TYPE, id );

		if ( (type & IMT_NET) == IMT_NET )
		{
			net = (int)IMessageDirect( IM_PLUG_NET, id );

			if ( !this->IsIgnored( net ) )
			{
				txt = (char*)IMessageDirect( IM_PLUG_NETNAME, id );
				if (txt)
				{
					cfg.net = net;
					cfg.use = true;
					if(mask)
						this->nets.push_back( cfg );
					k++;
				}
			}
		}
	}
	return k;
}

void kAway::Status::SaveNets()
{
	std::string buf;
	itemNet net;
	char v[10];

	for( std::list<itemNet>::iterator it = this->nets.begin(); it != this->nets.end(); it++ )
	{
		net = *it;
		buf += itoa( net.net, v, 10 );
		buf += "|";
		buf += net.use?"1":"0";
		buf += "|";
	}

	SETSTR( this->cfgCol, buf.c_str() );
}

int kAway::Status::LoadNets()
{
	std::string buf( GETSTRA( this->cfgCol ) );
	itemNet net;
	int pos = (int)buf.find( "|" );
	int off = 0;
	int i=0;

	while ( pos != std::string::npos )
	{
		net.net = atoi( buf.substr(off, pos-off).c_str() );
		off = pos + 1;
		pos = (int)buf.find( "|", off );
		net.use = atoi( buf.substr( off, pos-off).c_str() )?true:false;
		off = pos + 1;
		pos = (int)buf.find( "|", off );
		this->nets.push_back( net );
		i++;
	}
	return i;
}

bool kAway::Status::NetState( int net )
{
	itemNet v;
	for( std::list<itemNet>::iterator it = this->nets.begin(); it != this->nets.end(); it++ )
	{
		v = *it;
		if (v.net == net)
			return v.use;
	}
	return false;
}

bool kAway::Status::SetNet( int net, bool use )
{
	itemNet v;
	for( std::list<itemNet>::iterator it = this->nets.begin(); this->nets.end() != it; it++ )
	{
		v = *it;
		if (v.net == net && v.use != use)
		{
			v.use = use;
			*it = v;
			return true;
		}
	}
	return false;
}

void kAway::Status::AddInfo( char *info, int net )
{
	itemInfo in;
	std::string txt(info);
	bool t = false;

	for( std::list<itemInfo>::iterator it = this->info.begin(); it != this->info.end() && !t; it++ )
	{
		in = *it;
		if( in.net == net && in.info.compare( info ) )
		{
			t = true;
			in.info = txt;
			*it = in;
		}
	}
	
	if(!t)
	{
		in.info = txt;
		in.net = net;
        this->info.push_back( in );
	}
}

void kAway::Status::RememberInfo()
{
	char *st = 0;
	itemNet item;

	for (std::list<itemNet>::iterator it = this->nets.begin(); it != this->nets.end(); it++)
	{
		item = *it;

		if ( item.net == plugsNET::klan && ( ST_OFFLINE != IMessage( IM_GET_STATUS, item.net ) ) )
		{
			if ( this->onHidden )
			{
				st = (char*)IMessage(IM_GET_STATUSINFO, item.net);

				if (st)
					this->AddInfo( st, item.net );
			}
			else if (ST_HIDDEN != IMessage( IM_GET_STATUS, item.net ))
			{
				st = (char*)IMessage(IM_GET_STATUSINFO, item.net);

				if (st)
					this->AddInfo( st, item.net );
			}
		}
		else if (IMessage( IM_ISCONNECTED , item.net , IMT_PROTOCOL )  )
		{
			if ( this->onHidden )
			{
				st = (char*)IMessage(IM_GET_STATUSINFO, item.net);

				if (st)
					this->AddInfo( st, item.net );
			}
			else if (ST_HIDDEN != IMessage( IM_GET_STATUS, item.net ))
			{
				st = (char*)IMessage(IM_GET_STATUSINFO, item.net);

				if (st)
					this->AddInfo( st, item.net );
			}
		}
	}
}

void kAway::Status::RememberInfo( int net )
{
	char *st = NULL;

	if ( net == plugsNET::klan && ( ST_OFFLINE != IMessage( IM_GET_STATUS, net ) ) )
	{
		if ( this->onHidden )
		{
			st = (char*)IMessage(IM_GET_STATUSINFO, net);

			if (st)
				this->AddInfo( st, net );
		}
		else if (ST_HIDDEN != IMessage( IM_GET_STATUS, net ))
		{
			st = (char*)IMessage(IM_GET_STATUSINFO, net);

			if (st)
				this->AddInfo( st, net );
		}
	}
	else if (IMessage( IM_ISCONNECTED , net , IMT_PROTOCOL )  )
	{
		if ( this->onHidden )
		{
			st = (char*)IMessage(IM_GET_STATUSINFO, net);

			if (st)
				this->AddInfo( st, net );
		}
		else if (ST_HIDDEN != IMessage( IM_GET_STATUS, net ))
		{
			st = (char*)IMessage(IM_GET_STATUSINFO, net);

			if (st)
				this->AddInfo( st, net );
		}
	}
}

std::string kAway::Status::GetInfo( int net )
{
	std::string t("");
	itemInfo v;
	for( std::list<itemInfo>::iterator it = this->info.begin(); it != this->info.end(); it++ )
	{
		v = *it;
		if( v.net == net )
			return v.info;
	}
	return t;
}

void kAway::Status::BackInfo( int net )
{
	std::string status;

	if ( !this->IsIgnored( net ) )
	{
		if ( net == plugsNET::klan && ( ST_OFFLINE != IMessage( IM_GET_STATUS, net ) ) && ( ST_HIDDEN != IMessage( IM_GET_STATUS, net )) )
		{
			if ( this->NetState( net ) )
			{
				status = this->GetInfo( net );
				IMessage( IM_CHANGESTATUS , net , IMT_PROTOCOL , -1 , (int)status.c_str());
			}
		}
		else if (IMessage( IM_ISCONNECTED , net , IMT_PROTOCOL ) && ( ST_HIDDEN != IMessage( IM_GET_STATUS, net )))
		{
			if ( this->NetState( net ) )
			{
				status = this->GetInfo( net );
				IMessage( IM_CHANGESTATUS , net , IMT_PROTOCOL , -1 , (int)status.c_str());
			}
		}
	}
}

void kAway::Status::BackInfo()
{
	itemInfo v;

	for( std::list<itemInfo>::iterator it = this->info.begin(); it != this->info.end(); it++ )
	{
		v = *it;
		if ( this->NetState( v.net ) )
			IMessage( IM_CHANGESTATUS , v.net , IMT_PROTOCOL , -1 , (int)v.info.c_str());
	}
}

void kAway::Status::BackRemeberedInfo()
{
	itemInfo v;
	for( std::list<itemInfo>::iterator it = this->info.begin(); it != this->info.end(); it++ )
	{
		v = *it;
		this->BackInfo( v.net );
	}
}

bool kAway::Status::IsIgnored( int net )
{
	bool is = false;
	for (int i = 0; i < sizeof(ignoredNets)/sizeof(int) && !is; i++)
		if (net == ignoredNets[i])
			is = true;
	return is;
}

std::string kAway::CtrlStatus::Format( std::string txt, int net )
{
  //cPreg *preg = new cPreg();
  //
  //preg->setPattern("/\{([^a-z0-9]*)([a-z0-9]+)([^a-z0-9]*)\}/i");
  //preg->setSubject(txt);
  //preg->reset();

  //string result, buff;
  //int ret;
  //while( (ret = preg->match_global()) > 0)
  //{
	 // // na razie jest przystosowane do tego,
	 // // ¿e nie ma fsd dsf d, Sija jak poprawisz
	 // // to dojdzie jeden subs[i] na syf z poczatku
	 // // i jeden subs[i] na syf na koncu
  //  if ( this->GetVar( preg->subs[2], buff ) ) {
	 //   result += preg->subs[1];
		//  result += buff;
	 //   result += preg->subs[3];
  //  }
  //}
  //preg->reset();

  //delete preg; preg = NULL;
  return(txt);
}