#pragma once
#include"gameObject.h"
#define STAGENUM_MAX	3
class SelectStage : public GameObject {
private:
	class Polygon2D* m_stage[STAGENUM_MAX];
	class Polygon2D* m_light;

	int m_select = -1;
	bool m_use = false;

	class Audio* m_se;
public:
	void Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;

	void SetUse(bool use) {
		m_use = use;
	}
	int GetSelectNum() {
		return m_select;
	}
};