#include"main.h"
#include"titleObject.h"
#include"polygonObject.h"


#include<random>
#include<math.h>
void TitleObject::Init()
{

	m_object = new PolygonObject;
	m_object->SetTexture({ 1.0f, 1.0f, 1.0f }, "asset\\texture\\00.jpg");
	m_object->SetShader("shader\\titleobjVS.cso", "shader\\titleobjPS.cso");

	SetPosition({ 0.0f,0.0f,0.0f });
	SetRotation({ 0.0f,0.0f,0.0f });
	SetScale({ 1.0f,1.0f,1.0f });
}

void TitleObject::Uninit()
{
	m_object->Uninit();
	delete m_object;
}

void TitleObject::Update()
{
	switch (m_state)
	{
	case TITLEOBJECTSTATE_PARENTPUYOMOVE:
		UpdateParentPuyoMove();
		break;
	case TITLEOBJECTSTATE_RANDOMLINEMOVE:
		UpdateRandomLineMove();
		break;
	case TITLEOBJECTSTATE_PUYOMOVE:
		UpdatePuyoMove();
		break;
	default:
		break;
	}
}

void TitleObject::Draw()
{
	m_object->Draw(GetPosition(),GetRotation(),GetScale());
}

void TitleObject::UpdateRandomLineMove()
{
	static float t = 1.0f;
	static int cnt = 0;
	t += 0.01f;
	D3DXVec3Lerp(&m_Position, &m_min, &m_max, t);

	if (t >= 1.0f) {

		cnt += 1;

		std::random_device rd;
		std::mt19937 gen(rd());

		std::uniform_real_distribution<float> floatDis(-15.0f, 15.0f);
		m_min.y = floatDis(gen);
		if (cnt % 2 == 0) {
			m_min.x = -21.0f;
		}
		else {
			m_min.x = 21.0f;
		}

		m_max = m_min * (-1.0f);





		m_Position = m_min;
		
		t = 0.0f;

		SetRotation(D3DXVECTOR3(0.0f, 0.0f, atan2(m_max.y - m_min.y, m_max.x - m_min.x)));
	}
}

void TitleObject::UpdatePuyoMove()
{
	if (m_addition == true) {

		m_Scale.x *= m_speedAdd.x;
		m_Scale.y *= m_speedAdd.y;

		if (m_Scale.x >= m_max.x) {
			m_Scale.x = m_max.x;
		}
		if (m_Scale.y >= m_max.y) {
			m_Scale.y = m_max.y;
		}
		if (m_Scale.z >= m_max.z) {
			m_Scale.z = m_max.z;
		}

		if (m_Scale.x >= m_max.x && m_Scale.y >= m_max.y
			&& m_Scale.z >= m_max.z) {
			m_Scale = m_max;
			m_addition = false;
		}
	}
	else {

		m_Scale.x *= m_speedSub.x;
		m_Scale.y *= m_speedSub.y;

		if (m_Scale.x <= m_min.x) {
			m_Scale.x = m_min.x;
		}
		if (m_Scale.y <= m_min.y) {
			m_Scale.y = m_min.y;
		}
		if (m_Scale.z <= m_min.z) {
			m_Scale.z = m_min.z;
		}

		if (m_Scale.x <= m_min.x && m_Scale.y <= m_min.y
			&& m_Scale.z <= m_min.z) {
			m_Scale = m_min;
			m_addition = true;
		}
	}
}

void TitleObject::UpdateParentPuyoMove()
{

	if (m_parentAddition == true) {

		m_Scale.x *= m_speedAdd.x;
		m_Scale.y *= m_speedAdd.y;

		if (m_Scale.x >= m_max.x) {
			m_Scale.x = m_max.x;
		}
		if (m_Scale.y >= m_max.y) {
			m_Scale.y = m_max.y;
		}
		if (m_Scale.z >= m_max.z) {
			m_Scale.z = m_max.z;
		}

		if (m_Scale.x >= m_max.x && m_Scale.y >= m_max.y
			&& m_Scale.z >= m_max.z) {
			m_Scale = m_max;
			m_addition = false;
		}
	}
	else {

		m_Scale.x *= m_speedSub.x;
		m_Scale.y *= m_speedSub.y;

		if (m_Scale.x <= m_min.x) {
			m_Scale.x = m_min.x;
		}
		if (m_Scale.y <= m_min.y) {
			m_Scale.y = m_min.y;
		}
		if (m_Scale.z <= m_min.z) {
			m_Scale.z = m_min.z;
		}

		if (m_Scale.x <= m_min.x && m_Scale.y <= m_min.y
			&& m_Scale.z <= m_min.z) {
			m_Scale = m_min;
			m_addition = true;
		}
	}
}

void TitleObject::SetTexture(const char * TextureName)
{
	m_object->Uninit();
	m_object->SetTexture({ 1.0f, 1.0f, 1.0f }, TextureName);
}

void TitleObject::SetMoveState(TITLEOBJECTSTATE toms, 
	D3DXVECTOR3 min, D3DXVECTOR3 max,
	D3DXVECTOR3 speedAdd, D3DXVECTOR3 speedSub)
{


	m_state = toms;

	m_min = min;
	m_max = max;

	switch (m_state)
	{
	case TITLEOBJECTSTATE_RANDOMLINEMOVE:
		SetRotation(D3DXVECTOR3(0.0f, 0.0f, atan2(m_max.y - m_min.y, m_max.x - m_min.x)));
		break;
	case TITLEOBJECTSTATE_PUYOMOVE:
		m_min += GetScale();
		m_max += GetScale();
		break;
	case TITLEOBJECTSTATE_PARENTPUYOMOVE:
		m_min += GetScale();
		m_max += GetScale();
		m_Scale = m_max;
		break;
	default:
		break;
	}


	m_speedAdd = speedAdd;
	m_speedSub = speedSub;
}
