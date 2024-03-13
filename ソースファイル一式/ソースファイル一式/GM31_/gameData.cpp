#include "gameData.h"

int GameData::m_body;
int GameData::m_color;
int GameData::m_item[3];
int GameData::m_fieldSelect;

void GameData::Init()
{
	m_body = 0;
	m_color = 0;
	m_item[0] = 0;
	m_item[1] = 0;
	m_item[2] = 0;
	m_fieldSelect = 0;
}

void GameData::SetBodyData(int num)
{
	m_body = num;
}

void GameData::SetColorData(int num)
{
	m_color = num;
}

void GameData::SetItemData(int itemNum, int num)
{
	m_item[itemNum] = num;
}

void GameData::SetFieldSelect(int num)
{
	m_fieldSelect = num;
}


int GameData::GetBodyData()
{
	return m_body;
}

int GameData::GetColorData()
{
	return m_color;
}

int GameData::GetItemData(int itemNum)
{
	return m_item[itemNum];
}

int GameData::GetFieldSelect()
{
	return m_fieldSelect;
}
