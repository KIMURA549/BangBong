
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
#include "child.h"
#include"charactor.h"
//#include "createModel.h"
#include"input.h"

void Child::Init()
{
	//�ʒu�@��]�@���f���̑傫���@�����蔻��̑傫��
	GameObject::SetPosition(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	SetRotation(D3DXVECTOR3(0.0f, -D3DX_PI * 0.5f, 0.0f));
	SetScale(D3DXVECTOR3(1.0f, 1.0f, 1.0f));
	SetSize(D3DXVECTOR3(1.0f, 1.0f, 1.0f));

	//AddComponent<CreateModel>()->Init("asset\\model\\torus.obj");

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout,
		"shader\\vertexLightingVS.cso");
	Renderer::CreatePixelShader(&m_PixelShader,
		"shader\\vertexLightingPS.cso");


}
void Child::Uninit()
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
void Child::Update() {


	//m_move->SetNowPosition(m_transform->GetPosition());
	////�ړ�
	//if (Input::GetKeyPress('A')) {
	//	m_move->MoveLeft(0.05f);
	//}
	//else if (Input::GetKeyPress('D')) {
	//	m_move->MoveRight(0.05f);
	//}

	////�W�����v
	//if (Input::GetKeyTrigger('W')) {
	//	m_move->Jump(0.2f);
	//}

	////�R���|�[�l���g�̃A�b�v�f�[�g
	//GameObject::Update();

	//m_transform->SetPosition(m_move->GetNextPosition());




}
void Child::Draw()
{
	//���̓��C�A�E�g
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	//�V�F�[�_�[�ݒ�
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	//�}�g���N�X�ݒ�
	D3DXMATRIX world, scale, rot, trans;
	D3DXMatrixScaling(&scale, GetScale().x, GetScale().y, GetScale().z);
	D3DXMatrixRotationYawPitchRoll(&rot, GetRotation().y + D3DX_PI, GetRotation().x, GetRotation().z);
	D3DXMatrixTranslation(&trans, GetPosition().x, GetPosition().y, GetPosition().z);
	world = scale * rot  *trans * m_Parent->GetMatrix();
	Renderer::SetWorldMatrix(&world);

	GameObject::Draw();
}

void Child::SetPosition(float x, float y, float z)
{
	GameObject::SetPosition(D3DXVECTOR3(x, y, z));
}
