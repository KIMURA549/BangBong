#include"main.h"
#include "mouse.h"

D3DXVECTOR2 MouseInput::m_pos;

void MouseInput::SetMousePosX(float x) {
	m_pos.x = x;
}
void MouseInput::SetMousePosY(float y) {
	m_pos.y = y;
}

float MouseInput::GetMousePosX() {
	return m_pos.x - (SCREEN_WIDTH / 2);
}
float MouseInput::GetMousePosY() {
	return -1.0f * (m_pos.y - (SCREEN_HEIGHT / 2));
}

bool MouseInput::GetMouseButtonL() {
	if (GetAsyncKeyState(VK_LBUTTON) & 0x8000) {
		return  true;
	}
	return false;
}
bool MouseInput::GetMouseButtonR() {
	if (GetAsyncKeyState(VK_RBUTTON) & 0x8000) {
		return  true;
	}
	return false;
}