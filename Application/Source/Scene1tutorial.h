#ifndef SCENE1TUTORIAL_H
#define SCENE1TURORIAL_H

#include "Scene.h"
#include "Camera3.h"
#include "Mesh.h"
#include "MatrixStack.h"
#include "Light.h"
#include "Utility.h"

#include <string>
using std::string;

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
		GEO_QUAD,
		GEO_LIGHTBALL,
		GEO_LEFT,
		GEO_RIGHT,
		GEO_TOP,
		GEO_BOTTOM,
		GEO_FRONT,
		GEO_BACK,
		GEO_BIKE,
		GEO_TEXT,
		GEO_DEBUGBOX,
		GEO_ROBO8,
		GEO_ROCK,
		GEO_GROUND,
		GEO_BOULDER,
		NUM_GEOMETRY,
	};


private:
	unsigned m_vertexArrayID;
	unsigned m_programID;
	unsigned m_parameters[U_TOTAL];

	Mesh* meshList[NUM_GEOMETRY];

	string deltaTime;


	float rotateAngle;
	float translateX[3]; //Original code : float translateX; added [] to make 3 array
	float scaleAll;
	Camera3 camera;
	MS modelStack, viewStack, projectionStack;

	bool lightEnable;
	Light light[1];
	void RenderMesh(Mesh *mesh, bool enableLight);

	void RenderSkybox();

	void RenderText(Mesh* mesh, std::string text, Color color);
	void RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y);
	void RenderMeshOnScreen(Mesh* mesh, int x, int y, int sizex, int sizey);



public:
	Scene1tutorial();
	~Scene1tutorial();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();
};

#endif