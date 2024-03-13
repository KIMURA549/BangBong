#pragma once

#include "scene.h"

class Manager
{
private:
	static Scene* m_Scene;
public:
	static void Init();
	static void Uninit();
	static void Update();
	static void Draw();

	static Scene* GetScene() {
		return m_Scene;
	}


	template<typename T>
	static void SetScene()
	{
		if (m_Scene)
		{
			m_Scene->Uninit();
			delete m_Scene;
		}

		m_Scene = new T();
		m_Scene->Init();
	}

};