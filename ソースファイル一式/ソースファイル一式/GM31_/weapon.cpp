#include"main.h"
#include"manager.h"
#include"renderer.h"
#include"scene.h"
#include"parts.h"
#include"collision.h"
#include"weapon.h"
#include"field.h"
#include"block.h"
#include"bulletMaster.h"
#include"effect.h"
#include"polygonObject.h"
#include"audio.h"
void Weapon::Init()
{
	m_se = AddComponent<Audio>();
	m_se->Init();



	m_targetIcon = new PolygonObject;
	m_targetIcon->SetTexture({ 2.0f, 2.0f, 2.0f }, "asset\\texture\\target.png");
	m_targetIcon->SetUse(true);
}

void Weapon::Uninit()
{

	if (m_model != NULL) {
		m_model->Uninit();
		delete m_model;
	}

	m_se->Uninit();
}

void Weapon::Update()
{

	//持たれて無い
	switch (m_state)
	{
	case WEAPONSTATE_SPAWN:
		m_time -= 1.0f;
		if (m_effect != nullptr) {
			if (m_effect->GetAnimNum() >= 5) {
				m_state = WEAPONSTATE_MOVE;
				m_effect = nullptr;
			}
		}

		break;
	case WEAPONSTATE_STAY:
		m_haveDelay -= 1.0f;
		break;
	case WEAPONSTATE_HAVE://武器を所持している
		break;
	case WEAPONSTATE_STAND://武器を構える
		break;
	case WEAPONSTATE_FIRE://弾を放った
		UpdateFire();
		break;
	case WEAPONSTATE_MOVE:
		m_haveDelay -= 1.0f;
		SetRotation({ 0.0f, -D3DX_PI * 0.5f,0.0f });
		UpdateMove();
		break;
	case WEAPONSTATE_DELETE:
		UpdateMove();


		m_time -= 1.0f;
		m_Rotation.x += 0.25f;

		if (m_velocity.y < 0) {
			if (m_effect != nullptr) {
				m_effect->SetPosition(GetPosition());
				if (m_effect->GetAnimNum() <= 3) {
					SetDestroy();
				}
			}
			else {
				Scene* scene = Manager::GetScene();
				m_effect = scene->AddGameObject<Effect>(1);
				m_effect->SetEffect("asset\\texture\\deathweapon.png", { 20.0f,20.0f,20.0f }, 10, 1, true);
				m_effect->SetPosition(GetPosition());
				m_effect->SetScale({ 3.0f,3.0f,3.0f });
				m_effect->SetAnimationSlow(3);
				m_se->Load("asset\\audio\\se\\wdead01.wav");
				m_se->Play();
			}


		}

		break;
	default:
		break;
	}

	if (m_haveDelay <= 0.0f) {
		m_haveDelay = 0.0f;
		m_charactorNumber = 0;
	}
}

void Weapon::Draw()
{
	if (m_state != WEAPONSTATE_HAVE && m_state != WEAPONSTATE_SPAWN) {

		PARAMETER param;
		param.Color = { m_color.x,m_color.y,m_color.z,m_color.w };
		Renderer::SetParameter(param);
		m_model->Draw(GetPosition(), GetRotation(), GetScale());
	}
}

void Weapon::UpdateFire()
{
	float r = m_fireRotPowerNow;

	if (m_fireCnt <= 30.0f) {
		m_Rotation.x += r;
		m_fireRotPowerNow *= 0.99f;
	}
	else {
		m_Rotation.x -= r;
		m_fireRotPowerNow *= 0.95f;

		if (m_fireRotPowerNow <= 0.01f) {
			m_Scale *= 0.95f;
			if (m_Scale.x <= 0.01f) {
				SetScale(m_defoltScale);
				m_state = WEAPONSTATE_HAVE;
			}
		}
	}
	m_fireCnt += 1.0f;
}

void Weapon::UpdateMove()
{
	m_Position += m_velocity;
	//減衰
	m_velocity *= 0.999f;
	m_velocity.y -= 0.01f * m_weight;

	//ブロックとの当たり判定
	Scene* scene = Manager::GetScene();
	std::vector<Block*>blocks = scene->GetGameObjects<Block>();

	for (Block* block : blocks) {

		if (m_collision->CollisionBox2D({ m_Position.x, m_Position.y },
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
			if (abs(block->GetPosition().x - GetPosition().x) * x > std::abs(block->GetPosition().y - GetPosition().y) * y) {
				if (block->GetPosition().x - GetPosition().x > 0) {
					//左
					m_Position.x = block->GetPosition().x - block->GetCollision().GetCollisionSize().left - m_collision->GetCollisionSize().right;
					m_velocity.x = (m_weight - (m_elasticity * 1.0f)) * m_velocity.x;
				}
				else {
					//右
					m_Position.x = block->GetPosition().x + block->GetCollision().GetCollisionSize().right + m_collision->GetCollisionSize().left;
					m_velocity.x = (m_weight - (m_elasticity * 1.0f)) * m_velocity.x;
				}
			}
			else {
				if (block->GetPosition().y - GetPosition().y > 0) {
					//下
					m_Position.y = block->GetPosition().y - block->GetCollision().GetCollisionSize().down - m_collision->GetCollisionSize().up;
					m_velocity.y = (m_weight - (m_elasticity * 1.0f)) * m_velocity.y;

				}
				else {
					//上
					m_Position.y = block->GetPosition().y + block->GetCollision().GetCollisionSize().up + m_collision->GetCollisionSize().down;
					if (m_velocity.y >= -0.1f) {
						m_velocity = { 0.0f,0.0f,0.0f };
						m_state = WEAPONSTATE_STAY;
					}
					else {
						m_velocity.y = (m_weight - (m_elasticity * 1.0f)) * m_velocity.y;
					}
				}
			}
		}
	}

	//フィールドとの当たり判定
	// 左右
	if (m_limit.x > m_Position.x - m_collision->GetCollisionSize().left) {
		m_Position.x = m_limit.x + m_collision->GetCollisionSize().left;
		m_velocity.x = (m_weight - (m_elasticity * 1.0f)) * m_velocity.x;
	}
	else if (m_limit.z < m_Position.x + m_collision->GetCollisionSize().right) {
		m_Position.x = m_limit.z - m_collision->GetCollisionSize().right;
		m_velocity.x = (m_weight - (m_elasticity * 1.0f)) * m_velocity.x;
	}

	//上下
	if (m_limit.y > m_Position.y - m_collision->GetCollisionSize().down) {
		m_Position.y = m_limit.y + m_collision->GetCollisionSize().down;
		if (m_velocity.y >= -0.1f) {
			m_velocity = { 0.0f,0.0f,0.0f };
			m_ground = m_limit.y + m_collision->GetCollisionSize().down;
			m_Position.y = m_ground;
			m_state = WEAPONSTATE_STAY;
		}
		else {
			m_velocity.y = (m_weight - (m_elasticity * 1.0f)) * m_velocity.y;
		}
	}
	else if (m_limit.w < m_Position.y + m_collision->GetCollisionSize().up) {
		m_Position.y = m_limit.w - m_collision->GetCollisionSize().up;
		m_velocity.y = (m_weight - (m_elasticity * 1.0f)) * m_velocity.y;
	}


}


void Weapon::SetWeaponData(int select, D3DXVECTOR3 pos)
{
	m_model = new Parts;
	SetPosition(pos);
	m_collision = AddComponent<Collision>();
	switch (select)
	{
	case 2:
		m_model->SetModel("asset\\model\\weapon01.obj");
		m_model->SetShader("shader\\vertexLightingVS.cso", "shader\\vertexLightingPS.cso");
		SetScale({ 0.5f,0.5f,0.5f });
		m_standPos = { -0.0f,1.0f,0.0f };
		m_weight = 0.5f;
		m_elasticity = 0.8f;

		m_recoil = { 0.2f,0.0f,0.0f };
		m_fireRotPower = 0.05f;

		m_bulletMax = 2;
		m_bulletCnt = m_bulletMax;

		m_bulletNumber = 2;
		m_bulletPoint = { 0.0f,0.0f,0.0f };

		m_rotLimitMin = { -0.5f,-0.5f };
		m_rotLimitMax = { 0.5f,1.0f };

		m_collision->SetCollisionSize(0.5f, 1.0f, 0.5f, 0.5f);
		break;
	case 3:
		m_model->SetModel("asset\\model\\weapon04.obj");
		m_model->SetShader("shader\\vertexLightingVS.cso", "shader\\vertexLightingPS.cso");
		SetScale({ 0.3f,0.3f,0.3f });
		m_standPos = { 0.5f,0.25f,-0.5f };
		m_weight = 0.5f;
		m_elasticity = 0.8f;

		m_recoil = { 0.2f,0.0f,0.0f };
		m_fireRotPower = 0.05f;

		m_bulletMax = 1;
		m_bulletCnt = m_bulletMax;

		m_bulletNumber = 3;
		m_bulletPoint = { 0.0f,0.0f,0.0f };

		m_rotLimitMin = { -0.5f,-0.5f };
		m_rotLimitMax = { 0.5f,1.0f };

		m_collision->SetCollisionSize(0.5f, 1.0f, 0.5f, 0.5f);
		break;
	default:
		m_model->SetModel("asset\\model\\weapon00.obj");
		m_model->SetShader("shader\\vertexLightingVS.cso", "shader\\vertexLightingPS.cso");
		SetScale({ 0.5f,0.5f,0.5f });
		m_standPos = { 0.5f,0.25f,-0.5f };
		m_weight = 0.5f;
		m_elasticity = 1.0f;

		m_recoil = { 0.05f,0.1f,0.0f };
		m_fireRotPower = 0.05f;

		m_bulletMax = 3;
		m_bulletCnt = m_bulletMax;

		m_bulletNumber = 1;
		m_bulletPoint = { 0.75f,0.5f,0.0f };

		m_rotLimitMin = { -1.0f,-0.8f };
		m_rotLimitMax = { 1.0f,1.0f };

		m_collision->SetCollisionSize(0.05f, 1.0f, 0.25f, 0.25f);
		break;
		break;
	}

	SetRotation({ 0.0f, -D3DX_PI * 0.5f,0.0f });

	//m_limit = limit;
	m_used = false;


	m_state = WEAPONSTATE_SPAWN;


	m_se->Load("asset\\audio\\se\\wspown.wav");
	m_se->Play();

	m_defoltScale = m_Scale;

	Scene* scene = Manager::GetScene();
	m_limit = scene->GetGameObject<Field>()->GetLimit();

	m_color = { 0.5f,0.5f,0.5f,1.0f };

	m_effect = scene->AddGameObject<Effect>(1);
	m_effect->SetEffect("asset\\texture\\spawnweapon.png", { 20.0f,20.0f,20.0f }, 10, 1, false);
	m_effect->SetPosition(GetPosition());
	m_effect->SetScale({ 3.0f,3.0f,3.0f });
	m_effect->SetAnimationSlow(3);
}

void Weapon::HaveWeapon(int num)
{
	if (m_state == WEAPONSTATE_STAY || m_state == WEAPONSTATE_MOVE) {
		m_state = WEAPONSTATE_HAVE;
		m_used = true;
		m_charactorNumber = num;
		m_haveDelay = 100.0f;
	}
}

void Weapon::SetStandWeapon()
{
	m_state = WEAPONSTATE_STAND;
}

void Weapon::Fire()
{
	m_fireCnt = 0.0f;
	m_state = WEAPONSTATE_FIRE;

	if (m_bulletCnt > 0) {
		//銃の番号とキャラクターの識別番号を入れる
		CreateBullet(m_bulletNumber);

		m_bulletCnt -= 1;

		m_fireRotPowerNow = m_fireRotPower;
	}
	else {

		m_se->Load("asset\\audio\\se\\nonebullet.wav");
		m_se->Play();

		//スカ
		m_fireRotPowerNow = 0.0f;

		Scene* scene = Manager::GetScene();
		Effect* e = scene->AddGameObject<Effect>(1);
		e->SetEffect("asset\\texture\\sukaeffect.png", { 20.0f,20.0f,20.0f }, 4, 2, false);

		if (m_left == true) {
			e->SetPosition({ GetPosition().x - m_bulletPoint.x + (-GetForward().x * m_collision->GetCollisionSize().right * 1.5f) 
				, GetPosition().y + m_bulletPoint.y + (-GetForward().y * m_collision->GetCollisionSize().right * 1.5f), 0.0f });
		}
		else {
			e->SetPosition({ GetPosition().x + m_bulletPoint.x + (-GetForward().x * m_collision->GetCollisionSize().right * 1.5f)
				, GetPosition().y + m_bulletPoint.y + (-GetForward().y * m_collision->GetCollisionSize().right * 1.5f), 0.0f });
		}
		e->SetScale({ 5.0f,5.0f,5.0f });
		e->SetAnimationSlow(5);
	}
}

void Weapon::ReleaseWeapon(D3DXVECTOR3 vel)
{
	SetScale(m_defoltScale);
	SetRotation({ 0.0f, -D3DX_PI * 0.5f,0.0f });
	if (m_bulletCnt > 0) {

		m_velocity = vel;
		m_used = true;//一度使った
		m_state = WEAPONSTATE_MOVE;
	}
	else {
		m_velocity = { 0.0f,0.3f,0.0f };
		m_time = 100.0f;
		m_state = WEAPONSTATE_DELETE;

	}

	m_se->Load("asset\\audio\\se\\wrelese.wav");
	m_se->Play();
}

void Weapon::CreateBullet(int num)
{
	Scene* scene = Manager::GetScene();
	BulletMaster* bm = scene->AddGameObject<BulletMaster>(1);
	bm->Init();
	switch (num)
	{
	case 1:
		m_se->Load("asset\\audio\\se\\w1start.wav");
		m_se->Play();
		bm->SetBulletType(BULLETTYPE_NORMAL);

		bm->SetScale({ 1.5f,1.5f,1.5f });
		bm->SetRotation(GetRotation());
		bm->SetVelocity({ -GetForward().x, -GetForward().y,0.0f });
		bm->SetNumber(m_charactorNumber);
		bm->SetColor(m_color);
		break;
	case 2:
		m_se->Load("asset\\audio\\se\\w2start.wav");
		m_se->Play();
		bm->SetBulletType(BULLETTYPE_BOUND);

		bm->SetScale({ 1.0f,1.0f,1.0f });
		bm->SetRotation(GetRotation());
		bm->SetVelocity({ -GetForward().x * 3, -GetForward().y * 3,0.0f });
		bm->SetNumber(m_charactorNumber);
		bm->SetColor(m_color);
		break;
	case 3:
		m_se->Load("asset\\audio\\se\\w3start.wav");
		m_se->Play();
		bm->SetBulletType(BULLETTYPE_LASER);

		bm->SetScale({ 0.5f,0.5f,0.5f });
		bm->SetRotation({ 0.0f,0.0f,0.0f });
		bm->SetVelocity({ -GetForward().x * 0.3f, -GetForward().y * 0.3f,0.0f });
		bm->SetNumber(m_charactorNumber);
		bm->SetColor(m_color);
		break;
	default:
		break;
	}

	if (m_left == true) {
		bm->SetPosition({ GetPosition().x - m_bulletPoint.x + (-GetForward().x * m_collision->GetCollisionSize().right) , GetPosition().y + m_bulletPoint.y + (-GetForward().y * m_collision->GetCollisionSize().right), 0.0f });
	}
	else {
		bm->SetPosition({ GetPosition().x + m_bulletPoint.x + (-GetForward().x * m_collision->GetCollisionSize().right), GetPosition().y + m_bulletPoint.y + (-GetForward().y * m_collision->GetCollisionSize().right), 0.0f });
	}
}

void Weapon::DrawTargetIcon()
{
	if (m_state == WEAPONSTATE_STAND) {

		m_targetIcon->Draw(GetPosition() -GetForward() * 3, { 0.0f,0.0f,0.0f }, GetScale());
	}
}

void Weapon::SetStand(D3DXVECTOR3 pos, bool left, D3DXVECTOR3 rot)
{
	D3DXVECTOR3 sp;
	sp = m_standPos;
	if (left == true) {
		sp.x *= -1.0f;
		SetRotation(D3DXVECTOR3(D3DX_PI, D3DX_PI * 0.5f,0.0f ) - rot);
	}
	else {
		SetRotation(D3DXVECTOR3(0.0f, -D3DX_PI * 0.5f, 0.0f) + rot);
	}

	m_Position = pos + sp;
	m_left = left;
}

void Weapon::SetStandPos(D3DXVECTOR3 pos)
{
	D3DXVECTOR3 sp;
	sp = m_standPos;
	if (m_left == true) {
		sp.x *= -1.0f;
	}

	m_Position = pos + sp;
}

WEAPONSTATE Weapon::GetWeaponState()
{
	return m_state;
}

D3DXVECTOR3 Weapon::GetRecoil()
{
	D3DXVECTOR3 v = m_recoil;
	if (m_left == false) {
		v.x *= -1.0f;
	}
	return v;
}

Collision Weapon::GetCollision()
{
	return *m_collision;
}

