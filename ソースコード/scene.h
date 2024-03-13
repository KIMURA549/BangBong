#pragma once
#include"gameObject.h"
#include<list>
#include<typeinfo>
#include<vector>


#define LAYER 3

enum GAMESTATE {
	GAMESTATE_SET,
	GAMESTATE_START,
	GAMESTATE_GAME,
	GAMESTATE_PAUSE,
	GAMESTATE_FINISH,
	GAMESTATE_END,
};

class Scene 
{
private:
	bool m_stand = false;	//�����͂��邪�R���g���[���ł��Ȃ��t���O
	bool m_slow = false;	//���Ԃ��������ɂ���t���O
protected:
	std::list<GameObject*> m_GameObject[LAYER];
	GAMESTATE m_state;
	float m_time = 0.0f;
public:
	virtual void Init(){}
	virtual void Uninit() {

		for (int i = 0; i < LAYER; i++) {
			for (GameObject* gameobject : m_GameObject[i]) {
				gameobject->Uninit();
				delete gameobject;
			}
		}
	}
	virtual void Update() {

		for (int i = 0; i < LAYER; i++) {
			for (GameObject* gameobject : m_GameObject[i]) {
				gameobject->Update();
			}
		}


		for (int i = 0; i < LAYER; i++) {
			//remove_if  return��true�����������
			m_GameObject[i].remove_if([](GameObject* object) {return object->Destroy(); });
		}
	}
	//////////////////////////////////
	virtual void Draw() {
		for (int i = 0; i < LAYER; i++) {
			for (GameObject* gameobject : m_GameObject[i]) {
				gameobject->Draw();
			}
		}
	}


	template<typename T>  //�e���v���[�g�֐�
	T*  AddGameObject(int num) 
	{
		T* gameObject = new T();
		m_GameObject[num].push_back(gameObject);
		gameObject->Init();
		return gameObject;
	}

	template<typename T>  //�e���v���[�g�֐�
	T*  GetGameObject() {
		for (int i = 0; i < LAYER; i++) {
			for (GameObject* object : m_GameObject[i]) {
				//�^�𒲂ׂ�
				if (typeid(*object) == typeid(T)) {
					return (T*)object;
				}
			}
		}
		return nullptr;
	}

	//�ϒ��z��@�v�f�̒ǉ��ƍ폜���ł���
	template<typename T>  //�e���v���[�g�֐�
	std::vector<T*>  GetGameObjects() {
		std::vector<T*> objects;
		for (int i = 0; i < LAYER; i++) {
			for (GameObject* object : m_GameObject[i]) {
				//�^�𒲂ׂ�
				if (typeid(*object) == typeid(T)) {
					objects.push_back((T*)object);
				}
			}
		}
		return objects;
	}

	virtual void SetGameState(GAMESTATE gs) {
		m_state = gs;
	}

	GAMESTATE GetGameState() {
		return m_state;
	}

	void SetSlowFlag(bool flag) {
		m_slow = flag;
	}

	bool GetSlowFlag() {
		return m_slow;
	}

	void SetStandFlag(bool flag) {
		m_stand = flag;
	}

	bool GetStandFlag() {
		return m_stand;
	}

	void SetTime(float t) {
		m_time = t;
	}
};