#pragma once

#include"gameObject.h"

class Item : public GameObject
{
private:
	bool m_use = false;

	class Parts* m_model[3];
	class Charactor* m_body;

	D3DXCOLOR m_color = {};
public:
	void Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;

	void SetItem(int select, int itemNum);

	void SetCharactor(class Charactor* charactor);
};