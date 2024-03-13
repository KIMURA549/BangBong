#pragma once
#include"GameObject.h"
#include<vector>

struct FieldBlockData {
	D3DXVECTOR3 position = {};
	D3DXVECTOR3 scale = {};
};

class Field : public GameObject
{
private:
	class Parts* m_block;
	
	std::vector<FieldBlockData> m_leftBlocks;
	std::vector<FieldBlockData> m_rightBlocks;

	bool m_set = false;

	//左　下　右　上
	D3DXVECTOR4 m_fieldLimit;
	D3DXCOLOR m_color;
public:
	void Init()override;
	void Init(int height, int width, const char* name);
	void Uninit()override;
	void Update()override;
	void Draw()override;

	bool GetSetEnd() {
		return m_set;
	}

	D3DXVECTOR4 GetLimit() {
		return m_fieldLimit;
	}
};
