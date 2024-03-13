#pragma once
#include"GameObject.h"

enum WORDSTATE {
	WORDSTATE_FIGHT,
	WORDSTATE_KO,
	WORDSTATE_WIN,
	WORDSTATE_LOSE,
};

class FightWord : public GameObject
{
private:
	ID3D11VertexShader* m_VertexShader = nullptr;
	ID3D11PixelShader* m_PixelShader = nullptr;
	ID3D11InputLayout* m_VertexLayout = nullptr;

	class Sprite* m_sprite;

	bool m_use = false;

	float m_delay = 0.0f;

	bool m_timeUse = false;

	class Audio* m_voice;
public:
	void Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;

	void SetWordTexture(WORDSTATE ws);
	void SetTimeUse(float t);
	void SetUse(bool use) {
		m_use = use;
		m_timeUse = false;
		m_delay = 0.0f;
	}
	void Voice();

	bool GetUse() {
		return m_use;
	}
};