#pragma once
#include "lib/vector3.h"
#include <list>
#include <string>
#include <type_traits>
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
	virtual void Init(const Transform& trans = Transform(), const Vector3& pos_diff = { 0.0f, 0.0f, 0.0f }) 
	{
		SetTransform(trans);
		SetPositionOffset(pos_diff);
		// pos_diff加算
		SetPosition(GetPosition() + GetPositionOffset());
	};
	virtual void Uninit() {};
	virtual void Update(const Vector3& obj_pos);
	virtual void DrawCollider(); // デバッグ用の衝突判定の可視化

	// 外部からはこっちの関数使って判定してもらう
	std::list<GameObject*> GetHitObjectsByName(std::string name, GameObject* target);
	std::list<GameObject*> GetHitObjectsByTag(std::string tag);

	template<typename T>
	std::list<T*> GetHitObjectsByType()
	{
		// Collisionクラス自体がObject3Dクラスにしかないが、それをこっちから確認する方法がないため
		// コンパイル時に評価しObject2Dからの参照があった場合にはエラーを出す
		if constexpr (!std::is_base_of_v<Object3D, T>)
		{
			static_assert(std::is_base_of_v<Object3D, T>, "GetHitObjectsByType: T must be derived from Object3D");
			//return std::list<T*>();
		}
		else
		{
			// sceneから取得(上の条件が満たされているため安全にObject3Dとして扱える)
			// ここ、アロー演算子の後にtemplateをつけないとエラー出たけど原因がちゃんとわかってない
			std::list<T*> objects = Manager::GetCurrentScene()->template GetGameObjects<T>();
			std::list<Object3D*> cast_obj;
			for (auto obj : objects)
			{
				// ここでT*にキャストできるかチェック
				Object3D* casted = static_cast<T*>(obj);
				// nullptr(castできなかったとしても)とりあえず詰める
				cast_obj.push_back(casted);
			}

			// 取得したオブジェクト達がinputされたtargetと衝突しているかをチェック
			std::list<T*> result;
			for (auto& obj : cast_obj)
			{
				// nullptrなら(取得したobjが3dobjでない)スキップ
				if(!obj)
				{
					continue;
				}

				// コライダを持ってない(nullptr)場合はスキップ
				if (!obj->GetCollider())
				{
					continue;
				}
				// 自分自身ならスキップ
				if (obj->GetCollider() == this)
				{
					continue;
				}

				// 実行
				if (this->CheckCollision(*obj->GetCollider()))
				{
					// obj=Tと確定してないことになっちゃってる
					result.push_back(static_cast<T*>(obj));
				}
			}
			return result;
		}
	};

	// コリジョン取得(visitorパターン使用)
	virtual bool CheckCollision(const Collision& other) = 0;
	virtual bool CheckCollisionSphere(const Collision& other) const = 0;
	virtual bool CheckCollisionAABB(const Collision& other) const = 0;
	virtual bool CheckCollisionOBB(const Collision& other) const = 0;

	// データ取得
	inline const Transform& GetTransform() const { return m_Transform; }
	inline const Vector3& GetPosition() const { return m_Transform.GetPosition(); }
	inline const Vector3& GetScale() const { return m_Transform.GetScale(); }
	inline const Vector3& GetRotation() const { return m_Transform.GetRotation(); }
	inline const Vector3& GetRadian() const { return m_Transform.GetRadian(); }
	inline const Vector3& GetPositionOffset() const { return m_PositionOffset; } // 中心位置のオフセット
	inline const bool GetIsHit() const { return m_IsHit; } // 当たっているかどうか
	// データセット
	inline void SetTransform(const Transform trans) { m_Transform = trans; };
	inline void SetPosition(const Vector3& center) { m_Transform.SetPosition(center); }
	inline void SetScale(const Vector3& size) { m_Transform.SetScale(size); }
	inline void SetRotation(const Vector3& rotation) { m_Transform.SetRotation(rotation); }
	inline void SetPositionOffset(const Vector3& offset) { m_PositionOffset = offset; } // 中心位置のオフセット
	inline void SetIsHit(bool isHit) {m_IsHit = isHit;}
protected:
	//virtual void SetSphereProperty(const Vector3& center, const Vector3& size);
	//virtual void SetAABBProperty(const Vector3& center, const Vector3& size);
	//virtual void SetOBBProperty(const Vector3& center, const Vector3& size, const Vector3& rotation);
private:
	// 衝突判定のプロパティ
	Transform m_Transform = Transform(); // Transform情報(位置、回転、スケール)
	Vector3 m_PositionOffset = { 0.0f, 0.0f, 0.0f }; // 中心位置のオフセット(基本は0,0,0で良い)
	bool m_IsHit = false; // 当たっているかどうか(当たっている間はtrue)
};