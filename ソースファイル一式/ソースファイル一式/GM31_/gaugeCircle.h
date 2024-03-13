#pragma once
#include"gameObject.h"
class GaugeCircle : public GameObject{
private:
	ID3D11VertexShader* m_VertexShader = nullptr;
	ID3D11PixelShader* m_PixelShader = nullptr;
	ID3D11InputLayout* m_VertexLayout = nullptr;

	class Sprite* m_sprite;

	float m_gaugeMax;
	float m_gauge;
public:
	void Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;

	void SetSpritePosSize(float x, float y, float width, float height);
	void SetGauge(float gauge) {
		m_gauge = gauge;
	}
	void SetGaugeMax(float gaugeMax) {
		m_gaugeMax = gaugeMax;
	}
};