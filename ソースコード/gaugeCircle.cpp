#include"main.h"
#include"manager.h"
#include"renderer.h"
#include"gaugeCircle.h"
#include"camera.h"
#include"sprite.h"
void GaugeCircle::Init()
{

	m_sprite = new Sprite;
	m_sprite->InitSetCenter(0.0f, 0.0f, 200.0f, 200.0f, "asset\\texture\\00.jpg");

	SetPosition({ 0.0f , 0.0f, 0.0f });
	SetScale({ 200.0f, 200.0f,0.0f });

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout,
		"shader\\gaugeCircleVS.cso");
	Renderer::CreatePixelShader(&m_PixelShader,
		"shader\\gaugeCirclePS.cso");

}

void GaugeCircle::Uninit()
{

}

void GaugeCircle::Update()
{

}

void GaugeCircle::Draw()
{

	//入力レイアウト
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	//シェーダー設定
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	//マトリクス設定
	Renderer::SetWorldViewProjection2D();



	m_sprite->Draw();
}

void GaugeCircle::SetSpritePosSize(float x, float y, float width, float height)
{
	m_sprite->InitSetCenter(x, y, width, height, "asset\\texture\\00.jpg");
	SetPosition({ x , y, 0.0f });
	SetScale({ width, height,0.0f });
}
