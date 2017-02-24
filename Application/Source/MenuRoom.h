#ifndef MENU_ROOM_H
#define MENU_ROOM_H

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
#define numOfEnemy 20

class Menu_Room : public Scene
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

		//Lighting 2
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
		GEO_FLOOR_2,
		GEO_TEXT,
		GEO_DEBUGBOX,
		GEO_VENDINGBODY,
		GEO_VENDINGCOVER,
		GEO_ROBOBODY,
		GEO_ROBOARMS,
		GEO_ROBOLOCKED,
		GEO_ROBOARMSLOCKED,
		GEO_BULLET,
		GEO_VOTEX,
		GEO_BOTTOMVOTEX,
		GEO_COMPUTER,
		GEO_WALL,
		GEO_WALL_2,
		GEO_DOOR_FRAME,
		GEO_DOOR,
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

	string deltaTime;
	string cordx, cordy, cordz;

	MS modelStack, viewStack, projectionStack;

	bool lightEnable;
	bool coverOpened = false;
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
	GameObject* TriggerBox[7];
	Item* Locked[4];

	//Initialize
	void initializeObjects();

	//Render
	void RenderSkybox();
	void Walls();
	void EnemyField();

	//Player
	Fps_Camera camera;
	Player* player;
	Weapon* lasergun;
	float HPsizeX;
	string clipCount, ammoLeft;
	//Enemy
	Enemy* turret[numOfEnemy]; //NPC Need rename

	

public:
	Menu_Room();
	~Menu_Room();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();
};

#endif
