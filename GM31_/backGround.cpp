#include"main.h"
#include"backGround.h"
#include"polygonObject.h"

void BackGround::Init()
{
	m_back = new PolygonObject;
	m_back->Init();
	m_back->SetTexture({SCREEN_WIDTH / 25,SCREEN_HEIGHT / 25,1.0f}, "asset\\texture\\icefield.jpg");
	SetPosition({ 0.0f,-2.0f,1.0f });
}

void BackGround::Uninit()
{
	m_back->Uninit();
	delete m_back;
}

void BackGround::Update()
{
}

void BackGround::Draw()
{
	m_back->Draw(GetPosition(), GetRotation(), GetScale());
}

void BackGround::SetTexture(const char * TextureName)
{
	m_back = new PolygonObject;
	m_back->Init();
	m_back->SetTexture({ SCREEN_WIDTH / 25,SCREEN_HEIGHT / 25,1.0f }, TextureName);
}
