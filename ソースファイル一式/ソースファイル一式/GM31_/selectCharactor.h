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

	//ÇOêgëÃÅ@ÇPêFÅ@ÇQAÅ@ÇRBÅ@ÇSC
	void SetSelectNum(int itemNum, int select);
	//ÇOêgëÃÅ@ÇPêFÅ@ÇQAÅ@ÇRBÅ@ÇSC
	int GetSelect(int num) {

		return m_selectNum[num];
	}

	void SetUse(bool use) {
		m_use = use;
	}
};