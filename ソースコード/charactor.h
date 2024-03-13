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

	//���f ����
	
	int m_number = 0;//CHARACTER�ԍ�
	bool m_left = true;//����

	int m_life = 3;
	float m_weight = 0.0f;
	float m_elasticity = 0.0f;

	int m_power = 0;
	D3DXVECTOR3 m_velocity = {};

	CHARACTOR_STATE m_state = CHARACTOR_STATE_GROUND;
	float m_time = 0.0f;
	
	D3DXVECTOR4 m_limit = {};//�t�B�[���h�̌��E
	float m_ground = 0.0f;//���̍���

	D3DXVECTOR2 m_mouseStartPos = {};

	bool m_player = false;//���삷�邩�ǂ���

	//������
	class Parts* m_body;//���f��
	D3DXMATRIX m_Matrix;
	D3DXVECTOR4 m_color = {};



	//�����蔻��
	class Collision* m_collision;
	//����
	class Control* m_control;


	//���
	class PolygonObject* m_arrowMove;
	D3DXVECTOR2 m_arrowPos;


	//����Ɋւ���
	class Weapon* m_weapon;
	int m_selectWeaponNum = 0;
	bool m_attack = false;

	bool m_tackle = false;
	float d = 0;

	bool m_spawnSe = false;

	//�J�����i�h�炷���߁j
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

	//���삷�邩�ǂ���
	void SetPlayer() {
		m_player = true;
	}

	//�L�����̌�����
	void SetCharactor(D3DXVECTOR3 pos, D3DXVECTOR3 scl, D3DXVECTOR4 size, bool left, float w, float e, int num);
	void SetBody(int select);
	void SetColor(int select);
	D3DXVECTOR4 GetColor() {
		return m_color;
	}
	D3DXMATRIX GetMatrix() {
		return m_Matrix;
	}

	//������L���������������
	void StartMove(D3DXVECTOR3 vel);

	//���f�ޗ�
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

	//�������
	bool GetWeaponAttack() {
		return m_attack;
	}

	Weapon* GetWeapon() {
		return m_weapon;
	}

};
