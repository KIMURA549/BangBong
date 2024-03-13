#pragma once

#include"scene.h"

class GameStart :public Scene {
private:
	bool m_next;

	class Fade* m_fade;

	class Audio* m_seNext;
public:
	void Init()override;
	void Update()override;
};