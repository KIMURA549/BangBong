#include"main.h"
#include"manager.h"
#include"renderer.h"
#include "bulletMaster.h"
#include"parts.h"
#include"field.h"
#include"charactor.h"
#include"block.h"
#include"weapon.h"
#include"collision.h"
#include"effect.h"
#include"polygonObject.h"
#include"audio.h"
void BulletMaster::Init()
{
	Scene* scene = Manager::GetScene();
	m_limit = scene->GetGameObject<Field>()->GetLimit();

	m_se = AddComponent<Audio>();
	m_se->Init();
}
void BulletMaster::Uninit()
{
}
void BulletMaster::Update()
{
	if (m_param.move == true) {
		//�i��
		m_Position += m_velocity * m_param.speed;
		m_velocity.y -= 0.01f * m_param.weight;
		m_velocity *= 0.999f;

		if (m_type == BULLETTYPE_LASER) {
			m_Rotation.y += 0.1f;

		}

	}
	

	Scene* scene = Manager::GetScene();


	if (m_type == BULLETTYPE_BOUND) {
		BulletRotate();
	}

	//�����蔻��
	//�I�u�W�F�N�g�Ƃ̓����蔻��(�t�B�[���h�܂�)
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

			// x�������̋�����y�������̋��������傫�����ǂ����ŏ㉺���E�𔻒f
			if (abs(block->GetPosition().x - GetPosition().x) * x > std::abs(block->GetPosition().y - GetPosition().y) * y) {
				if (block->GetPosition().x - GetPosition().x > 0) {
					//��
					m_Position.x = block->GetPosition().x - block->GetCollision().GetCollisionSize().left - m_collision->GetCollisionSize().right;

					switch (m_type)
					{
					case BULLETTYPE_NORMAL:
						m_velocity.x = (m_param.weight - (m_param.elasticity * 1.0f)) * m_velocity.x;
						BulletRotate();
						break;
					case BULLETTYPE_BOUND:
						m_velocity.x *= -1.0f;
						break;
					case BULLETTYPE_LASER:
						m_velocity.x *= -1.0f;
						break;
					default:
						break;
					}
				}
				else {
					//�E
					m_Position.x = block->GetPosition().x + block->GetCollision().GetCollisionSize().right + m_collision->GetCollisionSize().left;

					switch (m_type)
					{
					case BULLETTYPE_NORMAL:
						m_velocity.x = (m_param.weight - (m_param.elasticity * 1.0f)) * m_velocity.x;
						BulletRotate();
						break;
					case BULLETTYPE_BOUND:
						m_velocity.x *= -1.0f;
						break;
					case BULLETTYPE_LASER:
						m_velocity.x *= -1.0f;
						break;
					default:
						break;
					}
				}
			}
			else {
				if (block->GetPosition().y - GetPosition().y > 0) {
					//��
					m_Position.y = block->GetPosition().y - block->GetCollision().GetCollisionSize().down - m_collision->GetCollisionSize().up;

					switch (m_type)
					{
					case BULLETTYPE_NORMAL:
						m_velocity.y = (m_param.weight - (m_param.elasticity * 1.0f)) * m_velocity.y;
						BulletRotate();
						break;
					case BULLETTYPE_BOUND:
						m_velocity.y *= -0.1f;
						break;
					case BULLETTYPE_LASER:
						m_velocity.y *= -1.0f;
						break;
					default:
						break;
					}

				}
				else {
					//��
					m_Position.y = block->GetPosition().y + block->GetCollision().GetCollisionSize().up + m_collision->GetCollisionSize().down;

					switch (m_type)
					{
					case BULLETTYPE_NORMAL:
						m_velocity.y = (m_param.weight - (m_param.elasticity * 1.0f)) * m_velocity.y;
						BulletRotate();
						break;
					case BULLETTYPE_BOUND:
						m_velocity.y = 2.0f;
						break;
					case BULLETTYPE_LASER:
						m_velocity.y *= -1.0f;
						break;
					default:
						break;
					}

				}
			}

			switch (m_type)
			{
			case BULLETTYPE_NORMAL:
				m_se->Load("asset\\audio\\se\\w1bound.wav");
				m_se->Play();
				break;
			case BULLETTYPE_BOUND:
				m_se->Load("asset\\audio\\se\\w2bound.wav");
				m_se->Play();
				break;
			case BULLETTYPE_LASER:
				m_se->Load("asset\\audio\\se\\w3bound.wav");
				m_se->Play();
				break;
			}

			//�����蔻��̉�
			m_param.baunsLimit -= 1;

		}
	}





	//�t�B�[���h�Ƃ̓����蔻��
	if (m_limit.x > m_Position.x) {
		m_Position.x = m_limit.x;

		switch (m_type)
		{
		case BULLETTYPE_NORMAL:
			m_velocity.x = (m_param.weight - (m_param.elasticity * 1.0f)) * m_velocity.x;
			BulletRotate();
			m_se->Load("asset\\audio\\se\\w1bound.wav");
			m_se->Play();
			break;
		case BULLETTYPE_BOUND:
			m_velocity.x *= -1.0f;
			m_se->Load("asset\\audio\\se\\w2bound.wav");
			m_se->Play();
			break;
		case BULLETTYPE_LASER:
			m_velocity.x *= -1.0f;
			m_se->Load("asset\\audio\\se\\w3bound.wav");
			m_se->Play();
			break;
		default:
			break;
		}

		m_param.baunsLimit -= 1;
	}
	else if (m_limit.z < m_Position.x) {
		m_Position.x = m_limit.z;

		switch (m_type)
		{
		case BULLETTYPE_NORMAL:
			m_velocity.x = (m_param.weight - (m_param.elasticity * 1.0f)) * m_velocity.x;
			BulletRotate();
			m_se->Load("asset\\audio\\se\\w1bound.wav");
			m_se->Play();
			break;
		case BULLETTYPE_BOUND:
			m_velocity.x *= -1.0f;
			m_se->Load("asset\\audio\\se\\w2bound.wav");
			m_se->Play();
			break;
		case BULLETTYPE_LASER:
			m_velocity.x *= -1.0f;
			m_se->Load("asset\\audio\\se\\w3bound.wav");
			m_se->Play();
			break;
		default:
			break;
		}

		m_param.baunsLimit -= 1;

	}

	//�㉺
	if (m_limit.y > m_Position.y - m_collision->GetCollisionSize().down) {
		m_Position.y = m_limit.y + m_collision->GetCollisionSize().down;

		switch (m_type)
		{
		case BULLETTYPE_NORMAL:
			m_velocity.y = (m_param.weight - (m_param.elasticity * 1.0f)) * m_velocity.y;
			BulletRotate();
			m_se->Load("asset\\audio\\se\\w1bound.wav");
			m_se->Play();
			break;
		case BULLETTYPE_BOUND:
			m_velocity.y = 2.0f;
			m_se->Load("asset\\audio\\se\\w2bound.wav");
			m_se->Play();
			break;
		case BULLETTYPE_LASER:
			m_velocity.y *= -1.0f;
			m_se->Load("asset\\audio\\se\\w3bound.wav");
			m_se->Play();
			break;
		default:
			break;
		}
		m_param.baunsLimit -= 1;
	}
	else if (m_limit.w < m_Position.y + m_collision->GetCollisionSize().up) {
		m_Position.y = m_limit.w - m_collision->GetCollisionSize().up;

		switch (m_type)
		{
		case BULLETTYPE_NORMAL:
			m_velocity.y = (m_param.weight - (m_param.elasticity * 1.0f)) * m_velocity.y;
			BulletRotate();
			m_se->Load("asset\\audio\\se\\w1bound.wav");
			m_se->Play();
			break;
		case BULLETTYPE_BOUND:
			m_velocity.y *= -1.0f;
			m_se->Load("asset\\audio\\se\\w2bound.wav");
			m_se->Play();
			break;
		case BULLETTYPE_LASER:
			m_velocity.y *= -1.0f;
			m_se->Load("asset\\audio\\se\\w3bound.wav");
			m_se->Play();
			break;
		default:
			break;
		}
		m_param.baunsLimit -= 1;
	}



	//�����̂���I�u�W�F�N�g�Ƃ̓����蔻��
	if (m_del == false ) {
		//���[�U�[�����̑���
		if (m_type == BULLETTYPE_LASER) {

			m_leaserSize.x += m_leaserIncrement;
			if (m_leaserSize.x >= 0.8f) {
				m_leaserSize.x = 0.8f;
				m_leaserIncrement = -0.015f;
			}
			else if(m_leaserSize.x <= 0.0f) {
				m_leaserSize.x = 0.0f;
				m_leaserIncrement = 0.015f;
				m_se->Load("asset\\audio\\se\\w3beam.wav");
				m_se->Play();
			}

			std::vector<Charactor*>charactors = scene->GetGameObjects<Charactor>();
			for (Charactor* charactor : charactors) {
				if(charactor->GetPosition().x - (charactor->GetScale().x / 2) < GetPosition().x + (m_leaserSize.x / 2) &&
					charactor->GetPosition().x + (charactor->GetScale().x / 2) > GetPosition().x - (m_leaserSize.x / 2)) {

					if (charactor->GetCharactorNumber() != m_number) {

						m_se->Load("asset\\audio\\se\\damagec.wav");
						m_se->Play();

						//���C�t������ā@�G������΂�
						D3DXVECTOR3 v;
						//�����ʒu���猻�ݒn�̃x�N�g���i���K���j
						v.x = GetPosition().x - charactor->GetPosition().x;
						v.y = GetPosition().y - charactor->GetPosition().y;
						//���K���@�����w��
						float n = sqrt(pow(v.x, 2) + pow(v.y, 2)) / 1.25f;

						//�L�����̈ʒu����
						v.x = -(v.x / n);
						v.y = (v.y / n);
						v.z = 0.0f;

						v *= 0.5f;

						if (v.y <= 0.5f) {
							v.y = 0.5f;
						}

						if (charactor->GetState() != CHARACTOR_STATE_DAMAGE) {
							charactor->Damage(v);
							//����LASER���L�����N�^�[����щ���Ă���Ԃɍ폜
							std::vector<BulletMaster*>bullets = scene->GetGameObjects<BulletMaster>();
							for (BulletMaster* bullet : bullets) {
								if (bullet->GetBulletType() == BULLETTYPE_LASER) {
									m_param.timeLimit = 200.0f;
									m_lifeTime = 0.0f;
								}
							}

							Effect* e = scene->AddGameObject<Effect>(1);
							e->SetEffect("asset\\texture\\hit01.png", { 20.0f,20.0f,20.0f }, 7, 2, false, 0);
							e->SetPosition({ charactor->GetPosition().x, charactor->GetPosition().y, 0.0f });
							e->SetScale({ 5.0f,5.0f,5.0f });
							e->SetAnimationSlow(4);
						}
						break;
					}
				}

			}


			//���̕���
			std::vector<Weapon*>weapons = scene->GetGameObjects<Weapon>();

			for (Weapon* weapon : weapons) {
				if (weapon->GetPosition().x - (weapon->GetScale().x / 2) < GetPosition().x + (m_leaserSize.x / 2) &&
					weapon->GetPosition().x + (weapon->GetScale().x / 2) > GetPosition().x - (m_leaserSize.x / 2)) {

					if (weapon->GetWeaponState() == WEAPONSTATE_STAY || weapon->GetWeaponState() == WEAPONSTATE_MOVE) {

						if (GetPosition().x < weapon->GetPosition().x) {
							weapon->ReleaseWeapon({ 0.2f,0.3f,0.0f });
						}
						else {
							weapon->ReleaseWeapon({ -0.2f,0.3f,0.0f });
						}
						weapon->Damage();

						Effect* e = scene->AddGameObject<Effect>(1);
						e->SetEffect("asset\\texture\\hit01.png", { 20.0f,20.0f,20.0f }, 7, 2, false, 0);
						e->SetPosition({ weapon->GetPosition().x, weapon->GetPosition().y, 0.0f });
						e->SetScale({ 3.0f,3.0f,3.0f });
						e->SetAnimationSlow(5);

						m_se->Load("asset\\audio\\se\\damagew.wav");
						m_se->Play();
					}
				}
			}

			//�e�Ƃ̓����蔻��i�ʂ̃L�����̒e��������j
			std::vector<BulletMaster*>bullets = scene->GetGameObjects<BulletMaster>();
			for (BulletMaster* bullet : bullets) {
				if (bullet->GetNumber() != m_number && bullet->GetBulletType() != BULLETTYPE_LASER) {

					if (bullet->GetPosition().x - (bullet->GetBulletParamData().length / 2) < GetPosition().x + (m_leaserSize.x / 2) &&
						bullet->GetPosition().x + (bullet->GetBulletParamData().length / 2) > GetPosition().x - (m_leaserSize.x / 2)) {
						//�G�t�F�N�g�ǉ�
						Effect* e = scene->AddGameObject<Effect>(1);
						e->SetEffect("asset\\texture\\hit01.png", { 20.0f,20.0f,20.0f }, 7, 2, false, 0);
						e->SetPosition({ bullet->GetPosition().x / 2, bullet->GetPosition().y, 0.0f });
						e->SetScale({ 3.0f,3.0f,3.0f });
						e->SetAnimationSlow(5);

						m_se->Load("asset\\audio\\se\\damagew.wav");
						m_se->Play();

						bullet->SetDestroy();
					}
				}
			}



			//���Ԍo�߁@���˕Ԃ���E�@�ŏ���
			m_lifeTime += 1.0f;
			if (m_param.timeLimit <= m_lifeTime || m_param.baunsLimit <= 0) {
				SetDestroy();
			}


		}
		else {

			std::vector<Charactor*>charactors = scene->GetGameObjects<Charactor>();
			for (Charactor* charactor : charactors) {

				if (m_collision->CollisionCircle2D(m_param.length, { GetPosition().x, GetPosition().y }, { charactor->GetPosition().x, charactor->GetPosition().y })) {

					if (charactor->GetCharactorNumber() != m_number) {

						m_se->Load("asset\\audio\\se\\damagec.wav");
						m_se->Play();

						//���C�t������ā@�G������΂�
						D3DXVECTOR3 v;
						//�����ʒu���猻�ݒn�̃x�N�g���i���K���j
						v.x = GetPosition().x - charactor->GetPosition().x;
						v.y = GetPosition().y - charactor->GetPosition().y;
						//���K���@�����w��
						float n = sqrt(pow(v.x, 2) + pow(v.y, 2)) / 1.25f;

						//�L�����̈ʒu����
						v.x = -(v.x / n);
						v.y = (v.y / n);
						v.z = 0.0f;

						v *= 0.5f;

						if (v.y <= 0.5f) {
							v.y = 0.5f;
						}

						if (scene->GetStandFlag() == false) {
							charactor->Damage(v);
							scene->SetSlowFlag(true);
							scene->SetTime(10.0f);
							scene->SetStandFlag(true);

							Effect* e = scene->AddGameObject<Effect>(1);
							e->SetEffect("asset\\texture\\hit02.png", { 20.0f,20.0f,20.0f }, 6, 4, false, 3);
							e->SetPosition({ (GetPosition().x + charactor->GetPosition().x) / 2,(GetPosition().y + charactor->GetPosition().y) / 2, 0.0f });
							e->SetScale({ 5.0f,5.0f,5.0f });
							e->SetAnimationSlow(4);
						}


						m_del = true;
						m_time = 0.0f;
						break;
					}
				}

			}


			//���̕���
			std::vector<Weapon*>weapons = scene->GetGameObjects<Weapon>();

			for (Weapon* weapon : weapons) {
				if (m_collision->CollisionBox2D({ m_Position.x, m_Position.y },
					weapon->GetCollision(), { weapon->GetPosition().x,weapon->GetPosition().y }) == true) {

					if (weapon->GetWeaponState() == WEAPONSTATE_STAY || weapon->GetWeaponState() == WEAPONSTATE_MOVE) {

						m_se->Load("asset\\audio\\se\\damagew.wav");
						m_se->Play();

						if (GetPosition().x < weapon->GetPosition().x) {
							weapon->ReleaseWeapon({ 0.2f,0.3f,0.0f });
						}
						else {
							weapon->ReleaseWeapon({ -0.2f,0.3f,0.0f });
						}
						weapon->Damage();

						Effect* e = scene->AddGameObject<Effect>(1);
						e->SetEffect("asset\\texture\\hit02.png", { 20.0f,20.0f,20.0f }, 6, 4, false, 3);
						e->SetPosition({ (GetPosition().x + weapon->GetPosition().x) / 2,(GetPosition().y + weapon->GetPosition().y) / 2, 0.0f });
						e->SetScale({ 3.0f,3.0f,3.0f });
						e->SetAnimationSlow(5);
						SetDestroy();
					}
				}
			}

			//�e�Ƃ̓����蔻��i�ʂ̃L�����̒e��������j
			std::vector<BulletMaster*>bullets = scene->GetGameObjects<BulletMaster>();
			for (BulletMaster* bullet : bullets) {
				if (bullet->GetNumber() != m_number) {

					if (m_collision->CollisionCircle2D(m_param.length, { GetPosition().x, GetPosition().y }, { bullet->GetPosition().x, bullet->GetPosition().y })) {
						//�G�t�F�N�g�ǉ�
						Effect* e = scene->AddGameObject<Effect>(1);
						e->SetEffect("asset\\texture\\hit02.png", { 20.0f,20.0f,20.0f }, 6, 4, false, 3);
						e->SetPosition({ (GetPosition().x + bullet->GetPosition().x) / 2,(GetPosition().y + bullet->GetPosition().y) / 2, 0.0f });
						e->SetScale({ 3.0f,3.0f,3.0f });
						e->SetAnimationSlow(5);

						m_se->Load("asset\\audio\\se\\damagew.wav");
						m_se->Play();

						bullet->SetDestroy();
						SetDestroy();
					}
				}

			}


			//���Ԍo�߁@���˕Ԃ���E�@�ŏ���
			m_lifeTime += 1.0f;
			if (m_param.timeLimit <= m_lifeTime || m_param.baunsLimit <= 0) {
				SetDestroy();
			}
		}
	}
	else {
		m_time += 1.0f;
		if (m_time >= 10.0f) {
			SetDestroy();
		}
	}
}

void BulletMaster::Draw()
{
	PARAMETER param;
	param.Color = { m_color.x,m_color.y,m_color.z,m_color.w };
	Renderer::SetParameter(param);
	m_model->Draw(GetPosition(), GetRotation(), GetScale());

	if (m_type == BULLETTYPE_LASER ) {
		if (m_leaser->GetUse() == true) {
			m_leaser->Draw(GetPosition(), { 0.0f,0.0f,0.0f }, { m_leaserSize.x,m_leaserSize.y,0.0f });
		}
	}
}

void BulletMaster::BulletRotate()
{
	D3DXVECTOR3 vec = m_velocity;
	float n = sqrt(pow(vec.x, 2) + pow(vec.y, 2)) / 2;
	vec.x = (m_velocity.x / n);
	vec.y = (m_velocity.y / n);
	SetRotation(D3DXVECTOR3(D3DX_PI - atan2(vec.y, vec.x), D3DX_PI * 0.5f, 0.0f));
}

void BulletMaster::SetBulletType(BULLETTYPE type)
{
	m_type = type;

	m_model = new Parts;

	m_collision = new Collision;
	switch (m_type)
	{
	case BULLETTYPE_NORMAL:
		m_model->SetModel("asset\\model\\bullet00.obj");
		m_model->SetShader("shader\\vertexLightingVS.cso", "shader\\vertexLightingPS.cso");

		m_param.speed = 0.25f;
		m_param.weight = 0.01f;
		m_param.elasticity = 1.0f;
		m_param.timeLimit = 200.0f;
		m_param.baunsLimit = 3;
		m_param.collision = true;
		m_param.move = true;
		m_param.collision = true;
		m_param.length = 1.0f;
		m_collision->SetCollisionSize(m_param.length / 2, m_param.length / 2, m_param.length / 2, m_param.length / 2);
		break;
	case BULLETTYPE_BOUND:
		m_model->SetModel("asset\\model\\bullet01.obj");
		m_model->SetShader("shader\\vertexLightingVS.cso", "shader\\vertexLightingPS.cso");

		m_param.speed = 0.05f;
		m_param.weight = 5.0f;
		m_param.elasticity = 3.0f;
		m_param.timeLimit = 300.0f;
		m_param.baunsLimit = 30;
		m_param.collision = true;
		m_param.move = true;
		m_param.length = 1.0f;
		m_collision->SetCollisionSize(m_param.length / 2, m_param.length / 2, m_param.length / 2, m_param.length / 2);
		break;
	case BULLETTYPE_LASER:
		m_model->SetModel("asset\\model\\bullet03.obj");
		m_model->SetShader("shader\\vertexLightingVS.cso", "shader\\vertexLightingPS.cso");

		m_param.speed = 0.05f;
		m_param.weight = 0.0f;
		m_param.elasticity = 3.0f;
		m_param.timeLimit = 600.0f;
		m_param.baunsLimit = 30;
		m_param.collision = true;
		m_param.move = true;
		m_param.length = 1.0f;
		m_collision->SetCollisionSize(m_param.length / 2, m_param.length / 2, m_param.length, m_param.length);

		m_leaserSize = { 0.8f,SCREEN_HEIGHT };

		m_leaser = new PolygonObject;
		m_leaser->Init();
		m_leaser->SetTexture({ 1.0f, 1.0f, 1.0f }, "asset\\texture\\00.jpg");
		m_leaser->SetUse(true);
		m_leaser->SetChangeColor(true);
		break;
	default:
		break;
	}

	m_lifeTime = 0.0f;

	SetPosition({ 0.0f, 0.0f, 0.0f });
	SetRotation({ 0.0f, 0.0f, 0.0f });
	SetScale({ 1.0f,1.0f,1.0f });
}