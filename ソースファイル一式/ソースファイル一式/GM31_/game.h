#include"scene.h"

#define		WEAPON_MAXNUM	5


class Game :public Scene {
private:

	bool m_charactorSet = false;
	float m_timtSlow = 0.0f;
	float m_timeWeapon = 0.0f;
	float m_left, m_right;

	bool m_end = false;
	bool m_next = false;

	class Fade* m_fade;
	class FightWord* m_word;

	class Audio* m_bgm;

	GAMESTATE m_state = GAMESTATE_START;

	void UpdateSet();
	void UpdateStart();
	void UpdateGame();
	void UpdateFinish();

	void CreatePlayer(D3DXVECTOR3 pos, bool left, int charactorNum);
	void CreateEnemy(D3DXVECTOR3 pos, bool left, int charactorNum);
	void CreateField();

	void SpawnWeapon();
public:
	void Init()override;
	void Update()override;
	void SetGameState(GAMESTATE gs)override;
};