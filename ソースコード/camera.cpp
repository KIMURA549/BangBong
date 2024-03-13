#include"main.h"
#include"manager.h"
#include"scene.h"
#include"renderer.h"
#include"component.h"
#include"charactor.h"
#include "camera.h"
#include"input.h"

void Camera::Init()
{
	SetPosition(D3DXVECTOR3(0.0f, 3.0f, -5.0f));
	SetScale(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	m_Target = { 0.0f, 1.5f, 0.0f };

	m_StartPos = m_Position;
}

void Camera::Update()
{
	SetPosition(m_StartPos + m_Target);
	//カメラシェイク
	m_shakeOffset = sinf(m_shakeTime * 1.5f) * m_shakeAmplitude;
	m_shakeTime++;
	m_shakeAmplitude *= 0.8f;
}

void Camera::Draw()
{
	//ビューマトリックス
	D3DXVECTOR3 up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	D3DXVECTOR3 position = m_Position + m_shakeOffset;
	D3DXVECTOR3 target = m_Target + m_shakeOffset;
	D3DXMatrixLookAtLH(&m_ViewMatrix, &position, &target, &up);

	Renderer::SetViewMatrix(&m_ViewMatrix);


	//プロジェクションマトリクス
	D3DXMATRIX projectionMatrix;
	D3DXMatrixPerspectiveFovLH(&projectionMatrix, 1.0f,
		(float)SCREEN_WIDTH / SCREEN_HEIGHT, 1.0f, 1000.0f);

	Renderer::SetProjectionMatrix(&projectionMatrix);
}
