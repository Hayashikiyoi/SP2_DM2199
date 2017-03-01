#include "SceneManager.h"
//#include "GL\glew.h"
#include "shader.hpp"

SceneManager::SceneManager() :currentLevel(0), nextLevel(0), offset(0), endCheck(false), levelCompleted(0)
{
	programID = LoadShaders("Shader//Texture.vertexshader", "Shader//Text.fragmentshader");
}
SceneManager::~SceneManager()
{
}

void SceneManager::changeScene(size_t levelnum)
{

	sceneManager[currentLevel]->Exit();
	currentLevel = 1;
	offset = levelnum;
	sceneManager[currentLevel]->Init();
}

void SceneManager::addScene(Scene* sceneName)
{
	sceneManager[nextLevel] = sceneName;
	nextLevel++;
}

void SceneManager::sceneUpdate(double dt)
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

void SceneManager::loadingScene()
{
	sceneManager[currentLevel]->Exit();
	currentLevel = offset;
	sceneManager[currentLevel]->Init();
}

unsigned int SceneManager::returnProg()
{
	return(this->programID);
}
