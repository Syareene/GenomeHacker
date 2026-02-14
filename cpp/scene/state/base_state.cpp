#include "main.h"
#include "scene/state/base_state.h"
#include "object/panel.h"

unsigned int State::m_ObjectIDCounter = 0;

State::~State() = default;

void State::Init()
{
    // override後、個別で追加したいものの追加
}

void State::Uninit()
{
    DeleteAllGameObject();
    // override後、個別で追加したいものの追加
}

void State::Update()
{
    // override後、個別で追加したいものの追加

    // また、個別で定義してあるどのupdateを呼ぶかを指定
}

void State::UpdateGPUData()
{
    // 3dオブジェクトのGPUデータ更新
    for (auto& objects3d : m_Objects3D)
    {
        if (objects3d)
        {
            objects3d->UpdateGPUData();
        }
    }
    // 2dオブジェクトのGPUデータ更新
    for (auto& objects2d : m_Objects2D)
    {
        if (objects2d)
        {
            objects2d->UpdateGPUData();
        }
    }
}

void State::SubmitDrawRequests(std::vector<RenderQueueData>& renderQueue)
{
    // 3dオブジェクトの描画要求を送信
    for (auto& objects3d : m_Objects3D)
    {
        if (objects3d)
        {
            objects3d->SubmitDrawRequests(renderQueue, GetDrawTargetTags());
        }
    }
    // 2dオブジェクトの描画要求を送信
    for (auto& objects2d : m_Objects2D)
    {
        if (objects2d)
        {
            objects2d->SubmitDrawRequests(renderQueue, GetDrawTargetTags());
        }
    }
}

void State::Draw()
{
    // updateと同じ
}

void State::UpdateFinal()
{
    // systemオブジェクトを更新
    for (auto& systemObject : m_SystemObjects)
    {
        if (systemObject)
        {
            systemObject->UpdateFinal();
        }
    }
}

void State::FlushPendingObjects()
{
    // 3dオブジェクトの保留中オブジェクトをフラッシュ
    for (auto& objects3d : m_Objects3D)
    {
        if (!objects3d)
        {
            continue;
        }
        objects3d->FlushPendingObjects();
    }
    ObjectManager<Panel>* panelManager = nullptr;

    // 2dオブジェクトの保留中オブジェクトをフラッシュ
    for (auto& objects2d : m_Objects2D)
    {
        if (!objects2d)
        {
            continue;
        }
        ObjectManager<Panel>* manager = dynamic_cast<ObjectManager<Panel>*>(objects2d.get());
        if (manager)
        {
            panelManager = manager;
        }
        objects2d->FlushPendingObjects();
    }

    // パネル内にもmanagerがあるためそちらもフラッシュ処理

    // これパネル継承したやつの場合実行されんね
    if (panelManager)
    {
        for (auto& panelObj : panelManager->GetGameObjects())
        {
            panelObj.FlushPendingObjects();
        }
    }
}

void State::DeleteGameObject()
{
    // 不要になった3dオブジェクトを削除
    for (auto& objects3d : m_Objects3D)
    {
        if (objects3d)
        {
            objects3d->RemoveDestroyedObjects();
        }
    }

    // 不要になった2dオブジェクトを削除
    for (auto& objects2d : m_Objects2D)
    {
        if (objects2d)
        {
            objects2d->RemoveDestroyedObjects();
        }
    }
}

void State::DeleteAllGameObject()
{
    // 3dオブジェクトの終了処理
    for (auto& objects3d : m_Objects3D)
    {
        if (objects3d)
        {
            objects3d->Uninit();
        }
    }
    // 3dオブジェクトの全削除
    m_Objects3D.clear();
    // 2dオブジェクトの終了処理
    for (auto& objects2d : m_Objects2D)
    {
        if (objects2d)
        {
            objects2d->Uninit();
        }
    }
    // 2dオブジェクトの全削除
    m_Objects2D.clear();
    // システムオブジェクトの終了処理
    for (auto& systemObject : m_SystemObjects)
    {
        if (systemObject)
        {
            systemObject->Uninit();
        }
    }
    // システムオブジェクトの全削除
    m_SystemObjects.clear();
}

void State::UpdateStateObject()
{
    // システムオブジェクトの更新
    for (auto& systemObject : m_SystemObjects)
    {
        if (systemObject)
        {
            systemObject->Update();
        }
    }

    // 3dオブジェクトの更新
    for (auto& objects3d : m_Objects3D)
    {
        if (objects3d)
        {
            objects3d->Update();
        }
    }

    // 2dオブジェクトの更新
    for (auto& objects2d : m_Objects2D)
    {
        if (objects2d)
        {
            objects2d->Update();
        }
    }
    DeleteGameObject();
    // 待機オブジェクトの反映
	FlushPendingObjects();
}

void State::UpdateStateObjectByTag(const std::string& tag)
{
    // システムオブジェクトの更新
    for (auto& systemObject : m_SystemObjects)
    {
        if (systemObject)
        {
            systemObject->Update();
        }
    }
    // 3dオブジェクトの更新
    for (auto& objects3d : m_Objects3D)
    {
        if (objects3d)
        {
            objects3d->UpdateObjectByTag(tag);
        }
    }
    // 2dオブジェクトの更新
    for (auto& objects2d : m_Objects2D)
    {
        if (objects2d)
        {
            objects2d->UpdateObjectByTag(tag);
        }
    }
    DeleteGameObject();
    // 待機オブジェクトの反映
    FlushPendingObjects();
}

void State::UpdateStateObjectByTags(const std::list<std::string>& tags)
{
    // システムオブジェクトの更新
    for (auto& systemObject : m_SystemObjects)
    {
        if (systemObject)
        {
            systemObject->Update();
        }
    }
    // 3dオブジェクトの更新
    for (auto& objects3d : m_Objects3D)
    {
        if (objects3d)
        {
            objects3d->UpdateObjectByTags(tags);
        }
    }
    // 2dオブジェクトの更新
    for (auto& objects2d : m_Objects2D)
    {
        if (objects2d)
        {
            objects2d->UpdateObjectByTags(tags);
        }
    }

    DeleteGameObject();
    // 待機オブジェクトの反映
    FlushPendingObjects();
}

void State::DrawStateObject()
{
    // 3dオブジェクトの描画
    for (auto& objects3d : m_Objects3D)
    {
        if (objects3d)
        {
            objects3d->Draw();
        }
    }

    // 2dオブジェクトの描画
    for (auto& objects2d : m_Objects2D)
    {
        if (objects2d)
        {
            objects2d->Draw();
        }
    }
}

void State::DrawStateObjectByTag(const std::string& tag)
{
    // 3dオブジェクトの描画
    for (auto& objects3d : m_Objects3D)
    {
        if (objects3d)
        {
            objects3d->DrawObjectByTag(tag);
        }
    }
    // 2dオブジェクトの描画
    for (auto& objects2d : m_Objects2D)
    {
        if (objects2d)
        {
            objects2d->DrawObjectByTag(tag);
        }
    }
}

void State::DrawStateObjectByTags(const std::list<std::string>& tags)
{
    // 3dオブジェクトの描画
    for (auto& objects3d : m_Objects3D)
    {
        if (objects3d)
        {
            objects3d->DrawObjectByTags(tags);
        }
    }
    // 2dオブジェクトの描画
    for (auto& objects2d : m_Objects2D)
    {
        if (objects2d)
        {
            objects2d->DrawObjectByTags(tags);
        }
    }
}