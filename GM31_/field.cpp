#include"main.h"
#include"renderer.h"
#include"component.h"
#include"field.h"
#include"parts.h"


void Field::Init()
{
}

void Field::Init(int width, int height, const char* name)
{
	m_block = new Parts;
	m_block->Init();
	m_block->SetModel(name);

	FieldBlockData fbd;
	//ç∂îºï™
	//â∫
	for (int i = 0; i < width / 2; i++) {
		fbd.position.x = (float)i;
		fbd.position.y = -(height / 2);
		fbd.position.z = 0.0f;
		fbd.scale = { 0.0f,0.0f,0.0f };
		m_leftBlocks.push_back(fbd);
	}
	//ç∂
	for (int i = 0; i < height + 1; i++) {
		fbd.position.x = (float)width / 2;
		fbd.position.y = -(height / 2) + i;
		fbd.position.z = 0.0f;
		fbd.scale = { 0.0f,0.0f,0.0f };
		m_leftBlocks.push_back(fbd);
	}
	//è„
	for (int i = 0; i < width / 2; i++) {
		fbd.position.x = width / 2 - 1.0f - (float)i;
		fbd.position.y = (height / 2);
		fbd.position.z = 0.0f;
		fbd.scale = { 0.0f,0.0f,0.0f };
		m_leftBlocks.push_back(fbd);
	}


	for (auto& lb : m_leftBlocks) {
		fbd = lb;
		fbd.position.x = -fbd.position.x;
		m_rightBlocks.push_back(fbd);
	}

	//ç∂âE
	m_fieldLimit.x = -(float)(width / 2) + 0.5f;
	m_fieldLimit.z = (float)(width / 2) - 0.5f;

	//è„â∫
	m_fieldLimit.y = -(float)(height / 2) + 0.5f;
	m_fieldLimit.w = (float)(height / 2) - 0.5f;

	m_set = true;


	m_color = { 0.0f,0.0f,0.0f,0.0f };
}
void Field::Uninit()
{
	m_block->Uninit();
	delete m_block;
}

void Field::Update() {
	/*if (m_set == false) {
		for (auto& lb : m_leftBlocks) {
			if (lb.scale.x >= 1.0f) {
				lb.scale.x = 1.0f;
				lb.scale.y = 1.0f;
				lb.scale.z = 1.0f;
			}
			else {
				lb.scale.x += 0.01f;
				lb.scale.y += 0.01f;
				lb.scale.z += 0.01f;
				break;
			}
		}

		for (auto& rb : m_rightBlocks) {
			if (rb.scale.x >= 1.0f) {
				rb.scale.x = 1.0f;
				rb.scale.y = 1.0f;
				rb.scale.z = 1.0f;
			}
			else {
				rb.scale.x += 0.01f;
				rb.scale.y += 0.01f;
				rb.scale.z += 0.01f;
				break;
			}
		}
		if (m_rightBlocks.back().scale.x >= 1.0f) {
			m_set = true;
		}
	}*/
}
void Field::Draw()
{
	PARAMETER param;
	param.Color = m_color;
	Renderer::SetParameter(param);
	for (auto& lb : m_leftBlocks) {
		m_block->Draw(lb.position, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f));
	}
	for (auto& rb : m_rightBlocks) {
		m_block->Draw(rb.position, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f));
	}
	
}
