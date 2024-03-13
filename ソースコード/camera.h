#pragma once
#include"GameObject.h"
class Camera : public GameObject
{
private:
	D3DXVECTOR3	m_Target{};
	D3DXVECTOR3	m_StartPos{};
	D3DXMATRIX m_ViewMatrix{};
public:
	void Init()override;
	void Update()override;
	void Draw()override;

	void SetTarget(D3DXVECTOR3 target) {
		m_Target = target;
	}


	void SetCameraPos(D3DXVECTOR3 pos) {
		SetPosition(pos);
		m_StartPos = pos;
	}

	D3DXVECTOR3	m_shakeAmplitude{};
	int		m_shakeTime{};
	D3DXVECTOR3	m_shakeOffset{};

	D3DXMATRIX GetViewMatrix() {
		return m_ViewMatrix;
	}

	void Shake(float amplitudeX, float amplitudeY, float amplitudeZ) {
		m_shakeAmplitude.x = amplitudeX;
		m_shakeAmplitude.y = amplitudeY;
		m_shakeAmplitude.z = amplitudeZ;
	}
};