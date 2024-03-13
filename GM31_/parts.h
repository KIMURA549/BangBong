#pragma once

#include"component.h"

class Parts :public Component
{
private:
	class Model* m_model{};

	ID3D11VertexShader* m_VertexShader = nullptr;
	ID3D11PixelShader* m_PixelShader = nullptr;
	ID3D11InputLayout* m_VertexLayout = nullptr;

	D3DXMATRIX m_WorldMatrix{};

public:

	void Init();
	void Uninit();
	void Draw(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scl);
	void Draw(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scl, D3DXMATRIX w);
	//void Draw(D3DXMATRIX w);


	void SetModel(const char* ModelName);
	void SetShader(const char* VSName, const char* PSName);

	D3DXMATRIX GetWorldMatrix();

};