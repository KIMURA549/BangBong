#pragma once
#include"main.h"
#include"parts.h"

#define SELECT_BODY_MAXNUM		3
#define SELECT_COLOR_MAXNUM	7
#define SELECT_ITEM_MAXNUM		15

class SelectModel {
public:
	static void GetBodyModel(int num, Parts* model);
	static void GetColor(int num, D3DXVECTOR4* c);
	static void GetItemModel(int num, Parts* model);
};