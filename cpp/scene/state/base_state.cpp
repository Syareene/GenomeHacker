#include "main.h"
#include "scene/state/base_state.h"

unsigned int State::m_ObjectIDCounter = 0;

State::~State() = default;

void State::Init()
{
    // override後、個別で追加したいものの追加


    //Scene::Init();
    //Scene::UpdateObjectByTag("system");
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

    // んー、これの欠点としてはscene->stateやstate->sceneの実行順序になってしまうのでレイヤーをちゃんと参照できなさそうなことか
    // まあstate変わっても常駐しているオブジェクトで常に画面の上に描画したいのってuiくらいだから一旦考えなくてもいいかも
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