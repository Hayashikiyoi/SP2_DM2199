#include "ChuanXu.h"
#include "GL\glew.h"
#include "Mtx44.h"
#include "Application.h"

#include "shader.hpp"

#include "MeshBuilder.h"
#include "LoadTGA.h"

#include "MyMath.h"
using namespace Math;

ChuanXu::ChuanXu()
{
}

ChuanXu::~ChuanXu()
{
}

void ChuanXu::Init()
{	
	HPsizeX = 21.5f;
	delayTime = 0;

	// Init VBO here

	lightEnable = true;


	//Emable depth test
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE); //Deletes the backface
	glEnable(GL_BLEND);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//Initialise camera
	camera.Init(Vector3(90, 30, 0), Vector3(0, -20, -210), Vector3(0, 1, 0));
	player = new Player("camera", Vector3(40, 30, 30));
	lasergun = new Weapon("Blaster", Vector3(-0.12f, -0.750, 2), 100);

	//Set background color to dark blue
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	//Generate a default VAO for now
	glGenVertexArrays(1, &m_vertexArrayID);
	glBindVertexArray(m_vertexArrayID);

	for (int i = 0; i < NUM_GEOMETRY; ++i)
	{
		meshList[i] = NULL;
		object[i] = 0;
	}
	for (int i = 0;i<5; ++i)
	{
		healthPack[i] = NULL;
		healing[i] = true;
	}

	for (int i = 0; i < Walls; ++i)
	{
		WallsObj[i] = NULL;
	}



	meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[GEO_TEXT]->textureID = LoadTGA("Image//Text//gothiclight.tga");

	meshList[GEO_HEALTH] = MeshBuilder::GenerateQuad("HealthBG", Color(1, 1, 1), 1.f, 1.f);
	meshList[GEO_HEALTH]->textureID = LoadTGA("Image//UI//healthBar.tga");

	meshList[GEO_STAMINA] = MeshBuilder::GenerateQuad("stamina", Color(1, 1, 1), 1.f, 1.f);
	meshList[GEO_STAMINA]->textureID = LoadTGA("Image//UI//staminaBar.tga");

	meshList[GEO_HEALTHBG] = MeshBuilder::GenerateQuad("HealthBG", Color(1, 1, 1), 1.f, 1.f);
	meshList[GEO_HEALTHBG]->textureID = LoadTGA("Image//UI//healthBG.tga");

	meshList[GEO_AMMOBG] = MeshBuilder::GenerateQuad("ammoBG", Color(1, 1, 1), 1, 1);
	meshList[GEO_AMMOBG]->textureID = LoadTGA("Image//UI//ammoBG.tga");

	meshList[GEO_DEBUGBOX] = MeshBuilder::GenerateCube("Debug", Color(1, 1, 1), 1.f, 1.f, 1.f);

	GenerateObj();

	meshList[GEO_FLOOR] = MeshBuilder::GenerateQuad("Floor", Color(1, 1, 1), 1.f, 1.f);
	meshList[GEO_FLOOR]->textureID = LoadTGA("Image//floor//floor.tga");

	//Load vertex and fragment shaders
	m_programID = LoadShaders("Shader//Texture.vertexshader", "Shader//Text.fragmentshader");
	m_parameters[U_MVP] = glGetUniformLocation(m_programID, "MVP");
	m_parameters[U_MODELVIEW] = glGetUniformLocation(m_programID, "MV");
	m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE] = glGetUniformLocation(m_programID, "MV_inverse_transpose");
	m_parameters[U_MATERIAL_AMBIENT] = glGetUniformLocation(m_programID, "material.kAmbient");
	m_parameters[U_MATERIAL_DIFFUSE] = glGetUniformLocation(m_programID, "material.kDiffuse");
	m_parameters[U_MATERIAL_SPECULAR] = glGetUniformLocation(m_programID, "material.kSpecular");
	m_parameters[U_MATERIAL_SHININESS] = glGetUniformLocation(m_programID, "material.kShininess");
	m_parameters[U_LIGHTENABLED] = glGetUniformLocation(m_programID, "lightEnabled");

	m_parameters[U_LIGHT0_POSITION] = glGetUniformLocation(m_programID, "lights[0].position_cameraspace");
	m_parameters[U_LIGHT0_COLOR] = glGetUniformLocation(m_programID, "lights[0].color");
	m_parameters[U_LIGHT0_POWER] = glGetUniformLocation(m_programID, "lights[0].power");
	m_parameters[U_LIGHT0_KC] = glGetUniformLocation(m_programID, "lights[0].kC");
	m_parameters[U_LIGHT0_KL] = glGetUniformLocation(m_programID, "lights[0].kL");
	m_parameters[U_LIGHT0_KQ] = glGetUniformLocation(m_programID, "lights[0].kQ");

	m_parameters[U_NUMLIGHTS] = glGetUniformLocation(m_programID, "numLights");
	m_parameters[U_LIGHT0_TYPE] = glGetUniformLocation(m_programID, "lights[0].type");
	m_parameters[U_LIGHT0_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[0].spotDirection");
	m_parameters[U_LIGHT0_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[0].cosCutoff");
	m_parameters[U_LIGHT0_COSINNER] = glGetUniformLocation(m_programID, "lights[0].cosInner");
	m_parameters[U_LIGHT0_EXPONENT] = glGetUniformLocation(m_programID, "lights[0].exponent");

	// Get a handle for our "colorTexture" uniform
	m_parameters[U_COLOR_TEXTURE_ENABLED] = glGetUniformLocation(m_programID, "colorTextureEnabled");
	m_parameters[U_COLOR_TEXTURE] = glGetUniformLocation(m_programID, "colorTexture");

	// Get a handle for our "TextColor" uniform
	m_parameters[U_TEXT_ENABLED] = glGetUniformLocation(m_programID, "textEnabled");
	m_parameters[U_TEXT_COLOR] = glGetUniformLocation(m_programID, "textColor");

	glUseProgram(m_programID);

	lightingfunc();

	Mtx44 projection;
	projection.SetToPerspective(45.0f, 4.0f / 3.0f, 0.1f, 1000.0f);
	projectionStack.LoadMatrix(projection);
}

void ChuanXu::lightingfunc()
{
	light[0].type = Light::LIGHT_DIRECTIONAL;
	light[0].position.Set(0, 1000, 0);
	light[0].color.Set(1, 1, 1);
	light[0].power = 1;
	light[0].kC = 1.f;
	light[0].kL = 0.01f;
	light[0].kQ = 0.001f;
	light[0].cosCutoff = cos(Math::DegreeToRadian(90));
	light[0].cosInner = cos(Math::DegreeToRadian(30));
	light[0].exponent = 3.f;
	light[0].spotDirection.Set(0.f, 1.f, 0.f);

	light[1].type = Light::LIGHT_SPOT;
	light[1].position.Set(0, 50, 0);
	light[1].color.Set(1, 0, 0);
	light[1].power = 10;
	light[1].kC = 1.f;
	light[1].kL = 0.01f;
	light[1].kQ = 0.001f;
	light[1].cosCutoff = cos(Math::DegreeToRadian(90));
	light[1].cosInner = cos(Math::DegreeToRadian(30));
	light[1].exponent = 3.f;
	light[1].spotDirection.Set(0.f, 1.f, 0.f);

	//Make sure you pass uniform parameters after glUseProgram()
	glUniform3fv(m_parameters[U_LIGHT0_COLOR], 1, &light[0].color.r);
	glUniform1f(m_parameters[U_LIGHT0_POWER], light[0].power);
	glUniform1f(m_parameters[U_LIGHT0_KC], light[0].kC);
	glUniform1f(m_parameters[U_LIGHT0_KL], light[0].kL);
	glUniform1f(m_parameters[U_LIGHT0_KQ], light[0].kQ);

	glUniform1i(m_parameters[U_LIGHT0_TYPE], light[0].type);
	glUniform3fv(m_parameters[U_LIGHT0_COLOR], 1, &light[0].color.r);
	glUniform1f(m_parameters[U_LIGHT0_POWER], light[0].power);
	glUniform1f(m_parameters[U_LIGHT0_KC], light[0].kC);
	glUniform1f(m_parameters[U_LIGHT0_KL], light[0].kL);
	glUniform1f(m_parameters[U_LIGHT0_KQ], light[0].kQ);
	glUniform1f(m_parameters[U_LIGHT0_COSCUTOFF], light[0].cosCutoff);
	glUniform1f(m_parameters[U_LIGHT0_COSINNER], light[0].cosInner);
	glUniform1f(m_parameters[U_LIGHT0_EXPONENT], light[0].exponent);

	//Make sure you pass uniform parameters after glUseProgram()
	glUniform3fv(m_parameters[U_LIGHT1_COLOR], 1, &light[1].color.r);
	glUniform1f(m_parameters[U_LIGHT1_POWER], light[1].power);
	glUniform1f(m_parameters[U_LIGHT1_KC], light[1].kC);
	glUniform1f(m_parameters[U_LIGHT1_KL], light[1].kL);
	glUniform1f(m_parameters[U_LIGHT1_KQ], light[1].kQ);

	glUniform1i(m_parameters[U_LIGHT1_TYPE], light[1].type);
	glUniform3fv(m_parameters[U_LIGHT1_COLOR], 1, &light[1].color.r);
	glUniform1f(m_parameters[U_LIGHT1_POWER], light[1].power);
	glUniform1f(m_parameters[U_LIGHT1_KC], light[1].kC);
	glUniform1f(m_parameters[U_LIGHT1_KL], light[1].kL);
	glUniform1f(m_parameters[U_LIGHT1_KQ], light[1].kQ);
	glUniform1f(m_parameters[U_LIGHT1_COSCUTOFF], light[1].cosCutoff);
	glUniform1f(m_parameters[U_LIGHT1_COSINNER], light[1].cosInner);
	glUniform1f(m_parameters[U_LIGHT1_EXPONENT], light[1].exponent);

	glUniform1i(m_parameters[U_NUMLIGHTS], 6);

}

void ChuanXu::GenerateObj()
{
	//Robot
	meshList[GEO_ROBOBODY] = MeshBuilder::GenerateOBJ("RoboBody", "OBJ//NPC//Robot_body.obj");
	meshList[GEO_ROBOBODY]->textureID = LoadTGA("Image//NPC//Robot_Body.tga");

	meshList[GEO_ROBOARMS] = MeshBuilder::GenerateOBJ("RoboArms", "OBJ//NPC//Robot_Arm.obj");
	meshList[GEO_ROBOARMS]->textureID = LoadTGA("Image//NPC//Robot_Arms.tga");


	meshList[GEO_WALL] = MeshBuilder::GenerateOBJ("Wall2", "OBJ//Wall//CX_WALL.obj");
	meshList[GEO_WALL]->textureID = LoadTGA("Image//Wall//MazeWall.tga");
	WallsObj[1] = new GameObject("right wall", Vector3(11, 0, -78));
	WallsObj[1]->setCollider(200
		, 3);
	WallsObj[1]->updateCurPos();

	WallsObj[2] = new GameObject("back wall", Vector3(100, 0, 11));
	WallsObj[2]->setCollider(3, 200);
	WallsObj[2]->updateCurPos();

	WallsObj[3] = new GameObject("left wall", Vector3(12, 0, 100));
	WallsObj[3]->setCollider(200, 3);
	WallsObj[3]->updateCurPos();

	WallsObj[4] = new GameObject("front wall", Vector3(-75, 0, 12));
	WallsObj[4]->setCollider(3, 200);
	WallsObj[4]->updateCurPos();

	WallsObj[6] = new GameObject("maze_wall_1", Vector3(-54.5, 0, 55.5));
	WallsObj[6]->setCollider(4, 52);
	WallsObj[6]->updateCurPos();

	WallsObj[5] = new GameObject("maze_wall_2", Vector3(1, 0, 33.5));
	WallsObj[5]->setCollider(115, 4);
	WallsObj[5]->updateCurPos();

	WallsObj[7] = new GameObject("maze_wall_3", Vector3(-34, 0, 66.6));
	WallsObj[7]->setCollider(4, 26);
	WallsObj[7]->updateCurPos();

	WallsObj[9] = new GameObject("maze_wall_4", Vector3(-11, 0, 88.8));
	WallsObj[9]->setCollider(4, 26);
	WallsObj[9]->updateCurPos();

	WallsObj[8] = new GameObject("maze_wall_5", Vector3(-1, 0, 55.5));
	WallsObj[8]->setCollider(72, 4);
	WallsObj[8]->updateCurPos();

	WallsObj[10] = new GameObject("maze_wall_6", Vector3(10, 0, 77.7));
	WallsObj[10]->setCollider(4, 52);
	WallsObj[10]->updateCurPos();
    
	WallsObj[12] = new GameObject("maze_wall_7", Vector3(44.5, 0, 77.7));
	WallsObj[12]->setCollider(26, 4);
	WallsObj[12]->updateCurPos();

	WallsObj[11] = new GameObject("maze_wall_8", Vector3(55, 0, 88.8));
	WallsObj[11]->setCollider(4, 26);
	WallsObj[11]->updateCurPos();

	WallsObj[13] = new GameObject("maze_wall_9", Vector3(56.5, 0, 44));
	WallsObj[13]->setCollider(4, 26);
	WallsObj[13]->updateCurPos();

	WallsObj[14] = new GameObject("maze_wall_10", Vector3(77, 0, 44.5));
	WallsObj[14]->setCollider(4, 70);
	WallsObj[14]->updateCurPos();

	WallsObj[15] = new GameObject("maze_wall_11", Vector3(77, 0, 11));
	WallsObj[15]->setCollider(47, 4);
	WallsObj[15]->updateCurPos();

	WallsObj[16] = new GameObject("maze_wall_12", Vector3(33, 0, 11));
	WallsObj[16]->setCollider(4, 47);
	WallsObj[16]->updateCurPos();

	WallsObj[17] = new GameObject("maze_wall_13", Vector3(-11, 0, 0));
	WallsObj[17]->setCollider(4, 68);
	WallsObj[17]->updateCurPos();

	WallsObj[18] = new GameObject("maze_wall_14", Vector3(0, 0, 11));
	WallsObj[18]->setCollider(26, 4);
	WallsObj[18]->updateCurPos();

	WallsObj[19] = new GameObject("maze_wall_15", Vector3(22.5, 0, -11));
	WallsObj[19]->setCollider(26, 4);
	WallsObj[19]->updateCurPos();

	WallsObj[20] = new GameObject("maze_wall_16", Vector3(77, 0, -11));
	WallsObj[20]->setCollider(47, 4);
	WallsObj[20]->updateCurPos();

	WallsObj[21] = new GameObject("maze_wall_17", Vector3(77.5, 0, -43.5));
	WallsObj[21]->setCollider(4, 26);
	WallsObj[21]->updateCurPos();

	WallsObj[22] = new GameObject("maze_wall_18", Vector3(55, 0, -67));
	WallsObj[22]->setCollider(4, 26);
	WallsObj[22]->updateCurPos();

	WallsObj[23] = new GameObject("maze_wall_19", Vector3(55, 0, -33));
	WallsObj[23]->setCollider(47, 4);
	WallsObj[23]->updateCurPos();

	WallsObj[24] = new GameObject("maze_wall_20", Vector3(33.25, 0, -55.5));
	WallsObj[24]->setCollider(4, 47);
	WallsObj[24]->updateCurPos();

	WallsObj[25] = new GameObject("maze_wall_21", Vector3(0.5, 0, -54.5));
	WallsObj[25]->setCollider(26, 4);
	WallsObj[25]->updateCurPos();

	WallsObj[26] = new GameObject("maze_wall_22", Vector3(-44, 0, -55.5));
	WallsObj[26]->setCollider(25, 4);
	WallsObj[26]->updateCurPos();

	WallsObj[27] = new GameObject("maze_wall_23", Vector3(-55, 0, -22.2));
	WallsObj[27]->setCollider(4, 26);
	WallsObj[27]->updateCurPos();

	WallsObj[28] = new GameObject("24", Vector3(-65.5, 0, -33.3));
	WallsObj[28]->setCollider(26, 4);
	WallsObj[28]->updateCurPos();

	WallsObj[29] = new GameObject("maze_wall_25", Vector3(-33.5, 0, -22.2));
	WallsObj[29]->setCollider(4, 74);
	WallsObj[29]->updateCurPos();

	WallsObj[30] = new GameObject("maze_wall_26", Vector3(-55.25, 0, 11.1));
	WallsObj[30]->setCollider(47, 4);
	WallsObj[30]->updateCurPos();

	WallsObj[31] = new GameObject("maze_wall_27", Vector3(11.5, 0, -32.75));
	WallsObj[31]->setCollider(4, 48);
	WallsObj[31]->updateCurPos();

	//Health packs
	meshList[GEO_RECOVERY] = MeshBuilder::GenerateOBJ("Recovery", "OBJ//Player//Can.obj");
	//meshList[GEO_RECOVERY]->textureID = LoadTGA("Image//Player//.tga");

	healthPack[1] = new Item("Health_Pack_1", Vector3(88, 0, 17),"Heal player");
	healthPack[1]->setCollider(5, 5);
	healthPack[1]->updateCurPos();

	healthPack[2] = new Item("Health_Pack_2", Vector3(40, 0, 88), "Heal player");
	healthPack[2]->setCollider(5, 5);
	healthPack[2]->updateCurPos();

	healthPack[3] = new Item("Health_Pack_3", Vector3(0, 0, 85), "Heal player");
	healthPack[3]->setCollider(5, 5);
	healthPack[3]->updateCurPos();

	healthPack[4] = new Item("Health_Pack_4", Vector3(45, 0, -60), "Heal player");
	healthPack[4]->setCollider(5, 5);
	healthPack[4]->updateCurPos();

	healthPack[5] = new Item("Health_Pack_5", Vector3(-65, 0, -15), "Heal player");
	healthPack[5]->setCollider(5, 5);
	healthPack[5]->updateCurPos();


}

void ChuanXu::RenderMaze() // Maze walls and healthpacks
{
	// Maze walls
	modelStack.PushMatrix();
	modelStack.Translate(WallsObj[5]->Position.x, WallsObj[5]->Position.y, WallsObj[5]->Position.z);
	modelStack.Scale(7.5, 1.5, 1.5);
	RenderMesh(meshList[GEO_WALL], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(WallsObj[6]->Position.x, WallsObj[6]->Position.y, WallsObj[6]->Position.z);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Scale(3, 1.5, 1.5);
	RenderMesh(meshList[GEO_WALL], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(WallsObj[7]->Position.x, WallsObj[7]->Position.y, WallsObj[7]->Position.z);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Scale(1.5, 1.5, 1.5);
	RenderMesh(meshList[GEO_WALL], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(WallsObj[8]->Position.x, WallsObj[8]->Position.y, WallsObj[8]->Position.z);
	modelStack.Scale(4.5, 1.5, 1.5);
	RenderMesh(meshList[GEO_WALL], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(WallsObj[9]->Position.x, WallsObj[9]->Position.y, WallsObj[9]->Position.z);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Scale(1.5, 1.5, 1.5);
	RenderMesh(meshList[GEO_WALL], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(WallsObj[10]->Position.x, WallsObj[10]->Position.y, WallsObj[10]->Position.z);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Scale(3, 1.5, 1.5);
	RenderMesh(meshList[GEO_WALL], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(WallsObj[11]->Position.x, WallsObj[11]->Position.y, WallsObj[11]->Position.z);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Scale(1.5, 1.5, 1.5);
	RenderMesh(meshList[GEO_WALL], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(WallsObj[12]->Position.x, WallsObj[12]->Position.y, WallsObj[12]->Position.z);
	modelStack.Scale(1.5, 1.5, 1.5);
	RenderMesh(meshList[GEO_WALL], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(WallsObj[13]->Position.x, WallsObj[13]->Position.y, WallsObj[13]->Position.z);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Scale(1.5, 1.5, 1.5);
	RenderMesh(meshList[GEO_WALL], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(WallsObj[14]->Position.x, WallsObj[14]->Position.y, WallsObj[14]->Position.z);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Scale(4.5, 1.5, 1.5);
	RenderMesh(meshList[GEO_WALL], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(WallsObj[15]->Position.x, WallsObj[15]->Position.y, WallsObj[15]->Position.z);
	modelStack.Scale(3, 1.5, 1.5);
	RenderMesh(meshList[GEO_WALL], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(WallsObj[16]->Position.x, WallsObj[16]->Position.y, WallsObj[16]->Position.z);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Scale(3, 1.5, 1.5);
	RenderMesh(meshList[GEO_WALL], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(WallsObj[17]->Position.x, WallsObj[17]->Position.y, WallsObj[17]->Position.z);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Scale(4.5, 1.5, 1.5);
	RenderMesh(meshList[GEO_WALL], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(WallsObj[18]->Position.x, WallsObj[18]->Position.y, WallsObj[18]->Position.z);
	modelStack.Scale(1.5, 1.5, 1.5);
	RenderMesh(meshList[GEO_WALL], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(WallsObj[19]->Position.x, WallsObj[19]->Position.y, WallsObj[19]->Position.z);
	modelStack.Scale(1.5, 1.5, 1.5);
	RenderMesh(meshList[GEO_WALL], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(WallsObj[20]->Position.x, WallsObj[20]->Position.y, WallsObj[20]->Position.z);
	modelStack.Scale(3, 1.5, 1.5);
	RenderMesh(meshList[GEO_WALL], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(WallsObj[21]->Position.x, WallsObj[21]->Position.y, WallsObj[21]->Position.z);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Scale(1.5, 1.5, 1.5);
	RenderMesh(meshList[GEO_WALL], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(WallsObj[22]->Position.x, WallsObj[22]->Position.y, WallsObj[22]->Position.z);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Scale(1.5, 1.5, 1.5);
	RenderMesh(meshList[GEO_WALL], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(WallsObj[23]->Position.x, WallsObj[23]->Position.y, WallsObj[23]->Position.z);
	modelStack.Scale(3, 1.5, 1.5);
	RenderMesh(meshList[GEO_WALL], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(WallsObj[24]->Position.x, WallsObj[24]->Position.y, WallsObj[24]->Position.z);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Scale(3, 1.5, 1.5);
	RenderMesh(meshList[GEO_WALL], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(WallsObj[25]->Position.x, WallsObj[25]->Position.y, WallsObj[25]->Position.z);
	modelStack.Scale(1.5, 1.5, 1.5);
	RenderMesh(meshList[GEO_WALL], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(WallsObj[26]->Position.x, WallsObj[26]->Position.y, WallsObj[26]->Position.z);
	modelStack.Scale(1.5, 1.5, 1.5);
	RenderMesh(meshList[GEO_WALL], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(WallsObj[27]->Position.x, WallsObj[27]->Position.y, WallsObj[27]->Position.z);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Scale(1.5, 1.5, 1.5);
	RenderMesh(meshList[GEO_WALL], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(WallsObj[28]->Position.x, WallsObj[28]->Position.y, WallsObj[28]->Position.z);
	modelStack.Scale(1.5, 1.5, 1.5);
	RenderMesh(meshList[GEO_WALL], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(WallsObj[29]->Position.x, WallsObj[29]->Position.y, WallsObj[29]->Position.z);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Scale(4.5, 1.5, 1.5);
	RenderMesh(meshList[GEO_WALL], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(WallsObj[30]->Position.x, WallsObj[30]->Position.y, WallsObj[30]->Position.z);
	modelStack.Scale(3, 1.5, 1.5);
	RenderMesh(meshList[GEO_WALL], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(WallsObj[31]->Position.x, WallsObj[31]->Position.y, WallsObj[31]->Position.z);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Scale(3, 1.5, 1.5);
	RenderMesh(meshList[GEO_WALL], true);
	modelStack.PopMatrix();

	//health pack
	if (healing[1])
	{
		modelStack.PushMatrix();
		modelStack.Translate(healthPack[1]->Position.x, 0, healthPack[1]->Position.z);
		modelStack.Scale(0.5, 0.5, 0.5);
		modelStack.Rotate(rotateAngle, 0, 1, 0);
		RenderMesh(meshList[GEO_RECOVERY], false);
		modelStack.PopMatrix();
	}

	if (healing[2])
	{
		modelStack.PushMatrix();
		modelStack.Translate(healthPack[2]->Position.x, 0, healthPack[2]->Position.z);
		modelStack.Scale(0.5, 0.5, 0.5);
		modelStack.Rotate(rotateAngle, 0, 1, 0);
		RenderMesh(meshList[GEO_RECOVERY], false);
		modelStack.PopMatrix();
	}

	if (healing[3])
	{
		modelStack.PushMatrix();
		modelStack.Translate(healthPack[3]->Position.x, 0, healthPack[3]->Position.z);
		modelStack.Scale(0.5, 0.5, 0.5);
		modelStack.Rotate(rotateAngle, 0, 1, 0);
		RenderMesh(meshList[GEO_RECOVERY], false);
		modelStack.PopMatrix();
	}

	if (healing[4])
	{
		modelStack.PushMatrix();
		modelStack.Translate(healthPack[4]->Position.x, 0, healthPack[4]->Position.z);
		modelStack.Scale(0.5, 0.5, 0.5);
		modelStack.Rotate(rotateAngle, 0, 1, 0);
		RenderMesh(meshList[GEO_RECOVERY], false);
		modelStack.PopMatrix();
	}

	if (healing[5])
	{
		modelStack.PushMatrix();
		modelStack.Translate(healthPack[5]->Position.x, 0, healthPack[5]->Position.z);
		modelStack.Scale(0.5, 0.5, 0.5);
		modelStack.Rotate(rotateAngle, 0, 1, 0);
		RenderMesh(meshList[GEO_RECOVERY], false);
		modelStack.PopMatrix();
	}

}

void ChuanXu::Update(double dt)
{
	static float translateLimit = 1;
	static Vector3 prevpos;
	static Vector3 prevposTarget;
	
	deltaTime = /*"Health" + std::to_string(player->getHealth()); */"FPS:" + std::to_string(1 / dt);
	HPsizeX = player->getHealth()*0.215;
	delayTime += (float)dt;
	rotateAngle += (float)(10 * dt);

	if (delayTime > 5)
	{
		player->DmgPlayer(5);
		delayTime = 0;
	}
	else if (delayTime < 5)
	{
		delayTime += (float)dt;
	}

	static float LSPEED = 10;

	if (Application::IsKeyPressed('V'))
		lightEnable = false;
	if (Application::IsKeyPressed('B'))
		lightEnable = true;

	if (Application::IsKeyPressed('Y'))
		coverOpened= true;

	if (Application::IsKeyPressed('0')) 
	{
		light[0].type = Light::LIGHT_POINT;
		glUniform1i(m_parameters[U_LIGHT0_TYPE], light[0].type);
	}
	if (Application::IsKeyPressed('9'))
	{
		light[0].type = Light::LIGHT_DIRECTIONAL;
		glUniform1i(m_parameters[U_LIGHT0_TYPE], light[0].type);
	}
	if (Application::IsKeyPressed('8'))
	{
		light[0].type = Light::LIGHT_SPOT;
		glUniform1i(m_parameters[U_LIGHT0_TYPE], light[0].type);
	}

	//Cull back face
	if (Application::IsKeyPressed('1'))
		glEnable(GL_CULL_FACE);
	if (Application::IsKeyPressed('2'))
		glDisable(GL_CULL_FACE);

	//Wireframe / not wireframe
	if (Application::IsKeyPressed('3'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	if (Application::IsKeyPressed('4'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


	player->Position = camera.position;
	player->updateCurPos();

	for (int i = 0; i < NUM_GEOMETRY; ++i)
	{
		if (object[i] && player->trigger(object[i]))
		{
			camera.position = prevpos;
			camera.target = prevposTarget;
			break;
		} 
	}

	for (int i = 0; i <5; ++i)
	{
		if (healthPack[i] && player->trigger(healthPack[i])&&healing[i])
		{
			player->HealPlayer(50);
			healing[i] = false;
			break;
		}
	}

	for (int i = 0; i < Walls; ++i)
	{
		if (WallsObj[i] && player->trigger(WallsObj[i]))
		{
			player->DmgPlayer(1);
			camera.position = prevpos;
			camera.target = prevposTarget;
			break;
		}
		else if (i == (Walls - 1))
		{
			prevpos = camera.position;
			prevposTarget = camera.target;
		}
	}

	AmmoLeft = std::to_string(lasergun->bulletLeft()) + "/45";
	clipCount = std::to_string(lasergun->canisterLeft());
	camera.Update(dt);

	
}

void ChuanXu::RenderWalls()
{	
	modelStack.PushMatrix();
	modelStack.Translate(WallsObj[1]->Position.x,WallsObj[1]->Position.y, WallsObj[1]->Position.z);
	modelStack.Scale(12, 1.5, 1.5);
	RenderMesh(meshList[GEO_WALL], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(WallsObj[2]->Position.x, WallsObj[2]->Position.y, WallsObj[2]->Position.z);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Scale(12, 1.5, 1.5);
	RenderMesh(meshList[GEO_WALL], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(WallsObj[3]->Position.x, WallsObj[3]->Position.y, WallsObj[3]->Position.z);
	modelStack.Scale(12, 1.5, 1.5);
	RenderMesh(meshList[GEO_WALL], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(WallsObj[4]->Position.x, WallsObj[4]->Position.y, WallsObj[4]->Position.z);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Scale(12, 1.5, 1.5);
	RenderMesh(meshList[GEO_WALL], true);
	modelStack.PopMatrix();

	//floor
	modelStack.PushMatrix();
	modelStack.Translate(12, 0, 11);
	modelStack.Rotate(90, 0, -1, 0);
	modelStack.Rotate(90, -1, 0, 0);
	modelStack.Scale(180, 180.f, 180.f);
	RenderMesh(meshList[GEO_FLOOR], false);
	modelStack.PopMatrix();
}

void ChuanXu::Render()
{
	if (light[0].type == Light::LIGHT_DIRECTIONAL)
	{
		Vector3 lightDir(light[0].position.x, light[0].position.y, light[0].position.z);
		Vector3 lightDirection_cameraspace = viewStack.Top() * lightDir;
		glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightDirection_cameraspace.x);
	}
	
	if (light[1].type == Light::LIGHT_SPOT)
	{
		Position lightPosition_cameraspace = viewStack.Top() * light[1].position;
		glUniform3fv(m_parameters[U_LIGHT1_POSITION], 1, &lightPosition_cameraspace.x);
		Vector3 spotDirection_cameraspace = viewStack.Top() * light[1].spotDirection;
		glUniform3fv(m_parameters[U_LIGHT1_SPOTDIRECTION], 1, &spotDirection_cameraspace.x);
		glUniform1f(m_parameters[U_LIGHT1_POWER], light[1].power);

	}
	//Initialize
	Mtx44 MVP;

	//Clear color buffer every frame
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Setup the view for camera
	viewStack.LoadIdentity();
	viewStack.LookAt(camera.position.x, camera.position.y, camera.position.z,
		camera.target.x, camera.target.y, camera.target.z,
		camera.up.x, camera.up.y, camera.up.z);
	modelStack.LoadIdentity();

	//------------------------------------------------------------------------------------
	 RenderWalls();
	 RenderMaze();

	//-------------------------------------------------------------------------------------

	 RenderMeshOnScreen(meshList[GEO_HEALTHBG], 15, 5, 30, 30, false);
	 RenderMeshOnScreen(meshList[GEO_HEALTH], 8.f, 5, HPsizeX, 30, true);
	 RenderMeshOnScreen(meshList[GEO_STAMINA], 8.1f, 5, camera.test2, 30, true);
	 RenderMeshOnScreen(meshList[GEO_AMMOBG], 65, 10, 30, 30, false);

	 RenderTextOnScreen(meshList[GEO_TEXT], clipCount, (0, 1, 0), 5, 11.7, 1.1);
	 RenderTextOnScreen(meshList[GEO_TEXT], AmmoLeft, (0, 1, 0), 3, 21.7, 0.2f);

	RenderTextOnScreen(meshList[GEO_TEXT], deltaTime, (0, 1, 0), 4, 5, 5);

	 RenderTextOnScreen(meshList[GEO_TEXT], std::to_string(player->Position.x), Color(0, 1, 0), 4, 3, 4);
	 RenderTextOnScreen(meshList[GEO_TEXT], std::to_string(player->Position.z), Color(0, 1, 0), 4, 3, 3);
}

void ChuanXu::RenderMesh(Mesh *mesh, bool enableLight)
{
	Mtx44 MVP, modelView, modelView_inverse_transpose;

	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	modelView = viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MODELVIEW], 1, GL_FALSE, &modelView.a[0]);
	if (enableLight && lightEnable)
	{
		glUniform1i(m_parameters[U_LIGHTENABLED], 1);
		modelView_inverse_transpose = modelView.GetInverse().GetTranspose();
		glUniformMatrix4fv(m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE], 1, GL_FALSE, &modelView_inverse_transpose.a[0]);

		//load material
 		glUniform3fv(m_parameters[U_MATERIAL_AMBIENT], 1, &mesh->material.kAmbient.r);
		glUniform3fv(m_parameters[U_MATERIAL_DIFFUSE], 1, &mesh->material.kDiffuse.r);
		glUniform3fv(m_parameters[U_MATERIAL_SPECULAR], 1, &mesh->material.kSpecular.r);
		glUniform1f(m_parameters[U_MATERIAL_SHININESS], mesh->material.kShininess);
	}
	else
	{
		glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	}

	if (mesh->textureID > 0)
	{
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mesh->textureID);
		glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	}
	else
	{
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 0);
	}
	mesh->Render(); //this line should only be called once
	if (mesh->textureID > 0)
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

void ChuanXu::RenderText(Mesh* mesh, std::string text, Color color)
{
	if (!mesh || mesh->textureID <= 0) //Proper error check
		return;

	glDisable(GL_DEPTH_TEST);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
	glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID);
	glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	for (unsigned i = 0; i < text.length(); ++i)
	{
		Mtx44 characterSpacing;
		characterSpacing.SetToTranslation(i * 1.0f, 0, 0); //1.0f is the spacing of each character, you may change this value
		Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

		mesh->Render((unsigned)text[i] * 6, 6);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
	glEnable(GL_DEPTH_TEST);
}

void ChuanXu::RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y)
{
	if (!mesh || mesh->textureID <= 0) //Proper error check
		return;

	glDisable(GL_DEPTH_TEST);
	Mtx44 ortho;
	ortho.SetToOrtho(0, 80, 0, 60, -10, 10); //size of screen UI
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity(); //No need camera for ortho mode
	modelStack.PushMatrix();
	modelStack.LoadIdentity(); //Reset modelStack
	modelStack.Scale(size, size, size);
	modelStack.Translate(x, y, 0);


	glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
	glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID);
	glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	for (unsigned i = 0; i < text.length(); ++i)
	{
		Mtx44 characterSpacing;
		characterSpacing.SetToTranslation(i + 1.f * .7f, 1, 0); //1.0f is the spacing of each character, you may change this value
		Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

		mesh->Render((unsigned)text[i] * 6, 6);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);

	//Add these code just before glEnable(GL_DEPTH_TEST);
	projectionStack.PopMatrix();
	viewStack.PopMatrix();
	modelStack.PopMatrix();

	glEnable(GL_DEPTH_TEST);
}

void ChuanXu::RenderMeshOnScreen(Mesh* mesh, int x, int y, int sizex, int sizey, bool isHealth)
{
	glDisable(GL_DEPTH_TEST);
	Mtx44 ortho;
	ortho.SetToOrtho(0, 80, 0, 60, -10, 10); //size of screen UI
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity(); //No need camera for ortho mode
	if (isHealth)
	{
		modelStack.PushMatrix();
		modelStack.LoadIdentity();
		//to do: scale and translate accordingly
		modelStack.Translate(x, y, 0);
		//modelStack.Translate(sizex * 0.5f, 0, 0);
		modelStack.Scale(sizex, sizey, 1);
		modelStack.Translate(0.5f, 0, 0); //Size of quad pls set to 1
		RenderMesh(mesh, false); //UI should not have light
		projectionStack.PopMatrix();
		viewStack.PopMatrix();
		modelStack.PopMatrix();
	}
	else
	{
		modelStack.PushMatrix();
		modelStack.LoadIdentity();
		//to do: scale and translate accordingly
		modelStack.Translate(x, y, 0);
		modelStack.Scale(sizex, sizey, 1);
		RenderMesh(mesh, false); //UI should not have light
		projectionStack.PopMatrix();
		viewStack.PopMatrix();
		modelStack.PopMatrix();
	}
	glEnable(GL_DEPTH_TEST);

}


void ChuanXu::Exit()
{
	for (int i = 0; i < NUM_GEOMETRY; ++i)
	{
		if (meshList[i] != NULL)
			delete meshList[i];
	}
	for (int i = 0; i < Walls; ++i)
	{
		if (WallsObj[i] != NULL)
			delete WallsObj[i];
	}
	for (int i = 0; i < 5; ++i)
	{
		if (healthPack[i] != NULL)
			delete healthPack[i];
	}
	delete player;
	delete lasergun;

	// Cleanup VBO here
	glDeleteVertexArrays(1, &m_vertexArrayID);

	//Cleanup color
	glDeleteProgram(m_programID);
}
