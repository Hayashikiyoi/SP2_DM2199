#ifndef ALOY_SCENE_H
#define ALOY_SCENE_H

#include "Scene.h"
#include "Camera4.h"
#include "Fps_Camera.h"
#include "Mesh.h"
#include "MatrixStack.h"
#include "Light.h"
#include "Utility.h"

#include <string>
using std::string;

class Aloy_Scene : public Scene
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
		GEO_TITLE,
		GEO_SELECT1,
		GEO_SELECT2,
		GEO_SELECT3,
		GEO_FRONT,
		GEO_CINAMATIC,
		GEO_TEXT,
		NUM_GEOMETRY,
	};


private:
	float transX;
	unsigned m_vertexArrayID;
	unsigned m_programID;
	unsigned m_parameters[U_TOTAL];

	Mesh* meshList[NUM_GEOMETRY];

	string deltaTime;

	float TextSize;//resizing the image when checking left or right
	float TextSize_2;//resizing text for scene for startgame
	float TextSize_3; //resizing text for scene for exit game
	float TextSize_4; //resizing text for scene for Credit
	bool TextChecking;//if they press Enter already
	bool TextSwitching;//Checking the left and right button
	bool TitleCheck;
	bool bSomethingHappen;
	int MenuSelect;
	float Delaytimer[3];

	float rotateAngle;
	float translateX[3]; //Original code : float translateX; added [] to make 3 array
	float scaleAll;
	float titlescale;
	float Translating[2];
	Camera4 camera;
	MS modelStack, viewStack, projectionStack;

	bool lightEnable;
	Light light[1];
	void RenderMesh(Mesh *mesh, bool enableLight);

	void RenderSkybox();

	void RenderText(Mesh* mesh, std::string text, Color color);
	void RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y);
	void RenderMeshOnScreen(Mesh* mesh, int x, int y, int sizex, int sizey);
public:
	Aloy_Scene();
	~Aloy_Scene();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();
};

#endif