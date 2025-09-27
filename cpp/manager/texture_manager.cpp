#include "manager/texture_manager.h"

// static変数の定義
std::unordered_map<int, TexData> TextureManager::m_TextureMap = {};
std::unordered_map<std::wstring, int> TextureManager::m_PathMap = {}; // ファイルパスをキーにしたマップ
int TextureManager::m_TextureIDCounter = 0; // テクスチャIDのカウンター
int TextureManager::m_CollectorCount = 0; // 起動時以降回り続けるのでinitでリセットする必要はない