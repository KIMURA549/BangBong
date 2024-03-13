#pragma once
#include"main.h"

class MouseInput {
private:
	static D3DXVECTOR2 m_pos;
public:
	static void SetMousePosX(float x);
	static void SetMousePosY(float y);
	static float GetMousePosX();
	static float GetMousePosY();
	static bool GetMouseButtonL();
	static bool GetMouseButtonR();
};




