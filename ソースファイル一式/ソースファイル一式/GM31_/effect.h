#pragma once
#include"GameObject.h"
class Effect : public GameObject
{
private:

	ID3D11Buffer* m_VertexBuffer = nullptr;
	ID3D11ShaderResourceView* m_Texture = nullptr;

	ID3D11VertexShader* m_VertexShader = nullptr;
	ID3D11PixelShader* m_PixelShader = nullptr;
	ID3D11InputLayout* m_VertexLayout = nullptr;

	int m_count;
	int m_countMax;
	int m_UCount;
	int m_VCount;
	float m_USizet;
	float m_VSizet;

	D3DXVECTOR3 m_pos;
	D3DXVECTOR3 m_scl;

	int m_time;
	int m_timeLimit;

	bool m_roop = false;
	bool m_rewind = false;
public:
	void Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;

	void SetEffect(const char * TextureName, D3DXVECTOR3 scl, int uCnt, int vCnt, bool rewind, int start = 0);

	void SetPos(D3DXVECTOR3 p) {
		m_pos = p;
	}
	void SetScl(D3DXVECTOR3 s) {
		m_scl = s;
	}
	//アニメーションを遅くする（数字が増えるほど遅くなる）
	void SetAnimationSlow(int s) {
		m_timeLimit = s;
	}

	int GetAnimNum() {
		return m_count;
	}

	void SetRoop(bool roop) {
		m_roop = roop;
	}
};
