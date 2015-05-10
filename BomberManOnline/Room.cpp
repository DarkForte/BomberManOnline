#include "stdafx.h"
#include "Room.h"
#include "D2D1Header.h"

using namespace std;

CRoom::CRoom(CResourceManager* p_res_manager)
{
	this->p_res_manager = p_res_manager;
	button[1].Init(1150, 120, 130, 80, 1, 1);
	button[2].Init(1150, 220, 130, 80, 1, 1);
	button[3].Init(1150, 320, 130, 80, 1, 1);
	button[4].Init(1150, 420, 130, 80, 1, 1);
	//back
	button[5].Init(1150, 520, 130, 80, 1, 1);
	//player
	button[6].Init(25, 211, 400, 105, 1, 1);
	button[7].Init(435, 211, 400, 105, 1, 1);
	button[8].Init(25, 326, 400, 105, 1, 1);
	button[9].Init(435, 326, 400, 105, 1, 1);
	//ready
	button[10].Init(885, 211, 245, 127, 1, 1);
	
	button[11].Init(885, 211, 245, 127, 1, 1);
	button[11].SetStatus(BUTTON_STATUS::DISABLE);

	//actor
	button[12].Init(25, 488, 120, 120, 1, 1);
	button[13].Init(163, 488, 120, 120, 1, 1);
	button[14].Init(301, 488, 120, 120, 1, 1);
	button[15].Init(439, 488, 120, 120, 1, 1);
	button[16].Init(577, 488, 120, 120, 1, 1);
	button[17].Init(715, 488, 120, 120, 1, 1);

	chat.Init(PointF(885, 582), "message", 245, 26, true, false, 10,true,188);
	chat.setClient(p_res_manager);
	chat.setIsChat(true);

	isMessage = false;
	msg_button.Init(838, 365, 90, 30, 1, 1);
}

GameState CRoom::HandleLButtonDown(CPoint point)
{
	if (isMessage)
	{
		if (point.x >= msg_button.GetXPixel() &&
			point.x <= msg_button.GetXPixel() + msg_button.GetWidth() &&
			point.y >= msg_button.GetYPixel() &&
			point.y <= msg_button.GetYPixel() + msg_button.GetHeight())
		{
			msg_button.SetStatus(BUTTON_STATUS::MOUSE_DOWN);
		}
	}
	else
	{
		for (int i = 1; i <= ROOM_MAX_BUTTON; i++)
		{
			if (point.x >= button[i].GetXPixel() &&
				point.x <= button[i].GetXPixel() + button[i].GetWidth() &&
				point.y >= button[i].GetYPixel() &&
				point.y <= button[i].GetYPixel() + button[i].GetHeight())
			{
				if (button[i].GetStatus() != BUTTON_STATUS::DISABLE)
				{
					button[i].SetStatus(BUTTON_STATUS::MOUSE_DOWN);
				}
			}
		}
	}
	return GameState::LOBBY;
}

GameState CRoom::HandleLButtonMove(CPoint point)
{
	if (isMessage)
	{
		if (point.x >= msg_button.GetXPixel() &&
			point.x <= msg_button.GetXPixel() + msg_button.GetWidth() &&
			point.y >= msg_button.GetYPixel() &&
			point.y <= msg_button.GetYPixel() + msg_button.GetHeight())
		{
			msg_button.SetStatus(BUTTON_STATUS::MOUSE_ON);
		}
		else
		{
			msg_button.SetStatus(BUTTON_STATUS::IDLE);
		}
	}
	else
	{
		for (int i = 1; i <= ROOM_MAX_BUTTON; i++)
		{
			if (point.x >= button[i].GetXPixel() &&
				point.x <= button[i].GetXPixel() + button[i].GetWidth() &&
				point.y >= button[i].GetYPixel() &&
				point.y <= button[i].GetYPixel() + button[i].GetHeight())
			{
				if (button[i].GetStatus() != BUTTON_STATUS::DISABLE)
				{
					button[i].SetStatus(BUTTON_STATUS::MOUSE_ON);
				}
			}
			else
			{
				if (button[i].GetStatus() != BUTTON_STATUS::DISABLE)
				{
					button[i].SetStatus(BUTTON_STATUS::IDLE);
				}
			}
		}
	}
	return GameState::LOBBY;
}

GameState CRoom::HandleLButtonUp(CPoint point)
{
	GameState state = GameState::ROOM;
	
	if (isMessage)
	{
		if (point.x >= msg_button.GetXPixel() &&
			point.x <= msg_button.GetXPixel() + msg_button.GetWidth() &&
			point.y >= msg_button.GetYPixel() &&
			point.y <= msg_button.GetYPixel() + msg_button.GetHeight())
		{
			isMessage = false;
		}
	}
	else
	{
		for (int i = 1; i <= ROOM_MAX_BUTTON; i++)
		{
			if (point.x >= button[i].GetXPixel() &&
				point.x <= button[i].GetXPixel() + button[i].GetWidth() &&
				point.y >= button[i].GetYPixel() &&
				point.y <= button[i].GetYPixel() + button[i].GetHeight())
			{
				//setting
				if (i == 3)
				{
					isMessage = true;
					msg_string = "IP:";
					msg_string.Append(p_res_manager->m_Client.GetIp());
					msg_string2.Format(L"Port:%d", p_res_manager->m_Client.GetPort());
				}
				//help
				else if (i == 4)
				{
					isMessage = true;
					msg_string = "Move:↑↓←→";
					msg_string2 = "Bomb:Space Tool:1~6";
				}
				//back
				else if (i == 5)
				{
					//定义发送消息/接收消息
					CMessage msg, recv_msg;

					//初始化消息类型
					msg.type1 = MSG_ROOM;
					msg.type2 = MSG_ROOM_EXIT;

					//设置参数
					msg.para1 = p_res_manager->account.room_id;
					msg.para2 = p_res_manager->account.seat_id;

					//发送消息
					recv_msg = p_res_manager->m_Client._SendMessage(msg);

					state = GameState::LOBBY;
				}
				//not ready
				else if (i == 10 && button[i].GetStatus() != BUTTON_STATUS::DISABLE)
				{
					button[10].SetStatus(BUTTON_STATUS::DISABLE);
					button[11].SetStatus(BUTTON_STATUS::IDLE);
					p_res_manager->account.ready = true;
					break;
				}
				//ready
				else if (i == 11 && button[i].GetStatus() != BUTTON_STATUS::DISABLE)
				{
					button[11].SetStatus(BUTTON_STATUS::DISABLE);
					button[10].SetStatus(BUTTON_STATUS::IDLE);
					p_res_manager->account.ready = false;
				}
				//choose actor
				else if (i >= 12 && i <= 17)
				{
					p_res_manager->account.actor_id = i - 12;
				}
			}
		}
	}
	return state;
}

void CRoom::Render(ID2D1HwndRenderTarget* render_target)
{
	ID2D1SolidColorBrush* brush;
	render_target->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &brush);

	p_res_manager->room_ui.DrawImage(render_target, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 0, 0);

	for (int i = 1; i <= ROOM_MAX_BUTTON; i++)
	{
		if (button[i].GetStatus() == BUTTON_STATUS::IDLE && i - 12!=p_res_manager->account.actor_id)
		{
			p_res_manager->room_button_sprite[i][0].DrawImage(render_target,
				button[i].GetXPixel(), button[i].GetYPixel(),
				button[i].GetWidth(), button[i].GetHeight(),
				0, 0,
				button[i].GetXScale(), button[i].GetYScale());
		}
		else if (button[i].GetStatus() == BUTTON_STATUS::MOUSE_ON || i - 12 == p_res_manager->account.actor_id)
		{
			p_res_manager->room_button_sprite[i][1].DrawImage(render_target,
				button[i].GetXPixel(), button[i].GetYPixel(),
				button[i].GetWidth(), button[i].GetHeight(),
				0, 0,
				button[i].GetXScale(), button[i].GetYScale());
		}
		else if (button[i].GetStatus() == BUTTON_STATUS::MOUSE_DOWN)
		{
			p_res_manager->room_button_sprite[i][2].DrawImage(render_target,
				button[i].GetXPixel(), button[i].GetYPixel(),
				button[i].GetWidth(), button[i].GetHeight(),
				0, 0,
				button[i].GetXScale(), button[i].GetYScale());
		}
	}

	chat.Draw(render_target, p_res_manager);

	//white background
	p_res_manager->room_icon_sprite[1].DrawImage(render_target, 25, 211, 105, 105, 0, 0);
	p_res_manager->room_icon_sprite[1].DrawImage(render_target, 435, 211, 105, 105, 0, 0);
	p_res_manager->room_icon_sprite[1].DrawImage(render_target, 25, 326, 105, 105, 0, 0);
	p_res_manager->room_icon_sprite[1].DrawImage(render_target, 435, 326, 105, 105, 0, 0);
	//user image
	p_res_manager->room_icon_sprite[2].DrawImage(render_target, 786, 56, 64, 64, 0, 0);
	//player icon
	p_res_manager->room_icon_sprite[2].DrawImage(render_target, 43, 230, 67, 67, 0, 0);
	p_res_manager->room_icon_sprite[2].DrawImage(render_target, 453, 230, 67, 67, 0, 0);
	p_res_manager->room_icon_sprite[2].DrawImage(render_target, 43, 345, 67, 67, 0, 0);
	p_res_manager->room_icon_sprite[2].DrawImage(render_target, 453, 345, 67, 67, 0, 0);

	wstring o_text;
	CString text;
	ID2D1SolidColorBrush* brush_black;
	ID2D1SolidColorBrush* brush_red;
	render_target->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), &brush_black);
	render_target->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Red), &brush_red);

	//lobby text
	text.Format(L"ROOM %d",p_res_manager->account.room_id+1);
	o_text = text.GetString();
	RenderText(render_target, o_text, 115, 48, p_res_manager->p_text_format_Arial_72_bold, brush);
	

	//user details
	text = p_res_manager->account.user_name;
	o_text = text.GetString();

	if (p_res_manager->account.VIP)
	{
		RenderText(render_target, o_text, 860, 51, p_res_manager->p_text_format_Arial_32_bold, brush_red);
	}
	else
	{
		RenderText(render_target, o_text, 860, 51, p_res_manager->p_text_format_Arial_32_bold, brush);
	}

	text.Format(L"EXP:%d", p_res_manager->account.exp);
	o_text = text.GetString();
	RenderText(render_target, o_text, 860, 88, p_res_manager->p_text_format_Arial_32_bold, brush);

	text.Format(L"$:%d", p_res_manager->account.money);
	o_text = text.GetString();
	RenderText(render_target, o_text, 978, 88, p_res_manager->p_text_format_Arial_32_bold, brush);

	//player number text
	text.Format(L"01");
	o_text = text.GetString();
	RenderText(render_target, o_text, 141, 222, p_res_manager->p_text_format_Arial_32_bold, brush);

	text.Format(L"02");
	o_text = text.GetString();
	RenderText(render_target, o_text, 551, 222, p_res_manager->p_text_format_Arial_32_bold, brush);

	text.Format(L"03");
	o_text = text.GetString();
	RenderText(render_target, o_text, 141, 337, p_res_manager->p_text_format_Arial_32_bold, brush);

	text.Format(L"04");
	o_text = text.GetString();
	RenderText(render_target, o_text, 551, 337, p_res_manager->p_text_format_Arial_32_bold, brush);

	//player name text
	if (p_res_manager->account.seat[0] != 0)
	{
		o_text = p_res_manager->account.seat_name[0].GetString();
	}
	else
	{
		text.Format(L"NO PLAYER");
		o_text = text.GetString();
	}
	RenderText(render_target, o_text, 187, 222, p_res_manager->p_text_format_Arial_32_bold, brush);

	if (p_res_manager->account.seat[1] != 0)
	{
		o_text = p_res_manager->account.seat_name[1].GetString();
	}
	else
	{
		text.Format(L"NO PLAYER");
		o_text = text.GetString();
	}
	RenderText(render_target, o_text, 597, 222, p_res_manager->p_text_format_Arial_32_bold, brush);

	if (p_res_manager->account.seat[2] != 0)
	{
		o_text = p_res_manager->account.seat_name[2].GetString();
	}
	else
	{
		text.Format(L"NO PLAYER");
		o_text = text.GetString();
	}
	RenderText(render_target, o_text, 187, 337, p_res_manager->p_text_format_Arial_32_bold, brush);

	if (p_res_manager->account.seat[3] != 0)
	{
		o_text = p_res_manager->account.seat_name[3].GetString();
	}
	else
	{
		text.Format(L"NO PLAYER");
		o_text = text.GetString();
	}
	RenderText(render_target, o_text, 597, 337, p_res_manager->p_text_format_Arial_32_bold, brush);

	//ready
	text.Format(L"READY");
	o_text = text.GetString();
	RenderText(render_target, o_text, 938, 251, p_res_manager->p_text_format_Arial_40_bold, brush);

	SafeRelease(&brush);

	if (isMessage)
	{
		RenderMessage(render_target);
	}

	return;
}

void CRoom::HandleKeyDown(UINT nchar)
{
	chat.HandleKeyDown(nchar);
}

GameState CRoom::Update()
{
	GameState state = GameState::ROOM;
	
	//定义发送消息/接收消息
	CMessage msg, recv_msg;

	//update player name

	//初始化消息类型
	msg.type1 = MSG_ROOM;
	msg.type2 = MSG_ROOM_NAME;

	//设置参数
	msg.para1 = p_res_manager->account.room_id;
	for (int j = 0; j < 4; j++)
	{
		msg.para2 = j;

		//发送消息
		recv_msg = p_res_manager->m_Client._SendMessage(msg);
		if (recv_msg.type1 == MSG_ROOM && recv_msg.type2 == MSG_ROOM_RETURN)
		{
			p_res_manager->account.seat[j] = recv_msg.para1;
			USES_CONVERSION;
			p_res_manager->account.seat_name[j] = CA2T(recv_msg.str1);
		}
		else if (recv_msg.type1 == MSG_ROOM && recv_msg.type2 == MSG_ROOM_EMPTY)
		{
			p_res_manager->account.seat[j] = 0;
			p_res_manager->account.seat_name[j] = "";
		}
	}

	//update if game start

	if (p_res_manager->account.ready)
	{
		//初始化消息类型
		msg.type1 = MSG_ROOM;
		msg.type2 = MSG_ROOM_READY;

		//设置参数
		msg.para1 = p_res_manager->account.room_id;
		msg.para2 = p_res_manager->account.seat_id;

		//发送消息
		recv_msg = p_res_manager->m_Client._SendMessage(msg);
		if (recv_msg.type1 == MSG_ROOM && recv_msg.type2 == MSG_ROOM_GAME)
		{
			state = GameState::INGAME;
			button[11].SetStatus(BUTTON_STATUS::DISABLE);
			button[10].SetStatus(BUTTON_STATUS::IDLE);
			p_res_manager->account.ready = false;
			//isMessage = true;
			//msg_string = "Start Game";
		}
		else
		{

		}
	}

	//update chat

	//初始化消息类型
	msg.type1 = MSG_CHAT;
	msg.type2 = MSG_CHAT_GET;

	//设置参数
	msg.para1 = chat.getTotalLineNum();

	//发送消息
	recv_msg = p_res_manager->m_Client._SendMessage(msg);
	while (recv_msg.type2 != MSG_CHAT_DENY)
	{
		if (recv_msg.type2 == MSG_NULL)
		{
			break;
		}
		CString str_tmp;
		USES_CONVERSION;
		str_tmp.Append(CA2T(recv_msg.str1));
		str_tmp.Append(L":");
		str_tmp.Append(CA2T(recv_msg.str2));
		chat.AddMessage(str_tmp);

		//设置参数
		msg.para1 = chat.getTotalLineNum();

		recv_msg = p_res_manager->m_Client._SendMessage(msg);
	}

	return state;
}