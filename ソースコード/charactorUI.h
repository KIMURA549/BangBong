#pragma once
#include"gameObject.h"
class CharactorUI : public GameObject {
private:
	class Polygon2D* m_back;
	class Polygon2D* m_weaponUI;

	int m_lifeMax = 3;
	int m_life = 0;
	class Polygon2D* m_heart[3];

	int m_bulletMax = 0;
	int m_bullet = 0;
	class GaugeCircle* m_gauge;

	bool m_stop = false;
	D3DXVECTOR4 m_color;
	int m_weaponType = 0;

	bool m_use = false;

	class Charactor* m_charactor;
	class Weapon* m_weapon;
public:
	void Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;

	void Stop() {
		m_stop = true;
	}

	void SetPosSize(float x, float y, float width, float height);
	void SetCharactor(class Charactor* chara);
	void SetWeapon(class Weapon* weapon);
	void SetUse(bool use) {
		m_use = use;
	}
};