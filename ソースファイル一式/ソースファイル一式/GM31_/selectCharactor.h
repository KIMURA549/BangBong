#pragma once

#include"gameObject.h"


class SelectCharactor : public GameObject {
private:
	class Parts* m_body;
	D3DXVECTOR4 m_color = {};
	class Parts* m_item[3];

	int m_selectNum[5];

	bool m_use = false;

	void SetBody(int select);
	void SetColor(int select);
	void SetItems(int itemNum, int select);
public:
	void Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;

	//�O�g�́@�P�F�@�QA�@�RB�@�SC
	void SetSelectNum(int itemNum, int select);
	//�O�g�́@�P�F�@�QA�@�RB�@�SC
	int GetSelect(int num) {

		return m_selectNum[num];
	}

	void SetUse(bool use) {
		m_use = use;
	}
};