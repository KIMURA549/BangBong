
#include"main.h"
#include"renderer.h"
#include"polygon2D.h"
#include"sprite.h"


void Polygon2D::Init()
{
	m_sprite = new Sprite;
	m_sprite->InitSetCenter(300.0f, 300.0f, 300.0f, 200.0f, "asset\\texture\\00.jpg");
	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, 
		"shader\\unlitTextureChangeColorVS.cso");
	Renderer::CreatePixelShader(&m_PixelShader, 
		"shader\\unlitTextureChangeColorPS.cso");

}
void Polygon2D::Uninit()
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
void Polygon2D::Update(){
	GameObject::Update();
}
void Polygon2D::Draw()
{
	//入力レイアウト
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	//シェーダー設定
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	//マトリクス設定
	Renderer::SetWorldViewProjection2D();

	m_sprite->Draw();
	GameObject::Draw();
}

void Polygon2D::SetPolygon2D(float x, float y, float width, float height, const char * TeextureName)
{
	m_sprite->InitSetCenter(x, y, width, height, TeextureName);
}

void Polygon2D::SetPolygon2DUV(float x, float y, float width, float height, const char * TeextureName, int u, int v, int num)
{

	m_sprite->InitSetUV(x, y, width, height, TeextureName, u, v, num);
}

void Polygon2D::SetShader(const char * PSName, const char * VSName)
{
	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, VSName);
	Renderer::CreatePixelShader(&m_PixelShader, PSName);
}

