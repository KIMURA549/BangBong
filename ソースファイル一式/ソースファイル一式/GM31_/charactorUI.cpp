#include"main.h"
#include"manager.h"
#include"renderer.h"

#include "charactorUI.h"
#include"charactor.h"
#include"weapon.h"
#include"gaugeCircle.h"
#include"polygon2D.h"
#include"charactor.h"
void CharactorUI::Init()
{
	m_back = new Polygon2D;
	m_back->Init();

	for (int i = 0; i < 3; i++) {
		m_heart[i] = new Polygon2D;
		m_heart[i]->Init();
	}

	m_gauge = new GaugeCircle;
	m_gauge->Init();

	m_weaponUI = new Polygon2D;
	m_weaponUI->Init();

	SetPosSize(250.0f, 250.0f, 250.0f, 150.0f);
	
	m_use = true;
}

void CharactorUI::Uninit()
{
}

void CharactorUI::Update()
{
	if (m_stop == false) {
		m_weapon = m_charactor->GetWeapon();

		if (m_weapon != nullptr) {
			m_bulletMax = m_weapon->GetBulletMax();
			m_bullet = m_weapon->GetBulletCnt();

			if (m_weaponType != m_weapon->GetBulletTypeNum()) {
				m_weaponType = m_weapon->GetBulletTypeNum();
				switch (m_weaponType)
				{
				case 1:
					m_weaponUI->SetPolygon2D(GetPosition().x - (GetScale().x / 8), GetPosition().y - (GetScale().y / 3.5f),
						GetScale().x / 3, GetScale().x / 3, "asset\\texture\\weapon01.png");
					break;
				case 2:
					m_weaponUI->SetPolygon2D(GetPosition().x - (GetScale().x / 5), GetPosition().y - (GetScale().y / 5.0f),
						GetScale().x / 2, GetScale().x / 2, "asset\\texture\\weapon02.png");
					break;
				case 3:
					m_weaponUI->SetPolygon2D(GetPosition().x - (GetScale().x / 5), GetPosition().y - (GetScale().y / 7.5f),
						GetScale().x / 2, GetScale().x / 2, "asset\\texture\\weapon03.png");
					break;
				default:
					break;
				}
			}

		}
		else {
			m_bullet = 0;
		}

		m_life = m_charactor->GetCharactorLife();
	}
}

void CharactorUI::Draw()
{
	if (m_use == true) {
		//マトリクス設定
		Renderer::SetWorldViewProjection2D();

		//背後の色
		PARAMETER param;

		D3DXVECTOR4 c = { 0.1f,0.1f,0.1f,0.1f };
		c = m_color * 0.8f;
		param.Color = { c.x,c.y,c.z,1.0f };
		Renderer::SetParameter(param);
		m_back->Draw();
		if (m_weapon != nullptr) {
			m_weaponUI->Draw();
		}

		//体力
		param.Color = { 1.0f,0.0f,0.0f,1.0f };

		Renderer::SetParameter(param);
		for (int i = 0; i < m_life; i++) {
			m_heart[i]->Draw();
		}

		param.Parameter[0] = (float)m_bulletMax;//MAX
		param.Parameter[1] = (float)m_bullet;//現在
		param.Color = { 0.0f,1.0f,0.0f,1.0f };
		Renderer::SetParameter(param);
		m_gauge->Draw();
	}
	
}



void CharactorUI::SetPosSize(float x, float y, float width, float height)
{
	SetPosition({ x , y, 0.0f });
	SetScale({ width, height,0.0f });
	m_back->SetPolygon2D(x, y, width, height, "asset\\texture\\00.jpg");

	m_heart[0]->SetPolygon2D(x - (GetScale().x / 3), y + (GetScale().y / 3.5f), width / 8, width / 8, "asset\\texture\\heart.png");
	m_heart[1]->SetPolygon2D(x - (GetScale().x / 6), y + (GetScale().y / 3.5f), width / 8, width / 8, "asset\\texture\\heart.png");
	m_heart[2]->SetPolygon2D(x, y + (GetScale().y / 3.5f), width / 8, width / 8, "asset\\texture\\heart.png");
	m_gauge->SetSpritePosSize(GetPosition().x + (GetScale().x / 3.5f), GetPosition().y, (GetScale().x + GetScale().y) / 5, (GetScale().x + GetScale().y) / 5);
}

void CharactorUI::SetCharactor(Charactor* chara)
{
	m_charactor = chara;
	m_lifeMax = m_charactor->GetCharactorLife();
	m_life = m_charactor->GetCharactorLife();
	m_color = chara->GetColor();
}

void CharactorUI::SetWeapon(Weapon * weapon)
{
	m_weapon = weapon;
}
