#pragma once
#include"gameObject.h"

enum FADESTATE {
	FADESTATE_FADEIN,
	FADESTATE_STOP,
	FADESTATE_FADEOUT,
};

class Fade : public GameObject {
private:

	ID3D11Buffer*				m_VertexBuffer = NULL;
	ID3D11ShaderResourceView*	m_Texture = NULL;

	ID3D11VertexShader*	m_VertexShader;
	ID3D11PixelShader*	m_PixelShader;
	ID3D11InputLayout*	m_VertexLayout;


	float m_threshold = 0.0f;

	FADESTATE m_state;
public:
	void Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;

	void SetFadeState(FADESTATE fs) {
		m_state = fs;
		switch (m_state)
		{
		case FADESTATE_FADEIN:
			m_threshold = 0.0f;
			break;
		case FADESTATE_FADEOUT:
			m_threshold = 1.0f;
			break;
		default:
			break;
		}
	}

	FADESTATE GetFadeState() {
		return m_state;
	}

	void SetFadeTexture(const char* textureName);
};