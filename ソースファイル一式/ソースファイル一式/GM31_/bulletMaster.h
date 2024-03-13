#pragma once

#include"gameObject.h"
struct BULLETPARAM {
	float speed = 0.0f;
	float weight = 0.0f;
	float elasticity = 0.0f;
	int baunsLimit = 0;
	float timeLimit = 0.0f;
	bool move = false;
	bool collision = false;
	float length = 0.0f;
};

enum BULLETTYPE {
	BULLETTYPE_NORMAL,
	BULLETTYPE_BOUND,
	BULLETTYPE_LASER,
};

class BulletMaster : public GameObject
{
protected:
	BULLETTYPE m_type;

	class Parts* m_model = NULL;

	D3DXVECTOR4 m_color = {};

	D3DXVECTOR3 m_velocity = {};
	float m_lifeTime = 0.0f;
	float m_time = 0.0f;
	BULLETPARAM m_param;

	int m_number = 0;

	bool m_del = false;

	//フィールドの限界
	D3DXVECTOR4 m_limit = {};


	//LASER用

	D3DXVECTOR2 m_leaserSize;
	class PolygonObject* m_leaser;
	float m_leaserIncrement = 0.03f;

	//当たり判定
	class Collision* m_collision;

	class Audio* m_se;
public:

	void Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;
	//virtual void BulletUpdate(){}

	void BulletRotate();

	void SetBulletType(BULLETTYPE type);

	BULLETTYPE GetBulletType() {
		return m_type;
	}

	void SetVelocity(D3DXVECTOR3 vel) {
		m_velocity = vel;
	}

	void SetNumber(int num) {
		m_number = num;
	}

	void SetColor(D3DXVECTOR4 color) {
		m_color = color;
	}

	int GetNumber() {
		return m_number;
	}

	BULLETPARAM GetBulletParamData() {
		return m_param;
	}

	D3DXVECTOR3 GetVelocity() {
		return m_velocity;
	}
};
