#pragma once

#include"component.h"
class PolygonObject : public Component
{
private:

	ID3D11Buffer* m_VertexBuffer = nullptr;
	ID3D11ShaderResourceView* m_Texture = nullptr;

	ID3D11VertexShader* m_VertexShader = nullptr;
	ID3D11PixelShader* m_PixelShader = nullptr;
	ID3D11InputLayout* m_VertexLayout = nullptr;

	bool m_use = false;
	bool m_changeColor = false;
public:
	void Init()override;
	void Uninit()override;
	void Update()override;
	void Draw(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scl);

	void SetTexture( D3DXVECTOR3 scl, const char * TextureName);
	void SetUse(bool use) {
		m_use = use;
	}
	bool GetUse() {
		return m_use;
	}

	void SetChangeColor(bool change) {
		m_changeColor = change;
	}

	void SetShader(const char * VSName, const char * PSName);

};