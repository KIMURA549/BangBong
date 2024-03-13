#include"main.h"

#include"manager.h"
#include"scene.h"
#include"charactor.h"
#include"weapon.h"
#include"block.h"

#include"collision.h"

#include "charactorController.h"
#include "mouse.h"
#include"input.h"

#include<random>


void Control::InputController()
{
	m_input = CONTROLSTATE_NONE;
	if (Input::GetKeyPress('S') == true) {
		m_input = CONTROLSTATE_S;
	}
	else if (Input::GetKeyPress('A') == true) {
		m_input = CONTROLSTATE_A;
	}
	else if (Input::GetKeyPress('D') == true) {
		m_input = CONTROLSTATE_D;
	}
	else if (MouseInput::GetMouseButtonL() == true) {
		m_input = CONTROLSTATE_MOUSE_L;

		if (MouseInput::GetMouseButtonR() == true) {
			m_input = CONTROLSTATE_RELESE;
		}
	}
	else if (MouseInput::GetMouseButtonR() == true) {
		m_input = CONTROLSTATE_MOUSE_R;
	}

}

CONTROL_STATE Control::GetControl()
{
	if (Manager::GetScene()->GetStandFlag() == true) {
		return CONTROLSTATE_NONE;
	}
	return m_input;
}

float Control::GetMousePointX()
{
	return MouseInput::GetMousePosX();
}

float Control::GetMousePointY()
{
	return MouseInput::GetMousePosY();
}

void Control::AIControlInit()
{

	m_sim = false;
	m_aiMousePoint = {};
	m_state = AISTATE_THINK;

	for (int i = 0; i < SIMULATION_NUM; i++) {
		m_sd[i].end = SIMENDSTATE_GROUND;
	}
	m_nextMove = false;
	m_moveCnt = 0;
}

void Control::AIController()
{

	//入力
	m_input = CONTROLSTATE_NONE;

	if (m_state != AISTATE_THINK) {
		m_ai.delay -= 1.0f;
		m_input = m_ai.input;
		if (m_ai.delay <= 0.0f) {
			m_ai.delay = -.0f;
			m_ai.input = CONTROLSTATE_NONE;
			m_input = CONTROLSTATE_NONE;
			m_state = AISTATE_THINK;
		}
	}

	if(m_sim == true){
		SimulationUpdate();
		SimulationUpdate();
		SimulationUpdate();
		SimulationUpdate();
		SimulationUpdate();

		bool s = false;
		for (int i = 0; i < SIMULATION_NUM; i++) {
			if (m_sd[i].end == SIMENDSTATE_NONE) {
				s = true;
				break;
			}
		}

		CheckSimMove(SIMENDSTATE_BLOCK);//とりあえずブロックの上に登りたい

		if (s == false) {
			m_moveCnt += 1;
			if (m_moveCnt >= 10) {
				m_moveCnt = 0;
				std::random_device rd;
				std::mt19937 gen(rd());
				std::uniform_int_distribution<int> sdis(0, SIMULATION_NUM - 1);

				int n = sdis(gen);

				SetAIMove(m_sd[n].setVel, 3);
			}
			else {
				m_moveCnt += 1;
			}
		}

	}
}


void Control::SetAIMove(D3DXVECTOR3 v, int p)
{
	if (m_state == AISTATE_THINK) {
		m_state = AISTATE_ACTION;
		m_ai.input = CONTROLSTATE_S;
		m_nextMove = false;
		m_sim = false;
		switch (p)
		{
		case 1:
			m_ai.delay = 5.0f;
			break;
		case 2:
			m_ai.delay = 65.0f;
			break;
		case 3:
			m_ai.delay = 133.0f;
			break;
		case 4:
			m_ai.delay = 180.0f;
			break;
		case 5:
			m_ai.delay = 257.0f;
			break;
		case 6:
			m_ai.delay = 348.0f;
			break;
		default:
			m_ai.delay = 0.0f;
			break;
		}

		m_aiMousePoint.x = v.x;
		m_aiMousePoint.y = v.y;

		m_input = CONTROLSTATE_S;
	}
	
}

void Control::SetAIMoveRandom()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> sdis(0, SIMULATION_NUM - 1);

	int n = sdis(gen);
	SetAIMove(m_sd[n].setVel, m_sd[n].power);
}

void Control::CheckSimMove(SIMENDSTATE ses)
{
	for (int i = 0; i < SIMULATION_NUM; i++) {
		if (m_sd[i].end == ses) {
			SetAIMove(m_sd[i].setVel, m_sd[i].power);
			break;
		}
	}
}

void Control::SetAIAttack(float delay)
{
	if (m_state == AISTATE_THINK && m_nextMove == false) {
		m_nextMove = true;
		m_state = AISTATE_ACTION;

		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_real_distribution<float> dis(50.0f, delay);

		m_ai.delay = dis(gen);
		m_ai.input = CONTROLSTATE_MOUSE_L;
	}
	else {
		SetAIMoveRandom();
	}
	
}

void Control::AIEscapeRandom(float minX, float maxX, float minY, float maxY)
{
	if (m_state != AISTATE_ESCAPE) {
		m_state = AISTATE_ESCAPE;

		m_sim = false;

		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_real_distribution<float> xdis(minX, maxX);
		std::uniform_real_distribution<float> ydis(minY, maxY);

		float x = xdis(gen);
		float y = ydis(gen);

		

		m_aiMousePoint.x = x;
		m_aiMousePoint.y = y;

		m_ai.input = CONTROLSTATE_S;
		m_input = CONTROLSTATE_S;
	}
}

void Control::AIEscape(float x, float y, int power)
{
	if (m_state != AISTATE_ESCAPE) {
		m_state = AISTATE_ESCAPE;

		m_sim = false;

		//Powerを貯める時間の設定
		switch (power)
		{
		case 1:
			m_ai.delay = 5.0f;
			break;
		case 2:
			m_ai.delay = 54.0f;
			break;
		case 3:
			m_ai.delay = 113.0f;
			break;
		case 4:
			m_ai.delay = 180.0f;
			break;
		case 5:
			m_ai.delay = 257.0f;
			break;
		case 6:
			m_ai.delay = 348.0f;
			break;
		default:
			m_ai.delay = 0.0f;
			break;
		}

		m_aiMousePoint.x = x;
		m_aiMousePoint.y = y;

		m_ai.input = CONTROLSTATE_S;
		m_input = CONTROLSTATE_S;
	}
}

void Control::AIEscapeEnd(float x, float y)
{
	if (m_state != AISTATE_ESCAPE || m_ai.delay > 1.0f) {
		m_sim = false;
		m_state = AISTATE_ESCAPE;
		m_aiMousePoint.x = x;
		m_aiMousePoint.y = y;
		m_ai.delay = 1.0f;
		m_ai.input = CONTROLSTATE_S;
		m_input = CONTROLSTATE_S;
	}
}

void Control::ResetThinkTime(float t)
{
	if (m_state == AISTATE_ESCAPE) {
		m_ai.delay = t;
	}
}


void Control::SimulationInit(int num, D3DXVECTOR3 pos, float w, float e, Collision* col, D3DXVECTOR4 limit)
{
	m_sim = true;
	std::random_device rd;
	std::mt19937 gen(rd());
	float x;

	for (int i = 0; i < SIMULATION_NUM / 3; i++) {
		std::uniform_real_distribution<float> dis(-1.0f, 1.0f);
		x = dis(gen);
		SetSdData((i * 3) + 0, pos, { x, 1.0f,0.0f, }, 2);
		SetSdData((i * 3) + 1, pos, { x, 1.0f,0.0f, }, 3);
		SetSdData((i * 3) + 2, pos, { x, 1.0f,0.0f, }, 4);
	}

	m_weight = w;
	m_elasticity = e;

	m_collision = col;
	m_limit = limit;
}

void Control::SetSdData(int n, D3DXVECTOR3 pos, D3DXVECTOR3 vel, int pow)
{
	m_sd[n].setVel = vel;
	m_sd[n].vel = vel * pow;
	m_sd[n].pos = pos;
	m_sd[n].power = pow;
	m_sd[n].end = SIMENDSTATE_NONE;
}

void Control::SimulationUpdate()
{
	D3DXVECTOR3 p;
	for (int i = 0; i < SIMULATION_NUM; i++) {
		if (m_sd[i].end == SIMENDSTATE_NONE) {
			p = m_sd[i].pos;

			m_sd[i].pos += m_sd[i].vel;

			//減衰
			m_sd[i].vel *= 0.999f;
			m_sd[i].vel.y -= 0.01f * m_weight;

			Scene* scene = Manager::GetScene();

			//フィールドにあるブロックとの当たり判定
			std::vector<Block*>blocks = scene->GetGameObjects<Block>();
			for (Block* block : blocks) {

				if (m_collision->CollisionBox2D({ m_sd[i].pos.x, m_sd[i].pos.y },
					block->GetCollision(), { block->GetPosition().x,block->GetPosition().y }) == true) {
					float x = (block->GetCollision().GetCollisionSize().left + block->GetCollision().GetCollisionSize().right);
					float y = (block->GetCollision().GetCollisionSize().up + block->GetCollision().GetCollisionSize().down);

					if (x < y) {
						x = y / x;
						y = 1.0f;
					}
					else if (x > y) {
						y = x / y;
						x = 1.0f;
					}
					else {
						x = 1.0f;
						y = 1.0f;
					}

					// x軸方向の距離がy軸方向の距離よりも大きいかどうかで上下左右を判断
					if (abs(block->GetPosition().x - m_sd[i].pos.x) * x > std::abs(block->GetPosition().y - m_sd[i].pos.y) * y) {
						if (block->GetPosition().x - m_sd[i].pos.x > 0) {
							//左
							m_sd[i].pos.x = block->GetPosition().x - block->GetCollision().GetCollisionSize().left - m_collision->GetCollisionSize().right;
							m_sd[i].vel.x = (m_weight - (m_elasticity * 1.0f)) * m_sd[i].vel.x;
						}
						else {
							//右
							m_sd[i].pos.x = block->GetPosition().x + block->GetCollision().GetCollisionSize().right + m_collision->GetCollisionSize().left;
							m_sd[i].vel.x = (m_weight - (m_elasticity * 1.0f)) * m_sd[i].vel.x;
						}
					}
					else {
						if (block->GetPosition().y - m_sd[i].pos.y > 0) {
							//下
							m_sd[i].pos.y = block->GetPosition().y - block->GetCollision().GetCollisionSize().down - m_collision->GetCollisionSize().up;
							m_sd[i].vel.y = (m_weight - (m_elasticity * 1.0f)) * m_sd[i].vel.y;
						}
						else {

							//上
							m_sd[i].pos.y = block->GetPosition().y + block->GetCollision().GetCollisionSize().up + m_collision->GetCollisionSize().down;
							if (m_sd[i].vel.y >= -0.1f) {
								m_sd[i].end = SIMENDSTATE_BLOCK;
							}
							else {
								m_sd[i].vel.y = (m_weight - (m_elasticity * 1.0f)) * m_sd[i].vel.y;
							}
						}
					}
				}
			}


			//左右
			if (m_limit.x > m_sd[i].pos.x - 0.5f || m_limit.z < m_sd[i].pos.x + 0.5f) {
				m_sd[i].pos = p;
				m_sd[i].vel.x = (m_weight - (m_elasticity * 1.0f)) * m_sd[i].vel.x;
			}

			//上下
			if (m_limit.y > m_sd[i].pos.y - 0.5f) {
				m_sd[i].pos = p;
				if (m_sd[i].vel.y >= -0.1f) {
					m_sd[i].end = SIMENDSTATE_GROUND;
				}
				else {
					m_sd[i].vel.y = (m_weight - (m_elasticity * 1.0f)) * m_sd[i].vel.y;
				}
			}
			else if (m_limit.w < m_sd[i].pos.y + 0.5f) {
				m_sd[i].pos = p;
				m_sd[i].vel.y = (m_weight - (m_elasticity * 1.0f)) * m_sd[i].vel.y;
			}

			//武器見つける
			std::vector<Weapon*>weapons = scene->GetGameObjects<Weapon>();

			for (Weapon* weapon : weapons) {

				if (m_collision->CollisionBox2D({ m_sd[i].pos.x, m_sd[i].pos.y },
					weapon->GetCollision(), { weapon->GetPosition().x,weapon->GetPosition().y }) == true && weapon->HaveWeapon() == false &&
					(weapon->GetWeaponState() == WEAPONSTATE_MOVE || weapon->GetWeaponState() == WEAPONSTATE_STAY)) {
					m_sd[i].end = SIMENDSTATE_WEAPON;
				}
			}
		}
	}
}

void Control::SetAIMousePoint(float x, float y)
{
	m_aiMousePoint.x = x;
	m_aiMousePoint.y = y;
}

float Control::GetAIMousePointX()
{
	return m_aiMousePoint.x;
}
float Control::GetAIMousePointY()
{
	return m_aiMousePoint.y;
}