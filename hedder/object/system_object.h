#pragma once

#include <list>
#include <string>

// 識別用のクラス
class SystemObject
{
public:
	virtual ~SystemObject() {}
	// エラー解消用にコメントアウト
	virtual void Init() {};
	virtual void Uninit() {};
	virtual void Update() {};
	virtual void Draw() {};

	void AddTag(const std::string& tag)
	{
		// タグを追加
		m_Tags.push_back(tag);
	}

	bool IsTagAvailable(const std::string& tag) const
	{
		// タグが存在するか確認
		for (const auto& t : m_Tags)
		{
			if (t == tag)
			{
				return true;
			}
		}
		return false;
	}
private:
	std::list<std::string> m_Tags; // タグ
};