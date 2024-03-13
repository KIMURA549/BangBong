#include"main.h"
#include"manager.h"
#include"renderer.h"
#include "selectStage.h"
#include"polygon2D.h"

#include"input.h"
#include"audio.h"
void SelectStage::Init()
{
	SetPosition({ SCREEN_WIDTH / 2,SCREEN_HEIGHT / 2,0.0f });
	SetScale({ 350.0f,180.0f,0.0f });

	for (int i = 0; i < STAGENUM_MAX; i++) {
		m_stage[i] = new Polygon2D;
		m_stage[i]->Init();
	}

	m_stage[0]->SetPolygon2D((SCREEN_WIDTH / 2), SCREEN_HEIGHT / 2 - (GetScale().y / 1.5f), GetScale().x, GetScale().y, "asset\\texture\\stage00.png");
	m_stage[1]->SetPolygon2D((SCREEN_WIDTH / 2) + (GetScale().x / 1.25f), SCREEN_HEIGHT / 2 + (GetScale().y / 1.5f), GetScale().x, GetScale().y, "asset\\texture\\stage01.png");
	m_stage[2]->SetPolygon2D((SCREEN_WIDTH / 2) - (GetScale().x / 1.25f), SCREEN_HEIGHT / 2 + (GetScale().y / 1.5f), GetScale().x, GetScale().y, "asset\\texture\\stage02.png");

	m_select = -1;
	m_use = true;


	m_se = AddComponent<Audio>();
	m_se->Init();
	m_se->Load("asset\\audio\\se\\selectud.wav");
}

void SelectStage::Uninit()
{
}

void SelectStage::Update()
{
	if (m_use == true) {

		if (Input::GetKeyTrigger('S') == true || Input::GetKeyTrigger('W') == true
			|| Input::GetKeyTrigger('A') == true || Input::GetKeyTrigger('D') == true) {

			m_se->Play();

			switch (m_select)
			{
			case 0:
				if (Input::GetKeyTrigger('A') == true || Input::GetKeyTrigger('S') == true) {
					m_select = 2;
				}
				else if (Input::GetKeyTrigger('D') == true || Input::GetKeyTrigger('W') == true) {
					m_select = 1;
				}
				break;
			case 1:
				if (Input::GetKeyTrigger('W') == true || Input::GetKeyTrigger('S') == true) {
					m_select = 0;
				}
				else if (Input::GetKeyTrigger('A') == true || Input::GetKeyTrigger('D') == true) {
					m_select = 2;
				}
				break;
			case 2:
				if (Input::GetKeyTrigger('W') == true || Input::GetKeyTrigger('S') == true) {
					m_select = 0;
				}
				else if (Input::GetKeyTrigger('A') == true || Input::GetKeyTrigger('D') == true) {
					m_select = 1;
				}
				break;
			default:

				if (Input::GetKeyTrigger('W') == true || Input::GetKeyTrigger('S') == true) {
					m_select = 0;
				}
				else if (Input::GetKeyTrigger('D') == true) {
					m_select = 1;
				}
				else if (Input::GetKeyTrigger('A') == true) {
					m_select = 2;
				}
				break;
			}


			switch (m_select)
			{
			case 0:
				m_stage[0]->SetPolygon2D((SCREEN_WIDTH / 2), SCREEN_HEIGHT / 2 - (GetScale().y / 1.5f), GetScale().x * 1.5f, GetScale().y * 1.5f, "asset\\texture\\stage00.png");
				m_stage[1]->SetPolygon2D((SCREEN_WIDTH / 2) + (GetScale().x / 1.25f), SCREEN_HEIGHT / 2 + (GetScale().y / 1.5f), GetScale().x, GetScale().y, "asset\\texture\\stage01.png");
				m_stage[2]->SetPolygon2D((SCREEN_WIDTH / 2) - (GetScale().x / 1.25f), SCREEN_HEIGHT / 2 + (GetScale().y / 1.5f), GetScale().x, GetScale().y, "asset\\texture\\stage02.png");

				break;
			case 1:
				m_stage[0]->SetPolygon2D((SCREEN_WIDTH / 2), SCREEN_HEIGHT / 2 - (GetScale().y / 1.5f), GetScale().x, GetScale().y, "asset\\texture\\stage00.png");
				m_stage[1]->SetPolygon2D((SCREEN_WIDTH / 2) + (GetScale().x / 1.25f), SCREEN_HEIGHT / 2 + (GetScale().y / 1.5f), GetScale().x * 1.5f, GetScale().y * 1.5f, "asset\\texture\\stage01.png");
				m_stage[2]->SetPolygon2D((SCREEN_WIDTH / 2) - (GetScale().x / 1.25f), SCREEN_HEIGHT / 2 + (GetScale().y / 1.5f), GetScale().x, GetScale().y, "asset\\texture\\stage02.png");
				break;
			case 2:
				m_stage[0]->SetPolygon2D((SCREEN_WIDTH / 2), SCREEN_HEIGHT / 2 - (GetScale().y / 1.5f), GetScale().x, GetScale().y, "asset\\texture\\stage00.png");
				m_stage[1]->SetPolygon2D((SCREEN_WIDTH / 2) + (GetScale().x / 1.25f), SCREEN_HEIGHT / 2 + (GetScale().y / 1.5f), GetScale().x, GetScale().y, "asset\\texture\\stage01.png");
				m_stage[2]->SetPolygon2D((SCREEN_WIDTH / 2) - (GetScale().x / 1.25f), SCREEN_HEIGHT / 2 + (GetScale().y / 1.5f), GetScale().x * 1.5f, GetScale().y * 1.5f, "asset\\texture\\stage02.png");
				break;
			default:
				break;
			}
		}
	}
}

void SelectStage::Draw()
{
	if (m_use == true) {
		//マトリクス設定
		Renderer::SetWorldViewProjection2D();

		//背後の色
		PARAMETER param;
		param.Color = { 1.0f,1.0f,1.0f,1.0f };
		Renderer::SetParameter(param);


		for (int i = 0; i < STAGENUM_MAX; i++) {
			m_stage[i]->Draw();
		}
	}
}
