#pragma once

#include"scene.h"

class Title :public Scene {
private:
	int m_objNum = 0;
	float m_time;
	bool m_next;

	class Fade* m_fade;
	class TitleObject* m_logo;

	class Audio* m_bgm;
	class Audio* m_seNext;
public:
	void Init()override;
	void Update()override;
};