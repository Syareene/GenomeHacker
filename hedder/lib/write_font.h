#pragma once

// https://github.com/Islington036/DirectWrite_CustomFont/tree/master 
// を元に自プロジェクト用に改変。
// AIとかも結構使ってるのでそのうち適切に変更する必要あり。

#include <string>						// 文字列
#include <vector>						// 動的配列
#include <list>
#include <memory>						// unique_ptr
#include <wrl.h>						// ComPtr

#pragma warning(push)
#pragma warning(disable:4005)

#include <d2d1.h>						// Direct2D
#include <DWrite.h>						// DirectWrite


#pragma warning(pop)

#pragma comment(lib,"d2d1.lib")			// Direct2D用
#pragma comment(lib,"Dwrite.lib")		// DirectWrite用

using namespace Microsoft;

class CustomFontCollectionLoader;

//=============================================================================
//		フォントの保存場所
//=============================================================================
namespace FontList
{
	const std::wstring FontPath[] =
	{
		L"asset/font/NotoSansJP-Black.ttf",
	};
}

//=============================================================================
//		フォント設定
//=============================================================================
struct FontData
{
	std::wstring font;							// フォント名
	IDWriteFontCollection* fontCollection;		// フォントコレクション
	DWRITE_FONT_WEIGHT fontWeight;				// フォントの太さ
	DWRITE_FONT_STYLE fontStyle;				// フォントスタイル
	DWRITE_FONT_STRETCH fontStretch;			// フォントの幅
	FLOAT fontSize;								// フォントサイズ
	WCHAR const* localeName;					// ロケール名
	DWRITE_TEXT_ALIGNMENT textAlignment;		// テキストの配置
	D2D1_COLOR_F Color;							// フォントの色

	D2D1_COLOR_F shadowColor;					// 影の色
	D2D1_POINT_2F shadowOffset;					// 影のオフセット

	// アウトライン（縁取り）設定
	D2D1_COLOR_F outlineColor;					// 縁取り色
	FLOAT outlineWidth;							// 縁取り幅（ピクセル）

	// デフォルト設定
	FontData()
	{
		font = L"";
		fontCollection = nullptr;
		fontWeight = DWRITE_FONT_WEIGHT::DWRITE_FONT_WEIGHT_NORMAL;
		fontStyle = DWRITE_FONT_STYLE::DWRITE_FONT_STYLE_NORMAL;
		fontStretch = DWRITE_FONT_STRETCH::DWRITE_FONT_STRETCH_NORMAL;
		fontSize = 20;
		localeName = L"ja-jp";
		textAlignment = DWRITE_TEXT_ALIGNMENT::DWRITE_TEXT_ALIGNMENT_LEADING;
		Color = D2D1::ColorF(D2D1::ColorF::White);

		shadowColor = D2D1::ColorF(D2D1::ColorF::Black);
		shadowOffset = D2D1::Point2F(2.0f, -2.0f);

		outlineColor = D2D1::ColorF(D2D1::ColorF::White);
		outlineWidth = 0.0f;
	}
};

// FontData 比較用の演算子(AI提案なので精査必要あり)
inline bool operator==(const D2D1_COLOR_F& a, const D2D1_COLOR_F& b) 
{
	return a.r == b.r && a.g == b.g && a.b == b.b && a.a == b.a;
}
inline bool operator==(const D2D1_POINT_2F& a, const D2D1_POINT_2F& b) 
{
	return a.x == b.x && a.y == b.y;
}
inline bool operator==(const FontData& a, const FontData& b) 
{
	return a.font == b.font &&
		a.fontWeight == b.fontWeight &&
		a.fontStyle == b.fontStyle &&
		a.fontStretch == b.fontStretch &&
		a.fontSize == b.fontSize &&
		wcscmp(a.localeName, b.localeName) == 0 &&
		a.textAlignment == b.textAlignment &&
		a.Color == b.Color &&
		a.shadowColor == b.shadowColor &&
		a.shadowOffset == b.shadowOffset &&
		a.outlineColor == b.outlineColor &&
		a.outlineWidth == b.outlineWidth;
}



// フォントプリセット
struct FontPreset
{
	int id; // プリセットID
	FontData data; // 元の設定値
	WRL::ComPtr<IDWriteTextFormat> textFormat;
	WRL::ComPtr<ID2D1SolidColorBrush> brush;
	WRL::ComPtr<ID2D1SolidColorBrush> shadowBrush;
	WRL::ComPtr<ID2D1SolidColorBrush> outlineBrush;
	// 必要なら追加
};


//=============================================================================
//		DirectWrite
//=============================================================================
class DirectWriteCustomFont
{
public:

	// シングルトン取得
	static DirectWriteCustomFont* GetInstance(FontData* set = nullptr)
	{
		if (instance == nullptr)
		{
			if (set == nullptr) 
			{
				// デフォルトFontDataを用意
				static FontData defaultFontData;
				set = &defaultFontData;
			}
			instance.reset(new DirectWriteCustomFont(set));
		}
		return instance.get();
	}

	ID2D1RenderTarget* GetRenderTarget() { return pRenderTarget.Get(); }

	// 初期化関数
	HRESULT Init(IDXGISwapChain* swapChain);

	// フォント設定
	// 第1引数：フォントデータ構造体
	//HRESULT SetFont(FontData set);

	// フォント設定
	// 第1引数：フォント名（L"メイリオ", L"Arial", L"Meiryo UI"等）
	// 第2引数：フォントの太さ（DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_WEIGHT_BOLD等）
	// 第3引数：フォントスタイル（DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STYLE_OBLIQUE, DWRITE_FONT_STYLE_ITALIC）
	// 第4引数：フォントの幅（DWRITE_FONT_STRETCH_NORMAL,DWRITE_FONT_STRETCH_EXTRA_EXPANDED等）
	// 第5引数：フォントサイズ（20, 30等）
	// 第6引数：ロケール名（L"ja-jp"等）
	// 第7引数：テキストの配置（DWRITE_TEXT_ALIGNMENT_LEADING：前, 等）
	// 第8引数：フォントの色（D2D1::ColorF(D2D1::ColorF::Black)：黒, D2D1::ColorF(D2D1::ColorF(0.0f, 0.2f, 0.9f, 1.0f))：RGBA指定等）
	// 第9引数：影の色（D2D1::ColorF(D2D1::ColorF::Black)：黒, D2D1::ColorF(D2D1::ColorF(0.0f, 0.2f, 0.9f, 1.0f))：RGBA指定等）
	// 第10引数：影のオフセット（D2D1::Point2F(2.0f, 2.0f)：右下にポイントずらす）
	/*
	HRESULT SetFont
	(
		WCHAR const* fontname,						// フォント名
		DWRITE_FONT_WEIGHT		fontWeight,			// フォントの太さ
		DWRITE_FONT_STYLE		fontStyle,			// フォントスタイル
		DWRITE_FONT_STRETCH		fontStretch,		// フォントの幅
		FLOAT					fontSize,			// フォントサイズ
		WCHAR const* localeName,					// ロケール名
		DWRITE_TEXT_ALIGNMENT	textAlignment,		// テキストの配置
		D2D1_COLOR_F			Color,				// フォントの色
		D2D1_COLOR_F			shadowColor,		// 影の色
		D2D1_POINT_2F			shadowOffset		// 影のオフセット
	);
	*/

	// テキストキャッシュ: SetTextでレイアウトを生成し、以降の描画で再利用する
	//HRESULT SetText(const std::string& str, FLOAT maxWidth = 0.0f, FLOAT maxHeight = 0.0f);
	//void ClearTextCache();

	HRESULT PreCacheTextLayout(FontData& fontData, const std::string& str, FLOAT maxWidth = 4096.0f, FLOAT maxHeight = 4096.0f);

	// 文字描画(利用する際は前もってGetPresetIDでプリセットIDを取得しておくこと)
	// string：文字列
	// pos：描画ポジション
	// options：テキストの整形
	HRESULT DrawString(const std::string& str, int presetId, const Vector2& pos, D2D1_DRAW_TEXT_OPTIONS options, bool shadow = false, bool outline = false);

	// 文字描画
	// string：文字列
	// rect：領域指定
	// options：テキストの整形
	//HRESULT DrawString(std::string str, D2D1_RECT_F rect, D2D1_DRAW_TEXT_OPTIONS options, bool shadow = false, bool outline = false);

	// 指定されたパスのフォントを読み込む
	HRESULT FontLoader();

	// フォント名を取得する
	static std::wstring GetFontName(int num);

	// プリセットidを取得
	int GetPresetID(const FontData& fontData);
	// 見た目部分の作成(idを返す)
	int FindOrCreateVisualPreset(const FontData& data);

	// 読み込んだフォント名の数を取得する
	int GetFontNameNum();

	// フォント名を取得し直す
	HRESULT GetFontFamilyName(IDWriteFontCollection* customFontCollection, const WCHAR* locale = L"en-us");

	// 全てのフォント名を取得し直す
	HRESULT GetAllFontFamilyName(IDWriteFontCollection* customFontCollection);

	// --- 追加: 生成したテキストのサイズ取得 ---
	// DIP（DIPs: device-independent pixels）単位で取得
	// outWidthDips / outHeightDips は nullptr ではないこと
	HRESULT GetTextSizeDips(const std::string& str, FLOAT* outWidthDips, FLOAT* outHeightDips) const;

	// 物理ピクセル単位で取得（レンダーターゲットの DPI を使用）
	HRESULT GetTextSizePixels(const std::string& str, FLOAT* outWidthPx, FLOAT* outHeightPx) const;

	// カスタムフォントコレクション
	WRL::ComPtr <IDWriteFontCollection> fontCollection = nullptr;

private:
	// シングルトン用保管変数
	static std::unique_ptr<DirectWriteCustomFont> instance;

	// デフォルトコンストラクタを制限
	DirectWriteCustomFont() = delete;

	// コンストラクタ
	// 第1引数：フォント設定
	DirectWriteCustomFont(FontData* set) :Setting(*set) {};

	FontPreset* GetPreset(int presetId);
	WRL::ComPtr<IDWriteTextLayout> FindOrCreateTextLayout(const std::string& str, int presetId, FLOAT maxWidth, FLOAT maxHeight);
	HRESULT ApplyVisualPreset(int presetId);	


	WRL::ComPtr<ID2D1Factory>			pD2DFactory = nullptr;		// Direct2Dリソース
	WRL::ComPtr<ID2D1RenderTarget>		pRenderTarget = nullptr;	// Direct2Dレンダーターゲット
	//std::list<WRL::ComPtr<ID2D1SolidColorBrush>>	pBrush;			// Direct2Dブラシ設定
	//std::list < WRL::ComPtr<ID2D1SolidColorBrush>>	pShadowBrush;		// Direct2Dブラシ設定（影）
	//std::list < WRL::ComPtr<ID2D1SolidColorBrush>>	pOutlineBrush;		// Direct2Dブラシ設定（縁取り）
	WRL::ComPtr<IDWriteFactory>		pDWriteFactory = nullptr;	// DirectWriteリソース
	//std::list<WRL::ComPtr<IDWriteTextFormat>>		pTextFormat;		// DirectWriteテキスト形式
	std::list < WRL::ComPtr<IDWriteTextLayout>>		pTextLayoutCache;		// DirectWriteテキスト書式 (キャッシュ用)
	WRL::ComPtr	<IDXGISurface>			pBackBuffer = nullptr;		// サーフェス情報

	std::unordered_map<int, std::list <FontPreset>::iterator> m_PresetCacheMap; // プリセットidから、使用順デキュー内へのイテレーターマップ

	// 使用順デキュー
	std::list<FontPreset> m_PresetUseOrderList;
	// アクティブなid
	int m_ActivePresetId = -1;
	// キャッシュ最大数
	static const size_t MAX_PRESET_CACHE_SIZE = 16;
	static const size_t MAX_TEXTLAYOUT_CACHE_SIZE = 48;
	// プリセットidカウンター
	int m_PresetIdCounter = 0;

	// テキストレイアウトキャッシュのキーを構造体にする
	struct TextLayoutKey 
	{
		int presetId;
		std::string text;
		bool operator==(const TextLayoutKey& other) const 
		{
			return presetId == other.presetId && text == other.text;
		}
	};
	// キーのハッシュ関数
	struct TextLayoutKeyHash
	{
		std::size_t operator()(const TextLayoutKey& k) const 
		{
			return std::hash<int>()(k.presetId) ^ (std::hash<std::string>()(k.text) << 1);
		}
	};

	using TextLayoutCachePair = std::pair<TextLayoutKey, WRL::ComPtr<IDWriteTextLayout>>;

	// 順序管理リスト
	std::list<TextLayoutCachePair> m_TextLayoutUsageList;

	// 4. キーからリストのイテレータを引くためのマップ
	std::unordered_map<TextLayoutKey, std::list<TextLayoutCachePair>::iterator, TextLayoutKeyHash> m_TextLayoutCacheMap;

	// フォントファイルリスト
	std::vector<WRL::ComPtr<IDWriteFontFile>> pFontFileList;

	// フォントデータ
	FontData  Setting = FontData();

	// フォント名リスト
	static std::vector<std::wstring> fontNamesList;

	// キャッシュしている文字列
	//std::string cachedText;
	//std::wstring cachedWText;
	//FLOAT cachedMaxWidth = 0.0f;
	//FLOAT cachedMaxHeight = 0.0f;

	// フォントのファイル名（拡張子なし）を取得する（動的割当しない安全な戻り値）
	std::wstring GetFontFileNameWithoutExtension(const std::wstring& filePath);

	// stringをwstringへ変換する
	std::wstring StringToWString(std::string oString) const;
};