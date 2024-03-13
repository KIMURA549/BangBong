#pragma once

#include"gameObject.h"

enum WEAPONSTATE {
	WEAPONSTATE_SPAWN,
	WEAPONSTATE_STAY,
	WEAPONSTATE_HAVE,
	WEAPONSTATE_STAND,
	WEAPONSTATE_FIRE,
	WEAPONSTATE_MOVE,
	WEAPONSTATE_DELETE,
};

class Weapon : public GameObject
{
private:
	class Parts* m_model;
	class Collision* m_collision;
	class Effect* m_effect;

	WEAPONSTATE m_state = WEAPONSTATE_SPAWN;

	D3DXVECTOR3 m_velocity = {};

	float m_weight = 0.0f;
	float m_elasticity = 0.0f;

	bool m_used = false;

	bool m_left = false;

	D3DXVECTOR3 m_standPos = {};//キャさクターの中央からどの位の位置で構えるか
	D3DXVECTOR2 m_rotLimitMin = {};//銃の構えられる角度の限界
	D3DXVECTOR2 m_rotLimitMax = {};//銃の構えられる角度の限界

	//武器の反動
	D3DXVECTOR3 m_recoil = {};
	float m_fireRotPower = 0.0f;
	float m_fireRotPowerNow = 0.0f;
	float m_fireCnt = 0.0f;
	D3DXVECTOR3 m_defoltScale = {};

	//弾
	D3DXVECTOR3 m_bulletPoint = {};//弾を出す位置
	int m_bulletNumber = 0;
	int m_bulletMax = 0;
	int m_bulletCnt = 0;
	float m_delay = 0.0f;

	int m_charactorNumber = 0;
	float m_haveDelay = 0.0f;//投げた後　再取得まで間隔を開ける
	
	//フィールドの限界
	D3DXVECTOR4 m_limit = {};
	D3DXVECTOR4 m_color = { 0.5f,0.5f,0.5f,1.0f };
	//床の高さ
	float m_ground = 0.0f;
	float m_time = 0.0f;

	class PolygonObject* m_targetIcon;

	class Audio* m_se;
public:
	void Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;

	void UpdateFire();
	void UpdateMove();

	void SetWeaponData(int select, D3DXVECTOR3 pos);
	void HaveWeapon(int num);
	void SetStandWeapon();
	void Fire();
	void ReleaseWeapon(D3DXVECTOR3 vel);
	void CreateBullet(int num);

	void DrawTargetIcon();

	void Damage() {
		m_bulletCnt -= 1;
	}

	void SetState(WEAPONSTATE ws) {
		m_state = ws;
	}
	void SetStand(D3DXVECTOR3 pos, bool left, D3DXVECTOR3 rot);

	void SetStandPos(D3DXVECTOR3 pos);

	void SetColor(D3DXVECTOR4 color) {
		m_color = color;
	}

	D3DXVECTOR2 GetRotLimitMin() {
		return m_rotLimitMin;
	}
	D3DXVECTOR2 GetRotLimitMax() {
		return m_rotLimitMax;
	}

	bool GetLeftFlag() {
		return m_left;
	}

	bool HaveWeapon() {
		if (m_charactorNumber == 0) {
			return false;
		}
		return true;
	}

	WEAPONSTATE GetWeaponState();

	D3DXVECTOR3 GetRecoil();

	class Collision GetCollision();

	int GetBulletMax() {
		return m_bulletMax;
	}

	int GetBulletCnt() {
		return m_bulletCnt;
	}

	int GetBulletTypeNum() {
		return m_bulletNumber;
	}

};