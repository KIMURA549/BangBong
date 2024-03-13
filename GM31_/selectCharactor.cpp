#include"main.h"
#include"manager.h"
#include"renderer.h"
#include"scene.h"
#include "selectCharactor.h"

#include"parts.h"
#include"collision.h"

#include"input.h"
#include"selectModel.h"

void SelectCharactor::Init()
{
	m_body = new Parts;
	m_body->SetModel("asset\\model\\body00.obj");
	m_body->SetShader("shader\\vertexLightingVS.cso", "shader\\vertexLightingPS.cso");
	SetPosition({ -2.0f,0.0f,0.0f });
	SetScale({ 2.5f,2.5f,2.5f });


	m_color = { 1.0f,1.0f,1.0f,1.0f };

	m_item[0] = nullptr;
	m_item[1] = nullptr;
	m_item[2] = nullptr;

	for (int i = 0; i < 5; i++) {
		m_selectNum[i] = 0;
	}

	m_use = true;
}

void SelectCharactor::Uninit()
{
	m_body->Uninit();
	delete m_body;
	for (int i = 0; i < 3; i++) {
		if (m_item[i] != nullptr) {
			m_item[i]->Uninit();
			delete m_item[i];
		}
	}

}

void SelectCharactor::Update()
{
	if (Input::GetKeyPress('Q') == true) {
		m_Rotation.y += 0.05f;
	}
	else if (Input::GetKeyPress('E') == true) {
		m_Rotation.y -= 0.05f;
	}

}

void SelectCharactor::Draw()
{
	if (m_use == true) {
		PARAMETER param;
		param.Color = { m_color.x,m_color.y,m_color.z,m_color.w };
		Renderer::SetParameter(param);

		m_body->Draw(GetPosition(), GetRotation(), GetScale());

		param.Color = { 1.0f, 1.0f,1.0f,1.0f };
		Renderer::SetParameter(param);

		if (m_item[0] != nullptr && m_selectNum[2] != 0) {
			m_item[0]->Draw(GetPosition(), GetRotation(), GetScale());
		}

		if (m_item[1] != nullptr && m_selectNum[3] != 0) {
			m_item[1]->Draw(GetPosition(), GetRotation(), GetScale());
		}

		if (m_item[2] != nullptr && m_selectNum[4] != 0) {
			m_item[2]->Draw(GetPosition(), GetRotation(), GetScale());
		}
	}
}

void SelectCharactor::SetSelectNum(int itemNum, int select)
{
	m_selectNum[itemNum] = select;


	if (itemNum == 0) {
		if (m_selectNum[itemNum] > SELECT_BODY_MAXNUM) {
			m_selectNum[itemNum] = 0;
		}
		else if (m_selectNum[itemNum] < 0) {
			m_selectNum[itemNum] = SELECT_BODY_MAXNUM;
		}
		SetBody(m_selectNum[itemNum]);
	}
	else if (itemNum == 1) {
		if (m_selectNum[itemNum] > SELECT_COLOR_MAXNUM) {
			m_selectNum[itemNum] = 0;
		}
		else if (m_selectNum[itemNum] < 0) {
			m_selectNum[itemNum] = SELECT_COLOR_MAXNUM;
		}
		SetColor(m_selectNum[itemNum]);
	}
	else {

		if (m_selectNum[itemNum] > SELECT_ITEM_MAXNUM) {
			m_selectNum[itemNum] = 0;
		}
		else if (m_selectNum[itemNum] < 0) {
			m_selectNum[itemNum] = SELECT_ITEM_MAXNUM;
		}

		if (m_selectNum[itemNum] != 0) {
			bool end;
			while (1) {
				end = true;
				for (int i = 2; i < 5; i++) {
					if (itemNum != i) {
						if (m_selectNum[itemNum] == m_selectNum[i]) {
							end = false;
							m_selectNum[itemNum] = m_selectNum[itemNum] + 1;
							if (m_selectNum[itemNum] > SELECT_ITEM_MAXNUM) {
								m_selectNum[itemNum] = 0;
							}
							else if (m_selectNum[itemNum] < 0) {
								m_selectNum[itemNum] = SELECT_ITEM_MAXNUM;
							}
							break;
						}
					}
				}

				if (end = true) {
					break;
				}
			}
		}
		
		SetItems(itemNum - 2, m_selectNum[itemNum]);
	}
}

void SelectCharactor::SetBody(int select)
{
	SelectModel::GetBodyModel(select, m_body);
}

void SelectCharactor::SetColor(int select)
{
	SelectModel::GetColor(select, &m_color);
}

void SelectCharactor::SetItems(int itemNum, int select)
{
	if (select == 0) {
		if (m_item[itemNum] != nullptr) {
			m_item[itemNum]->Uninit();
			delete m_item[itemNum];
			m_item[itemNum] = nullptr;
		}
	}
	else {
		if (m_item[itemNum] == nullptr) {

			m_item[itemNum] = new Parts;
		}
		SelectModel::GetItemModel(select, m_item[itemNum]);
	}
}
