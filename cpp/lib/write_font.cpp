#pragma once

#include "main.h"
#include "lib/write_font.h"

#include <cmath>

// フォントコレクションローダー
WRL::ComPtr<CustomFontCollectionLoader> pFontCollectionLoader = nullptr;
std::vector<std::wstring> DirectWriteCustomFont::fontNamesList;
std::unique_ptr<DirectWriteCustomFont> DirectWriteCustomFont::instance;

//=============================================================================
// カスタムファイルローダー
//=============================================================================
class CustomFontFileEnumerator : public IDWriteFontFileEnumerator
{
public:
    CustomFontFileEnumerator(IDWriteFactory* factory, const std::vector<std::wstring>& fontFilePaths)
        : refCount_(0), factory_(factory), fontFilePaths_(fontFilePaths), currentFileIndex_(-1)
    {
        factory_->AddRef();
    }

    ~CustomFontFileEnumerator()
    {
        factory_->Release();
    }

    IFACEMETHODIMP QueryInterface(REFIID iid, void** ppvObject) override
    {
        if (ppvObject == nullptr) return E_POINTER;
        *ppvObject = nullptr;
        if (iid == __uuidof(IUnknown) || iid == __uuidof(IDWriteFontFileEnumerator))
        {
            *ppvObject = this;
            AddRef();
            return S_OK;
        }
        return E_NOINTERFACE;
    }

    IFACEMETHODIMP_(ULONG) AddRef() override { return InterlockedIncrement(&refCount_); }
    IFACEMETHODIMP_(ULONG) Release() override
    {
        ULONG newCount = InterlockedDecrement(&refCount_);
        if (newCount == 0) delete this;
        return newCount;
    }

    IFACEMETHODIMP MoveNext(OUT BOOL* hasCurrentFile) override
    {
        if (++currentFileIndex_ < static_cast<int>(fontFilePaths_.size()))
        {
            *hasCurrentFile = TRUE;
            return S_OK;
        }
        *hasCurrentFile = FALSE;
        return S_OK;
    }

    IFACEMETHODIMP GetCurrentFontFile(OUT IDWriteFontFile** fontFile) override
    {
        return factory_->CreateFontFileReference(fontFilePaths_[currentFileIndex_].c_str(), nullptr, fontFile);
    }

private:
    ULONG refCount_;
    IDWriteFactory* factory_;
    std::vector<std::wstring> fontFilePaths_;
    int currentFileIndex_;
};

//=============================================================================
// カスタムフォントコレクションローダー
//=============================================================================
class CustomFontCollectionLoader : public IDWriteFontCollectionLoader
{
public:
    CustomFontCollectionLoader() : refCount_(0) {}

    IFACEMETHODIMP QueryInterface(REFIID iid, void** ppvObject) override
    {
        if (ppvObject == nullptr) return E_POINTER;
        *ppvObject = nullptr;
        if (iid == __uuidof(IUnknown) || iid == __uuidof(IDWriteFontCollectionLoader))
        {
            *ppvObject = this;
            AddRef();
            return S_OK;
        }
        return E_NOINTERFACE;
    }

    IFACEMETHODIMP_(ULONG) AddRef() override { return InterlockedIncrement(&refCount_); }
    IFACEMETHODIMP_(ULONG) Release() override
    {
        ULONG newCount = InterlockedDecrement(&refCount_);
        if (newCount == 0) delete this;
        return newCount;
    }

    IFACEMETHODIMP CreateEnumeratorFromKey(
        IDWriteFactory* factory,
        void const* collectionKey,
        UINT32 collectionKeySize,
        OUT IDWriteFontFileEnumerator** fontFileEnumerator) override
    {
        std::vector<std::wstring> fontFilePaths(std::begin(FontList::FontPath), std::end(FontList::FontPath));
        *fontFileEnumerator = new (std::nothrow) CustomFontFileEnumerator(factory, fontFilePaths);
        if (*fontFileEnumerator == nullptr) return E_OUTOFMEMORY;
        return S_OK;
    }

private:
    ULONG refCount_;
};

//=============================================================================
// カスタムテキストレンダラー（ジオメトリを得て塗り + 縁取り）
//=============================================================================
class OutlineTextRenderer : public IDWriteTextRenderer, public IDWritePixelSnapping
{
public:
    OutlineTextRenderer(ID2D1Factory* d2dFactory, ID2D1RenderTarget* renderTarget,
                        ID2D1SolidColorBrush* fillBrush, ID2D1SolidColorBrush* outlineBrush, FLOAT outlineWidth)
        : refCount_(1), d2dFactory_(d2dFactory), renderTarget_(renderTarget), fillBrush_(fillBrush), outlineBrush_(outlineBrush), outlineWidth_(outlineWidth)
    {
        if (d2dFactory_) d2dFactory_->AddRef();
        if (renderTarget_) renderTarget_->AddRef();
        if (fillBrush_) fillBrush_->AddRef();
        if (outlineBrush_) outlineBrush_->AddRef();
    }

    ~OutlineTextRenderer()
    {
        if (d2dFactory_) d2dFactory_->Release();
        if (renderTarget_) renderTarget_->Release();
        if (fillBrush_) fillBrush_->Release();
        if (outlineBrush_) outlineBrush_->Release();
    }

    IFACEMETHODIMP QueryInterface(REFIID iid, void** ppvObject) override
    {
        if (ppvObject == nullptr) return E_POINTER;
        *ppvObject = nullptr;
        if (iid == __uuidof(IUnknown)) *ppvObject = static_cast<IUnknown*>(static_cast<IDWriteTextRenderer*>(this));
        else if (iid == __uuidof(IDWriteTextRenderer)) *ppvObject = static_cast<IDWriteTextRenderer*>(this);
        else if (iid == __uuidof(IDWritePixelSnapping)) *ppvObject = static_cast<IDWritePixelSnapping*>(static_cast<IDWriteTextRenderer*>(this));
        else return E_NOINTERFACE;
        AddRef();
        return S_OK;
    }

    IFACEMETHODIMP_(ULONG) AddRef() override { return InterlockedIncrement(&refCount_); }
    IFACEMETHODIMP_(ULONG) Release() override
    {
        ULONG newCount = InterlockedDecrement(&refCount_);
        if (newCount == 0) delete this;
        return newCount;
    }

    IFACEMETHODIMP DrawGlyphRun(
        void* clientDrawingContext,
        FLOAT baselineOriginX,
        FLOAT baselineOriginY,
        DWRITE_MEASURING_MODE measuringMode,
        DWRITE_GLYPH_RUN const* glyphRun,
        DWRITE_GLYPH_RUN_DESCRIPTION const* glyphRunDescription,
        IUnknown* clientDrawingEffect) override
    {
        if (!glyphRun || !d2dFactory_ || !renderTarget_) return E_FAIL;

        IDWriteFontFace* fontFace = glyphRun->fontFace;
        if (!fontFace) return E_FAIL;

        WRL::ComPtr<ID2D1PathGeometry> pathGeometry;
        HRESULT hr = d2dFactory_->CreatePathGeometry(pathGeometry.GetAddressOf());
        if (FAILED(hr)) return hr;

        WRL::ComPtr<ID2D1GeometrySink> geometrySink;
        hr = pathGeometry->Open(geometrySink.GetAddressOf());
        if (FAILED(hr)) return hr;

        hr = fontFace->GetGlyphRunOutline(
            glyphRun->fontEmSize,
            glyphRun->glyphIndices,
            glyphRun->glyphAdvances,
            glyphRun->glyphOffsets,
            glyphRun->glyphCount,
            glyphRun->isSideways,
            glyphRun->bidiLevel % 2,
            geometrySink.Get());
        if (FAILED(hr)) { geometrySink->Close(); return hr; }

        geometrySink->Close();

        D2D1::Matrix3x2F translate = D2D1::Matrix3x2F::Translation(baselineOriginX, baselineOriginY);

        WRL::ComPtr<ID2D1TransformedGeometry> transformedGeometry;
        hr = d2dFactory_->CreateTransformedGeometry(pathGeometry.Get(), translate, transformedGeometry.GetAddressOf());
        if (FAILED(hr)) return hr;

        if (outlineBrush_ && outlineWidth_ > 0.0f)
        {
            D2D1_STROKE_STYLE_PROPERTIES strokeProps = D2D1::StrokeStyleProperties();
            strokeProps.startCap = D2D1_CAP_STYLE_ROUND;
            strokeProps.endCap = D2D1_CAP_STYLE_ROUND;
            strokeProps.dashCap = D2D1_CAP_STYLE_ROUND;
            strokeProps.lineJoin = D2D1_LINE_JOIN_ROUND;
            strokeProps.miterLimit = 10.0f;

            WRL::ComPtr<ID2D1StrokeStyle> strokeStyle;
            hr = d2dFactory_->CreateStrokeStyle(&strokeProps, nullptr, 0, strokeStyle.GetAddressOf());
            if (SUCCEEDED(hr)) renderTarget_->DrawGeometry(transformedGeometry.Get(), outlineBrush_, outlineWidth_, strokeStyle.Get());
            else renderTarget_->DrawGeometry(transformedGeometry.Get(), outlineBrush_, outlineWidth_);
        }

        if (fillBrush_) renderTarget_->FillGeometry(transformedGeometry.Get(), fillBrush_);

        return S_OK;
    }

    IFACEMETHODIMP DrawUnderline(void* , FLOAT, FLOAT, DWRITE_UNDERLINE const*, IUnknown*) override { return S_OK; }
    IFACEMETHODIMP DrawStrikethrough(void* , FLOAT, FLOAT, DWRITE_STRIKETHROUGH const*, IUnknown*) override { return S_OK; }
    IFACEMETHODIMP DrawInlineObject(void* , FLOAT, FLOAT, IDWriteInlineObject*, BOOL, BOOL, IUnknown*) override { return E_NOTIMPL; }

    IFACEMETHODIMP IsPixelSnappingDisabled(void* , BOOL* isDisabled) override { *isDisabled = FALSE; return S_OK; }
    IFACEMETHODIMP GetCurrentTransform(void* , DWRITE_MATRIX* transform) override
    {
        if (!renderTarget_ || !transform) return E_FAIL;
        D2D1::Matrix3x2F m; renderTarget_->GetTransform(&m);
        transform->m11 = m._11; transform->m12 = m._12; transform->m21 = m._21; transform->m22 = m._22; transform->dx = m._31; transform->dy = m._32;
        return S_OK;
    }
    IFACEMETHODIMP GetPixelsPerDip(void* , FLOAT* pixelsPerDip) override
    {
        if (!renderTarget_ || !pixelsPerDip) return E_FAIL;
        FLOAT dpiX, dpiY; renderTarget_->GetDpi(&dpiX, &dpiY);
        *pixelsPerDip = dpiX / 96.0f; return S_OK;
    }

private:
    LONG refCount_;
    ID2D1Factory* d2dFactory_ = nullptr;
    ID2D1RenderTarget* renderTarget_ = nullptr;
    ID2D1SolidColorBrush* fillBrush_ = nullptr;
    ID2D1SolidColorBrush* outlineBrush_ = nullptr;
    FLOAT outlineWidth_ = 0.0f;
};

//=============================================================================
// DirectWriteCustomFont 実装
//=============================================================================
HRESULT DirectWriteCustomFont::Init(IDXGISwapChain* swapChain)
{
    HRESULT result = S_OK;

    result = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, pD2DFactory.GetAddressOf());
    if (FAILED(result)) return result;

    result = swapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
    if (FAILED(result)) return result;

    UINT dpi = GetDpiForWindow(GetWindow());
    FLOAT dpiF = static_cast<FLOAT>(dpi);

    D2D1_RENDER_TARGET_PROPERTIES props = D2D1::RenderTargetProperties(
        D2D1_RENDER_TARGET_TYPE_DEFAULT,
        D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED),
        dpiF, dpiF);

    result = pD2DFactory->CreateDxgiSurfaceRenderTarget(pBackBuffer.Get(), &props, pRenderTarget.GetAddressOf());
    if (FAILED(result)) return result;

    pRenderTarget->SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE_CLEARTYPE);

    result = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(pDWriteFactory.GetAddressOf()));
    if (FAILED(result)) return result;

    pFontCollectionLoader = new CustomFontCollectionLoader();
    result = pDWriteFactory->RegisterFontCollectionLoader(pFontCollectionLoader.Get());
    if (FAILED(result)) return result;

    result = FontLoader();
    if (FAILED(result)) return result;

	int id = GetPresetID(Setting);
	if (id == -1) return E_FAIL;

    return S_OK;
}

HRESULT DirectWriteCustomFont::FontLoader()
{
    HRESULT result = S_OK;
    result = pDWriteFactory->CreateCustomFontCollection(pFontCollectionLoader.Get(), pFontFileList.data(), (UINT32)pFontFileList.size(), &fontCollection);
    if (FAILED(result)) return result;

    result = GetFontFamilyName(fontCollection.Get());
    if (FAILED(result)) return result;

    return S_OK;
}

std::wstring DirectWriteCustomFont::GetFontName(int num)
{
    if (fontNamesList.empty()) return L"";

    if (num >= static_cast<int>(fontNamesList.size())) return fontNamesList[0];

    return fontNamesList[num];
}

int DirectWriteCustomFont::GetPresetID(const FontData& fontData)
{
    // これヘルパー関数として作ってたけど新規作成じゃないとだめ
    for(const auto& preset : m_PresetUseOrderList)
    {
        // FontDataを比較
        // 演算子オーバーロードしたので比較可能
        if (preset.data == fontData)
        {
            return preset.id; // 見つかったプリセットのindexを返す
        }
	}
	return -1; // 見つからなければ-1を返す
}

int DirectWriteCustomFont::GetFontNameNum() { return (int)fontNamesList.size(); }

// プリセットを検索または新規作成して、そのindexを返す
int DirectWriteCustomFont::FindOrCreateVisualPreset(const FontData& data)
{
    // 既存のプリセットを検索
	int count = 0;
    for (const auto& preset : m_PresetUseOrderList)
    {
        // FontDataを比較
        // 演算子オーバーロードしたので比較可能
        if (preset.data == data)
        {
            return count; // 見つかったプリセットのindexを返す
        }
        count++;
    }

    // 見つからなければ新規作成
    int newId = m_PresetIdCounter++;

    // キャッシュが最大数に達していたら末尾を削除
    if (m_PresetUseOrderList.size() >= MAX_PRESET_CACHE_SIZE)
    {
        // マップから削除
        int oldId = m_PresetUseOrderList.back().id;
        m_PresetCacheMap.erase(oldId);
        // リストから削除
        m_PresetUseOrderList.pop_back();
    }

    // 新しいプリセットを作成してリストの先頭に追加
    FontPreset newPreset;
    newPreset.data = data;
    newPreset.id = newId;

    // リソースを作成 (TextFormat, Brushes)
	HRESULT hr = S_OK;
    hr = pDWriteFactory->CreateTextFormat(
        GetFontFileNameWithoutExtension(newPreset.data.font).c_str(), 
        fontCollection.Get(), 
        newPreset.data.fontWeight,
        newPreset.data.fontStyle,
        newPreset.data.fontStretch,
        newPreset.data.fontSize,
        newPreset.data.localeName,
        newPreset.textFormat.GetAddressOf()
    );

    if (FAILED(hr)) return -1;

    hr = newPreset.textFormat->SetTextAlignment(newPreset.data.textAlignment);
    if (FAILED(hr)) return -1;

    hr = pRenderTarget->CreateSolidColorBrush(newPreset.data.Color, newPreset.brush.GetAddressOf());
    if (FAILED(hr)) return -1;

    hr = pRenderTarget->CreateSolidColorBrush(newPreset.data.shadowColor, newPreset.shadowBrush.GetAddressOf());
    if (FAILED(hr)) return -1;

    if (newPreset.data.outlineWidth > 0.0f)
    {
        hr = pRenderTarget->CreateSolidColorBrush(newPreset.data.outlineColor, newPreset.outlineBrush.GetAddressOf());
        if (FAILED(hr)) return -1;
    }
    else
    {
        newPreset.outlineBrush.Reset();
	}

	// 作成したプリセットをリストとマップに登録
    m_PresetUseOrderList.push_front(newPreset);
    m_PresetCacheMap[newId] = m_PresetUseOrderList.begin();
    // 先頭に登録しているためindexは0である
    return 0;
}

WRL::ComPtr<IDWriteTextLayout> DirectWriteCustomFont::FindOrCreateTextLayout(const int& preset_index, const std::string& str, FLOAT maxWidth, FLOAT maxHeight)
{
    // これ引数に描画対象となるformatの追加必要だーね

	// キャッシュを検索
    for(auto& cached : m_TextLayoutCache)
    {
        if (cached.first == str && fabs(cached.second->GetMaxWidth() - maxWidth) < 0.0001f && fabs(cached.second->GetMaxHeight() - maxHeight) < 0.0001f)
        {
            return cached.second; // 見つかったキャッシュのレイアウトを返す
        }
	}

	// 見つからなければ新規作成
    WRL::ComPtr<IDWriteTextLayout> tempLayout; // 一時レイアウト
	const std::wstring wstr = StringToWString(str); // マルチバイト文字列をワイド文字列に変換し変数として保持

    HRESULT hr = pDWriteFactory->CreateTextLayout(
        wstr.c_str(),
        (UINT32)wstr.size(),
        m_PresetUseOrderList.at(preset_index).textFormat.Get(),
        maxWidth,
        maxHeight,
        tempLayout.GetAddressOf()
	);
    if (FAILED(hr)) return nullptr;

    // 問題ないので代入を試みる
	// キャッシュが最大数に達していたら最初の要素を削除
    if (m_TextLayoutCache.size() >= MAX_TEXTLAYOUT_CACHE_SIZE)
    {
        m_TextLayoutCache.erase(m_TextLayoutCache.begin());
    }
    // 新しいレイアウトをキャッシュに追加(
	m_TextLayoutCache.insert({ str, tempLayout });
	return tempLayout;
}

// 指定したIDのプリセットを適用する
HRESULT DirectWriteCustomFont::ApplyVisualPreset(int presetId)
{
    auto it = m_PresetCacheMap.find(presetId);
    if (it == m_PresetCacheMap.end())
    {
        return E_INVALIDARG; // プリセットが見つからない
    }

    // 見つかったプリセットをstd::rotateを利用しリストの先頭に移動する
	std::rotate(m_PresetUseOrderList.begin(), it->second, std::next(it->second));

    // このプリセットをアクティブにする
    m_ActivePresetId = presetId;

    // 描画に使うポインタを、アクティブなプリセットのものに設定
    // (もしクラスのメンバとして pTextFormat 等を保持する場合)
    // pTextFormat = m_presetUsageList.front().textFormat;
    // pBrush = m_presetUsageList.front().brush;
    // ...

    return S_OK;
}


/*
HRESULT DirectWriteCustomFont::SetFont(FontData set)
{
    HRESULT result = S_OK;

    Setting = set;

	// プリセットを検索または作成
    int presetId = FindOrCreateVisualPreset(set);
    // 適応
	result = ApplyVisualPreset(presetId);
	if (FAILED(result)) return result;

    result = pDWriteFactory->CreateTextFormat(GetFontFileNameWithoutExtension(Setting.font).c_str(), fontCollection.Get(), Setting.fontWeight, Setting.fontStyle, Setting.fontStretch, Setting.fontSize, Setting.localeName, pTextFormat.end()->GetAddressOf());
    if (FAILED(result)) return result;

    result = pTextFormat->SetTextAlignment(Setting.textAlignment);
    if (FAILED(result)) return result;

    result = pRenderTarget->CreateSolidColorBrush(Setting.Color, pBrush.GetAddressOf());
    if (FAILED(result)) return result;

    result = pRenderTarget->CreateSolidColorBrush(Setting.shadowColor, pShadowBrush.GetAddressOf());
    if (FAILED(result)) return result;

    if (Setting.outlineWidth > 0.0f)
    {
        result = pRenderTarget->CreateSolidColorBrush(Setting.outlineColor, pOutlineBrush.GetAddressOf());
        if (FAILED(result)) return result;
    }
    else
    {
        pOutlineBrush.Reset();
    }

    // キャッシュされたレイアウトをクリア（フォント/サイズが変わったため）
    //ClearTextCache();

    return S_OK;
}
*/

/*
HRESULT DirectWriteCustomFont::SetFont(WCHAR const* fontname, DWRITE_FONT_WEIGHT fontWeight, DWRITE_FONT_STYLE fontStyle, DWRITE_FONT_STRETCH fontStretch, FLOAT fontSize, WCHAR const* localeName, DWRITE_TEXT_ALIGNMENT textAlignment, D2D1_COLOR_F Color, D2D1_COLOR_F shadowColor, D2D1_POINT_2F shadowOffset)
{
    HRESULT result = S_OK;
    result = pDWriteFactory->CreateTextFormat(GetFontFileNameWithoutExtension(std::wstring(fontname)).c_str(), fontCollection.Get(), fontWeight, fontStyle, fontStretch, fontSize, localeName, pTextFormat.GetAddressOf());
    if (FAILED(result)) return result;

    result = pTextFormat->SetTextAlignment(textAlignment);
    if (FAILED(result)) return result;

    result = pRenderTarget->CreateSolidColorBrush(Color, pBrush.GetAddressOf());
    if (FAILED(result)) return result;

    result = pRenderTarget->CreateSolidColorBrush(shadowColor, pShadowBrush.GetAddressOf());
    if (FAILED(result)) return result;

    pOutlineBrush.Reset();
    ClearTextCache();
    return S_OK;
}
*/

// キャッシュされた文字列用レイアウトを生成
/*
HRESULT DirectWriteCustomFont::SetText(const std::string& str, FLOAT maxWidth, FLOAT maxHeight)
{
    // ここの第一引数にid必要なのでsettextにもidないしはfontdataいるかも
	FindOrCreateTextLayout(m_ActivePresetId, str, maxWidth, maxHeight);

    // old

    // 同じ文字列かつ同じサイズ制限が既にキャッシュされている場合は何もしない
    D2D1_SIZE_F targetSize = pRenderTarget->GetSize();
    FLOAT wLimit = (maxWidth > 0.0f) ? maxWidth : targetSize.width;
    FLOAT hLimit = (maxHeight > 0.0f) ? maxHeight : targetSize.height;

    // 浮動小数比較用の小さなイプシロン
    const FLOAT EPS = 0.0001f;
    if (!cachedText.empty() && cachedText == str && fabs(cachedMaxWidth - wLimit) < EPS && fabs(cachedMaxHeight - hLimit) < EPS)
    {
        return S_OK; // 既にキャッシュ済み
    }

    // wstringに変換
    std::wstring w = StringToWString(str);

    // 失敗時に既存の pTextLayout を上書きしないよう、一時レイアウトを作成
    WRL::ComPtr<IDWriteTextLayout> tempLayout;
    HRESULT hr = pDWriteFactory->CreateTextLayout(w.c_str(), (UINT32)w.size(), pTextFormat.Get(), wLimit, hLimit, tempLayout.GetAddressOf());
    if (FAILED(hr))
    {
        return hr;
    }

    // 成功：一時レイアウトをキャッシュに反映（以前の pTextLayout は安全に解放される）
    pTextLayout = tempLayout;
    cachedText = str;
    cachedWText = std::move(w);
    cachedMaxWidth = wLimit;
    cachedMaxHeight = hLimit;

    return S_OK;
}
*/

/*
void DirectWriteCustomFont::ClearTextCache()
{
    // 自動的に管理されるからこの関数多分いらない

    //pTextLayout.Reset();
    //cachedText.clear();
    //cachedWText.clear();
}
*/

// 事前キャッシュ(第一引数preset_idからfontdata?
HRESULT DirectWriteCustomFont::PreCacheTextLayout(FontData& fontData, const std::string& str, FLOAT maxWidth, FLOAT maxHeight)
{
    // 1. FontData からプリセットIDを取得（または作成）
    int presetId = FindOrCreateVisualPreset(fontData);
    if (presetId == -1)
    {
        return E_FAIL; // プリセットの作成に失敗
    }

    // 2. テキストレイアウトを検索または作成
    WRL::ComPtr<IDWriteTextLayout> layout = FindOrCreateTextLayout(presetId, str, maxWidth, maxHeight);
    if (layout == nullptr)
    {
        return E_FAIL; // レイアウトの作成に失敗
    }

    return S_OK;
}


// DrawString: pos バージョン
HRESULT DirectWriteCustomFont::DrawString(const int& preset_id, std::string str, const Vector2& pos, D2D1_DRAW_TEXT_OPTIONS options, bool shadow, bool outline)
{
    HRESULT result = S_OK;
    std::wstring wstr = StringToWString(str);

    // 使用するレイアウトを決定：キャッシュ済みか一時か
    WRL::ComPtr<IDWriteTextLayout> layoutToUse;
    if(m_TextLayoutCache.find(str) != m_TextLayoutCache.end())
    {
        layoutToUse = m_TextLayoutCache[str];
    }
    else
    {
        // レイアウトを作成

        // ここid入れてるがindexでないといけない
        layoutToUse = FindOrCreateTextLayout(preset_id, str, 4096.0f, 4096.0f); // ここのmaxw/hは見直す必要あるかも
        if(layoutToUse == nullptr)
        {
            return E_FAIL;
		}
    }
	// 現在のactiveIdに対応するプリセットを取得
    const FontPreset& activePreset = m_PresetUseOrderList[preset_id]; // preset_idを使用してプリセットを取得
	FontData activeData = activePreset.data;
    WRL::ComPtr<ID2D1SolidColorBrush> brush = activePreset.brush;
    WRL::ComPtr<ID2D1SolidColorBrush> shadowBrush = activePreset.shadowBrush;
	WRL::ComPtr<ID2D1SolidColorBrush> outlineBrush = activePreset.outlineBrush;


	// 原点の設定
    D2D1_POINT_2F origin = D2D1::Point2F(pos.x, pos.y);


    // 影の描画
    if (shadow)
    {
        pRenderTarget->BeginDraw();
        pRenderTarget->DrawTextLayout(D2D1::Point2F(origin.x - activeData.shadowOffset.x, origin.y - activeData.shadowOffset.y), layoutToUse.Get(), shadowBrush.Get(), options);
        result = pRenderTarget->EndDraw();
        if (FAILED(result)) return result;
    }

    // カスタムレンダラーを使用した縁取り描画
    if (outline && activeData.outlineWidth > 0.0f && outlineBrush)
    {
        OutlineTextRenderer* renderer = new (std::nothrow) OutlineTextRenderer(pD2DFactory.Get(), pRenderTarget.Get(), brush.Get(), outlineBrush.Get(), activeData.outlineWidth);
        if (!renderer) return E_OUTOFMEMORY;

        pRenderTarget->BeginDraw();
        result = layoutToUse->Draw(nullptr, renderer, origin.x, origin.y);
        if (FAILED(result))
        {
            // フォールバック描画
            pRenderTarget->DrawTextLayout(origin, layoutToUse.Get(), brush.Get(), options);
        }
        HRESULT endHr = pRenderTarget->EndDraw();
        renderer->Release();
        if (FAILED(endHr)) return endHr;
    }
    else
    {
        pRenderTarget->BeginDraw();
        pRenderTarget->DrawTextLayout(origin, layoutToUse.Get(), brush.Get(), options);
        result = pRenderTarget->EndDraw();
        if (FAILED(result)) return result;
    }

    return S_OK;
}

/*
// DrawString: 矩形バージョン(多分使わないのでコメントアウト)
HRESULT DirectWriteCustomFont::DrawString(std::string str, D2D1_RECT_F rect, D2D1_DRAW_TEXT_OPTIONS options, bool shadow, bool outline)
{
    HRESULT result = S_OK;
    std::wstring wstr = StringToWString(str);

    WRL::ComPtr<IDWriteTextLayout> layoutToUse;
    if (!cachedText.empty() && str == cachedText && pTextLayout)
    {
        layoutToUse = pTextLayout;
    }
    else
    {
        result = pDWriteFactory->CreateTextLayout(wstr.c_str(), (UINT32)wstr.size(), pTextFormat.Get(), rect.right - rect.left, rect.bottom - rect.top, layoutToUse.GetAddressOf());
        if (FAILED(result)) return result;
    }

    if (shadow)
    {
        pRenderTarget->BeginDraw();
        pRenderTarget->DrawText(wstr.c_str(), (UINT32)wstr.size(), pTextFormat.Get(), D2D1::RectF(rect.left - Setting.shadowOffset.x, rect.top - Setting.shadowOffset.y, rect.right - Setting.shadowOffset.x, rect.bottom - Setting.shadowOffset.y), pShadowBrush.Get(), options);
        result = pRenderTarget->EndDraw();
        if (FAILED(result)) return result;
    }

    if (outline && Setting.outlineWidth > 0.0f && pOutlineBrush)
    {
        OutlineTextRenderer* renderer = new (std::nothrow) OutlineTextRenderer(pD2DFactory.Get(), pRenderTarget.Get(), pBrush.Get(), pOutlineBrush.Get(), Setting.outlineWidth);
        if (!renderer) return E_OUTOFMEMORY;

        pRenderTarget->BeginDraw();
        result = layoutToUse->Draw(nullptr, renderer, rect.left, rect.top);
        if (FAILED(result)) pRenderTarget->DrawText(wstr.c_str(), (UINT32)wstr.size(), pTextFormat.Get(), rect, pBrush.Get(), options);
        HRESULT endHr = pRenderTarget->EndDraw();
        renderer->Release();
        if (FAILED(endHr)) return endHr;
    }
    else
    {
        pRenderTarget->BeginDraw();
        pRenderTarget->DrawText(wstr.c_str(), (UINT32)wstr.size(), pTextFormat.Get(), rect, pBrush.Get(), options);
        result = pRenderTarget->EndDraw();
        if (FAILED(result)) return result;
    }

    return S_OK;
}
*/

HRESULT DirectWriteCustomFont::GetFontFamilyName(IDWriteFontCollection* customFontCollection, const WCHAR* locale)
{
    HRESULT result = S_OK;
    fontNamesList.clear();

    UINT32 familyCount = customFontCollection->GetFontFamilyCount();
    for (UINT32 i = 0; i < familyCount; ++i)
    {
        WRL::ComPtr<IDWriteFontFamily> fontFamily;
        result = customFontCollection->GetFontFamily(i, fontFamily.GetAddressOf());
        if (FAILED(result)) return result;

        WRL::ComPtr<IDWriteLocalizedStrings> familyNames;
        result = fontFamily->GetFamilyNames(familyNames.GetAddressOf());
        if (FAILED(result)) return result;

        UINT32 index = 0; BOOL exists = FALSE;
        result = familyNames->FindLocaleName(locale, &index, &exists);
        if (FAILED(result)) return result;
        if (!exists) { result = familyNames->FindLocaleName(L"en-us", &index, &exists); if (FAILED(result)) return result; }

        UINT32 length = 0;
        result = familyNames->GetStringLength(index, &length);
        if (FAILED(result)) return result;

        std::wstring name(length + 1, L'\0');
        result = familyNames->GetString(index, &name[0], length + 1);
        if (FAILED(result)) return result;
        name.resize(length);

        fontNamesList.push_back(name);
    }

    return S_OK;
}

HRESULT DirectWriteCustomFont::GetAllFontFamilyName(IDWriteFontCollection* customFontCollection)
{
    HRESULT result = S_OK;
    fontNamesList.clear();

    UINT32 familyCount = customFontCollection->GetFontFamilyCount();
    for (UINT32 i = 0; i < familyCount; ++i)
    {
        WRL::ComPtr<IDWriteFontFamily> fontFamily;
        result = customFontCollection->GetFontFamily(i, fontFamily.GetAddressOf());
        if (FAILED(result)) return result;

        WRL::ComPtr<IDWriteLocalizedStrings> familyNames;
        result = fontFamily->GetFamilyNames(familyNames.GetAddressOf());
        if (FAILED(result)) return result;

        UINT32 nameCount = familyNames->GetCount();
        for (UINT32 j = 0; j < nameCount; ++j)
        {
            UINT32 length = 0;
            result = familyNames->GetStringLength(j, &length);
            if (FAILED(result)) return result;

            std::wstring name(length + 1, L'\0');
            result = familyNames->GetString(j, &name[0], length + 1);
            if (FAILED(result)) return result;
            name.resize(length);

            fontNamesList.push_back(name);
        }
    }

    return S_OK;
}

std::wstring DirectWriteCustomFont::GetFontFileNameWithoutExtension(const std::wstring& filePath)
{
    size_t start = filePath.find_last_of(L"/\\");
    if (start == std::wstring::npos) start = 0; else start = start + 1;
    size_t end = filePath.find_last_of(L'.');
    if (end == std::wstring::npos || end < start) end = filePath.length();
    return filePath.substr(start, end - start);
}

// stringをwstringへ変換する
std::wstring DirectWriteCustomFont::StringToWString(std::string oString) const
{
    int iBufferSize = MultiByteToWideChar(CP_ACP, 0, oString.c_str(), -1, nullptr, 0);
    if (iBufferSize <= 0) return L"";

    std::wstring buf(iBufferSize, L'\0');
    MultiByteToWideChar(CP_ACP, 0, oString.c_str(), -1, &buf[0], iBufferSize);
    buf.resize(iBufferSize - 1);
    return buf;
}

HRESULT DirectWriteCustomFont::GetTextSizeDips(const std::string& str, FLOAT* outWidthDips, FLOAT* outHeightDips) const
{
    if (!outWidthDips || !outHeightDips) return E_POINTER;
    if (!pDWriteFactory) return E_FAIL;

    WRL::ComPtr<IDWriteTextLayout> layoutToUse;
    if(m_TextLayoutCache.find(str) != m_TextLayoutCache.end())
    {
        layoutToUse = m_TextLayoutCache.at(str);
	}
    else
    {
        // レイアウトは作成せずFAILを返す
		return E_FAIL;
    }

    if (!layoutToUse) return E_FAIL;

    DWRITE_TEXT_METRICS metrics = {};
    HRESULT hr = layoutToUse->GetMetrics(&metrics);
    if (FAILED(hr)) return hr;

    DWRITE_OVERHANG_METRICS overhang = {};
    hr = layoutToUse->GetOverhangMetrics(&overhang);
    if (FAILED(hr)) return hr;

    // overhang の負値は「はみ出していない」ことを示すので、正の値のみ加算する
    FLOAT addLeft = (overhang.left  > 0.0f) ? overhang.left  : 0.0f;
    FLOAT addTop  = (overhang.top   > 0.0f) ? overhang.top   : 0.0f;
    FLOAT addRight= (overhang.right > 0.0f) ? overhang.right : 0.0f;
    FLOAT addBottom=(overhang.bottom> 0.0f) ? overhang.bottom: 0.0f;

    FLOAT rawWidth  = metrics.widthIncludingTrailingWhitespace + addLeft + addRight;
    FLOAT rawHeight = metrics.height + addTop + addBottom;

    // 安全のためクランプ
    *outWidthDips  = (rawWidth  > 0.0f) ? rawWidth  : 0.0f;
    *outHeightDips = (rawHeight > 0.0f) ? rawHeight : 0.0f;

    // デバッグ出力（必要なら有効化）
    /*
    {
        wchar_t dbg[512];
        swprintf_s(dbg, L"Metrics: w=%.2f wInc=%.2f h=%.2f | Overhang L=%.2f T=%.2f R=%.2f B=%.2f -> result w=%.2f h=%.2f\n",
            metrics.width, metrics.widthIncludingTrailingWhitespace, metrics.height,
            overhang.left, overhang.top, overhang.right, overhang.bottom,
            *outWidthDips, *outHeightDips);
        OutputDebugStringW(dbg);
    }
    */

    return S_OK;
}

HRESULT DirectWriteCustomFont::GetTextSizePixels(const std::string& str, FLOAT* outWidthPx, FLOAT* outHeightPx) const
{
    if (!outWidthPx || !outHeightPx) return E_POINTER;

    FLOAT wDip = 0.0f, hDip = 0.0f;
    HRESULT hr = GetTextSizeDips(str, &wDip, &hDip);
    if (FAILED(hr)) return hr;

    // DPI 取得（デフォルト 96）
    FLOAT dpiX = 96.0f, dpiY = 96.0f;
    if (pRenderTarget) pRenderTarget->GetDpi(&dpiX, &dpiY);

    FLOAT scaleX = dpiX / 96.0f;
    FLOAT scaleY = dpiY / 96.0f;

    *outWidthPx = wDip * scaleX;
    *outHeightPx = hDip * scaleY;
    return S_OK;
}