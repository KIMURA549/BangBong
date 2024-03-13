
#include"main.h"
#include"renderer.h"
#include"selectArrow.h"
#include"sprite.h"
#include"mouse.h"

void SelectArrow::Init()
{
	m_sprite = new Sprite;
	m_sprite->InitSetCenter(300.0f, 300.0f, 300.0f, 200.0f, "asset\\texture\\00.jpg");
	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout,
		"shader\\unlitTextureChangeColorVS.cso");
	Renderer::CreatePixelShader(&m_PixelShader,
		"shader\\unlitTextureChangeColorPS.cso");

}
void SelectArrow::Uninit()
{
	if (m_VertexLayout != NULL) {
		m_VertexLayout->Release();
	}
	if (m_VertexShader != NULL) {
		m_VertexShader->Release();
	}
	if (m_PixelShader != NULL) {
		m_PixelShader->Release();
	}

	GameObject::Uninit();
}
void SelectArrow::Update() {

	m_time -= 1.0f;
	if (m_time <= 0.0f) {
		m_use = false;
		m_time = 0.0f;
	}
}
void SelectArrow::Draw()
{
	//入力レイアウト
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	//シェーダー設定
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	//マトリクス設定
	Renderer::SetWorldViewProjection2D();


	PARAMETER param;
	D3DXVECTOR4 c;
	if (m_use == true) {
		c = { 0.0f,1.0f,0.5f,1.0f };
		param.Color = { c.x,c.y,c.z,1.0f };
	}
	else {
		c = { 1.0f,1.0f,1.0f,1.0f };
		param.Color = { c.x,c.y,c.z,1.0f };
	}
	Renderer::SetParameter(param);


	m_sprite->Draw();
	GameObject::Draw();
}

void SelectArrow::SetArrowL(float x, float y, float size)
{
	m_sprite->InitSetUV(x, y, size, size * 2, "asset\\texture\\selectArrow.png", 2, 1, 1);
	m_use = false;
}

void SelectArrow::SetArrowR(float x, float y, float size)
{
	m_sprite->InitSetUV(x, y, size, size * 2, "asset\\texture\\selectArrow.png", 2, 1, 2);
	m_use = false;
}
