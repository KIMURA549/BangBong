#pragma once
#include"gameObject.h"

class BackGround : public GameObject
{
private:
	class PolygonObject* m_back;
public:
	void Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;

	void SetTexture(const char* TextureName);
};
