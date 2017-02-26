#ifndef SCENE4_BOSS_H
#define SCENE4_BOSS_H

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
#include "Bullet.h"
#include "Weapon.h"

#undef numOfEnemy
#undef numOfBullets
#undef numOfWalls
#undef numOfGun

#define numOfEnemy 25
#define numOfBullets 6
#define numOfWalls 10
#define numOfGun 6

class Scene4_Boss : public Scene
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
		GEO_QUAD,
		GEO_LIGHTBALL,
		GEO_LEFT,
		GEO_RIGHT,
		GEO_TOP,
		GEO_BOTTOM,
		GEO_FRONT,
		GEO_BACK,
		GEO_FLOOR,
		GEO_TEXT,
		GEO_DEBUGBOX,
		GEO_VENDINGBODY,
		GEO_VENDINGCOVER,
		GEO_ROBOBODY,
		GEO_ROBOARMS,
		GEO_TURRETHEAD,
		GEO_TURRETBODY,
		GEO_TURRETHEAD_2,
		GEO_TURRETBODY_2,
		GEO_CARDKEY,
		GEO_CARDKEY_2,
		GEO_BULLET,
		GEO_WALL,
		GEO_WALL_2,
		GEO_DOOR_FRAME,
		GEO_DOOR,
		GEO_DOOR_2,
		GEO_BLASTER,
		GEO_PBULLET,
		GEO_HEALTHBG,
		GEO_HEALTH,
		GEO_STAMINA,
		GEO_AMMOBG,
		GEO_BOSSTESTBG,
		GEO_BOSSTEST,
		NUM_GEOMETRY,
	};


private:
	unsigned m_vertexArrayID;
	unsigned m_programID;
	unsigned m_parameters[U_TOTAL];
	Mesh* meshList[NUM_GEOMETRY];

	//GameObj
	GameObject* object[NUM_GEOMETRY];
	GameObject* CamObj;
	GameObject* TriggerBox[1];
	Item* Keys[1];
	GameObject* WallsObj[numOfWalls];
	Player* player;

	string deltaTime;
	string BossH;
	string AmmoLeft;
	string MagsLeft;
	string cordx, cordy, cordz;
	Vector3 Robot;

	//Vector3 Bullet;
	Bullet bullet_1;
	Enemy* turret[numOfEnemy];
	Bullet* bullet[numOfBullets];

	//Weapon for Player
	Weapon* lasergun;
	
	float rotateAngle;
	//float translateX[3]; //Original code : float translateX; added [] to make 3 array
	float scaleAll;
	float openCover = 0;
	float DelayTimer = 0;
	void skyBox();
	bool shootBullet = false;

	Fps_Camera camera;
	MS modelStack, viewStack, projectionStack;

	bool lightEnable;
	bool coverOpened = false;
	Light light[6];
	void RenderMesh(Mesh *mesh, bool enableLight);

	void lightingfunc();
	void RenderSkybox();
	void Walls();
	void EnemyField();
	void GUI();

	void RenderText(Mesh* mesh, std::string text, Color color);
	void RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y);
	void RenderMeshOnScreen(Mesh* mesh, int x, int y, int sizex, int sizey, bool isHealth);
	//Game Object
	void GenerateOBJ();

	//Player
	float moveBullet;
	float HPsizeX;
	float test2;
	float test3;
	string clipCount, ammoLeft;
public:
	Scene4_Boss();
	~Scene4_Boss();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();
};

#endif