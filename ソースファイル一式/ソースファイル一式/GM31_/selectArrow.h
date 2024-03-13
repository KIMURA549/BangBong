#pragma once

#include"GameObject.h"
class SelectArrow : public GameObject
{
private:
	ID3D11VertexShader* m_VertexShader = nullptr;
	ID3D11PixelShader* m_PixelShader = nullptr;
	ID3D11InputLayout* m_VertexLayout = nullptr;
	class Sprite* m_sprite;
	bool m_use = false;
	float m_time = 0.0f;
public:
	void Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;

	void SetArrowL(float x, float y, float size);
	void SetArrowR(float x, float y, float size);

	void Select() {
		m_use = true;
		m_time = 30.0f;
	}
};