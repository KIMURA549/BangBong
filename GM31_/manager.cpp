#include "main.h"
#include "manager.h"
#include "renderer.h"
#include"scene.h"
#include"audio.h"
#include"input.h"
#include"title.h"
#include"gameData.h"
Scene* Manager::m_Scene{}; //静的メンバ変数は再宣言が必要

void Manager::Init()
{
	Renderer::Init();
	Input::Init();

	GameData::Init();

	Audio::InitMaster();
	SetScene<Title>();
}

void Manager::Uninit()
{
	m_Scene->Uninit();
	Audio::UninitMaster();
	Input::Uninit();
	Renderer::Uninit();
}

void Manager::Update()
{
	Input::Update();
	m_Scene->Update();
}

void Manager::Draw()
{
	Renderer::Begin();


	LIGHT light;
	light.Enable = true;
	light.Direction = D3DXVECTOR4(0.0f, -3.0f, 3.0f, 0.0f);
	D3DXVec4Normalize(&light.Direction, &light.Direction);
	light.Ambient = D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.0f);
	light.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	Renderer::SetLight(light);


	light.Enable = false;
	Renderer::SetLight(light);
	m_Scene->Draw();

	Renderer::End();
}
