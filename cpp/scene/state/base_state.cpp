#include "main.h"
#include "scene/state/base_state.h"

void State::Init()
{
	Scene::Init();
	Scene::UpdateObjectByTag("system");
}