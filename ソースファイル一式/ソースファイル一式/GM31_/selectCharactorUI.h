#pragma once
#include"gameObject.h"

class SelectCharactorUI : public GameObject {
private:

	class Polygon2D* m_back;
	class Polygon2D* m_text;
	class Polygon2D* m_light;
	class Polygon2D* m_explanation;

	int m_select = -1;
	bool m_use = false;


	class Audio* m_se;

	class SelectCharactor* m_sc;
public:
	void Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;

	void SetSelectCharactor(class SelectCharactor* sc) {
		m_sc = sc;
	}

	void SetUse(bool use) {
		m_use = use;
	}

	void Random();
};