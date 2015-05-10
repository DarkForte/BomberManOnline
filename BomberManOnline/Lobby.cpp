#include "stdafx.h"
#include "Lobby.h"
#include "D2D1Header.h"

using namespace std;

GameState ButtonDown_lobby_ROOM()
{
	return GameState::ROOM;
}

GameState ButtonDown_lobby_CLOSE()
{
	//PostMessage(NULL, WM_QUIT,0,0);
	return GameState::LOGIN;
}

CLobby::CLobby(CResourceManager* p_res_manager)
{
	this->p_res_manager = p_res_manager;
	button[1].Init(1150, 120, 130, 80, 1, 1);
	button[2].Init(1150, 220, 130, 80, 1, 1);
	button[3].Init(1150, 320, 130, 80, 1, 1);
	button[4].Init(1150, 420, 130, 80, 1, 1);
	button[5].Init(1150, 520, 130, 80, 1, 1);
	button[6].Init(25, 211, 400, 105, 1, 1);
	button[7].Init(435, 211, 400, 105, 1, 1);
	button[8].Init(25, 326, 400, 105, 1, 1);
	button[9].Init(435, 326, 400, 105, 1, 1);
	button[10].Init(25, 441, 400, 105, 1, 1);
	button[11].Init(435, 441, 400, 105, 1, 1);
	button[12].Init(25, 557, 400, 105, 1, 1);
	button[13].Init(435, 557, 400, 105, 1, 1);

	chat.Init(PointF(885, 635), "message", 245, 26, true, false, 10,true,424);
	chat.setClient(p_res_manager);
	chat.setIsChat(true);

	isMessage = false;
	msg_button.Init(838, 365, 90, 30, 1, 1);
}

GameState CLobby::HandleLButtonDown(CPoint point)
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
		for (int i = 1; i <= LOBBY_MAX_BUTTON; i++)
		{
			if (point.x >= button[i].GetXPixel() &&
				point.x <= button[i].GetXPixel() + button[i].GetWidth() &&
				point.y >= button[i].GetYPixel() &&
				point.y <= button[i].GetYPixel() + button[i].GetHeight())
			{
				button[i].SetStatus(BUTTON_STATUS::MOUSE_DOWN);
			}
		}

		chat.HandleLButtonDown(point);
	}
	return GameState::LOBBY;
}

GameState CLobby::HandleLButtonMove(CPoint point)
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
		for (int i = 1; i <= LOBBY_MAX_BUTTON; i++)
		{
			if (point.x >= button[i].GetXPixel() &&
				point.x <= button[i].GetXPixel() + button[i].GetWidth() &&
				point.y >= button[i].GetYPixel() &&
				point.y <= button[i].GetYPixel() + button[i].GetHeight())
			{
				button[i].SetStatus(BUTTON_STATUS::MOUSE_ON);
			}
			else
			{
				button[i].SetStatus(BUTTON_STATUS::IDLE);
			}
		}
	}
	return GameState::LOBBY;
}

GameState CLobby::HandleLButtonUp(CPoint point)
{
	GameState state = GameState::LOBBY;

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
		for (int i = 1; i <= LOBBY_MAX_BUTTON; i++)
		{
			if (point.x >= button[i].GetXPixel() &&
				point.x <= button[i].GetXPixel() + button[i].GetWidth() &&
				point.y >= button[i].GetYPixel() &&
				point.y <= button[i].GetYPixel() + button[i].GetHeight())
			{
				//user data
				if (i == 1)
				{
					isMessage = true;
					msg_string = "Name:";
					msg_string.Append(p_res_manager->account.user_name);
					msg_string.AppendFormat(L" Id:%d", p_res_manager->account.user_id);
					msg_string2.Format(L"Money:%d Exp:%d", p_res_manager->account.money,p_res_manager->account.exp);
				}
				//shop
				else if (i == 2)
				{
					isMessage = true;
					msg_string = "Shop is comming soon!";
					msg_string2 = "";
				}
				//setting
				else if (i == 3)
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
					state = GameState::LOGIN;
				}
				//room
				else if (i >= 6 && i <= 13)
				{
					if (p_res_manager->m_Client.state == ClientState::CONNECT)
					{
						//定义发送消息/接收消息
						CMessage msg, recv_msg;

						//读取字符串的中间变量
						std::string strTemp;
						CStringA temp;

						//初始化消息类型
						msg.type1 = MSG_ROOM;
						msg.type2 = MSG_ROOM_TRY;

						//设置参数
						msg.para1 = i - 6;
						msg.para2 = p_res_manager->account.user_id;

						//发送消息
						recv_msg = p_res_manager->m_Client._SendMessage(msg);

						if (recv_msg.type1 == MSG_ROOM && recv_msg.type2 == MSG_ROOM_CONFIRM)
						{
							state = GameState::ROOM;
							p_res_manager->account.room_id = i - 6;
							p_res_manager->account.seat_id = recv_msg.para1;
							p_res_manager->account.ready = false;
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
							
						}
						else if (recv_msg.type1 == MSG_ROOM && recv_msg.type2 == MSG_ROOM_DENY)
						{
							isMessage = true;
							msg_string = "Can't entry the room!";
						}
						else
						{
							isMessage = true;
							msg_string = "Undefined Error!";
						}
					}
					else
					{
						isMessage = true;
						msg_string = "Can not connect Server!";
					}
				}
			}
		}
	}
	return state;
}

void CLobby::Render(ID2D1HwndRenderTarget* render_target)
{
	ID2D1SolidColorBrush* brush;
	render_target->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &brush);

	p_res_manager->lobby_ui.DrawImage(render_target, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 0, 0);

	for (int i = 1; i <= LOBBY_MAX_BUTTON; i++)
	{
		if (button[i].GetStatus() == BUTTON_STATUS::IDLE)
		{
			p_res_manager->lobby_button_sprite[i][0].DrawImage(render_target,
				button[i].GetXPixel(), button[i].GetYPixel(),
				button[i].GetWidth(), button[i].GetHeight(),
				0, 0,
				button[i].GetXScale(), button[i].GetYScale());
		}
		else if (button[i].GetStatus() == BUTTON_STATUS::MOUSE_ON)
		{
			p_res_manager->lobby_button_sprite[i][1].DrawImage(render_target,
				button[i].GetXPixel(), button[i].GetYPixel(),
				button[i].GetWidth(), button[i].GetHeight(),
				0, 0,
				button[i].GetXScale(), button[i].GetYScale());
		}
		else if (button[i].GetStatus() == BUTTON_STATUS::MOUSE_DOWN)
		{
			p_res_manager->lobby_button_sprite[i][2].DrawImage(render_target,
				button[i].GetXPixel(), button[i].GetYPixel(),
				button[i].GetWidth(), button[i].GetHeight(),
				0, 0,
				button[i].GetXScale(), button[i].GetYScale());
		}
	}

	chat.Draw(render_target, p_res_manager);

	//white background
	p_res_manager->lobby_icon_sprite[1].DrawImage(render_target, 25, 211, 105, 105, 0, 0);
	p_res_manager->lobby_icon_sprite[1].DrawImage(render_target, 435, 211, 105, 105, 0, 0);
	p_res_manager->lobby_icon_sprite[1].DrawImage(render_target, 25, 326, 105, 105, 0, 0);
	p_res_manager->lobby_icon_sprite[1].DrawImage(render_target, 435, 326, 105, 105, 0, 0);
	p_res_manager->lobby_icon_sprite[1].DrawImage(render_target, 25, 441, 105, 105, 0, 0);
	p_res_manager->lobby_icon_sprite[1].DrawImage(render_target, 435, 441, 105, 105, 0, 0);
	p_res_manager->lobby_icon_sprite[1].DrawImage(render_target, 25, 557, 105, 105, 0, 0);
	p_res_manager->lobby_icon_sprite[1].DrawImage(render_target, 435, 557, 105, 105, 0, 0);
	//user image
	p_res_manager->lobby_icon_sprite[2].DrawImage(render_target, 786, 56, 64, 64, 0, 0);
	//map icon
	p_res_manager->lobby_icon_sprite[3].DrawImage(render_target, 43, 230, 67, 67, 0, 0);
	p_res_manager->lobby_icon_sprite[3].DrawImage(render_target, 453, 230, 67, 67, 0, 0);
	p_res_manager->lobby_icon_sprite[3].DrawImage(render_target, 43, 345, 67, 67, 0, 0);
	p_res_manager->lobby_icon_sprite[3].DrawImage(render_target, 453, 345, 67, 67, 0, 0);
	p_res_manager->lobby_icon_sprite[3].DrawImage(render_target, 43, 460, 67, 67, 0, 0);
	p_res_manager->lobby_icon_sprite[3].DrawImage(render_target, 453, 460, 67, 67, 0, 0);
	p_res_manager->lobby_icon_sprite[3].DrawImage(render_target, 43, 576, 67, 67, 0, 0);
	p_res_manager->lobby_icon_sprite[3].DrawImage(render_target, 453, 576, 67, 67, 0, 0);

	wstring o_text;
	CString text;
	ID2D1SolidColorBrush* brush_black;
	ID2D1SolidColorBrush* brush_red;
	render_target->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), &brush_black);
	render_target->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Red), &brush_red);

	//lobby text
	text.Format(L"LOBBY");
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

	text.Format(L"EXP:%d",p_res_manager->account.exp);
	o_text = text.GetString();
	RenderText(render_target, o_text, 860, 88, p_res_manager->p_text_format_Arial_32_bold, brush);

	text.Format(L"$:%d", p_res_manager->account.money);
	o_text = text.GetString();
	RenderText(render_target, o_text, 978, 88, p_res_manager->p_text_format_Arial_32_bold, brush);

	//room number text
	text.Format(L"01");
	o_text = text.GetString();
	RenderText(render_target, o_text, 141, 222, p_res_manager->p_text_format_Arial_32_bold, brush);

	text.Format(L"02");
	o_text = text.GetString();
	RenderText(render_target, o_text, 551, 222, p_res_manager->p_text_format_Arial_32_bold, brush);

	text.Format(L"03");
	o_text = text.GetString();
	RenderText(render_target, o_text, 141, 336, p_res_manager->p_text_format_Arial_32_bold, brush);

	text.Format(L"04");
	o_text = text.GetString();
	RenderText(render_target, o_text, 551, 336, p_res_manager->p_text_format_Arial_32_bold, brush);

	text.Format(L"05");
	o_text = text.GetString();
	RenderText(render_target, o_text, 141, 451, p_res_manager->p_text_format_Arial_32_bold, brush);

	text.Format(L"06");
	o_text = text.GetString();
	RenderText(render_target, o_text, 551, 451, p_res_manager->p_text_format_Arial_32_bold, brush);

	text.Format(L"07");
	o_text = text.GetString();
	RenderText(render_target, o_text, 141, 568, p_res_manager->p_text_format_Arial_32_bold, brush);

	text.Format(L"08");
	o_text = text.GetString();
	RenderText(render_target, o_text, 551, 568, p_res_manager->p_text_format_Arial_32_bold, brush);

	//room name text
	text.Format(L"ROOM");
	o_text = text.GetString();
	RenderText(render_target, o_text, 187, 222, p_res_manager->p_text_format_Arial_32_bold, brush);

	text.Format(L"ROOM");
	o_text = text.GetString();
	RenderText(render_target, o_text, 597, 222, p_res_manager->p_text_format_Arial_32_bold, brush);

	text.Format(L"ROOM");
	o_text = text.GetString();
	RenderText(render_target, o_text, 187, 336, p_res_manager->p_text_format_Arial_32_bold, brush);

	text.Format(L"ROOM");
	o_text = text.GetString();
	RenderText(render_target, o_text, 597, 336, p_res_manager->p_text_format_Arial_32_bold, brush);

	text.Format(L"ROOM");
	o_text = text.GetString();
	RenderText(render_target, o_text, 187, 451, p_res_manager->p_text_format_Arial_32_bold, brush);

	text.Format(L"ROOM");
	o_text = text.GetString();
	RenderText(render_target, o_text, 597, 451, p_res_manager->p_text_format_Arial_32_bold, brush);

	text.Format(L"ROOM");
	o_text = text.GetString();
	RenderText(render_target, o_text, 187, 568, p_res_manager->p_text_format_Arial_32_bold, brush);

	text.Format(L"ROOM");
	o_text = text.GetString();
	RenderText(render_target, o_text, 597, 568, p_res_manager->p_text_format_Arial_32_bold, brush);

	//room detail

	//定义发送消息/接收消息
	CMessage msg, recv_msg;
	
	//初始化消息类型
	msg.type1 = MSG_LOBBY;
	msg.type2 = MSG_LOBBY_ROOM;

	//设置参数
	msg.para1 = 0;
	//发送消息
	recv_msg = p_res_manager->m_Client._SendMessage(msg);
	//格式化信息
	text.Format(L"%d/4",recv_msg.para1);
	o_text = text.GetString();
	RenderText(render_target, o_text, 355, 265, p_res_manager->p_text_format_Arial_32_bold, brush);

	//设置参数
	msg.para1 = 1;
	//发送消息
	recv_msg = p_res_manager->m_Client._SendMessage(msg);
	//格式化信息
	text.Format(L"%d/4", recv_msg.para1);
	o_text = text.GetString();
	RenderText(render_target, o_text, 765, 265, p_res_manager->p_text_format_Arial_32_bold, brush);

	//设置参数
	msg.para1 = 2;
	//发送消息
	recv_msg = p_res_manager->m_Client._SendMessage(msg);
	//格式化信息
	text.Format(L"%d/4", recv_msg.para1);
	o_text = text.GetString();
	RenderText(render_target, o_text, 355, 380, p_res_manager->p_text_format_Arial_32_bold, brush);

	//设置参数
	msg.para1 = 3;
	//发送消息
	recv_msg = p_res_manager->m_Client._SendMessage(msg);
	//格式化信息
	text.Format(L"%d/4", recv_msg.para1);
	o_text = text.GetString();
	RenderText(render_target, o_text, 765, 380, p_res_manager->p_text_format_Arial_32_bold, brush);

	//设置参数
	msg.para1 = 4;
	//发送消息
	recv_msg = p_res_manager->m_Client._SendMessage(msg);
	//格式化信息
	text.Format(L"%d/4", recv_msg.para1);
	o_text = text.GetString();
	RenderText(render_target, o_text, 355, 495, p_res_manager->p_text_format_Arial_32_bold, brush);

	//设置参数
	msg.para1 = 5;
	//发送消息
	recv_msg = p_res_manager->m_Client._SendMessage(msg);
	//格式化信息
	text.Format(L"%d/4", recv_msg.para1);
	o_text = text.GetString();
	RenderText(render_target, o_text, 765, 495, p_res_manager->p_text_format_Arial_32_bold, brush);

	//设置参数
	msg.para1 = 6;
	//发送消息
	recv_msg = p_res_manager->m_Client._SendMessage(msg);
	//格式化信息
	text.Format(L"%d/4", recv_msg.para1);
	o_text = text.GetString();
	RenderText(render_target, o_text, 355, 611, p_res_manager->p_text_format_Arial_32_bold, brush);

	//设置参数
	msg.para1 = 7;
	//发送消息
	recv_msg = p_res_manager->m_Client._SendMessage(msg);
	//格式化信息
	text.Format(L"%d/4", recv_msg.para1);
	o_text = text.GetString();
	RenderText(render_target, o_text, 765, 611, p_res_manager->p_text_format_Arial_32_bold, brush);

	SafeRelease(&brush);
	
	if (isMessage)
	{
		RenderMessage(render_target);
	}
	
	return;
}

void CLobby::HandleKeyDown(UINT nchar)
{
	chat.HandleKeyDown(nchar);
}

GameState CLobby::Update()
{
	GameState state = GameState::LOBBY;
	//定义发送消息/接收消息
	CMessage msg, recv_msg;

	//初始化消息类型
	msg.type1 = MSG_CHAT;
	msg.type2 = MSG_CHAT_GET;

	//设置参数
	msg.para1 = chat.getTotalLineNum();

	//发送消息
	recv_msg = p_res_manager->m_Client._SendMessage(msg);
	while (recv_msg.type2!=MSG_CHAT_DENY)
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