#pragma once
#include "component.h"
#include<list>
class GameObject
{
protected:
	bool m_Destroy = false;
	std::list<Component*> m_Component;
	D3DXVECTOR3 m_Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 m_Rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 m_Scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	D3DXVECTOR3 m_Size = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
public:
	virtual void Init() {}
	virtual void Uninit() {

		for (Component* component : m_Component) {
			component->Uninit();
			delete component;
		}

		m_Component.clear();
	}
	virtual void Update() {
		for (Component* component : m_Component) {
			component->Update();
		}
	}
	virtual void Draw() {
		for (Component* component : m_Component) {
			component->Draw();
		}
	}

	void SetDestroy() {
		m_Destroy = true;
	}

	bool Destroy() {
		if (m_Destroy) {
			Uninit();
			delete this;
			return true;
		}
		else {
			return false;
		}
	}

	bool GetDestroy() {
		return m_Destroy;
	}

	template<typename T>  //テンプレート関数
	T*  AddComponent()
	{
		Component* component = new T();
		m_Component.push_back(component);
		component->Init();
		return (T*)component;
	}

	//template<typename T>  //テンプレート関数
	//T*  GetComponent() {
	//	for (Component* component : m_Component) {
	//		//型を調べる
	//		if (typeid(*object) == typeid(T)) {
	//			return (T*)object;
	//		}
	//	}
	//	return nullptr;
	//}

	void SetPosition(D3DXVECTOR3 pos) {
		m_Position = pos;
	}
	void SetRotation(D3DXVECTOR3 rot) {
		m_Rotation = rot;
	}
	void SetScale(D3DXVECTOR3 scl) {
		m_Scale = scl;
	}
	void SetSize(D3DXVECTOR3 size) {
		m_Size = size;
	}


	D3DXVECTOR3 GetPosition() {
		return m_Position;
	}
	D3DXVECTOR3 GetRotation() {
		return m_Rotation;
	}
	D3DXVECTOR3 GetScale() {
		return m_Scale;
	}
	D3DXVECTOR3 GetSize() {
		return m_Size;
	}


	//前方ベクトル
	D3DXVECTOR3 GetForward() {

		//4×4マトリクス
		D3DXMATRIX rot;
		//長さも１になっている（正規化もする）
		D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);

		D3DXVECTOR3 forward;
		forward.x = rot._31;
		forward.y = rot._32;
		forward.z = rot._33;

		return forward;
	}

	//右ベクトル
	D3DXVECTOR3 GetRight() {

		//4×4マトリクス
		D3DXMATRIX rot;
		//長さも１になっている（正規化もする）
		D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);

		D3DXVECTOR3 forward;
		forward.x = rot._11;
		forward.y = rot._12;
		forward.z = rot._13;

		return forward;
	}
};