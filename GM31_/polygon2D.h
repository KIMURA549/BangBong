#pragma once

#include"GameObject.h"
class Polygon2D : public GameObject
{
private:
	ID3D11VertexShader* m_VertexShader = nullptr;
	ID3D11PixelShader* m_PixelShader = nullptr;
	ID3D11InputLayout* m_VertexLayout = nullptr;
	class Sprite* m_sprite;
public:
	void Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;


	void SetPolygon2D(float x, float y, float width, float height, const char* TeextureName);
	void SetPolygon2DUV(float x, float y, float width, float height, const char* TeextureName, int u, int v, int num);
	void SetShader(const char* PSName, const char* VSName);
};