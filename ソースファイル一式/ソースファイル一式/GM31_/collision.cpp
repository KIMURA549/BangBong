#include"main.h"
#include "collision.h"

bool Collision::CollisionBox2D(D3DXVECTOR2 mepos, Collision enemy, D3DXVECTOR2 enemypos)
{
	if (mepos.x - m_collisionSize.left < enemypos.x + enemy.GetCollisionSize().right &&
		mepos.x + m_collisionSize.right> enemypos.x - enemy.GetCollisionSize().left) {
		if (mepos.y + m_collisionSize.up > enemypos.y - enemy.GetCollisionSize().down &&
			mepos.y - m_collisionSize.down < enemypos.y + enemy.GetCollisionSize().up) {

			return true;
		}
	}

	return false;
}

bool Collision::CollisionCircle2D(float length, D3DXVECTOR2 mepos, D3DXVECTOR2 enemypos)
{
	D3DXVECTOR2 direction = mepos - enemypos;
	float l = D3DXVec2Length(&direction);

	if (l < length) {
		return true;
	}
	return false;
}
