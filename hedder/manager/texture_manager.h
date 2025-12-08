#pragma once

#include "main.h"
#include "lib/renderer.h"
#include <string>
#include <unordered_map>

const int UNLOAD_TEXTURE_COUNT = 100;

// シーンとかを設定できるようにして、シーンが変わったら自動開放とかできるといいよねーって感じ
// タイムラインとかもつけて、一定時間経過したら開放とかもあり


// これ、定期的にテクスチャの数見てあげて、countよりでかくてかつ参照0のテクスチャがあったら消すようにしないと
// 参照数0のテクスチャが無限に貯まる


// テクスチャデータ
struct TexData
{
    ID3D11ShaderResourceView* Texture = nullptr; // テクスチャデータ
	int TextureCount = 0; // テクスチャの参照カウント
	std::wstring FilePath; // ファイルパス
};

class TextureManager
{
public:
    TextureManager() = default;
    ~TextureManager() = default;

    static void GarbageCollection()
    {
        // 定期的に実行され、要素数が0のテクスチャを放棄する

        // カウントインクリメント
        m_CollectorCount++;
        // カウンタチェック
        if (m_CollectorCount >= COLLECTION_TIME)
        {
            // カウントリセ
            m_CollectorCount = 0;

            for (auto it = m_TextureMap.begin(); it != m_TextureMap.end();)
            {
                // もし読み込み数が0なら開放
                if (it->second.TextureCount <= 0)
                {
                    // 事前にファイルパスを取得
                    const std::wstring filePath = it->second.FilePath;

                    // テクスチャを解放
                    it->second.Texture->Release();

                    // m_TextureMapから削除（itが次の要素を指すようになる）
                    it = m_TextureMap.erase(it);

                    // m_PathMapからも削除
                    m_PathMap.erase(filePath);
                }
                else
                {
                    it++;
                }
            }
        }
    }

    static void UnloadTexture(int id)
    {
        // 参照カウントを減らす
		m_TextureMap[id].TextureCount--;

        // もし0かつ読み込まれているテクスチャ量が一定数以上なら開放
		if (m_TextureMap[id].TextureCount <= 0 && m_PathMap.size() >= UNLOAD_TEXTURE_COUNT)
		{
			// テクスチャを解放
			m_TextureMap[id].Texture->Release();
			// 要素の削除
			m_TextureMap.erase(id);
		}
    }

    static void UnloadAllTexture()
    {
        // テクスチャを解放
        for (auto& texture : m_TextureMap)
        {
            texture.second.Texture->Release();
        }
        // pathmapからも開放
        m_TextureMap.clear();

        // 要素の削除
        m_TextureMap.clear();
    }

    // テクスチャの取得
    // 注意: シーン系はこの関数をコンストラクタで使用せず、Init関数内で使用してください!(これどうなるかわからん)
    static int LoadTexture(const std::wstring filePath)
    {
        // すでに読み込まれているテクスチャならそれを返す
        auto it = m_PathMap.find(filePath);
        if (it != m_PathMap.end())
        {
			// テクスチャIDを取得
			int textureID = it->second;

            // idを元にマップからデータを取得
            // 参照数追加
			m_TextureMap[textureID].TextureCount++;
			// idを返す
			return textureID;
        }

        // そうでないなら新たにテクスチャを読み込む
        TexMetadata metadata;
        ScratchImage image;
        ID3D11ShaderResourceView* texture;
        // wstringにあるdataを使うとwstringからchar*に変換できる(c++17~)
        LoadFromWICFile(filePath.data(), WIC_FLAGS_NONE, &metadata, image);
        CreateShaderResourceView(Renderer::GetDevice(), image.GetImages(), image.GetImageCount(), metadata, &texture);
        assert(texture);

        //LoadFromWICFile(filePath.data(), WIC_FLAGS_NONE, &metadata, image);
        // フォーマットを下げる
        //ScratchImage convered_image;
        //Convert(image.GetImages(), image.GetImageCount(), image.GetMetadata(), DXGI_FORMAT_R8_UNORM, TEX_FILTER_DEFAULT, TEX_THRESHOLD_DEFAULT, convered_image);

        //CreateShaderResourceView(GetDevice(), convered_image.GetImages(), convered_image.GetImageCount(), metadata, &texture);
        //assert(texture);

		// テクスチャIDを生成
		int textureID = m_TextureIDCounter++;
        // 最大値なら0に戻す
		if (m_TextureIDCounter >= INT_MAX)
        {
            m_TextureIDCounter = 0;
		}

		// m_TextureMapに追加
		m_TextureMap[textureID].FilePath = filePath; // ファイルパスを保存
		m_TextureMap[textureID].Texture = texture; // テクスチャデータを保存
		m_TextureMap[textureID].TextureCount = 1; // 初期は1

		// m_PathMapにファイルパスとテクスチャIDを追加
		m_PathMap[filePath] = textureID;

        // テクスチャidを返す
		return textureID;
    }

    // 重い版読み込み(std::threadを使った非同期読み込み処理)->実装予定

	// テクスチャの取得
	static inline ID3D11ShaderResourceView* GetTexture(const int id)
	{
		// idが存在しない場合はnullptrを返す
		if (m_TextureMap.find(id) == m_TextureMap.end())
		{
			return nullptr;
		}
		// テクスチャを返す
		return m_TextureMap[id].Texture;
	}
private:
    static std::unordered_map<int, TexData> m_TextureMap;
	static std::unordered_map<std::wstring, int> m_PathMap; // ファイルパスをキーにしたマップ
	static int m_TextureIDCounter; // テクスチャIDのカウンター
    static int m_CollectorCount; // ガべコレタイマー
    constexpr static int COLLECTION_TIME = 900; // 30s事にコレクション
	//const static int UNLOAD_TEXTURE_COUNT = 100; // unloadが呼ばれたときにこの値以上のテクスチャがすでにあれば開放する
    // 逆にこの値以下ならメモリに余裕があるので頻繁にload/unloadしないようにする
};