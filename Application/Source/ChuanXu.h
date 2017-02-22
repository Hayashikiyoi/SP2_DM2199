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

#include <string>
using std::string;
#undef numOfEnemy

#define numOfEnemy 5
#define numOfBullet 5
#define Walls 4
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
		GEO_TEXT,
		GEO_DEBUGBOX,
		GEO_VENDINGBODY,
		GEO_VENDINGCOVER,
		GEO_ROBOBODY,
		GEO_ROBOARMS,
		GEO_TURRETHEAD,
		GEO_TURRETBODY,
		GEO_BULLET,
		GEO_BOSSHEAD,
		GEO_BOSSBODY,
		GEO_CROSSHAIR,
		GEO_WALL,
		GEO_FLOOR,
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
	GameObject* Wall[Walls];
	Enemy* turret[numOfEnemy];
	Bullet* bullet[numOfBullet];
	GameObject * WallsObj[Walls];
	string deltaTime;
	//Vector3 Robot;
	Bullet bullet_1;
	Vector3 CrossHair;
	float rotateAngle ;
	float translateX[3]; //Original code : float translateX; added [] to make 3 array
	float scaleAll;
	float openCover = 0;
	float DelayTimer=0;
	void skyBox();
	void RenderWalls();
	void GenerateObj();

	Fps_Camera camera;
	MS modelStack, viewStack, projectionStack;
	Player* player;

	bool shootBullet = false;
	bool lightEnable;
	bool coverOpened = false;
	Light light[1];

	void RenderMesh(Mesh *mesh, bool enableLight);

	
	void RenderText(Mesh* mesh, std::string text, Color color);
	void RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y);
	void RenderMeshOnScreen(Mesh* mesh, int x, int y, int sizex, int sizey);



public:
	ChuanXu();
	~ChuanXu();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();
};

#endif