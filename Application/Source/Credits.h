#ifndef CREDITS_H
#define CREDITS_H

#include "Scene.h"

#include "Mesh.h"
#include "MatrixStack.h"
#include "Light.h"
#include "Utility.h"

#include <string>
using std::string;

//GameObject Folder
#include "Camera4.h"
#include "Enemy.h"
#include "GameObject.h"
#include "Item.h"
#include "Player.h"
#include "Weapon.h"

class Credits : public Scene
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
		GEO_TEXT,
		GEO_DEBUGBOX, //Old Stuff
		GEO_IMAGE1, //animation
		GEO_IMAGE2,
		//GEO_IMAGE3,
		//GEO_IMAGE4,
		//GEO_IMAGE5,
		//GEO_IMAGE6,
		//GEO_IMAGE7,
		//GEO_IMAGE8,
		//GEO_IMAGE9,
		//GEO_IMAGE10,
		//GEO_IMAGE11, //End of animation
		//GEO_TEST, //Test picture size
		//GEO_HEALTHBG,
		NUM_GEOMETRY,
	};


private:
	unsigned m_vertexArrayID;
	unsigned m_programID;
	unsigned m_parameters[U_TOTAL];
	Mesh* meshList[NUM_GEOMETRY];

	string deltaTime;

	MS modelStack, viewStack, projectionStack;

	bool lightEnable;
	Light light[2];
	void RenderMesh(Mesh *mesh, bool enableLight);

	void RenderText(Mesh* mesh, std::string text, Color color);
	void RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y);
	void RenderMeshOnScreen(Mesh* mesh, float x, float y, float sizex, float sizey, bool isHealth);

	//Call Game Object
	void GenerateGEOMESH();
	void GenerateCredits();

	//Initialize
	void initializeObjects();

	//Render
	void displayOnScreen();
	float translateImg;

	//Player
	Camera4 camera;

public:
	Credits();
	~Credits();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();
};

#endif
