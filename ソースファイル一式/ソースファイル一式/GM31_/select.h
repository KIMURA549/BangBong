#include"scene.h"
enum SELECTSTATE {
	SELECTSTATE_CHARACTOR,
	SELECTSTATE_STAGE,
};
class Select :public Scene {
private:
	SELECTSTATE m_state;
	float m_time = 0.0f;
	float m_timeWeapon = 0.0f;
	float m_timeSlow = 0.0f;
	float m_left, m_right;
	bool m_next;

	class Audio* m_bgm;
	class Audio* m_se;
	class Fade* m_fade;
public:
	void Init()override;
	void Update()override;
};