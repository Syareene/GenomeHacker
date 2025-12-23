#pragma once

#include "main.h"
#include "lib/write_font.h"
#include "lib/renderer.h"

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
    // ref: 
    // https://learn.microsoft.com/ja-jp/windows/win32/direct2d/getting-started-with-direct2d
    // 基本的にfactoryは一こ作ればいいのでrendererあたりでdirect2d作ってそのfactoryをfontで使うのがいいね

    // 後本来作るpD2DFactory->CreateHwndRenderTargetがねえ
    // 終了時はSafeRelease(pD2DFactory);

    HRESULT result = S_OK;

	// ID2D1Factory作成
    //result = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, pD2DFactory.GetAddressOf());
    //if (FAILED(result)) return result;

    //result = swapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
    //if (FAILED(result)) return result;

    //UINT dpi = GetDpiForWindow(GetWindow());
    //FLOAT dpiF = static_cast<FLOAT>(dpi);

    //D2D1_RENDER_TARGET_PROPERTIES props = D2D1::RenderTargetProperties(
    //    D2D1_RENDER_TARGET_TYPE_DEFAULT,
    //    D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED),
    //    dpiF, dpiF);

    //result = pD2DFactory->CreateDxgiSurfaceRenderTarget(pBackBuffer.Get(), &props, pRenderTarget.GetAddressOf());
    //if (FAILED(result)) return result;

    Renderer::GetID2D1DeviceContext()->SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE_CLEARTYPE);

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

// プリセットを検索または新規作成して、そのidを返す
int DirectWriteCustomFont::FindOrCreateVisualPreset(const FontData& data)
{
    // 既存のプリセットを検索
    for (auto it = m_PresetUseOrderList.begin(); it != m_PresetUseOrderList.end(); ++it)
    {
        if (it->data == data)
        {
            // 見つかった場合、リストの先頭に移動してIDを返す
            m_PresetUseOrderList.splice(m_PresetUseOrderList.begin(), m_PresetUseOrderList, it);
            return it->id;
        }
    }

    // 見つからなければ新規作成
    if (m_PresetUseOrderList.size() >= MAX_PRESET_CACHE_SIZE)
    {
        int oldId = m_PresetUseOrderList.back().id;
        // mapとlistの両方に対し削除
        m_PresetCacheMap.erase(oldId);
        m_PresetUseOrderList.pop_back();
    }

    // 新しいプリセットを作成してリストの先頭に追加
    FontPreset newPreset;
    newPreset.data = data;
    newPreset.id = m_PresetIdCounter++;

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

    hr = Renderer::GetID2D1DeviceContext()->CreateSolidColorBrush(newPreset.data.Color, newPreset.brush.GetAddressOf());
    if (FAILED(hr)) return -1;

    hr = Renderer::GetID2D1DeviceContext()->CreateSolidColorBrush(newPreset.data.shadowColor, newPreset.shadowBrush.GetAddressOf());
    if (FAILED(hr)) return -1;

    if (newPreset.data.outlineWidth > 0.0f)
    {
        hr = Renderer::GetID2D1DeviceContext()->CreateSolidColorBrush(newPreset.data.outlineColor, newPreset.outlineBrush.GetAddressOf());
        if (FAILED(hr)) return -1;
    }
    else
    {
        newPreset.outlineBrush.Reset();
	}

	// 作成したプリセットをリストとマップに登録
    m_PresetUseOrderList.push_front(newPreset);
    m_PresetCacheMap[newPreset.id] = m_PresetUseOrderList.begin();
    return newPreset.id;
}

FontPreset* DirectWriteCustomFont::GetPreset(int presetId)
{
    auto it = m_PresetCacheMap.find(presetId);
    if (it == m_PresetCacheMap.end()) {
        return nullptr;
    }
    // アクセスされたのでリストの先頭に移動
    m_PresetUseOrderList.splice(m_PresetUseOrderList.begin(), m_PresetUseOrderList, it->second);
    return &(*m_PresetUseOrderList.begin());
}

WRL::ComPtr<IDWriteTextLayout> DirectWriteCustomFont::FindOrCreateTextLayout(const std::string& str, int presetId, FLOAT maxWidth, FLOAT maxHeight)
{
    TextLayoutKey key = { presetId, str };

    // キャッシュを検索
    auto map_it = m_TextLayoutCacheMap.find(key);
    if (map_it != m_TextLayoutCacheMap.end())
    {
        // 見つかった要素をリストの先頭に移動
        m_TextLayoutUsageList.splice(m_TextLayoutUsageList.begin(), m_TextLayoutUsageList, map_it->second);
        // 先頭に移動した要素のComPtrを返す
        return map_it->second->second;
    }
    
    // ヒットしなかったため新規作成

    // キャッシュが最大数に達していたら、一番古いもの（リストの末尾）を削除
    if (m_TextLayoutCacheMap.size() >= MAX_TEXTLAYOUT_CACHE_SIZE)
    {
        // a. 削除する要素のキーを取得
        const TextLayoutKey& oldKey = m_TextLayoutUsageList.back().first;
        // b. マップから削除
        m_TextLayoutCacheMap.erase(oldKey);
        // c. リストから削除
        m_TextLayoutUsageList.pop_back();
    }

    // プリセット（TextFormat）を取得
    FontPreset* preset = GetPreset(presetId);
    if (!preset) return nullptr;

    // 新しいレイアウトを作成
    WRL::ComPtr<IDWriteTextLayout> newLayout;
    const std::wstring wstr = StringToWString(str);

    HRESULT hr = pDWriteFactory->CreateTextLayout(
        wstr.c_str(),
        (UINT32)wstr.size(),
        preset->textFormat.Get(), // 正しいTextFormatを使用
        maxWidth,
        maxHeight,
        &newLayout
    );
    if (FAILED(hr)) return nullptr;

    // 作成したレイアウトをリストの先頭に追加
    m_TextLayoutUsageList.push_front({ key, newLayout });
    // 新しい要素を指すイテレータをマップに登録
    m_TextLayoutCacheMap[key] = m_TextLayoutUsageList.begin();

    return newLayout;
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

    return S_OK;
}

// 事前キャッシュ(第一引数preset_idからfontdata?
HRESULT DirectWriteCustomFont::PreCacheTextLayout(FontData& fontData, const std::string& str, FLOAT maxWidth, FLOAT maxHeight)
{
    // FontDataからプリセットIDを取得（または作成）
    int presetId = FindOrCreateVisualPreset(fontData);
    if (presetId == -1)
    {
        return E_FAIL; // プリセットの作成に失敗
    }

    // テキストレイアウトを検索または作成
    WRL::ComPtr<IDWriteTextLayout> layout = FindOrCreateTextLayout(str, presetId, maxWidth, maxHeight);
    if (layout == nullptr)
    {
        return E_FAIL; // レイアウトの作成に失敗
    }

    return S_OK;
}


// DrawString: pos バージョン
HRESULT DirectWriteCustomFont::DrawString(const std::string& str, int presetId, const Vector2& pos, D2D1_DRAW_TEXT_OPTIONS options, bool shadow, bool outline)
{
    FontPreset* preset = GetPreset(presetId);
    if (!preset) return E_INVALIDARG;

    WRL::ComPtr<IDWriteTextLayout> layout = FindOrCreateTextLayout(str, presetId, 4096.f, 4096.f);
    if (!layout) return E_FAIL;

	// 揃え補正のためにメトリクスを取得
    DWRITE_TEXT_METRICS metrics;
    layout->GetMetrics(&metrics);

    D2D1_POINT_2F origin = { pos.x, pos.y };

    // テキストのアライメント設定に応じて描画開始座標を補正する
    switch (preset->data.textAlignment)
    {
    case DWRITE_TEXT_ALIGNMENT_CENTER:
        // 中央揃え： テキストの幅の半分+左側の領域だけ左にずらす
		// 中央揃えの場合left(フォントが描画されてないスペース)+width(フォントが描画された幅)/2を引く
        origin.x -= (metrics.width / 2) + metrics.left;
        // yも中央揃えする
		origin.y -= (metrics.height / 2) + metrics.top;
        break;
    case DWRITE_TEXT_ALIGNMENT_TRAILING:
        // 右揃え： テキストの幅分+左側の領域だけ左にずらす
        origin.x -= metrics.width + metrics.left;
        break;
    case DWRITE_TEXT_ALIGNMENT_LEADING:
    default:
        // 左揃え: 何もしない
        break;
    }

	//pRenderTarget->BeginDraw();
    if (shadow && preset->shadowBrush)
    {
        Renderer::GetID2D1DeviceContext()->DrawTextLayout({ origin.x + preset->data.shadowOffset.x, origin.y + preset->data.shadowOffset.y }, layout.Get(), preset->shadowBrush.Get(), options);
    }

    if (outline && preset->data.outlineWidth > 0.0f && preset->outlineBrush)
    {
        OutlineTextRenderer renderer(Renderer::GetID2D1Factory(), Renderer::GetID2D1DeviceContext(), preset->brush.Get(), preset->outlineBrush.Get(), preset->data.outlineWidth);
        layout->Draw(nullptr, &renderer, origin.x, origin.y);
    }
    else
    {
        Renderer::GetID2D1DeviceContext()->DrawTextLayout(origin, layout.Get(), preset->brush.Get(), options);
    }
	//pRenderTarget->EndDraw();

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

    WRL::ComPtr<IDWriteTextLayout> layoutToUse = nullptr;
    for(auto& presetPair : m_TextLayoutCacheMap)
    {
        if (presetPair.first.text == str)
        {
            layoutToUse = presetPair.second->second;
			break;
        }
	}

    if (!layoutToUse) return E_FAIL;

    DWRITE_TEXT_METRICS metrics = {};
    HRESULT hr = layoutToUse->GetMetrics(&metrics);
    if (FAILED(hr)) return hr;

    DWRITE_OVERHANG_METRICS overhang = {};
    hr = layoutToUse->GetOverhangMetrics(&overhang);
    if (FAILED(hr)) return hr;

    // overhang の負値ははみ出していないことを示すため正の値のみ加算
    FLOAT addLeft = (overhang.left  > 0.0f) ? overhang.left  : 0.0f;
    FLOAT addTop  = (overhang.top   > 0.0f) ? overhang.top   : 0.0f;
    FLOAT addRight= (overhang.right > 0.0f) ? overhang.right : 0.0f;
    FLOAT addBottom=(overhang.bottom> 0.0f) ? overhang.bottom: 0.0f;

    FLOAT rawWidth  = metrics.widthIncludingTrailingWhitespace + addLeft + addRight;
    FLOAT rawHeight = metrics.height + addTop + addBottom;

    // 安全のためクランプ
    *outWidthDips  = (rawWidth  > 0.0f) ? rawWidth  : 0.0f;
    *outHeightDips = (rawHeight > 0.0f) ? rawHeight : 0.0f;

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
    if (Renderer::GetID2D1DeviceContext()) Renderer::GetID2D1DeviceContext()->GetDpi(&dpiX, &dpiY);

    FLOAT scaleX = dpiX / 96.0f;
    FLOAT scaleY = dpiY / 96.0f;

    *outWidthPx = wDip * scaleX;
    *outHeightPx = hDip * scaleY;
    return S_OK;
}