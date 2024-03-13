
//#include"manager.h"
//#include"renderer.h"
//#include"player.h"
//#include"createModel.h"
//#include"input.h"
//#include"scene.h"
//#include"bullet.h"
//#include"cylinder.h"

#include"main.h"
#include"renderer.h"
#include "enemy.h"
#include "charactorMotion.h"

#include"input.h"

void Enemy::Init()
{
	//位置　回転　モデルの大きさ　当たり判定の大きさ
	SetPosition(D3DXVECTOR3(2.5f, 0.0f, 0.0f));
	SetRotation(D3DXVECTOR3(0.0f, -D3DX_PI * 0.5f, 0.0f));
	SetScale(D3DXVECTOR3(1.0f, 1.0f, 1.0f));
	SetSize(D3DXVECTOR3(1.0f, 1.0f, 1.0f));


	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout,
		"shader\\vertexLightingVS.cso");
	Renderer::CreatePixelShader(&m_PixelShader,
		"shader\\vertexLightingPS.cso");

	m_charactor = new CharactorMotion;
	m_charactor->Init();

	PLAYER_STATE m_PlayerState = PLAYER_STATE_GROUND;



	m_left = true;
	if (m_left == true) {
		SetRotation(D3DXVECTOR3(0.0f, D3DX_PI * -0.5f, 0.0f));
	}
	else {
		SetRotation(D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f));
	}

}
void Enemy::Uninit()
{
	GameObject::Uninit();
	
	if (m_VertexLayout != NULL) {
		m_VertexLayout->Release();
	}
	if (m_VertexShader != NULL) {
		m_VertexShader->Release();
	}
	if (m_PixelShader != NULL) {
		m_PixelShader->Release();
	}
}
void Enemy::Update() {


	//m_move->SetNowPosition(m_transform->GetPosition());
	////移動
	//if (Input::GetKeyPress('A')) {
	//	m_move->MoveLeft(0.05f);
	//}
	//else if (Input::GetKeyPress('D')) {
	//	m_move->MoveRight(0.05f);
	//}

	////ジャンプ
	//if (Input::GetKeyTrigger('W')) {
	//	m_move->Jump(0.2f);
	//}

	////コンポーネントのアップデート
	//GameObject::Update();

	//m_transform->SetPosition(m_move->GetNextPosition());




}
void Enemy::Draw()
{
	//入力レイアウト
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	//シェーダー設定
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	//マトリクス設定
	D3DXMATRIX world, scale, rot, trans;
	D3DXMatrixScaling(&scale, GetScale().x, GetScale().y, GetScale().z);
	D3DXMatrixRotationYawPitchRoll(&rot, GetRotation().y + D3DX_PI, GetRotation().x, GetRotation().z);
	D3DXMatrixTranslation(&trans, GetPosition().x, GetPosition().y, GetPosition().z);
	world = scale * rot  *trans;
	Renderer::SetWorldMatrix(&world);

	GameObject::Draw();
}
