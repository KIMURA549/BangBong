#pragma once

#include"GameObject.h"
#define TITLEOBJECT_NUM	10
#define BOUND_COUNT		30
enum TITLEOBJECTSTATE {
	TITLEOBJECTSTATE_RANDOMLINEMOVE,
	TITLEOBJECTSTATE_PUYOMOVE,
	TITLEOBJECTSTATE_PARENTPUYOMOVE,

	TITLEOBJECTSTATE_NONE,
};

class TitleObject : public GameObject
{
private:
	class PolygonObject* m_object;

	TITLEOBJECTSTATE m_state = TITLEOBJECTSTATE_NONE;

	D3DXVECTOR3 m_default = {};
	D3DXVECTOR3 m_min = {};
	D3DXVECTOR3 m_max = {};

	D3DXVECTOR3 m_speedAdd = {};
	D3DXVECTOR3 m_speedSub = {};

	bool m_addition = false;
	float m_speed;
	bool m_parentAddition = false;
public:
	void Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;

	void UpdateRandomLineMove();
	void UpdatePuyoMove();
	void UpdateParentPuyoMove();

	void SetTexture(const char* TextureName);
	void SetMoveState(TITLEOBJECTSTATE toms,
		D3DXVECTOR3 min, D3DXVECTOR3 max, D3DXVECTOR3 speedAdd, D3DXVECTOR3 speedSub);
	void SetAddition(bool ad) {
		m_addition = ad;
	}
	void SetSpeed(float speed) {
		m_speed = speed;
	}

	void SetParentAddition(bool pa) {
		m_parentAddition = pa;
	}

	bool GetAddition() {
		return m_addition;
	}
};