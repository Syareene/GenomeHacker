#pragma once
#include "lib/vector3.h"
#include <list>
#include <string>
#include "manager.h"
#include "object/game_object.h"
#include "object/3d_object.h"

class Object3D; // includeはしているが互いに認識が必要なので前方宣言
class Manager; // includeはしているが互いに認識が必要なので前方宣言

class Collision
{
	// Gameobjectにアタッチする用のプロパティ
public:
	virtual ~Collision() = default; // 未定義警告解消用。基本どの継承元クラスもvirtual destructorを持つべきかなぁ。
	virtual void Init() {};
	virtual void Uninit() {};
	virtual void Update() {};
	virtual void DrawCollider(); // デバッグ用の衝突判定の可視化

	// 外部からはこっちの関数使って判定してもらう
	std::list<GameObject*> GetHitObjectsByName(std::string name, GameObject* target);
	std::list<GameObject*> GetHitObjectsByTag(std::string tag);

	template<typename T>
	std::list<T*> GetHitObjectsByType()
	{
		// Collisionクラス自体がObject3Dクラスにしかないが、それをこっちから確認する方法がないため
		// コンパイル時に評価しObject2Dからの参照があった場合にはエラーを出す
		if constexpr (!std::is_base_of_v<T, Object3D>)
		{
			static_assert(false, "GetHitObjectsByType: T must be derived from Object3D");
			//return std::list<T*>();
		}
		else
		{
			// sceneから取得(上の条件が満たされているため安全にObject3Dとして扱える)
			// ここ、アロー演算子の後にtemplateをつけないとエラー出たけど原因がちゃんとわかってない
			std::list<Object3D*> objects = Manager::GetCurrentScene()->template GetGameObjects<T>();
			
			// 取得したオブジェクト達がinputされたtargetと衝突しているかをチェック
			std::list<T*> result;
			for (auto& obj : objects)
			{
				// コライダを持ってない(nullptr)場合はスキップ
				if (!obj->GetCollider())
				{
					continue;
				}
				// 実行
				if (this->CheckCollision(*obj->GetCollider()))
				{
					// obj=Tと確定してないことになっちゃってる
					result.push_back(obj);
				}
			}
			return result;
		}
	};

	// コリジョン取得(visitorパターン使用)
	virtual bool CheckCollision(const Collision& other) const = 0;
	virtual bool CheckCollisionSphere(const Collision& other) const = 0;
	virtual bool CheckCollisionAABB(const Collision& other) const = 0;
	virtual bool CheckCollisionOBB(const Collision& other) const = 0;

	// データ取得
	Vector3 GetCenter() const { return m_Center; }
	Vector3 GetScale() const { return m_Scale; }
	Vector3 GetRotation() const { return m_Rotation; }
	//float GetRadius() const { return m_Radius; } // Sphere用
	// データセット
	void SetCenter(const Vector3& center) { m_Center = center; }
	void SetScale(const Vector3& size) { m_Scale = size; }
	void SetRotation(const Vector3& rotation) { m_Rotation = rotation; }
	//void SetRadius(float radius) { m_Radius = radius; } // Sphere
protected:
	virtual void SetSphereProperty(const Vector3& center, const Vector3& size);
	virtual void SetAABBProperty(const Vector3& center, const Vector3& size);
	virtual void SetOBBProperty(const Vector3& center, const Vector3& size, const Vector3& rotation);
private:
	// 衝突判定のプロパティ
	Vector3 m_Center = { 0.0f, 0.0f, 0.0f }; // 中心位置
	Vector3 m_Scale = { 1.0f, 1.0f, 1.0f }; // サイズ(AABBとOBB用)
	Vector3 m_Rotation = { 0.0f, 0.0f, 0.0f }; // 回転値(OBB用)
	//float m_Radius = 1.0f; // Sphereの半径->scaleに統合しても良くないか?

	// collisiontypeを判定するためにこれをoverrideして使ったほうがいいかなぁって気がしなくもない。
};