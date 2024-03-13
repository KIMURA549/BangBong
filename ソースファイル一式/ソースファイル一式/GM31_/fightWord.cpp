#include"main.h"
#include"renderer.h"
#include"component.h"
#include "fightWord.h"
#include"sprite.h"
#include"audio.h"

void FightWord::Init()
{
	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout,
		"shader\\unlitTextureChangeColorVS.cso");
	Renderer::CreatePixelShader(&m_PixelShader,
		"shader\\unlitTextureChangeColorPS.cso");

	m_sprite = AddComponent<Sprite>();

	m_voice = AddComponent<Audio>();
	m_voice->Init();
}
void FightWord::Uninit()
{
	m_voice->Uninit();
	m_sprite->Uninit();
	if (m_VertexLayout != NULL) {
		m_VertexLayout->Release();
	}
	if (m_VertexShader != NULL) {
		m_VertexShader->Release();
	}
	if (m_PixelShader != NULL) {
		m_PixelShader->Release();
	}

}
void FightWord::Update() {

	m_voice->Update();

	if (m_timeUse == true) {
		m_delay -= 1.0f;
		if (m_delay <= 0.0f) {
			m_timeUse = false;
			m_use = false;
		}
	}

	GameObject::Update();
}
void FightWord::Draw()
{
	if (m_use == true) {

		PARAMETER param;
		param.Color = { 1.0f,1.0f,1.0f,1.0f };
		Renderer::SetParameter(param);
		//入力レイアウト
		Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

		//シェーダー設定
		Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
		Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

		//マトリクス設定
		Renderer::SetWorldViewProjection2D();

		m_sprite->Draw();
	}
}

void FightWord::SetWordTexture(WORDSTATE ws)
{
	switch (ws)
	{
	case WORDSTATE_FIGHT:
		m_sprite->InitSetCenter(SCREEN_WIDTH / 2, (SCREEN_HEIGHT / 2) - (SCREEN_HEIGHT / 10), 700.0f, 250.0f, "asset\\texture\\fight.png");
		m_voice->Load("asset\\audio\\se\\fight.wav");
		break;
	case WORDSTATE_KO:
		m_sprite->InitSetCenter(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - (SCREEN_HEIGHT / 10), 500.0f, 300.0f, "asset\\texture\\ko.png");
		m_voice->Load("asset\\audio\\se\\ko.wav");
		break;
	case WORDSTATE_WIN:
		m_sprite->InitSetCenter(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - (SCREEN_HEIGHT / 10), 1000.0f, 250.0f, "asset\\texture\\win.png");
		m_voice->Load("asset\\audio\\se\\win.wav");
		break;
	case WORDSTATE_LOSE:
		m_sprite->InitSetCenter(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - (SCREEN_HEIGHT / 10), 1080.0f, 250.0f, "asset\\texture\\lose.png");
		m_voice->Load("asset\\audio\\se\\lose.wav");
		break;
	default:
		break;
	}
}

void FightWord::SetTimeUse(float t)
{
	m_use = true;
	m_timeUse = true;
	m_delay = t;
}

void FightWord::Voice()
{
	m_voice->Play();
}


