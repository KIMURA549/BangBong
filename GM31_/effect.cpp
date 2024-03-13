#include"main.h"
#include"manager.h"
#include"renderer.h"
#include "effect.h"
#include"scene.h"
#include"camera.h"

void Effect::Init()
{
	VERTEX_3D vertex[4];

	vertex[0].Position = D3DXVECTOR3(-1.0f, 1.0f, 0.0f);
	vertex[0].Normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	vertex[0].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[0].TexCoord = D3DXVECTOR2(0.0f, 0.0f);

	vertex[1].Position = D3DXVECTOR3(1.0f, 1.0f, 0.0f);
	vertex[1].Normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	vertex[1].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].TexCoord = D3DXVECTOR2(1.0f, 0.0f);

	vertex[2].Position = D3DXVECTOR3(-1.0f, -1.0f, 0.0f);
	vertex[2].Normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	vertex[2].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].TexCoord = D3DXVECTOR2(0.0f, 1.0f);

	vertex[3].Position = D3DXVECTOR3(1.0f, -1.0f, 0.0f);
	vertex[3].Normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	vertex[3].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].TexCoord = D3DXVECTOR2(1.0f, 1.0f);



	//頂点バッファの生成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC; // D3D11_USAGE_DEFAULT　を書き換え
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; //0から書き換えができるように変更

	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.pSysMem = vertex;

	Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);



	//テクスチャ読み込み
	D3DX11CreateShaderResourceViewFromFile(Renderer::GetDevice(),
		"asset\\texture\\00.jpg",
		NULL,
		NULL,
		&m_Texture,
		NULL);
	assert(m_Texture);



	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout,
		"shader\\defaultEffectVS.cso");
	Renderer::CreatePixelShader(&m_PixelShader,
		"shader\\defaultEffectPS.cso");

}
void Effect::Uninit()
{
	if (m_VertexBuffer != NULL) {
		m_VertexBuffer->Release();
	}
	if (m_Texture != NULL) {
		m_Texture->Release();
	}

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
void Effect::Update() {

	m_time++;
	if (m_timeLimit <= m_time) {

		if (m_rewind == false) {
			m_count++;
			if (m_count >= m_countMax) {
				if (m_roop == true) {
					m_count = 0;
				}
				else {
					SetDestroy();
				}
				return;
			}
		}
		else {
			m_count--;
			if (m_count <= 0) {
				if (m_roop == true) {
					m_count = m_countMax;
				}
				else {
					SetDestroy();
				}
				return;
			}
		}
		m_time = 0;
	}


}
void Effect::Draw()
{
	float x = m_count % m_UCount * (1.0f / m_UCount);
	float y = m_count / m_UCount * (1.0f / m_VCount);

	D3D11_MAPPED_SUBRESOURCE msr;

	Renderer::GetDeviceContext()->Map(m_VertexBuffer, 0,
		D3D11_MAP_WRITE_DISCARD, 0, &msr);

	VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

	vertex[0].Position = D3DXVECTOR3(GetPosition().x - (GetScale().x / 2), GetPosition().y + (GetScale().y / 2), 0.0f);
	vertex[0].Normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	vertex[0].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[0].TexCoord = D3DXVECTOR2(x, y);

	vertex[1].Position = D3DXVECTOR3(GetPosition().x + (GetScale().x / 2), GetPosition().y + (GetScale().y / 2), 0.0f);
	vertex[1].Normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	vertex[1].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].TexCoord = D3DXVECTOR2(x + m_USizet, y);

	vertex[2].Position = D3DXVECTOR3(GetPosition().x - (GetScale().x / 2), GetPosition().y - (GetScale().y / 2), 0.0f);
	vertex[2].Normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	vertex[2].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].TexCoord = D3DXVECTOR2(x, y + m_VSizet);

	vertex[3].Position = D3DXVECTOR3(GetPosition().x + (GetScale().x / 2), GetPosition().y - (GetScale().y / 2), 0.0f);
	vertex[3].Normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	vertex[3].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].TexCoord = D3DXVECTOR2(x + m_USizet, y + m_VSizet);

	Renderer::GetDeviceContext()->Unmap(m_VertexBuffer, 0);


	PARAMETER param;
	param.Color = { 1.0f,1.0f,1.0f,1.0f };
	Renderer::SetParameter(param);

	//入力レイアウト
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	//シェーダー設定
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	Scene* scene = Manager::GetScene();
	Camera* camera = scene->GetGameObject<Camera>();
	D3DXMATRIX view = camera->GetViewMatrix();

	D3DXMATRIX invView;
	D3DXMatrixInverse(&invView, NULL, &view);
	//移動の成分を0　（回転はさせるが移動はさせない）
	invView._41 = 0.0f;
	invView._42 = 0.0f;
	invView._43 = 0.0f;


	//マトリクス設定
	D3DXMATRIX world, scale, rot, trans;
	D3DXMatrixScaling(&scale, m_scl.x, m_scl.y, m_scl.z);
	//D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);
	D3DXMatrixTranslation(&trans, m_pos.x, m_pos.y, m_pos.z);
	world = scale * invView  *trans;
	Renderer::SetWorldMatrix(&world);


	//頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

	//マテリアル設定
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	material.TextureEnable = true;
	Renderer::SetMaterial(material);

	//テクスチャ設定
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);

	//プリミティとポロジ設定
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//ポリゴン描画
	Renderer::GetDeviceContext()->Draw(4, 0);

}

void Effect::SetEffect(const char * TextureName, D3DXVECTOR3 scl, int uCnt, int vCnt, bool rewind, int start)
{
	VERTEX_3D vertex[4];

	vertex[0].Position = D3DXVECTOR3(-(scl.x / 2), (scl.y / 2), 0.0f);
	vertex[0].Normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	vertex[0].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[0].TexCoord = D3DXVECTOR2(0.0f, 0.0f);

	vertex[1].Position = D3DXVECTOR3((scl.x / 2), (scl.y / 2), 0.0f);
	vertex[1].Normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	vertex[1].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].TexCoord = D3DXVECTOR2(1.0f, 0.0f);

	vertex[2].Position = D3DXVECTOR3(-(scl.x / 2), -(scl.y / 2), 0.0f);
	vertex[2].Normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	vertex[2].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].TexCoord = D3DXVECTOR2(0.0f, 1.0f);

	vertex[3].Position = D3DXVECTOR3((scl.x / 2), -(scl.y / 2), 0.0f);
	vertex[3].Normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	vertex[3].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].TexCoord = D3DXVECTOR2(1.0f, 1.0f);


	//頂点バッファの生成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC; // D3D11_USAGE_DEFAULT　を書き換え
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; //0から書き換えができるように変更

	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.pSysMem = vertex;

	Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);

	//テクスチャ読み込み
	D3DX11CreateShaderResourceViewFromFile(Renderer::GetDevice(),
		TextureName,
		NULL,
		NULL,
		&m_Texture,
		NULL);
	assert(m_Texture);

	m_UCount = uCnt;
	m_VCount = vCnt;
	m_USizet = 1.0f / m_UCount;
	m_VSizet = 1.0f / m_VCount;

	m_scl = { 1.0f,1.0f,1.0f };

	m_rewind = rewind;
	m_time = 0;
	if (m_rewind == false) {
		m_count = start;
		m_countMax = uCnt * vCnt - 1;
	}
	else {
		m_count = uCnt * vCnt - 1 - start;
		m_countMax = m_count;
	}

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout,
		"shader\\defaultEffectVS.cso");
	Renderer::CreatePixelShader(&m_PixelShader,
		"shader\\defaultEffectPS.cso");

	m_roop = false;
}


