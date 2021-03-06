#ifndef CHUAN_XU_H
#define CHUAN_XU_H

#include "Scene.h"
#include "Camera3.h"
#include "Mesh.h"
#include "MatrixStack.h"
#include "Light.h"
#include "Utility.h"
#include "Enemy.h"
#include "Bullet.h"
#include"Fps_Camera.h"
#include"Player.h"
#include"Weapon.h"
#include"Item.h"

#include <string>
using std::string;

#define Walls 31
#define numOfIdleNPC 5
#define numOfEnemy 2

class ChuanXu : public Scene 
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

		U_LIGHT1_POSITION,
		U_LIGHT1_COLOR,
		U_LIGHT1_POWER,
		U_LIGHT1_KC,
		U_LIGHT1_KL,
		U_LIGHT1_KQ,
		U_LIGHT1_TYPE,
		U_LIGHT1_SPOTDIRECTION,
		U_LIGHT1_COSCUTOFF,
		U_LIGHT1_COSINNER,
		U_LIGHT1_EXPONENT,


		U_TEXT_ENABLED,
		U_TEXT_COLOR,

		U_NUMLIGHTS,
		U_TOTAL,
	};

	enum GEOMETRY_TYPE
	{
		GEO_TEXT,
		GEO_DEBUGBOX,
		GEO_ROBOBODY,
		GEO_ROBOARMS,
		GEO_WALL,
		GEO_FLOOR,
		GEO_HEALTH,
		GEO_STAMINA,
		GEO_AMMOBG,
		GEO_RECOVERY,
		GEO_HEALTHBG,
		GEO_BLASTER,
		GEO_PBULLET,
		GEO_END,
		GEO_SIMPLEENEMY,
		GEO_EXPLANATIONS,
		NUM_GEOMETRY,
	};


private:
	unsigned m_vertexArrayID;
	unsigned m_programID;
	unsigned m_parameters[U_TOTAL];
	Mesh* meshList[NUM_GEOMETRY];

	//GameObj
	GameObject* object[NUM_GEOMETRY];
	GameObject * WallsObj[Walls];
	GameObject* TriggerBox[1];
	Enemy* SimpleEnemy[numOfEnemy];
	float rotateAngle = 0;
	float DelayTimer=0;
	void RenderWalls();
	void GenerateObj();
	void RenderMaze();
	void lightingfunc();

	string AmmoLeft;
	string MagsLeft;
	Fps_Camera camera;
	MS modelStack, viewStack, projectionStack;
	Player* player;
	Weapon* lasergun;
	string clipCount, ammoLeft;
	Item* healthPack[5];
	float HPsizeX;
	float delayTime;

	bool lightEnable;
	bool startLevel;
	bool healing[5];
	bool coverOpened = false;
	Light light[6];

	void RenderMesh(Mesh *mesh, bool enableLight);
	void RenderText(Mesh* mesh, std::string text, Color color);
	void RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y);
	void RenderMeshOnScreen(Mesh* mesh, int x, int y, int sizex, int sizey,bool isHealth);



public:
	ChuanXu();
	~ChuanXu();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();
};

#endif