#pragma once

#include "object/game_object.h"
#include "manager.h"
#include "collider/sphere.h"
#include "collider/cube.h"
#include "lib/vector3.h"
#include <algorithm>

// これ、判定を計算するクラスだけど
// 判定のプロパティを設定できるクラスも作らないとかなぁ
// 必ずしもオブジェクトのサイズや場所とコライダの位置が一致するとは限らないしいくつか生えてくるかもしれないからね
class ColliderBase
{
public:
	// 衝突判定のチェック
	//static bool CheckCollision(GameObject& obj1, GameObject& obj2)
	//{
		// inputされたときにgameobjectの型を見て、適切な衝突判定を勝手に呼び出すようにする
	//}

	// 名前指定で指定した対象と衝突しているオブジェクトを取得
	static std::list<GameObject*> GetHitObjects(std::string name, GameObject* target)
	{
		// 実装したいならsceneに名前指定でobjectを取得できる関数を作る
	}

	// タグ指定で指定した対象と衝突しているオブジェクトを取得
	static std::list<GameObject*> GetHitObjectsByTag(std::string tag, GameObject* target)
	{

	}

	// 型指定で対象と衝突しているオブジェクトを取得
	template<typename T>
	static std::list<GameObject*> GetHitObjectsByType(GameObject* target)
	{
		// sceneから取得
		std::list<T*> objects = Manager::GetCurrentScene()->GetGameObjects<T>();

		// 取得したオブジェクト達がinputされたtargetと衝突しているかをチェック
	}





	// debug時に衝突判定の可視化を行う

private:

	static bool CheckCollisionType(GameObject& obj1, GameObject& obj2)
	{
	}

	// AABB-AABB(回転なしのbox当たり判定)
	static bool CheckAABBAABB(GameObject& obj1, GameObject& obj2)
	{
		// 純粋にxyzの範囲を比較する
		Vector3 pos1 = obj1.GetPosition();
		Vector3 pos2 = obj2.GetPosition();

		Vector3 scale1 = obj1.GetScale();
		Vector3 scale2 = obj2.GetScale();

		// AABBの範囲を計算
		Vector3 min1 = pos1 - scale1 * 0.5f;
		Vector3 max1 = pos1 + scale1 * 0.5f;
		Vector3 min2 = pos2 - scale2 * 0.5f;
		Vector3 max2 = pos2 + scale2 * 0.5f;
		// 衝突判定
		return (min1.x <= max2.x && max1.x >= min2.x) &&
			   (min1.y <= max2.y && max1.y >= min2.y) &&
			   (min1.z <= max2.z && max1.z >= min2.z);
	}
	// AABB-Sphere
	static bool CheckAABBSphere(GameObject& obj1, GameObject& obj2)
	{
		// collisionへ移行
		/*
		// AABBの中心位置と半径を取得
		Vector3 pos1 = obj1.GetPosition();
		Vector3 scale1 = obj1.GetScale();
		Vector3 min1 = pos1 - scale1 * 0.5f;
		Vector3 max1 = pos1 + scale1 * 0.5f;
		Vector3 pos2 = obj2.GetPosition();
		float radius2 = obj2.GetScale().x * 0.5f; // 半径はスケールの一辺の半分と仮定

		// AABBの中心からSphereの中心までの最近点を計算
		Vector3 closestPoint(
			std::max(min1.x, std::min(pos2.x, max1.x)),
			std::max(min1.y, std::min(pos2.y, max1.y)),
			std::max(min1.z, std::min(pos2.z, max1.z))
		);
		// 最近点とSphereの中心との距離を計算
		Vector3 diff = closestPoint - pos2;
		float distanceSquared = diff.lengthSquared();
		// 衝突判定
		return distanceSquared <= (radius2 * radius2);
		*/
	}
	// OBB-OBB
	// OBB-Sphere
	// Sphere-Sphere
	static bool CheckSphereSphere(Sphere& obj1, Sphere& obj2)
	{
		// collisionへ移行
		/*
		// Sphereの中心位置と半径を取得
		Vector3 pos1 = obj1.GetCenter();
		Vector3 pos2 = obj2.GetCenter();
		float radius1 = obj1.GetScale().x * 0.5f; // 半径はスケールの一辺の半分と仮定
		float radius2 = obj2.GetScale().x * 0.5f;
		// 中心間の距離を計算
		Vector3 diff = pos1 - pos2;
		float distanceSquared = diff.lengthSquared();
		// 半径の和を計算
		float radiusSum = radius1 + radius2;
		
		// 衝突判定
		return distanceSquared <= (radiusSum * radiusSum);
		*/
	}
};