#include"main.h"
#include"renderer.h"
#include "item.h"
#include "charactor.h"
#include"parts.h"
#include"selectModel.h"
void Item::Init()
{
	m_model[0] = nullptr;
	m_model[1] = nullptr;
	m_model[2] = nullptr;

	SetPosition({ 0.0f,0.0f,0.0f });
	SetRotation({ 0.0f,0.0f,0.0f });
	SetScale({ 1.0f,1.0f,1.0f });


	m_color = { 0.0f,0.0f,0.0f,0.0f };
}

void Item::Uninit()
{
}

void Item::Update()
{
	if (m_body->GetDestroy() == true) {
		SetDestroy();
	}
}

void Item::Draw()
{
	PARAMETER param;
	param.Color = m_color;
	Renderer::SetParameter(param);

	for (int i = 0; i < 3; i++) {
		if (m_model[i] != nullptr) {
			m_model[i]->Draw(GetPosition(), GetRotation(), GetScale(), m_body->GetMatrix());
		}
	}
}

void Item::SetItem(int select, int itemNum)
{
	if (select > 0) {
		m_model[itemNum] = new Parts;
		SelectModel::GetItemModel(select, m_model[itemNum]);
	}
}

void Item::SetCharactor(Charactor * charactor)
{
	m_body = charactor;
}
