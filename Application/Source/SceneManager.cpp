#include "SceneManager.h"

SceneManager::SceneManager() :currentLevel(0), nextLevel(0), endCheck(false)
{
}
SceneManager::~SceneManager()
{
}

void SceneManager::changeScene(size_t levelnum)
{
	sceneManager[currentLevel]->Exit();
	currentLevel = levelnum;
	sceneManager[currentLevel]->Init();
}

void SceneManager::addScene(Scene* sceneName)
{
	sceneManager[nextLevel] = sceneName;
	nextLevel++;
}

void SceneManager::sceneUpdate(float dt)
{
	sceneManager[currentLevel]->Update(dt);
}

void SceneManager::startScreen()
{
	currentLevel = 0;
	sceneManager[currentLevel]->Init();
}

void SceneManager::render()
{
	sceneManager[currentLevel]->Render();
}

void SceneManager::end()
{
	sceneManager[currentLevel]->Exit();
}

void SceneManager::EndGame(bool end)
{
	endCheck = end;
}

bool SceneManager::EndCheck()
{
	return endCheck;
}
