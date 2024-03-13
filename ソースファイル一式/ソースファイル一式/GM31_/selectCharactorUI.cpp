#include"main.h"
#include"manager.h"
#include"renderer.h"

#include "selectCharactorUI.h"
#include "selectCharactor.h"
#include"polygon2D.h"
#include"selectArrow.h"

#include"input.h"
#include"audio.h"

#include<random>

void SelectCharactorUI::Init()
{
	m_back = new Polygon2D;
	m_back->Init();
	m_back->SetPolygon2D((SCREEN_WIDTH / 2) + (SCREEN_WIDTH / 4), SCREEN_HEIGHT / 2, SCREEN_WIDTH / 2, SCREEN_HEIGHT, "asset\\texture\\00.jpg");

	m_text = new Polygon2D;
	m_text->Init();
	m_text->SetPolygon2D((SCREEN_WIDTH / 2) + (SCREEN_WIDTH / 4), SCREEN_HEIGHT / 2, SCREEN_WIDTH / 2, SCREEN_HEIGHT, "asset\\texture\\selectText.png");

	m_explanation = new Polygon2D;
	m_explanation->Init();
	m_explanation->SetPolygon2D((SCREEN_WIDTH / 2) - (SCREEN_WIDTH / 4), 600.0f, SCREEN_WIDTH / 2, 200.0f, "asset\\texture\\charactorselectexplanation.png");
	
	m_light = new Polygon2D;
	m_light->Init();
	m_light->SetPolygon2D((SCREEN_WIDTH / 2) + (SCREEN_WIDTH / 4), 650.0f, SCREEN_WIDTH / 2, 150.0f, "asset\\texture\\00.jpg");

	m_use = true;


	m_se = AddComponent<Audio>();
	m_se->Init();

}

void SelectCharactorUI::Uninit()
{
	m_back->Uninit();
	delete m_back;
	m_text->Uninit();
	delete m_text;
	m_light->Uninit();
	delete m_light;
	m_explanation->Uninit();
	delete m_explanation;
}

void SelectCharactorUI::Update()
{
	if (Input::GetKeyTrigger('S') == true || Input::GetKeyTrigger('W') == true) {

		m_se->Load("asset\\audio\\se\\selectud.wav");
		m_se->Play();
		
		if (Input::GetKeyTrigger('S') == true) {
			m_select += 1;
			if (m_select > 4) {
				m_select = 0;
			}
		}
		else if (Input::GetKeyTrigger('W') == true) {
			m_select -= 1;
			if (m_select < 0) {
				m_select = 4;
			}
		}

		switch (m_select)
		{
		case 0:
			m_light->SetPolygon2D((SCREEN_WIDTH / 2) + (SCREEN_WIDTH / 4), 90.0f, SCREEN_WIDTH / 2, 180.0f, "asset\\texture\\00.jpg");
			break;
		case 1:
			m_light->SetPolygon2D((SCREEN_WIDTH / 2) + (SCREEN_WIDTH / 4), 250.0f, SCREEN_WIDTH / 2, 140.0f, "asset\\texture\\00.jpg");
			break;
		case 2:
			m_light->SetPolygon2D((SCREEN_WIDTH / 2) + (SCREEN_WIDTH / 4), 390.0f, SCREEN_WIDTH / 2, 120.0f, "asset\\texture\\00.jpg");
			break;
		case 3:
			m_light->SetPolygon2D((SCREEN_WIDTH / 2) + (SCREEN_WIDTH / 4), 510.0f, SCREEN_WIDTH / 2, 120.0f, "asset\\texture\\00.jpg");
			break;
		case 4:
			m_light->SetPolygon2D((SCREEN_WIDTH / 2) + (SCREEN_WIDTH / 4), 650.0f, SCREEN_WIDTH / 2, 150.0f, "asset\\texture\\00.jpg");
			break;
		default:
			break;
		}
	}


	if (m_select >= 0) {
		if (Input::GetKeyTrigger('A') == true || Input::GetKeyTrigger('D') == true) {

			m_se->Load("asset\\audio\\se\\selectlr.wav");
			m_se->Play();
			
			
			int num = m_sc->GetSelect(m_select);

			if (Input::GetKeyTrigger('A') == true) {
				num -= 1;
			}
			else if (Input::GetKeyTrigger('D') == true) {
				num += 1;
			}

			m_sc->SetSelectNum(m_select, num);

		}
	}

	if (Input::GetKeyTrigger('R') == true) {
		m_se->Load("asset\\audio\\se\\selectrandom.wav");
		m_se->Play();
		Random();
	}

}

void SelectCharactorUI::Draw()
{
	if (m_use == true) {
		//マトリクス設定
		Renderer::SetWorldViewProjection2D();

		//背後の色
		PARAMETER param;

		D3DXVECTOR4 c = { 0.1f,0.1f,0.1f,0.1f };
		param.Color = { c.x,c.y,c.z,1.0f };
		Renderer::SetParameter(param);
		m_back->Draw();
		c = { 1.0f,1.0f,0.0f,1.0f };
		param.Color = { c.x,c.y,c.z,1.0f };
		Renderer::SetParameter(param);
		if (m_select >= 0) {
			m_light->Draw();
		}
		c = { 1.0f,1.0f,1.0f,1.0f };
		param.Color = { c.x,c.y,c.z,1.0f };
		Renderer::SetParameter(param);
		m_text->Draw();
		m_explanation->Draw();
		//m_one->Draw();
		//m_body->Draw();
	}
}

void SelectCharactorUI::Random()
{

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> bodyDis(0, 3);
	int bodyNum = bodyDis(gen);

	m_sc->SetSelectNum(0, bodyNum);


	std::uniform_int_distribution<int> colorDis(0, 7);

	m_sc->SetSelectNum(1, colorDis(gen));

	std::uniform_int_distribution<int> dis(1, 10);
	int pattern = dis(gen);

	int itemA, itemB, itemC;
	itemA = 0;
	itemB = 0;
	itemC = 0;



	if (pattern % 2 == 0) {
		//特定
		std::uniform_int_distribution<int> selectDis(0, 4);

		switch (selectDis(gen))
		{
		case 1:
			itemA = 1;
			itemB = 6;
			itemC = 12;
			break;
		case 2:
			itemA = 6;
			itemB = 13;
			break;
		case 3:
			itemA = 4;
			itemB = 9;
			itemC = 5;
			break;
		case 4:
			itemA = 1;
			itemB = 14;
			break;
		default:
			itemA = 2;
			itemB = 15;
			break;
		}
	}
	else {
		//顔アイテムあり

		std::uniform_int_distribution<int> aDis(0, 4);
		int a = aDis(gen);

		switch (a)
		{
		case 0:
			itemA = 1;
			itemB = 4;
			break;
		case 1:
			itemA = 4;
			itemB = 3;
			break;
		case 2:
			itemA = 4;
			itemB = 5;
			break;
		case 3:
			itemA = 4;
			break;
		default:
			break;
		}

		if (a >= 3) {

			std::uniform_int_distribution<int> bDis(5, 11);
			itemB = bDis(gen);
			if (itemB == 5) {
				itemB = 0;
			}

			if (bodyNum == 2) {
				if (itemB != 6 && itemB != 10 && itemB != 11) {
					itemB = 0;
				}
			}

			std::uniform_int_distribution<int> cDis(11, 15);
			itemC = cDis(gen);

			if (itemC == 11) {
				itemC = 0;
			}

			if (bodyNum == 2) {
				if (itemC != 12 && itemC != 13 && itemC != 14) {
					itemC = 0;
				}
			}
		}
		else {


			std::uniform_int_distribution<int> cDis(5, 15);
			itemC = cDis(gen);
			if (itemC == 5) {
				itemC = 0;
			}

			if (bodyNum == 2) {
				if (itemC != 6 && itemC != 10 && itemC != 11 &&
					itemC != 12 && itemC != 13 && itemC != 14) {
					itemC = 0;
				}
			}

		}
	}

	if (bodyNum == 3) {
		if (itemB == 3 || itemB == 5 || itemB == 10) {
			itemB = 0;
		}

		if (itemC == 3 || itemC == 5 || itemC == 10) {
			itemC = 0;
		}
	}

	m_sc->SetSelectNum(2, itemA);
	m_sc->SetSelectNum(3, itemB);
	m_sc->SetSelectNum(4, itemC);
}
