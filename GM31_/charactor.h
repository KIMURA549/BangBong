#pragma once
#include"gameObject.h"

enum CHARACTOR_STATE {
	CHARACTOR_STATE_GROUND,
	CHARACTOR_STATE_JUMP,
	CHARACTOR_STATE_MOVE,
	CHARACTOR_STATE_DAMAGE,
	CHARACTOR_STATE_DEAD,

	NONE,
};

class Charactor : public GameObject
{
private:

	//判断 判別
	
	int m_number = 0;//CHARACTER番号
	bool m_left = true;//向き

	int m_life = 3;
	float m_weight = 0.0f;
	float m_elasticity = 0.0f;

	int m_power = 0;
	D3DXVECTOR3 m_velocity = {};

	CHARACTOR_STATE m_state = CHARACTOR_STATE_GROUND;
	float m_time = 0.0f;
	
	D3DXVECTOR4 m_limit = {};//フィールドの限界
	float m_ground = 0.0f;//床の高さ

	D3DXVECTOR2 m_mouseStartPos = {};

	bool m_player = false;//操作するかどうか

	//見た目
	class Parts* m_body;//モデル
	D3DXMATRIX m_Matrix;
	D3DXVECTOR4 m_color = {};



	//当たり判定
	class Collision* m_collision;
	//操作
	class Control* m_control;


	//矢印
	class PolygonObject* m_arrowMove;
	D3DXVECTOR2 m_arrowPos;


	//武器に関する
	class Weapon* m_weapon;
	int m_selectWeaponNum = 0;
	bool m_attack = false;

	bool m_tackle = false;
	float d = 0;

	bool m_spawnSe = false;

	//カメラ（揺らすため）
	class Camera* m_camera;
	class Effect* m_effect;

	class Audio* m_se;

	void PlayerControl();
	void AIControl();

	void MoveArrowDraw();

	void PickUpWeapon();
public:
	void Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;

	void UpdateGround();
	void UpdateJump();
	void UpdateMove();
	void UpdateDamage();

	void UpdateWeapon();
	void ThrowWeapon();
	void ThrowWeaponUP();

	void Damage(D3DXVECTOR3 vel);

	void DeadCharactor();

	//操作するかどうか
	void SetPlayer() {
		m_player = true;
	}

	//キャラの見た目
	void SetCharactor(D3DXVECTOR3 pos, D3DXVECTOR3 scl, D3DXVECTOR4 size, bool left, float w, float e, int num);
	void SetBody(int select);
	void SetColor(int select);
	D3DXVECTOR4 GetColor() {
		return m_color;
	}
	D3DXMATRIX GetMatrix() {
		return m_Matrix;
	}

	//他からキャラが動かされる
	void StartMove(D3DXVECTOR3 vel);

	//判断材料
	int GetCharactorNumber() {
		return m_number;
	}
	int GetCharactorLife() {
		return m_life;
	}
	CHARACTOR_STATE GetState() {
		return m_state;
	}
	bool GetTackleFlag() {
		return m_tackle;
	}

	//武器周り
	bool GetWeaponAttack() {
		return m_attack;
	}

	Weapon* GetWeapon() {
		return m_weapon;
	}

};
