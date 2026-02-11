#pragma once
#include <concepts>

// T型(GameObject)がstatic void SetPipelineState()を持っているかチェック
template <typename T>
concept HasPipelineState = requires 
{
    { T::SetPipelineState() } -> std::same_as<void>;
};