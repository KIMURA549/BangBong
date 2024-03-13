#include"main.h"
#include"manager.h"
#include "title.h"
#include"select.h"

#include"camera.h"
#include"backGround.h"
#include"titleObject.h"
#include"effect.h"
#include"fade.h"
#include"gameData.h"

#include"audio.h"
#include"input.h"

void Title::Init()
{
	Camera* camera = Scene::AddGameObject<Camera>(0);
	camera->SetCameraPos({ 0.0f,0.0f,-20.0f });
	camera->SetTarget({ 0.0f,0.0f,0.0f });


	BackGround* b = AddGameObject<BackGround>(1);
	b->SetPosition({ 0.0f,0.0f,5.0f });
	b->SetTexture("asset\\texture\\titleback.png");

	Effect* e;

	e = AddGameObject<Effect>(1);
	e->SetEffect("asset\\texture\\titlefire00.png",
		{ 1.0f, 1.0f, 1.0f }, 4, 3, false);
	e->SetScale({ 40.0f,40.0f,40.0f });
	e->SetPosition({ 0.0f, 0.0f,0.0f });
	e->SetAnimationSlow(10);
	e->SetRoop(true);
	e = AddGameObject<Effect>(1);
	e->SetEffect("asset\\texture\\titlefire00.png",
		{ 1.0f, 1.0f, 1.0f }, 4, 3, false);
	e->SetScale({ 40.0f,40.0f,40.0f });
	e->SetPosition({ 0.0f, 0.0f,0.0f });
	e->SetAnimationSlow(15);
	e->SetRoop(true);

	e = AddGameObject<Effect>(1);
	e->SetEffect("asset\\texture\\titlefire.png",
		{ 1.0f, 1.0f, 1.0f }, 4, 3, false);
	e->SetScale({ 40.0f,40.0f,40.0f });
	e->SetPosition({ 0.0f, 0.0f,0.0f });
	e->SetAnimationSlow(10);
	e->SetRoop(true);
	e = AddGameObject<Effect>(1);
	e->SetEffect("asset\\texture\\titlefire.png",
		{ 1.0f, 1.0f, 1.0f }, 4, 3, false);
	e->SetScale({ 40.0f,40.0f,40.0f });
	e->SetPosition({ 0.0f, 0.0f,0.0f });
	e->SetAnimationSlow(15);
	e->SetRoop(true);


	b = AddGameObject<BackGround>(1);
	b->SetPosition({ 0.0f,0.0f,0.0f });
	b->SetScale({ 0.8f,0.8f,0.8f });
	b->SetTexture("asset\\texture\\titlebackwarning.png");


	TitleObject* to;
	//ïêäÌ3
	to = AddGameObject<TitleObject>(1);
	to->SetTexture("asset\\texture\\titleobject06.png");
	to->SetPosition({ 7.0f,3.0f,0.0f });
	to->SetRotation({ 0.0f,0.0f,D3DX_PI * 1.25f });
	to->SetScale({ 5.0f,5.0f,5.0f });
	to->SetMoveState(TITLEOBJECTSTATE_PARENTPUYOMOVE,
		{ -1.5f,-1.5f,0.0f }, { 1.5f,1.5f,0.0f },
		{ 1.01f,1.01f,0.0f }, { 0.99f,0.99f,0.0f });

	//ïêäÌ2
	to = AddGameObject<TitleObject>(1);
	to->SetTexture("asset\\texture\\titleobject07.png");
	to->SetPosition({ -1.0f,4.0f,0.0f });
	to->SetRotation({ 0.0f,0.0f,0.0f });
	to->SetScale({ 7.0f,7.0f,7.0f });
	to->SetMoveState(TITLEOBJECTSTATE_PARENTPUYOMOVE,
		{ -1.5f,-1.5f,0.0f }, { 1.5f,1.5f,0.0f },
		{ 1.01f,1.01f,0.0f }, { 0.99f,0.99f,0.0f });

	//ïêäÌÇP
	to = AddGameObject<TitleObject>(1);
	to->SetTexture("asset\\texture\\titleobject09.png");
	to->SetPosition({ -16.0f,7.0f,0.0f });
	to->SetRotation({ 0.0f,0.0f,0.0f });
	to->SetScale({ 5.0f,5.0f,5.0f });
	to->SetMoveState(TITLEOBJECTSTATE_PARENTPUYOMOVE,
		{ -1.5f,-1.5f,0.0f }, { 1.5f,1.5f,0.0f },
		{ 1.01f,1.01f,0.0f }, { 0.99f,0.99f,0.0f });

	//óŒ
	to = AddGameObject<TitleObject>(1);
	to->SetTexture("asset\\texture\\titleobject03.png");
	to->SetPosition({ 12.0f,7.0f, 0.0f });
	to->SetRotation({ 0.0f,0.0f,0.0f });
	to->SetScale({ 8.0f,8.0f,8.0f });
	to->SetMoveState(TITLEOBJECTSTATE_PARENTPUYOMOVE,
		{ -1.5f,-1.5f,0.0f }, { 1.5f,1.5f,0.0f },
		{ 1.01f,1.01f,0.0f }, { 0.99f,0.99f,0.0f });

	//éá
	to = AddGameObject<TitleObject>(1);
	to->SetTexture("asset\\texture\\titleobject02.png");
	to->SetPosition({ -8.0f,7.0f,0.0f });
	to->SetRotation({ 0.0f,0.0f,D3DX_PI * 1.75f });
	to->SetScale({ 7.0f,7.0f,7.0f });
	to->SetMoveState(TITLEOBJECTSTATE_PARENTPUYOMOVE,
		{ -1.5f,-1.5f,0.0f }, { 1.5f,1.5f,0.0f },
		{ 1.01f,1.01f,0.0f }, { 0.99f,0.99f,0.0f });

	to = AddGameObject<TitleObject>(1);
	to->SetTexture("asset\\texture\\titleobject04.png");
	to->SetPosition({ 4.0f,-5.0f,0.0f });
	to->SetRotation({ 0.0f,0.0f,0.0f });
	to->SetScale({ 8.0f,8.0f,8.0f });
	to->SetMoveState(TITLEOBJECTSTATE_PARENTPUYOMOVE,
		{ -1.5f,-1.5f,0.0f }, { 1.5f,1.5f,0.0f },
		{ 1.01f,1.01f,0.0f }, { 0.99f,0.99f,0.0f });

	//ê¬
	to = AddGameObject<TitleObject>(1);
	to->SetTexture("asset\\texture\\titleobject01.png");
	to->SetPosition({ 17.0f,-6.0f,0.0f });
	to->SetRotation({ 0.0f,0.0f,0.0f });
	to->SetScale({ 23.0f,23.0f,23.0f });
	to->SetMoveState(TITLEOBJECTSTATE_PARENTPUYOMOVE,
		{ -1.5f,-1.5f,0.0f }, { 1.5f,1.5f,0.0f },
		{ 1.01f,1.01f,0.0f }, { 0.99f,0.99f,0.0f });
	//ê‘
	to = AddGameObject<TitleObject>(1);
	to->SetTexture("asset\\texture\\titleobject05.png");
	to->SetPosition({ -13.0f,-3.0f,0.0f });
	to->SetRotation({ 0.0f,0.0f,D3DX_PI * 0.1f });
	to->SetScale({ 20.0f,20.0f,20.0f });
	to->SetMoveState(TITLEOBJECTSTATE_PARENTPUYOMOVE,
		{ -1.5f,-1.5f,0.0f }, { 1.5f,1.5f,0.0f },
		{ 1.01f,1.01f,0.0f }, { 0.99f,0.99f,0.0f });

	//íe
	to = AddGameObject<TitleObject>(1);
	to->SetTexture("asset\\texture\\titleobject08.png");
	to->SetPosition({ 20.0f,0.0f,0.0f });
	to->SetRotation({ 0.0f,0.0f,0.0f });
	to->SetScale({ 4.0f,4.0f,4.0f });
	to->SetMoveState(TITLEOBJECTSTATE_RANDOMLINEMOVE,
		{ -21.0f,-15.0f,0.0f }, { 21.0f,15.0f,0.0f },
		{}, {});

	//TitleLogo
	m_logo = AddGameObject<TitleObject>(1);
	m_logo->SetTexture("asset\\texture\\titlelogo.png");
	m_logo->SetMoveState(TITLEOBJECTSTATE_PUYOMOVE,
		{ 16.0f,6.0f,0.0f }, { 18.0f,8.0f,0.0f },
		{ 1.015f,1.01f,0.0f }, { 0.99f,0.975f,0.0f });
	m_logo->SetAddition(true);
	m_logo->SetPosition({ 0.0f, 1.0f, -1.0f });

	to = AddGameObject<TitleObject>(1);
	to->SetTexture("asset\\texture\\titletext.png");
	to->SetScale({ 10.0f, 1.5f, 10.0f });
	to->SetPosition({ 0.0f, -6.0f, 0.0f });


	m_fade = AddGameObject<Fade>(2);
	m_fade->SetFadeState(FADESTATE_FADEIN);

	m_next = false;


	m_bgm = AddGameObject<GameObject>(0)->AddComponent<Audio>();
	m_bgm->Load("asset\\audio\\bgm\\title03.wav");
	m_bgm->Play(true);

	m_seNext = AddGameObject<GameObject>(0)->AddComponent<Audio>();
	m_seNext->Load("asset\\audio\\se\\titlestart.wav");

	GameData().Init();
}

void Title::Update()
{
	Scene::Update();

	if (m_fade->GetFadeState() == FADESTATE_STOP) {
		if (m_next == false) {

			std::vector<TitleObject*>tos = GetGameObjects<TitleObject>();
			for (TitleObject* to : tos) {
				to->SetParentAddition(m_logo->GetAddition());
			}

			if (Input::GetKeyTrigger(VK_RETURN)) {
				m_fade->SetFadeState(FADESTATE_FADEOUT);
				m_next = true;
				m_seNext->Play();
			}

		}
		else {
			Manager::SetScene<Select>();
		}
	}
}

