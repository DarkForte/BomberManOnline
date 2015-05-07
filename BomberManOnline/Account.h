#pragma once
#include "stdafx.h"
using namespace std;

class CAccount
{
public:
	int user_id;
	CString user_name;
	CString seat_name[4];
	int room_id;
	int seat_id;
	bool ready;
	int actor_id;
	bool VIP;
	int money;
	int seat[4];
	pair<CString, CString> chat_message[100];
	int chat_num;
	int exp;
};