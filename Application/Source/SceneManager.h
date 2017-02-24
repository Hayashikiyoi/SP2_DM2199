#ifndef SceneManager_H
#define SceneManager_H

#include <map>
#include "Scene.h"

//Level include here
#include "SceneUI.h"

using std::map;

class SceneManager
{
public:
	//Constructor / deconstructor
	SceneManager();
	~SceneManager();
	static SceneManager *instance() //For singleton global value
	{
		if (!s_instance)
			s_instance = new SceneManager;
		return s_instance;
	}

	//Init / end screen
	void startScreen();
	void end();

	//Render objects to screen
	void render();

	//Change level
	void changeScene(size_t levelnum);

	//Add amount of level
	void addScene(Scene* sceneName);

	//Update scene
	void sceneUpdate(double dt);
	void loadingScene();

	//End Game bool
	void EndGame(bool end);
	bool EndCheck();
	
	size_t levelCompleted;
private:
	//Here be levels
	size_t currentLevel; //Shows current level
	size_t nextLevel; //For adding new level
	size_t offset; //For after loading scene is done
	map <size_t, Scene*> sceneManager; //Store level and level name
	static SceneManager *s_instance; //For singleton scene management
	bool endCheck;
};

#endif
