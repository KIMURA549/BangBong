#include"main.h"
#include"manager.h"
#include"renderer.h"
#include"scene.h"

#include "charactor.h"
#include"camera.h"
#include"field.h"
#include"weapon.h"
#include"block.h"
#include"bulletMaster.h"
#include"polygonObject.h"
#include"collision.h"
#include"parts.h"
#include"charactorController.h"
#include"effect.h"
#include"gameData.h"

#include"input.h"
#include"mouse.h"
#include"selectModel.h"
#include"audio.h"

#include<math.h>
void Charactor::Init()
{
	//位置　回転　モデルの大きさ　当たり判定の大きさ
	SetPosition(D3DXVECTOR3(-1.0f, 0.0f, 0.0f));
	SetRotation(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	SetScale(D3DXVECTOR3(0.01f, 0.01f, 0.01f));
	SetSize(D3DXVECTOR3(1.0f, 1.0f, 1.0f));

	/*m_motion = new CharactorMotion;
	m_motion->Init();*/
	m_body = new Parts;
	m_body->Init();
	m_body->SetModel("asset\\model\\body00.obj");
	m_body->SetShader("shader\\vertexLightingVS.cso", "shader\\vertexLightingPS.cso");

	CHARACTOR_STATE m_PlayerState = CHARACTOR_STATE_GROUND;

	Scene* scene = Manager::GetScene();
	m_camera = scene->GetGameObject<Camera>();
	m_limit = scene->GetGameObject<Field>()->GetLimit();

	m_arrowMove = new PolygonObject;
	m_arrowMove->SetTexture({ 0.5f, 0.5f, 0.5f }, "asset\\texture\\arrow.png");

	m_collision = new Collision;
	m_collision->SetCollisionSize(1.0f, 1.0f, 1.0f, 1.0f);

	m_weapon = nullptr;

	m_control = new Control;
	m_control->AIControlInit();

	m_se = AddComponent<Audio>();
	m_se->Init();


}
void Charactor::Uninit()
{
	if (m_body != NULL) {
		m_body->Uninit();
		delete m_body;
	}
	if (m_collision != NULL) {
		m_collision->Uninit();
		delete m_collision;
	}
	if (m_arrowMove != NULL) {
		m_arrowMove->Uninit();
		delete m_arrowMove;
	}
	GameObject::Uninit();
}
void Charactor::Update() {


	if (m_spawnSe == false) {
		m_se->Load("asset\\audio\\se\\spawnc.wav");
		m_se->Play();
		m_spawnSe = true;
	}

	if (m_effect != nullptr) {
		if (m_life > 0) {
			if (m_effect->GetAnimNum() >= 13) {
				m_effect = nullptr;
			}
		}
		else {
			if (m_effect->GetAnimNum() >= 5) {
				SetDestroy();
			}
		}
		
	}
	else {
		//操作
		if (Manager::GetScene()->GetStandFlag() == false) {
			if (m_player == true) {
				PlayerControl();
			}
			else {
				AIControl();
			}
		}
		

		switch (m_state)
		{
		case CHARACTOR_STATE_GROUND:
			UpdateGround();
			UpdateWeapon();
			break;
		case CHARACTOR_STATE_JUMP:
			UpdateJump();
			break;
		case CHARACTOR_STATE_MOVE:
			UpdateMove();
			UpdateWeapon();
			break;
		case CHARACTOR_STATE_DAMAGE:
			UpdateDamage();
			break;
		default:
			break;
		}


		//コンポーネントのアップデート
		GameObject::Update();

		if (m_left == true) {
			SetRotation(D3DXVECTOR3(0.0f, D3DX_PI * -0.35f, 0.0f));
		}
		else {
			SetRotation(D3DXVECTOR3(0.0f, D3DX_PI * 0.35f, 0.0f));
		}
	}
}

void Charactor::Draw()
{
	if (m_effect == nullptr) {
		Scene* scene = Manager::GetScene();
		Camera* camera = scene->GetGameObject<Camera>();

		//見え方の補正
		float ry = -(D3DX_PI / 360 * ((m_camera->GetPosition().x - m_Position.x) * 3));

		//マトリクス設定
		D3DXMATRIX world, scale, rot, trans;
		D3DXMatrixScaling(&scale, GetScale().x, GetScale().y, GetScale().z);
		D3DXMatrixRotationYawPitchRoll(&rot, GetRotation().y + ry, GetRotation().x, GetRotation().z);
		D3DXMatrixTranslation(&trans, GetPosition().x, GetPosition().y, GetPosition().z);

		world = scale * rot *trans;
		m_Matrix = world;
		Renderer::SetWorldMatrix(&world);


		PARAMETER param;
		param.Color = { m_color.x,m_color.y,m_color.z,m_color.w };
		Renderer::SetParameter(param);

		m_body->Draw(m_Position, m_Rotation, m_Scale);

		if (m_arrowMove->GetUse() == true) {
			MoveArrowDraw();//更新も入ってる
		}

		if (m_player == true) {

			if (m_attack == true && m_weapon != nullptr) {
				m_weapon->DrawTargetIcon();
			}
		}
	}
}

void Charactor::UpdateGround()
{
	//移動
	//GetMouseButtonL()
	//Input::GetKeyPress('S')
	if (m_control->GetControl() == CONTROLSTATE_S) {

		//Cursorの初期位置を獲得　初期は少し下にずらす
		if (m_arrowMove->GetUse() == false) {
			m_arrowMove->SetUse(true);
			m_mouseStartPos.x = m_control->GetMousePointX();
			m_mouseStartPos.y = m_control->GetMousePointY() - 5.0f;
		}

		//薄くなって広がる
		if (m_Scale.y >= 0.5f) {
			m_Scale.y *= 0.998f;
			m_Scale.x *= 1.001f;
			m_Scale.z *= 1.001f;
			//力をScaleに応じて大きくする
			if (m_Scale.y < 0.5f) {
				if (m_power < 6) {
					m_se->Load("asset\\audio\\se\\charge.wav");
					m_se->Play();
				}
				m_power = 6;
			}
			else if (m_Scale.y < 0.6f) {
				if (m_power < 5) {
					m_se->Load("asset\\audio\\se\\charge.wav");
					m_se->Play();
					m_power = 5;
				}
			}
			else if (m_Scale.y < 0.7f) {
				if (m_power < 4) {
					m_se->Load("asset\\audio\\se\\charge.wav");
					m_se->Play();
					m_power = 4;
				}
			}
			else if (m_Scale.y < 0.8f) {
				if (m_power < 3) {
					m_se->Load("asset\\audio\\se\\charge.wav");
					m_se->Play();
					m_power = 3;
				}
			}
			else if (m_Scale.y < 0.9f) {
				if (m_power < 2) {
					m_se->Load("asset\\audio\\se\\charge.wav");
					m_se->Play();
					m_power = 2;
				}
			}
			else if (m_Scale.y < 1.0f) {
				if (m_power < 1) {
					m_se->Load("asset\\audio\\se\\charge.wav");
					m_se->Play();
					m_power = 1;
				}
			}
		}
	}
	else {

		m_arrowMove->SetUse(false);
		if (m_Scale.y < 1.0f) {

			//ベクトルから
			m_state = CHARACTOR_STATE_JUMP;
			//iconの初期位置を０にしてから長さを減らして　力の大きさを与える
			m_velocity.x = (m_arrowPos.x)  * (m_power * 0.1f);
			m_velocity.y = (m_arrowPos.y + 0.1f)  *(m_power * 0.1f);

		}
	}

	m_Position.y = m_ground + (m_Scale.y / 2);


	if (m_player == false && m_attack == false) {
		PickUpWeapon();
	}

	if (m_tackle == true) {
		m_tackle = false;
	}
}


void Charactor::UpdateJump()
{
	//Scaleを戻す
	if (m_Scale.y < 1.0f) {
		m_Scale.y *= 1.5f;
		if (m_Scale.y >= 1.0f) {
			m_Scale.y = 1.0f;
		}
	}
	else if (m_Scale.y > 1.0f) {
		m_Scale.y *= 0.5f;
		if (m_Scale.y <= 1.0f) {
			m_Scale.y = 1.0f;
		}
	}

	if (m_Scale.x < 1.0f) {
		m_Scale.x *= 1.1f;
		if (m_Scale.x >= 1.0f) {
			m_Scale.x = 1.0f;
		}
	}
	else if (m_Scale.x > 1.0f) {
		m_Scale.x *= 0.9f;
		if (m_Scale.x <= 1.0f) {
			m_Scale.x = 1.0f;
		}
	}

	if (m_Scale.z < 1.0f) {
		m_Scale.z *= 1.1f;
		if (m_Scale.z >= 1.0f) {
			m_Scale.z = 1.0f;
		}
	}
	else if (m_Scale.z > 1.0f) {
		m_Scale.z *= 0.9f;
		if (m_Scale.z <= 1.0f) {
			m_Scale.z = 1.0f;
		}
	}

	//スケールが戻ったら移動開始
	if (m_Scale.x == 1.0f && m_Scale.y == 1.0f && m_Scale.z == 1.0f) {
		m_state = CHARACTOR_STATE_MOVE;
		if (m_power < 3) {
			m_se->Load("asset\\audio\\se\\jump1.wav");
			m_se->Play();
		}
		else if (m_power < 5) {
			m_se->Load("asset\\audio\\se\\jump2.wav");
			m_se->Play();
		}
		else {
			m_se->Load("asset\\audio\\se\\jump3.wav");
			m_se->Play();
		}

		m_power = 0;
	}
}

void Charactor::UpdateMove()
{
	m_power = 0;

	m_Position += m_velocity;

	//減衰
	m_velocity *= 0.999f;
	m_velocity.y -= 0.01f * m_weight;
	
	Scene* scene = Manager::GetScene();

	//フィールドにあるブロックとの当たり判定
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

				m_se->Load("asset\\audio\\se\\bound.wav");
				m_se->Play();
			}
			else {
				if (block->GetPosition().y - GetPosition().y > 0) {
					//下
					m_Position.y = block->GetPosition().y - block->GetCollision().GetCollisionSize().down - m_collision->GetCollisionSize().up;
					m_velocity.y = (m_weight - (m_elasticity * 1.0f)) * m_velocity.y;
					m_se->Load("asset\\audio\\se\\bound.wav");
					m_se->Play();

				}
				else {

					//上
					m_Position.y = block->GetPosition().y + block->GetCollision().GetCollisionSize().up + m_collision->GetCollisionSize().down;
					if (m_velocity.y >= -0.1f) {
						
						m_state = CHARACTOR_STATE_GROUND;
						m_velocity = { 0.0f,0.0f,0.0f };
						m_ground = block->GetPosition().y + block->GetCollision().GetCollisionSize().up;
					}
					else {
						m_velocity.y = (m_weight - (m_elasticity * 1.0f)) * m_velocity.y;
						m_se->Load("asset\\audio\\se\\bound.wav");
						m_se->Play();
					}
					
					
				}

			}
		}
	}

	//敵キャラクターに当たる
	//CHARACTERとの当たり判定
	std::vector<Charactor*>charactors = scene->GetGameObjects<Charactor>();
	for (Charactor* charactor : charactors) {

		if (m_collision->CollisionCircle2D(GetScale().x, { GetPosition().x, GetPosition().y }, { charactor->GetPosition().x, charactor->GetPosition().y })) {

			if (charactor->GetCharactorNumber() != m_number) {


				m_velocity *= -0.9f;

				//敵吹き飛ばす 武器を捨てさせる
				//自分が体当たりされていなかったら
				if (m_tackle == false) {

					m_camera->Shake(0.0f, 0.0f, 1.0f);
					charactor->ThrowWeapon();

					if (m_Position.x < charactor->GetPosition().x) {
						charactor->StartMove({ 0.1f,0.1f,0.0f });
					}
					else {
						charactor->StartMove({ -0.1f,0.1f,0.0f });
					}

					//相手が動いている場合
					if (charactor->GetState() == CHARACTOR_STATE_MOVE) {
						//相手も体当たりされていない場合
						if (charactor->GetTackleFlag() == false) {
							//自分も吹き飛ぶ
							ThrowWeapon();
							if (m_Position.x < charactor->GetPosition().x) {
								StartMove({ -0.1f,0.1f,0.0f });
							}
							else {
								StartMove({ 0.1f,0.1f,0.0f });
							}
						}
					}

					m_se->Load("asset\\audio\\se\\tackle.wav");
					m_se->Play();
				}


				
			}
		}

	}

	//左右
	if (m_limit.x > m_Position.x - (m_Scale.y / 2)) {
		m_Position.x = m_limit.x + (m_Scale.y / 2);
		m_velocity.x = (m_weight - (m_elasticity * 1.0f)) * m_velocity.x;
		m_se->Load("asset\\audio\\se\\bound.wav");
		m_se->Play();
	}
	else if(m_limit.z < m_Position.x + (m_Scale.y / 2)) {
		m_Position.x = m_limit.z- (m_Scale.y / 2);
		m_velocity.x = (m_weight - (m_elasticity * 1.0f)) * m_velocity.x;
		m_se->Load("asset\\audio\\se\\bound.wav");
		m_se->Play();
	}

	//上下
	if (m_limit.y > m_Position.y - (m_Scale.y / 2)) {
		m_Position.y = m_limit.y + (m_Scale.y / 2);
		if (m_velocity.y >= -0.1f) {
			m_state = CHARACTOR_STATE_GROUND;
			m_velocity = { 0.0f,0.0f,0.0f };
			m_ground = m_limit.y;
		}
		else {
			m_velocity.y = (m_weight - (m_elasticity * 1.0f)) * m_velocity.y;
			m_se->Load("asset\\audio\\se\\bound.wav");
			m_se->Play();
		}
	}
	else if (m_limit.w < m_Position.y + (m_Scale.y / 2)) {
		m_Position.y = m_limit.w - (m_Scale.y / 2);
		m_velocity.y = (m_weight - (m_elasticity * 1.0f)) * m_velocity.y;
		m_se->Load("asset\\audio\\se\\bound.wav");
		m_se->Play();
	}


	if (m_player == false && m_attack == false) {
		PickUpWeapon();
	}

}

void Charactor::UpdateDamage()
{

	m_Position += m_velocity;
	Scene* scene = Manager::GetScene();

	scene->SetStandFlag(true);

	if (m_time <= 300.0f) {
		m_time += 1.0f;
		//フィールドにあるブロックとの当たり判定
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
						m_velocity.x *= -1.0f;
						m_camera->Shake(1.0f, 0.0f, 0.0f);
					}
					else {
						//右
						m_Position.x = block->GetPosition().x + block->GetCollision().GetCollisionSize().right + m_collision->GetCollisionSize().left;
						m_velocity.x *= -1.0f;
						m_camera->Shake(-1.0f, 0.0f, 0.0f);
					}
				}
				else {
					if (block->GetPosition().y - GetPosition().y > 0) {
						//下
						m_Position.y = block->GetPosition().y - block->GetCollision().GetCollisionSize().down - m_collision->GetCollisionSize().up;
						m_velocity.y *= -1.0f;
						m_camera->Shake(0.0f, 1.0f, 0.0f);

					}
					else {
						//上
						m_Position.y = block->GetPosition().y + block->GetCollision().GetCollisionSize().up + m_collision->GetCollisionSize().down;
						m_velocity.y *= -1.0f;
						m_camera->Shake(0.0f, -1.0f, 0.0f);
					}
				}

				m_se->Load("asset\\audio\\se\\damagebound.wav");
				m_se->Play();
			}
		}

		//フィールドとの当たり判定
		//左右
		if (m_limit.x > m_Position.x) {
			m_Position.x = m_limit.x;
			m_velocity.x *= -1.0f;
			m_camera->Shake(1.0f, 0.0f, 0.0f);
			m_se->Load("asset\\audio\\se\\damagebound.wav");
			m_se->Play();
		}
		else if (m_limit.z < m_Position.x) {
			m_Position.x = m_limit.z;
			m_velocity.x *= -1.0f;
			m_camera->Shake(-1.0f, 0.0f, 0.0f);
			m_se->Load("asset\\audio\\se\\damagebound.wav");
			m_se->Play();
		}

		//上下
		if (m_limit.y > m_Position.y - (m_Scale.y / 2)) {
			m_Position.y = m_limit.y + (m_Scale.y / 2);

			m_velocity.y *= -1.0f;
			m_camera->Shake(0.0f, 1.0f, 0.0f);
			m_se->Load("asset\\audio\\se\\damagebound.wav");
			m_se->Play();
		}
		else if (m_limit.w < m_Position.y + (m_Scale.y / 2)) {
			m_Position.y = m_limit.w - (m_Scale.y / 2);
			m_velocity.y *= -1.0f;
			m_camera->Shake(0.0f, -1.0f, 0.0f);
			m_se->Load("asset\\audio\\se\\damagebound.wav");
			m_se->Play();
		}
	}
	else {
		//減衰
		m_velocity *= 0.999f;
		m_velocity.y -= 0.01f * m_weight;


		std::vector<Charactor*>charactors = scene->GetGameObjects<Charactor>();
		for (Charactor* charactor : charactors) {

			if (m_collision->CollisionCircle2D(GetScale().x, { GetPosition().x, GetPosition().y }, { charactor->GetPosition().x, charactor->GetPosition().y })) {

				if (charactor->GetCharactorNumber() != m_number) {
					m_velocity *= -1.0f;
					m_se->Load("asset\\audio\\se\\bound.wav");
					m_se->Play();
				}
			}

		}

		//フィールドにあるブロックとの当たり判定
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

							scene->SetStandFlag(false);
							m_state = CHARACTOR_STATE_GROUND;
							m_velocity = { 0.0f,0.0f,0.0f };
							m_time = 0.0f;
							m_ground = block->GetPosition().y + block->GetCollision().GetCollisionSize().up;

							m_Position.y = m_ground + (m_Scale.y / 2);

							if (m_life <= 0) {
								//ゲーム終了
								scene->SetStandFlag(true);
								scene->SetGameState(GAMESTATE_FINISH);
								DeadCharactor();
							}
						}
						else {
							m_velocity.y = (m_weight - (m_elasticity * 1.0f)) * m_velocity.y;
						}
					}
				}

				m_se->Load("asset\\audio\\se\\bound.wav");
				m_se->Play();
			}
		}


		//フィールドとの当たり判定
		//左右
		if (m_limit.x > m_Position.x) {
			m_Position.x = m_limit.x;
			m_velocity.x = (m_weight - (m_elasticity * 1.0f)) * m_velocity.x;
			m_se->Load("asset\\audio\\se\\bound.wav");
			m_se->Play();
		}
		else if (m_limit.z < m_Position.x) {
			m_Position.x = m_limit.z;
			m_velocity.x = (m_weight - (m_elasticity * 1.0f)) * m_velocity.x;
			m_se->Load("asset\\audio\\se\\bound.wav");
			m_se->Play();
		}

		//上下
		if (m_limit.y > m_Position.y - (m_Scale.y / 2)) {
			m_Position.y = m_limit.y + (m_Scale.y / 2);
			if (m_velocity.y >= -0.1f) {

				scene->SetStandFlag(false);
				m_state = CHARACTOR_STATE_GROUND;
				m_velocity = { 0.0f,0.0f,0.0f };
				m_time = 0.0f;
				m_ground = m_limit.y;
				if (m_life <= 0) {
					//ゲーム終了
					scene->SetStandFlag(true);
					scene->SetGameState(GAMESTATE_FINISH);
					DeadCharactor();
				}
			}
			else {
				m_velocity.y = (m_weight - (m_elasticity * 1.0f)) * m_velocity.y;
				m_se->Load("asset\\audio\\se\\bound.wav");
				m_se->Play();
			}
		}
		else if (m_limit.w < m_Position.y + (m_Scale.y / 2)) {
			m_Position.y = m_limit.w - (m_Scale.y / 2);
			m_velocity.y = (m_weight - (m_elasticity * 1.0f)) * m_velocity.y;
			m_se->Load("asset\\audio\\se\\bound.wav");
			m_se->Play();
		}

	}


}

void Charactor::UpdateWeapon()
{
	if (m_attack == true) {
		//武器を構えている場合
		float n, rx;//正規化した値　角度
		//初期位置から現在地のベクトル（正規化）
		if (m_player == true) {
			m_arrowPos.x = m_control->GetMousePointX() - m_mouseStartPos.x;
			m_arrowPos.y = m_control->GetMousePointY() - m_mouseStartPos.y;
		}
		else{

			m_arrowPos.x = m_control->GetAIMousePointX();
			m_arrowPos.y = m_control->GetAIMousePointY();
			
		}
		

		if (m_weapon != nullptr) {

			//正規化　長さ指定
			n = sqrt(pow(m_arrowPos.x, 2) + pow(m_arrowPos.y, 2));
			//キャラの位置から
			m_arrowPos.x = (m_arrowPos.x / n);
			m_arrowPos.y = (m_arrowPos.y / n);


			//武器の回転限界
			if (m_arrowPos.x <= m_weapon->GetRotLimitMin().x) {
				m_arrowPos.x = m_weapon->GetRotLimitMin().x;
			}
			else if (m_arrowPos.x > m_weapon->GetRotLimitMax().x) {
				m_arrowPos.x = m_weapon->GetRotLimitMax().x;
			}

			if (m_arrowPos.y <= m_weapon->GetRotLimitMin().y) {
				m_arrowPos.y = m_weapon->GetRotLimitMin().y;
			}
			else if (m_arrowPos.y > m_weapon->GetRotLimitMax().y) {
				m_arrowPos.y = m_weapon->GetRotLimitMax().y;
			}

			//キャラの位置から
			rx = atan2(m_arrowPos.y, m_arrowPos.x);

			if (m_arrowPos.x < 0) {
				m_weapon->SetStand(GetPosition(), true, { rx, 0.0f, 0.0f });
				m_left = false;
			}
			else {
				m_weapon->SetStand(GetPosition(), false, { rx, 0.0f, 0.0f });
				m_left = true;
			}


			if (m_control->GetControl() == CONTROLSTATE_RELESE) {
				ThrowWeapon();
				m_attack = false;
			}
			else if (m_control->GetControl() != CONTROLSTATE_MOUSE_L) {
				//武器が弾を作る
				m_weapon->Fire();
				//武器に設定された反動を受け取る
				m_state = CHARACTOR_STATE_MOVE;
				m_velocity = m_weapon->GetRecoil();
				//攻撃フラグをOFF
				m_attack = false;
			}
		}
	}
	else {
		//通常の状態

		//武器を拾う
		if (m_control->GetControl() == CONTROLSTATE_MOUSE_R) {
			PickUpWeapon();
		}

		//武器構え
		if (m_control->GetControl() == CONTROLSTATE_MOUSE_L) {
			if (m_weapon != nullptr && m_weapon->GetWeaponState() == WEAPONSTATE_HAVE) {
				m_attack = true;
				m_mouseStartPos.x = m_control->GetMousePointX() - 1.0f;
				m_mouseStartPos.y = m_control->GetMousePointY();
				m_weapon->SetStandWeapon();
			}
		}

	}

	if (m_weapon != nullptr) {
		m_weapon->SetStandPos(m_Position);
	}
}

void Charactor::Damage( D3DXVECTOR3 vel)
{
	m_time = 0.0f;

	m_life -= 1;
	m_velocity = vel;
	m_Position.y += 0.1f;
	m_attack = false;
	m_state = CHARACTOR_STATE_DAMAGE;

	SetScale({ 1.0f,1.0f,1.0f });

	m_arrowMove->SetUse(false);
	m_power = 0;

	if (m_weapon != nullptr) {
		m_weapon->SetState(WEAPONSTATE_HAVE);
	}
}

void Charactor::DeadCharactor()
{
	Scene* scene = Manager::GetScene();
	m_effect = scene->AddGameObject<Effect>(1);
	m_effect->SetEffect("asset\\texture\\deathcharactor.png", { 20.0f,20.0f,20.0f }, 10, 1, false);
	m_effect->SetPosition(GetPosition());
	m_effect->SetScale({ 5.0f,5.0f,5.0f });
	m_effect->SetAnimationSlow(3);

	if (m_weapon != nullptr) {
		ThrowWeaponUP();
	}

}

void Charactor::SetCharactor(D3DXVECTOR3 pos, D3DXVECTOR3 scl, D3DXVECTOR4 size, bool left, float w, float e, int num)
{
	//位置　回転　モデルの大きさ　当たり判定の大きさ
	SetPosition(pos);
	SetScale(scl);
	m_collision->SetCollisionSize(size.x, size.y, size.z, size.w);

	m_left = left;
	if (m_left == true) {
		SetRotation(D3DXVECTOR3(0.0f, D3DX_PI * -0.35f, 0.0f));
	}
	else {
		SetRotation(D3DXVECTOR3(0.0f, D3DX_PI * 0.35f, 0.0f));
	}

	m_weight = w;
	m_elasticity = e;
	m_number = num;

	m_state = CHARACTOR_STATE_MOVE;

	Scene* scene = Manager::GetScene();
	m_effect = scene->AddGameObject<Effect>(1);
	m_effect->SetEffect("asset\\texture\\spawncharactor.png", { 20.0f,20.0f,20.0f }, 5, 4, false);
	m_effect->SetPosition(GetPosition());
	m_effect->SetScale({ 5.0f,5.0f,5.0f });
	m_effect->SetAnimationSlow(3);
	m_spawnSe = false;
}

void Charactor::SetBody(int select)
{
	m_body = new Parts;
	m_body->Init();
	SelectModel::GetBodyModel(select, m_body);
	m_body->SetShader("shader\\vertexLightingVS.cso", "shader\\vertexLightingPS.cso");
}

void Charactor::SetColor(int select)
{
	SelectModel::GetColor(select, &m_color);
}

void Charactor::StartMove(D3DXVECTOR3 vel)
{
	m_velocity = vel * 2;
	m_state = CHARACTOR_STATE_MOVE;
	m_tackle = true;
	SetScale({ 1.0f,1.0f,1.0f });
}

void Charactor::PickUpWeapon()
{
	if (m_weapon == nullptr) {
		Scene* scene = Manager::GetScene();
		std::vector<Weapon*>weapons = scene->GetGameObjects<Weapon>();

		for (Weapon* weapon : weapons) {

			if (m_collision->CollisionBox2D({ m_Position.x, m_Position.y },
				weapon->GetCollision(), { weapon->GetPosition().x,weapon->GetPosition().y }) == true && weapon->HaveWeapon() == false &&
				(weapon->GetWeaponState() == WEAPONSTATE_MOVE || weapon->GetWeaponState() == WEAPONSTATE_STAY)) {

				m_weapon = weapon;
				m_weapon->HaveWeapon(m_number);
				m_weapon->SetColor(m_color);
				m_se->Load("asset\\audio\\se\\pickup.wav");
				m_se->Play();
				break;
			}
		}
	}
}

void Charactor::ThrowWeapon()
{
	if (m_weapon != nullptr) {
		m_weapon->SetPosition(m_Position);

		m_weapon->SetColor({ 0.5f,0.5f,0.5f,1.0f });

		if (m_left == true) {
			m_weapon->ReleaseWeapon({ 0.3f,0.3f,0.0f });
		}
		else {
			m_weapon->ReleaseWeapon({ -0.3f,0.3f,0.0f });
		}

		m_weapon = nullptr;

		m_attack = false;
	}
}

void Charactor::ThrowWeaponUP()
{
	if (m_weapon != nullptr) {
		m_weapon->SetPosition(m_Position);

		m_weapon->SetColor({ 0.5f,0.5f,0.5f,1.0f });

		m_weapon->ReleaseWeapon({ 0.0f,0.3f,0.0f });

		m_weapon = nullptr;

		m_attack = false;
	}
}

void Charactor::MoveArrowDraw()
{
	float n, rz;//正規化した値　角度
	//初期位置から現在地のベクトル（正規化）
	if (m_player == true) {
		m_arrowPos.x = m_control->GetMousePointX() - m_mouseStartPos.x;
		m_arrowPos.y = m_control->GetMousePointY() - m_mouseStartPos.y;
	}
	else {
		m_arrowPos.x = m_control->GetAIMousePointX();
		m_arrowPos.y = m_control->GetAIMousePointY();
	}

	if (m_arrowPos.y <= 0.0f) {
		m_arrowPos.y = 0.0f;
	}

	if (m_arrowPos.y < m_arrowPos.x / 2) {
		m_arrowPos.y = m_arrowPos.x / 2;
	}
	else if (m_arrowPos.y < -m_arrowPos.x / 2) {
		m_arrowPos.y = (-m_arrowPos.x) / 2;
	}
	if (m_arrowPos.x < 0.0f) {
		m_left = false;
	}
	else {

		m_left = true;
	}
	
	//正規化　長さ指定
	n = sqrt(pow(m_arrowPos.x, 2) + pow(m_arrowPos.y, 2));

	//キャラの位置から
	m_arrowPos.x = (m_arrowPos.x / n) ;
	m_arrowPos.y = (m_arrowPos.y / n) ;
	//アングル　から　９０度引く（初期が真上だから）
	rz = atan2(m_arrowPos.y, m_arrowPos.x) - (D3DX_PI / 2);

	if (m_player == true) {
		m_arrowMove->Draw({ m_arrowPos.x + m_Position.x,m_arrowPos.y + m_Position.y, 0.0f }, { 0.0f,0.0f, rz },
			{ 0.75f * m_power, 1.0f * m_power  ,1.0f * m_power });
	}

}


void Charactor::PlayerControl()
{
	m_control->InputController();

	Scene* scene = Manager::GetScene();
	if (scene->GetStandFlag() == true) {
		m_control->SetController(CONTROLSTATE_NONE);
	}
}

void Charactor::AIControl()
{
	Scene* scene = Manager::GetScene();

	//回避
	//敵が近い場合は体当たり
	std::vector<Charactor*>charactors = scene->GetGameObjects<Charactor>();
	for (Charactor* charactor : charactors) {
		if (charactor->GetCharactorNumber() != m_number) {
			if (charactor->GetWeapon() != nullptr) {
				if (charactor->GetWeapon()->GetBulletCnt() > 0) {
					//相手との距離が得近ければ体当たり
					if (sqrt(pow(charactor->GetPosition().x - m_Position.x, 2) + pow(charactor->GetPosition().y - m_Position.y, 2)) <= 1.5f) {
						m_control->AIEscapeEnd(charactor->GetPosition().x - m_Position.x, charactor->GetPosition().y - m_Position.y);
					}
					else if (sqrt(pow(charactor->GetPosition().x - m_Position.x, 2) + pow(charactor->GetPosition().y - m_Position.y, 2)) <= 5.0f) {
						m_control->AIEscape(charactor->GetPosition().x - m_Position.x, charactor->GetPosition().y - m_Position.y, 2);
					}
					else {
						//相手が武器を構えたら
						if (charactor->GetWeaponAttack() == true) {
							if (charactor->GetPosition().x > m_Position.x) {
								m_control->AIEscapeRandom(-0.5f, -0.25f, 0.8f, 1.0f);
								m_control->ResetThinkTime(100.0f);
							}
							else {
								m_control->AIEscapeRandom(0.25f, 0.5f, 0.8f, 1.0f);
								m_control->ResetThinkTime(100.0f);
							}
						}
					}

					

					break;
				}
			}

		}
	}

	//弾丸
	std::vector<BulletMaster*>bullets = scene->GetGameObjects<BulletMaster>();
	for (BulletMaster* bullet : bullets) {
		if (m_number != bullet->GetNumber()) {
			switch (bullet->GetBulletType())
			{
			case BULLETTYPE_NORMAL:
				//特定の距離に弾が近づく
				if (sqrt(pow(bullet->GetPosition().x - m_Position.x, 2) + pow(bullet->GetPosition().y - m_Position.y, 2)) <= 10.0f) {
					//弾が左側
					if (bullet->GetPosition().x < m_Position.x) {
						m_control->AIEscapeRandom(0.5f, 1.0f, 0.8f, 1.0f);
					}
					else {
						m_control->AIEscapeRandom(-1.0f, -0.5f, 0.8f, 1.0f);
					}
					m_control->ResetThinkTime(100.0f);
				}

				//ギリギリまでひきつける
				if (m_Scale.y < 0.9f) {
					if (sqrt(pow(bullet->GetPosition().x - m_Position.x, 2) + pow(bullet->GetPosition().y - m_Position.y, 2)) <= 5.0f) {
						//弾が当たる軌道の場合
						for (int i = 1; i < 5; i++) {
							if (sqrt(pow((bullet->GetPosition().x - (bullet->GetForward().x * i)) - m_Position.x, 2)
								+ pow(bullet->GetPosition().y - (bullet->GetForward().y * i) - m_Position.y, 2)) <= 1.0f) {

								//自分より上から来てる
								if (GetPosition().y + 0.5f < bullet->GetPosition().y) {
									//右から来てる
									if (GetPosition().x < bullet->GetPosition().x) {
										m_control->AIEscapeEnd(-1.0f, 0.3f);
									}
									else {
										m_control->AIEscapeEnd(1.0f, 0.3f);
									}
								}
								else {
									//右から来てる
									if (GetPosition().x < bullet->GetPosition().x) {
										m_control->AIEscapeEnd(-0.1f, 1.0f);
									}
									else {
										m_control->AIEscapeEnd(0.1f, 1.0f);
									}

								}


								break;
							}
						}
					}
				}

				break;
			case  BULLETTYPE_LASER:
				if (sqrt(pow(bullet->GetPosition().x - m_Position.x, 2) + pow(bullet->GetPosition().y - m_Position.y, 2)) <= 5.0f) {
					if (bullet->GetPosition().x < m_Position.x) {
						m_control->AIEscape(1.0f, 1.0f, 1);
					}
					else {
						m_control->AIEscape(-1.0f, 1.0f, 1);
					}
				}
				break;
			default:
				break;
			}
		}

	}

	//移動のシミュレーション
	if (m_state == CHARACTOR_STATE_GROUND && m_control->GetSimurationFlag() == false && m_control->GetAIState() == AISTATE_THINK) {
		m_control->SimulationInit(m_number, GetPosition(), m_weight, m_elasticity, m_collision, m_limit);
	}

	//武器を持っていない場合
	//近くにあれば武器を拾う
	if (m_weapon == nullptr) {
		std::vector<Weapon*>weapons = scene->GetGameObjects<Weapon>();
		for (Weapon* weapon : weapons) {
			if(weapon->GetWeaponState() == WEAPONSTATE_STAY || weapon->GetWeaponState() == WEAPONSTATE_MOVE) {
				if (sqrt(pow(weapon->GetPosition().x - m_Position.x, 2) + pow(weapon->GetPosition().y - m_Position.y, 2)) <= 3.0f) {
					if (weapon->GetPosition().y > m_Position.y) {
						m_control->SetAIMove(weapon->GetPosition() + D3DXVECTOR3(0.0f,3.0f,0.0f) -GetPosition(), 2);
					}
					else {
						m_control->SetAIMove(weapon->GetPosition() - GetPosition(), 1);
					}
				}
			}
		}
		//武器があれば拾いに行く
		if (m_control->GetSimurationFlag() == true) {
			m_control->CheckSimMove(SIMENDSTATE_WEAPON);
		}
	}
	else {
		std::vector<Charactor*>charactors = scene->GetGameObjects<Charactor>();
		for (Charactor* charactor : charactors) {
			if (charactor->GetCharactorNumber() != m_number) {

				switch (m_weapon->GetBulletTypeNum())
				{
				case 1:
					m_control->SetAIAttack(200.0f);
					m_control->SetAIMousePoint(charactor->GetPosition().x - GetPosition().x, charactor->GetPosition().y - GetPosition().y + 0.5f);
					break;
				case 2:
					m_control->SetAIAttack(200.0f);
					m_control->SetAIMousePoint(charactor->GetPosition().x - GetPosition().x, charactor->GetPosition().y - GetPosition().y);
					break;
				case 3:
					if (sqrt(pow(charactor->GetPosition().x - m_Position.x, 2) + pow(charactor->GetPosition().y - m_Position.y, 2)) <= 8.0f) {
						m_control->SetAIAttack(200.0f);
						m_control->SetAIMousePoint(charactor->GetPosition().x - GetPosition().x, charactor->GetPosition().y + 5.0f - GetPosition().y);
					}
					else {
						m_control->SetAIMoveRandom();
					}
					break;
				default:
					break;
				}
				break;

			}
		}
		
		
		//武器の弾数が０だったら武器捨てる
		if (m_weapon->GetBulletCnt() <= 0) {
			ThrowWeapon();
		}
	}


	m_control->AIController();

}