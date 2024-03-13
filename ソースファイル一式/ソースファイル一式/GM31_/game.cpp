#include"main.h"
#include"manager.h"
#include"game.h"
#include"title.h"

#include"camera.h"
#include"backGround.h"
#include"field.h"
#include"charactor.h"
#include"item.h"
#include"weapon.h"
#include"block.h"
#include"charactorUI.h"
#include"fightWord.h"
#include"fade.h"
#include"effect.h"


#include"gameData.h"
#include"selectModel.h"
#include"input.h"
#include"mouse.h"
#include"audio.h"

#include <ctime>
#include<vector>
#include<random>


void Game::Init()
{
	Camera* camera = Scene::AddGameObject<Camera>(0);
	camera->SetCameraPos({ 0.0f,0.0f,-20.0f });
	camera->SetTarget({ 0.0f,-2.25f,0.0f });

	CreateField();


	m_state = GAMESTATE_SET;

	m_word = AddGameObject<FightWord>(2);

	m_fade = AddGameObject<Fade>(2);
	m_fade->SetFadeState(FADESTATE_FADEIN);

	m_end = false;
	m_next = false;

	m_bgm = AddGameObject<GameObject>(0)->AddComponent<Audio>();
	m_bgm->Load("asset\\audio\\bgm\\game.wav");
	m_bgm->Play(true);
}

void Game::Update()
{
	switch (m_state)
	{
	case GAMESTATE_SET:
		UpdateSet();
		break;
	case GAMESTATE_START:
		UpdateStart();
		break;
	case GAMESTATE_GAME:
		UpdateGame();
		break;
	case GAMESTATE_PAUSE:
		break;
	case GAMESTATE_FINISH:
		UpdateFinish();
		break;
	default:
		break;
	}
}

void Game::SetGameState(GAMESTATE gs)
{
	if (gs == GAMESTATE_FINISH) {
		m_word->SetWordTexture(WORDSTATE_KO);
		m_word->SetTimeUse(100.0f);
		m_word->Voice();

		std::vector<CharactorUI*>charactorUIs = GetGameObjects<CharactorUI>();
		for (CharactorUI* charactorUI : charactorUIs) {
			
			charactorUI->Stop();
		}
	}
	m_state = gs;
}

void Game::SpawnWeapon()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> floatDis(m_left, m_right);

	float x = floatDis(gen);

	Weapon* w = AddGameObject<Weapon>(1);

	int select = 0;

	std::uniform_int_distribution<int> intDis(0, 5);
	select = intDis(gen);
	if (select % 2 == 0) {
		w->SetWeaponData(1, { x,5.0f,0.0f });
	}
	else if (select == 1) {
		w->SetWeaponData(3, { x,5.0f,0.0f });
	}
	else {
		w->SetWeaponData(2, { x,5.0f,0.0f });
	}

}

void Game::UpdateSet()
{

	if (m_fade->GetFadeState() == FADESTATE_STOP) {
		CreatePlayer(D3DXVECTOR3(-4.0f, 0.0f, 0.0f), true, 1);
		CreateEnemy(D3DXVECTOR3(4.0f, 0.0f, 0.0f), false, 2);

		//CHARACTERのUIを作る
		CharactorUI* cui;
		std::vector<Charactor*>charactors = GetGameObjects<Charactor>();
		int cnt = 0;
		for (int i = 0; i < charactors.size(); i++) {
			//UI追加
			cui = AddGameObject<CharactorUI>(2);
			cui->SetCharactor(charactors[i]);
			cui->Init();
			//大きさと位置を設定
			cui->SetPosSize((float)(SCREEN_WIDTH / (charactors.size() + 1) * (i + 1)), SCREEN_HEIGHT - (SCREEN_HEIGHT / 8),
				SCREEN_WIDTH / 5.5f, SCREEN_HEIGHT / 5);
		}



		m_state = GAMESTATE_START;
	}
	else {
		m_fade->Update();
		Camera* cam = GetGameObject<Camera>();
		cam->Update();
	}
	
}

void Game::UpdateStart()
{

	Scene::Update();

	bool next = false;
	std::vector<Charactor*>charactors = GetGameObjects<Charactor>();
	for (Charactor* charactor : charactors) {
		if (charactor->GetState() != CHARACTOR_STATE_GROUND) {
			next = false;
			break;
		}
		else {
			next = true;
		}
	}

	if (next == true) {
		m_state = GAMESTATE_GAME;

		Weapon* w = AddGameObject<Weapon>(1);
		w->SetWeaponData(0, { 0.0f,5.0f,0.0f });

		m_word->SetWordTexture(WORDSTATE_FIGHT);
		m_word->SetTimeUse(100.0f);
		m_word->Voice();
	}
}

void Game::UpdateGame()
{
	if (GetSlowFlag() == true) {
		m_timtSlow += 1.0f;
		if (m_timtSlow >= 5.0f) {
			Scene::Update();
			m_timtSlow = 0.0f;
			m_time -= 1.0f;
			if (m_time <= 0.0f) {
				SetSlowFlag(false);
			}
		}
	}
	else {
		Scene::Update();
	}

	m_timeWeapon += 1.0f;
	//時間経過
	if (m_timeWeapon >= 300.0f) {
		m_timeWeapon = 0.0f;
		//武器の最大数に満たなかったら
		if (GetGameObjects<Weapon>().size() < WEAPON_MAXNUM) {

			SpawnWeapon();
		}
	}
}

void Game::UpdateFinish()
{

	if (m_end == true) {
		if (m_fade->GetFadeState() == FADESTATE_STOP) {
			if (m_next == false) {
				if (Input::GetKeyTrigger(VK_RETURN) || Input::GetKeyTrigger('S') ||
					MouseInput::GetMouseButtonL() || MouseInput::GetMouseButtonR()) {

					std::vector<CharactorUI*>charactorUIs = GetGameObjects<CharactorUI>();
					for (CharactorUI* charactorUI : charactorUIs) {

						charactorUI->SetUse(false);
					}
					m_fade->SetFadeState(FADESTATE_FADEOUT);
					m_next = true;
				}
			}
			else {
				Manager::SetScene<Title>();
			}

		}
		else {
			m_fade->Update();
		}
		
	}
	else {

		Scene::Update();
		if (m_word->GetUse() == false) {
			int cnt = 0;
			std::vector<Charactor*>charactors = GetGameObjects<Charactor>();
			for (Charactor* charactor : charactors) {
				cnt += 1;
			}

			if (cnt <= 1) {
				Charactor* c = GetGameObject<Charactor>();
				if (c->GetCharactorNumber() == 1) {
					m_word->SetWordTexture(WORDSTATE_WIN);
					m_word->SetUse(true);
					m_word->Voice();
				}
				else {
					m_word->SetWordTexture(WORDSTATE_LOSE);
					m_word->SetUse(true);
					m_word->Voice();
				}

				m_end = true;
			}
		}
	}
}

void Game::CreatePlayer(D3DXVECTOR3 pos, bool left, int charactorNum)
{
	Charactor* c = Scene::AddGameObject<Charactor>(1);
	c->Init();
	c->SetCharactor(pos, D3DXVECTOR3(1.0f, 1.0f, 1.0f), D3DXVECTOR4(0.5f, 0.5f, 0.5f, 0.5f), left, 1.0f, 1.5f, charactorNum);
	c->SetPlayer();
	c->SetBody(GameData::GetBodyData());
	c->SetColor(GameData::GetColorData());
	Item* i = Scene::AddGameObject<Item>(1);
	i->SetCharactor(c);
	i->SetItem(GameData::GetItemData(0), 0);
	i->SetItem(GameData::GetItemData(1), 1);
	i->SetItem(GameData::GetItemData(2), 2);
}

void Game::CreateEnemy(D3DXVECTOR3 pos, bool left, int charactorNum)
{
	Charactor* chara = Scene::AddGameObject<Charactor>(1);
	chara->Init();
	chara->SetCharactor(pos, D3DXVECTOR3(1.0f, 1.0f, 1.0f), D3DXVECTOR4(0.5f, 0.5f, 0.5f, 0.5f), left, 1.0f, 1.5f, charactorNum);
	
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dis(0, 100);
	int base = dis(gen);;

	int bodyNum = base % 4;
	chara->SetBody(bodyNum);

	int colorNum = base % 8;
	if (colorNum == GameData::GetColorData()) {
		colorNum += 1;
		if (colorNum > 7) {
			colorNum = 0;
		}
	}

	chara->SetColor(colorNum);

	int itemA, itemB, itemC;
	itemA = 0;
	itemB = 0;
	itemC = 0;

	if (base % 2 == 0) {
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

	Item* i = Scene::AddGameObject<Item>(1);
	i->SetCharactor(chara);
	i->SetItem(itemA, 0);
	i->SetItem(itemB, 1);
	i->SetItem(itemC, 2);



	//Item* i = Scene::AddGameObject<Item>(1);
	//i->SetCharactor(chara);
	//i->SetItem(itemA, 0);
	//i->SetItem(itemB, 1);
	//i->SetItem(itemC, 2);


}

void Game::CreateField()
{
	Field* f;
	Block* b;
	BackGround* bg;
	switch (GameData::GetFieldSelect())
	{
	case 1:
		bg = AddGameObject<BackGround>(1);
		bg->SetTexture("asset\\texture\\field01.jpg");

		f = AddGameObject<Field>(1);
		f->Init(36, 16, "asset\\model\\block01.obj");
		m_left = f->GetLimit().x;
		m_right = f->GetLimit().z;

		b = AddGameObject<Block>(1);
		b->SetModels("asset\\model\\block01.obj", "shader\\vertexLightingVS.cso", "shader\\vertexLightingPS.cso");
		b->SetBlockData({ 0.0f,0.0f,0.0f }, { 1.0f,1.0f,1.0f }, 3, 0);

		//左
		b = AddGameObject<Block>(1);
		b->SetModels("asset\\model\\block01.obj", "shader\\vertexLightingVS.cso", "shader\\vertexLightingPS.cso");
		b->SetBlockData({ -9.0f,0.0f,0.0f }, { 1.0f,1.0f,1.0f }, 0, 2);
		b = AddGameObject<Block>(1);
		b->SetModels("asset\\model\\block01.obj", "shader\\vertexLightingVS.cso", "shader\\vertexLightingPS.cso");
		b->SetBlockData({ -11.0f,-3.0f,0.0f }, { 1.0f,1.0f,1.0f }, 3, 0);


		//右
		b = AddGameObject<Block>(1);
		b->SetModels("asset\\model\\block01.obj", "shader\\vertexLightingVS.cso", "shader\\vertexLightingPS.cso");
		b->SetBlockData({ 11.0f,-3.0f,0.0f }, { 1.0f,1.0f,1.0f }, 3, 0);
		b = AddGameObject<Block>(1);
		b->SetModels("asset\\model\\block01.obj", "shader\\vertexLightingVS.cso", "shader\\vertexLightingPS.cso");
		b->SetBlockData({ 9.0f,0.0f,0.0f }, { 1.0f,1.0f,1.0f }, 0, 2);

		break;
	case 2:
		bg = AddGameObject<BackGround>(1);
		bg->SetTexture("asset\\texture\\field02.jpg");

		f = AddGameObject<Field>(1);
		f->Init(36, 16, "asset\\model\\block03.obj");
		m_left = f->GetLimit().x;
		m_right = f->GetLimit().z;


		for (int i = 0; i < 4; i++) {
			b = AddGameObject<Block>(1);
			b->SetModels("asset\\model\\block03.obj", "shader\\vertexLightingVS.cso", "shader\\vertexLightingPS.cso");
			b->SetBlockData({ -12.0f + (i * 8.0f),5.0f,0.0f }, { 1.0f,1.0f,1.0f }, 1, 0);

			b = AddGameObject<Block>(1);
			b->SetModels("asset\\model\\block03.obj", "shader\\vertexLightingVS.cso", "shader\\vertexLightingPS.cso");
			b->SetBlockData({ -12.0f + (i * 8.0f),-4.0f,0.0f }, { 1.0f,1.0f,1.0f }, 1, 0);
		}


		for (int i = 0; i < 3; i++) {
			b = AddGameObject<Block>(1);
			b->SetModels("asset\\model\\block03.obj", "shader\\vertexLightingVS.cso", "shader\\vertexLightingPS.cso");
			b->SetBlockData({ -8.0f + (i * 8.0f),0.5f,0.0f }, { 1.0f,1.0f,1.0f }, 1, 0);
		}


		for (int i = 0; i < 8; i++) {
			b = AddGameObject<Block>(1);
			b->SetModels("asset\\model\\block03.obj", "shader\\vertexLightingVS.cso", "shader\\vertexLightingPS.cso");
			b->SetBlockData({ -14.0f + (i * 4.0f),-7.0f,0.0f }, { 1.0f,1.0f,1.0f }, 1, 0);
		}
		break;
	default:
		bg = AddGameObject<BackGround>(1);
		bg->SetTexture("asset\\texture\\field00.jpg");

		f = AddGameObject<Field>(1);
		f->Init(36, 16, "asset\\model\\block02.obj");
		m_left = f->GetLimit().x;
		m_right = f->GetLimit().z;
		for (int i = 0; i < 5; i++) {

			b = AddGameObject<Block>(1);
			b->SetModels("asset\\model\\block02.obj", "shader\\vertexLightingVS.cso", "shader\\vertexLightingPS.cso");
			b->SetBlockData({ 0.0f, -3.0f - (i * 1.0f),0.0f }, { 1.0f,1.0f,1.0f }, 3 + (i * 2), 0);
		}


		b = AddGameObject<Block>(1);
		b->SetModels("asset\\model\\block02.obj", "shader\\vertexLightingVS.cso", "shader\\vertexLightingPS.cso");
		b->SetBlockData({ -10.0f,0.0f,0.0f }, { 1.0f,1.0f,1.0f }, 3, 0);


		b = AddGameObject<Block>(1);
		b->SetModels("asset\\model\\block02.obj", "shader\\vertexLightingVS.cso", "shader\\vertexLightingPS.cso");
		b->SetBlockData({ 10.0f,0.0f,0.0f }, { 1.0f,1.0f,1.0f }, 3, 0);

		break;
	}

}
