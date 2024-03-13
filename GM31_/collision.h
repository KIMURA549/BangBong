#pragma once
#include"component.h"

struct CollisionSize
{
	float left = 0.0f;
	float right = 0.0f;
	float up = 0.0f;
	float down = 0.0f;
};

class Collision : public Component
{
private:
	CollisionSize m_collisionSize = {};
public:
	void SetCollisionSize(float left, float right, float up, float down) {
		m_collisionSize.left = left;
		m_collisionSize.right = right;
		m_collisionSize.up = up;
		m_collisionSize.down = down;
	}

	CollisionSize GetCollisionSize() {
		return m_collisionSize;
	}

	bool CollisionBox2D(D3DXVECTOR2 mepos, Collision enemy, D3DXVECTOR2 enemypos);
	bool CollisionCircle2D(float length, D3DXVECTOR2 mepos, D3DXVECTOR2 enemypos);
};
