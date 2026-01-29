#pragma once

#include <list>
#include <string>

// 識別用のクラス
class SystemObject
{
public:
	static constexpr size_t MAX_OBJECTS = 1; // オブジェクトvector最大数。継承先クラスで変更可能。

	virtual ~SystemObject() {}
	virtual void Init() {};
	virtual void Uninit() {};
	virtual void Update() {};
	virtual void UpdateFinal() {};


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

	template <typename T>
	static int getTypeId()
	{
		static int id = nextTypeId();
		return id;
	}
private:
	static int nextTypeId()
	{
		static int id = 0;
		return id++;
	}
	std::list<std::string> m_Tags; // タグ
};