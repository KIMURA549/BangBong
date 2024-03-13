#pragma once
#include"gameObject.h"
class Child : public GameObject
{
private:

	ID3D11VertexShader* m_VertexShader = nullptr;
	ID3D11PixelShader* m_PixelShader = nullptr;
	ID3D11InputLayout* m_VertexLayout = nullptr;

	class Charactor* m_Parent;
public:
	void Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;

	void SetPosition(float x, float y, float z);

	void SetParent(Charactor* Parent) { m_Parent = Parent; }
};
