#pragma once
#include"main.h"
class GameData {
private:
	static int m_body;
	static int m_color;
	static int m_item[3];
	static int m_fieldSelect;

public:
	static void Init();

	static void SetBodyData(int num);
	static void SetColorData(int num);
	static void SetItemData(int itemNum, int num);
	static void SetFieldSelect(int num);

	static int GetBodyData();
	static int GetColorData();
	static int GetItemData(int itemNum);
	static int GetFieldSelect();
};