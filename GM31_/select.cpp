#include"main.h"
#include"manager.h"
#include"select.h"
#include"input.h"

#include"camera.h"
#include"backGround.h"

#include"selectCharactor.h"
#include"selectCharactorUI.h"
#include"selectStage.h"
#include"fade.h"

#include"gameData.h"
#include"gamestart.h"
#include"audio.h"

void Select::Init()
{
	Camera* camera = Scene::AddGameObject<Camera>(0);
	camera->SetCameraPos({ 0.0f,0.0f,-8.0f });
	camera->SetTarget({ 0.0f,0.0f,0.0f });

	BackGround* b = AddGameObject<BackGround>(1);
	b->SetPosition({ 0.0f,0.0f,17.0f });
	b->SetTexture("asset\\texture\\charactorselectback.png");

	SelectCharactor* sc = AddGameObject<SelectCharactor>(1);
	sc->SetPosition({ -4.0f,0.0f,0.0f });

	SelectCharactorUI* scui = AddGameObject<SelectCharactorUI>(2);
	scui->SetSelectCharactor(sc);
	

	SelectStage* ss = AddGameObject<SelectStage>(2);
	ss->Init();
	ss->SetUse(false);

	m_fade = AddGameObject<Fade>(2);
	m_fade->SetFadeState(FADESTATE_FADEIN);

	m_next = false;
	m_state = SELECTSTATE_CHARACTOR;


	m_bgm = AddGameObject<GameObject>(0)->AddComponent<Audio>();
	m_bgm->Load("asset\\audio\\bgm\\select01.wav");
	m_bgm->Play(true);

	m_se = AddGameObject<GameObject>(0)->AddComponent<Audio>();
	m_se->Load("asset\\audio\\se\\selectnext.wav");
}

void Select::Update()
{
	switch (m_state)
	{
	case SELECTSTATE_CHARACTOR:
		if (m_fade->GetFadeState() == FADESTATE_STOP) {
			if (m_next == false) {

				Scene::Update();

				if (Input::GetKeyTrigger(VK_RETURN)) {
					m_fade->SetFadeTexture("asset/texture/wipe02.png");
					m_fade->SetFadeState(FADESTATE_FADEOUT);
					m_next = true;
					m_se->Play();
				}
			}
			else {
				SelectCharactor* sc = GetGameObject<SelectCharactor>();
				GameData::SetBodyData(sc->GetSelect(0));
				GameData::SetColorData(sc->GetSelect(1));
				GameData::SetItemData(0, sc->GetSelect(2));
				GameData::SetItemData(1, sc->GetSelect(3));
				GameData::SetItemData(2, sc->GetSelect(4));
				sc->SetUse(false);
				sc->SetDestroy();
				SelectCharactorUI* scui = GetGameObject<SelectCharactorUI>();
				scui->SetUse(false);
				scui->SetDestroy();

				m_state = SELECTSTATE_STAGE;
				m_fade->SetFadeState(FADESTATE_FADEIN);
				m_next = false;

				SelectStage* ss = GetGameObject<SelectStage>();
				ss->SetUse(true);


				BackGround* b = GetGameObject<BackGround>();
				b->SetTexture("asset\\texture\\fieldselectback.png");
			}

		}
		else {
			m_fade->Update();
		}

		break;
	case SELECTSTATE_STAGE:
		if (m_fade->GetFadeState() == FADESTATE_STOP) {
			if (m_next == false) {

				Scene::Update();

				if (Input::GetKeyTrigger(VK_RETURN)) {

					SelectStage* ss = Scene::GetGameObject<SelectStage>();
					if (ss->GetSelectNum() >= 0) {
						m_fade->SetFadeState(FADESTATE_FADEOUT);
						m_next = true;
						m_se->Play();
					}
				}
			}
			else {
				SelectStage* ss = Scene::GetGameObject<SelectStage>();
				GameData::SetFieldSelect(ss->GetSelectNum());
				Manager::SetScene<GameStart>();
			}

		}
		else {
			m_fade->Update();
		}

		
		break;
	}
	
	
}