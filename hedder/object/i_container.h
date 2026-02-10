#pragma once
#include <concepts>
#include <vector>
#include "manager/object_manager.h"

// ゲームオブジェクトの中に更に管理しているオブジェクトがあるような場合のためのインターフェース
// その様な場合はこれを継承する必要がある
class IContainer
{
public:
    virtual ~IContainer() = default;
    virtual void SubmitDrawRequests(std::vector<IGameObjectManager::RenderQueueData>& renderQueue) = 0;
    virtual void UpdateGPUData(IGameObjectManager::InstanceBufferData& data) = 0;
};

// C++20 コンセプト定義
// T型が IContainer の機能を持っているか（または継承しているか）を判定
template <typename T>
concept ContainerObject = std::derived_from<T, IContainer> || requires(T t, std::vector<IGameObjectManager::RenderQueueData>&q) 
{
    { t.SubmitDrawRequests(q) } -> std::same_as<void>;
    { t.UpdateGPUData() } -> std::same_as<void>;
};