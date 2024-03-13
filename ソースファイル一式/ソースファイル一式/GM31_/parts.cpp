#include"main.h"
#include"renderer.h"

#include"model.h"
#include"parts.h"

void Parts::Init()
{

	m_model = new Model();
	m_model->Load("asset\\model\\body00.obj");

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout,
		"shader\\vertexLightingVS.cso");
	Renderer::CreatePixelShader(&m_PixelShader,
		"shader\\vertexLightingPS.cso");
}

void Parts::Uninit()
{
	m_model->Unload();
	delete m_model;

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

void Parts::Draw(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scl)
{
	// ワールドマトリックスの初期化（単位行列を作る関数）
	D3DXMatrixIdentity(&m_WorldMatrix);
	//入力レイアウト
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	//シェーダー設定
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	//マトリクス設定
	D3DXMATRIX world, scale, r, trans;
	D3DXMatrixScaling(&scale, scl.x, scl.y, scl.z);
	D3DXMatrixRotationYawPitchRoll(&r, rot.y, rot.x, rot.z);
	D3DXMatrixTranslation(&trans, pos.x, pos.y, pos.z);

	world = scale * r *trans;
	Renderer::SetWorldMatrix(&world);

	m_model->Draw();

	//スケールを1 1 1
	D3DXMatrixScaling(&scale, 1.0f, 1.0, 1.0);

	m_WorldMatrix = scale * r * trans;


}


void Parts::Draw(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scl, D3DXMATRIX w)
{
	// ワールドマトリックスの初期化（単位行列を作る関数）
	D3DXMatrixIdentity(&m_WorldMatrix);
	//入力レイアウト
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	//シェーダー設定
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	//マトリクス設定
	D3DXMATRIX world, scale, r, trans;
	D3DXMatrixScaling(&scale, scl.x, scl.y, scl.z);
	D3DXMatrixRotationYawPitchRoll(&r, rot.y, rot.x, rot.z);
	D3DXMatrixTranslation(&trans, pos.x, pos.y, pos.z);

	world = scale * r *trans;
	world *= w;
	Renderer::SetWorldMatrix(&world);

	m_model->Draw();

	//スケールを1 1 1
	D3DXMatrixScaling(&scale, 1.0f, 1.0, 1.0);

	m_WorldMatrix = scale * r * trans * w;


}


void Parts::SetModel(const char * ModelName)
{
	if (m_model == NULL) {
		m_model = new Model;
	}
	m_model->Load(ModelName);
}

void Parts::SetShader(const char * VSName, const char * PSName)
{
	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, VSName);
	Renderer::CreatePixelShader(&m_PixelShader, PSName);

}

D3DXMATRIX Parts::GetWorldMatrix()
{
	return m_WorldMatrix;
}

