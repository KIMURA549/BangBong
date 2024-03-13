#include"main.h"
#include"manager.h"
#include"renderer.h"
#include"scene.h"
#include "block.h"

#include"parts.h"
#include"collision.h"

void Block::Init()
{
}

void Block::Uninit()
{
}

void Block::Update()
{
}

void Block::Draw()
{

	PARAMETER param;
	param.Color = m_color;
	Renderer::SetParameter(param);
	if (m_numX > 0) {
		for (int i = 0; i < m_numX; i++) {

			m_model->Draw({ GetPosition().x - ((GetScale().x / 2) * (m_numX - 1)) + (i * (GetScale().x)),GetPosition().y,GetPosition().z }, GetRotation(), GetScale());
		}
	}
	else if (m_numY > 0) {

		for (int i = 0; i < m_numY; i++) {
			m_model->Draw({ GetPosition().x,GetPosition().y - ((GetScale().y / 2) * (m_numY - 1)) + (i * (GetScale().y)),GetPosition().z }, GetRotation(), GetScale());

		}
	}
}

void Block::SetModels(const char * ModelName, const char * VSName, const char * PSName)
{
	m_model = new Parts;
	m_model->SetModel(ModelName);
	m_model->SetShader(VSName, PSName);

	m_color = { 0.0f,0.0f,0.0f,0.0f };
}

void Block::SetBlockData(D3DXVECTOR3 pos, D3DXVECTOR3 scl, int x, int y)
{
	SetPosition(pos);
	SetScale(scl);
	SetRotation({ 0.0f,0.0f,0.0f });

	m_numX = x;
	m_numY = y;

	m_collision = new Collision;

	if (x > 0) {
		m_collision->SetCollisionSize((GetScale().x / 2) * x, (GetScale().x / 2) * x, GetScale().y / 2, GetScale().y / 2);
	}
	else {
		m_collision->SetCollisionSize(GetScale().x / 2, GetScale().x / 2, (GetScale().y / 2) * y, (GetScale().y / 2) * y);

	}
}

Collision Block::GetCollision()
{
	return *m_collision;
}
