
#ifndef APPLICATION_H
#define APPLICATION_H

#include "timer.h"

//Scene Manager
#include "SceneManager.h"

class Application
{
public:
	Application();
	~Application();
	void Init();
	void Run();
	void Exit();
	static bool IsKeyPressed(unsigned short key);
	static void changeScene(int sceneNum);

private:
	//Declare a window object
	StopWatch m_timer;
};

#endif