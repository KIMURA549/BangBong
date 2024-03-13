#pragma once
#include "component.h"

class Sprite :public Component
{
private:
	ID3D11Buffer* m_VertexBuffer = nullptr;
	ID3D11ShaderResourceView* m_Texture = nullptr;
public:
	void InitSetCenter(float x, float y, float width, float height, const char* TeextureName);
	void InitSetLeftTop(float x, float y, float width, float height, const char* TeextureName);
	void InitSetRightTop(float x, float y, float width, float height, const char* TeextureName);
	void InitSetLeftDown(float x, float y, float width, float height, const char* TeextureName);
	void InitSetRightDown(float x, float y, float width, float height, const char* TeextureName);

	void Uninit();
	void Update();
	void Draw();

	void InitSetUV(float x, float y, float width, float height, const char* TeextureName, int u, int v, int num);
};