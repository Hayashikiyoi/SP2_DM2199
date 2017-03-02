#ifndef SCENE1TUTORIAL_H
#define SCENE1TUTORIAL_H

#include "Scene.h"
#include "Fps_Camera.h"
#include "Mesh.h"
#include "MatrixStack.h"
#include "Light.h"
#include "Utility.h"

#include <string>
using std::string;

//GameObject Folder
#include "Enemy.h"
#include "GameObject.h"
#include "Item.h"
#include "Player.h"
#include "Weapon.h"
#undef numOfEnemy
#define numOfEnemy 5
#define numOfBullets 5
#define numOfRocks 55

class Scene1tutorial : public Scene
{

	enum UNIFORM_TYPE
	{
		U_MVP = 0,
		U_MODELVIEW,
		U_MODELVIEW_INVERSE_TRANSPOSE,
		U_MATERIAL_AMBIENT,
		U_MATERIAL_DIFFUSE,
		U_MATERIAL_SPECULAR,
		U_MATERIAL_SHININESS,
		U_LIGHTENABLED,

		U_LIGHT0_POSITION,
		U_LIGHT0_COLOR,
		U_LIGHT0_POWER,
		U_LIGHT0_KC,
		U_LIGHT0_KL,
		U_LIGHT0_KQ,
		U_LIGHT0_TYPE,
		U_LIGHT0_SPOTDIRECTION,
		U_LIGHT0_COSCUTOFF,
		U_LIGHT0_COSINNER,
		U_LIGHT0_EXPONENT,

		U_COLOR_TEXTURE_ENABLED,
		U_COLOR_TEXTURE,

		U_TEXT_ENABLED,
		U_TEXT_COLOR,

		U_NUMLIGHTS,
		U_TOTAL,
	};

	enum GEOMETRY_TYPE
	{
		GEO_AXES,
		GEO_LEFT,
		GEO_RIGHT,
		GEO_TOP,
		GEO_BOTTOM,
		GEO_FRONT,
		GEO_BACK,
		GEO_TEXT,
		GEO_DEBUGBOX,
		GEO_BLASTER,
		GEO_PBULLET,
		GEO_HEALTHBG,
		GEO_HEALTH,
		GEO_STAMINA,
		GEO_AMMOBG,
		GEO_WALLROW, //A Row Of Walls
		GEO_QUADFLOOR, //Floor texture on quad
		GEO_QUADCEILING, //Ceiling texture on quad
		GEO_GROUND,
		GEO_ROCK,
		GEO_BUTTON,	//Button to go to next stage (Pressure plate)
		//GEO_SERVERPROP,//Servers around the room
		GEO_TUTORIAL4, //Tutorial 1 -animation
		GEO_TUTORIAL5,
		GEO_TUTORIAL6,
		GEO_TUTORIAL7,
		GEO_TUTORIAL8,//Tutorial 3 -end animation
		GEO_RECOVERY,
		GEO_TURRETHEAD_2,
		GEO_TURRETBODY_2,
		GEO_BULLET,
		NUM_GEOMETRY,
	};


private:
	unsigned m_vertexArrayID;
	unsigned m_programID;
	unsigned m_parameters[U_TOTAL];
	Mesh* meshList[NUM_GEOMETRY];

	MS modelStack, viewStack, projectionStack;

	bool lightEnable;
	Light light[2];
	void RenderMesh(Mesh *mesh, bool enableLight);

	void RenderText(Mesh* mesh, std::string text, Color color);
	void RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y);
	void RenderMeshOnScreen(Mesh* mesh, float x, float y, float sizex, float sizey, bool isHealth);

	//Call Game Object
	void GenerateOBJ();
	void GenerateGEOMESH();
	void GenerateSkybox();

	//GameObj
	GameObject* object[NUM_GEOMETRY];
	GameObject* walls[5]; //Enclosed room
	GameObject* Rock[numOfRocks];

	//Initialize
	void initializeObjects();

	//Render
	void RenderSkybox();
	void RenderWalls();
	void RenderFloor();
	void RenderGameItems();
	void RenderRocks();

	//Vector3 Bullet;
	Bullet bullet_1;
	Enemy* turret[numOfEnemy];
	Bullet* bullet[numOfBullets];

	//Player
	Fps_Camera camera;
	Player* player;
	Weapon* lasergun;
	float HPsizeX;
	string clipCount, ammoLeft;
	/*string tempPlayerposX, tempPlayerposZ;*/

	//Trigger box (Instructions)(To next level)
	GameObject* triggerbox[8];
	float Maware;
	float DelayTimer = 0;

	//Tutorial
	bool tutorial;
	int tutorialNum;

public:
	Scene1tutorial();
	~Scene1tutorial();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();
};
#endif