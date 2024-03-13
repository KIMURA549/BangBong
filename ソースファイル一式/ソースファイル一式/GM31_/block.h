#pragma once

#include"gameObject.h"
class Block : public GameObject {
private:
	class Parts* m_model;
	class Collision* m_collision;

	int m_numX = 0;
	int m_numY = 0;

	D3DXCOLOR m_color = {};
public:
	void Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;

	void SetModels(const char* ModelName, const char* VSName, const char* PSName);
	//X‚©Y‚Ì‚Ç‚¿‚ç‚©‚Í‚O
	void SetBlockData(D3DXVECTOR3 pos, D3DXVECTOR3 scl, int x, int y);

	class Collision GetCollision();
};