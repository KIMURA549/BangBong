#pragma once

#define SIMULATION_NUM 60

enum CONTROL_STATE {
	CONTROLSTATE_S,
	CONTROLSTATE_A,
	CONTROLSTATE_D,
	CONTROLSTATE_MOUSE_L,
	CONTROLSTATE_MOUSE_R,
	CONTROLSTATE_RELESE,
	CONTROLSTATE_NONE,
};

enum AI_STATE {
	AISTATE_THINK,
	AISTATE_ACTION,
	AISTATE_ESCAPE,
};


enum SIMENDSTATE {
	SIMENDSTATE_GROUND,
	SIMENDSTATE_BLOCK,
	SIMENDSTATE_WEAPON,

	SIMENDSTATE_NONE,
};

struct AICONTROLLERDATA {
	CONTROL_STATE input = CONTROLSTATE_NONE;
	float delay = 0.0f;
};

struct SimulationData {
	D3DXVECTOR3 setVel = {};
	D3DXVECTOR3 pos = {};
	D3DXVECTOR3 vel = {};
	int power = 0;
	SIMENDSTATE end = SIMENDSTATE_NONE;
};

class Control {
private:
	CONTROL_STATE m_input = CONTROLSTATE_NONE;

	//AIに使用
	AICONTROLLERDATA m_ai;

	AI_STATE m_state = AISTATE_THINK;
	SimulationData m_sd[SIMULATION_NUM];

	bool m_sim = false;//移動先シミュレーション
	int m_moveCnt = 0;
	D3DXVECTOR2 m_aiMousePoint = {};//移動などの方向
	bool m_nextMove = false;


	//int m_number = 0;
	float m_weight = 0.0f;
	float m_elasticity = 0.0f;
	Collision* m_collision;
	void SetSdData(int n, D3DXVECTOR3 pos, D3DXVECTOR3 vel, int pow);

	//フィールドの限界
	D3DXVECTOR4 m_limit = {};
public:

	void InputController();
	void SetController(CONTROL_STATE c) {
		m_input = c;
	}

	CONTROL_STATE GetControl();

	float GetMousePointX();
	float GetMousePointY();


	AI_STATE GetAIState() {
		return m_state;
	}

	void AIControlInit();
	void AIController();

	//移動
	void SetAIMove(D3DXVECTOR3 v, int p);
	void SetAIMoveRandom();
	void CheckSimMove(SIMENDSTATE ses);
	//武器で攻撃
	void SetAIAttack(float delay);

	//回避
	void AIEscapeRandom(float minX, float maxX, float minY, float maxY);
	void AIEscape(float x, float y, int power);
	void AIEscapeEnd(float x, float y);

	void ResetThinkTime(float t);

	void SimulationInit(int num, D3DXVECTOR3 pos, float w, float e, Collision* col, D3DXVECTOR4 limit);
	void SimulationUpdate();

	void SetAIMousePoint(float x, float y);


	bool GetSimurationFlag() {
		return m_sim;
	}

	bool GetNextMoveFlag() {
		return m_nextMove;
	}

	float GetAIMousePointX();
	float GetAIMousePointY();
};