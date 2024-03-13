#include"main.h"
#include"manager.h"

#include "gamestart.h"
#include"camera.h"
#include"backGround.h"
#include"fade.h"
#include"game.h"

#include"audio.h"
#include"input.h"
void GameStart::Init()
{

	Camera* camera = Scene::AddGameObject<Camera>(0);
	camera->SetCameraPos({ 0.0f,0.0f,-20.0f });
	camera->SetTarget({ 0.0f,0.0f,0.0f });

	BackGround* b;
	b = AddGameObject<BackGround>(1);
	b->SetPosition({ 0.0f,0.0f,0.0f });
	b->SetScale({ 0.8f,0.8f,0.8f });
	b->SetTexture("asset\\texture\\controlexplanation.png");


	m_fade = AddGameObject<Fade>(2);
	m_fade->SetFadeTexture("asset/texture/wipe02.png");
	m_fade->SetFadeState(FADESTATE_FADEIN);

	m_next = false;

	m_seNext = AddGameObject<GameObject>(0)->AddComponent<Audio>();
	m_seNext->Load("asset\\audio\\se\\gamestart.wav");
}

void GameStart::Update()
{
	Scene::Update();

	if (m_fade->GetFadeState() == FADESTATE_STOP) {
		if (m_next == false) {

			if (Input::GetKeyTrigger(VK_RETURN)) {
				m_fade->SetFadeTexture("asset/texture/wipe04.png");
				m_fade->SetFadeState(FADESTATE_FADEOUT);
				m_next = true;
				m_seNext->Play();
			}

		}
		else {
			Manager::SetScene<Game>();
		}
	}
}
