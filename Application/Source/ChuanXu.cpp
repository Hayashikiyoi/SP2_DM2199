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

	// Init VBO here
	rotateAngle = 0;
	translateX[0] = 0;
	translateX[1] = 0;
	translateX[2] = 0;
	scaleAll = 1;
	lightEnable = true;


	//Emable depth test
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE); //Deletes the backface
	glEnable(GL_BLEND);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//Initialise camera
	camera.Init(Vector3(40, 30, 30), Vector3(0, 0, -10), Vector3(0, 1, 0));
	player = new Player("camera", Vector3(40, 30, 30));

	//Set background color to dark blue
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	//Generate a default VAO for now
	glGenVertexArrays(1, &m_vertexArrayID);
	glBindVertexArray(m_vertexArrayID);

	for (int i = 0; i < NUM_GEOMETRY; ++i)
	{
		meshList[i] = NULL;
		object[i] = 0;
	}
	CamObj = new GameObject("camera", Vector3(0, 0, 0));
	for (int i = 0; i < numOfEnemy; ++i)
	{
		turret[i] = NULL;
	}
	for (int i = 0; i < numOfBullet; ++i)
	{
		bullet[i] = NULL;
	}
	for (int i = 0; i < Walls; ++i)
	{
		WallsObj[i] = 0;
	}

	meshList[GEO_AXES] = MeshBuilder::GenerateAxes("reference", 1000, 1000, 1000);
	meshList[GEO_QUAD] = MeshBuilder::GenerateQuad("quad", Color(1, 1, 1), 1, 1);

	meshList[GEO_FRONT] = MeshBuilder::GenerateQuad("front", Color(1, 1, 1), 1.f, 1.f);
	meshList[GEO_FRONT]->textureID = LoadTGA("Image//skybox//front.tga");

	meshList[GEO_BACK] = MeshBuilder::GenerateQuad("back", Color(1, 1, 1), 1.f, 1.f);
	meshList[GEO_BACK]->textureID = LoadTGA("Image//skybox//back.tga");

	meshList[GEO_BOTTOM] = MeshBuilder::GenerateQuad("bottom", Color(1, 1, 1), 1.f, 1.f);
	meshList[GEO_BOTTOM]->textureID = LoadTGA("Image//skybox//bottom.tga");

	meshList[GEO_TOP] = MeshBuilder::GenerateQuad("top", Color(1, 1, 1), 1.f, 1.f);
	meshList[GEO_TOP]->textureID = LoadTGA("Image//skybox//top.tga");

	meshList[GEO_RIGHT] = MeshBuilder::GenerateQuad("right", Color(1, 1, 1), 1.f, 1.f);
	meshList[GEO_RIGHT]->textureID = LoadTGA("Image//skybox//right.tga");

	meshList[GEO_LEFT] = MeshBuilder::GenerateQuad("left", Color(1, 1, 1), 1.f, 1.f);
	meshList[GEO_LEFT]->textureID = LoadTGA("Image//skybox//left.tga");

	meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[GEO_TEXT]->textureID = LoadTGA("Image//Text//gothiclight.tga");

	meshList[GEO_DEBUGBOX] = MeshBuilder::GenerateCube("Debug", Color(1, 1, 1), 1.f, 1.f, 1.f);

	meshList[GEO_LIGHTBALL] = MeshBuilder::GenerateSphere("LSphere", Color(1, 1, 1), 12, 12, 1);

	GenerateObj();
	meshList[GEO_VENDINGBODY] = MeshBuilder::GenerateOBJ("Vending machine", "OBJ//NPC//Vending_Machine.obj");
	meshList[GEO_VENDINGBODY]->textureID = LoadTGA("Image//NPC//Vending_Machine.tga");

	meshList[GEO_VENDINGCOVER] = MeshBuilder::GenerateOBJ("Vending machine cover", "OBJ//NPC//Vending_Cover.obj");
	meshList[GEO_VENDINGCOVER]->textureID = LoadTGA("Image//NPC//Vending_Cover.tga");
	
	meshList[GEO_CROSSHAIR] = MeshBuilder::GenerateText("Cross hair", 16, 16);
	meshList[GEO_CROSSHAIR]->textureID = LoadTGA("Image//UI//Cross_Hair");
	meshList[GEO_WALL]->textureID = LoadTGA("Image//CX_Scene//Wall_Texture.tga");
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

	light[0].type = Light::LIGHT_SPOT;
	light[0].position.Set(0, 20, 0);
	light[0].color.Set(1, 1, 1);
	light[0].power = 1;
	light[0].kC = 1.f;
	light[0].kL = 0.01f;
	light[0].kQ = 0.001f;
	light[0].cosCutoff = cos(Math::DegreeToRadian(90));
	light[0].cosInner = cos(Math::DegreeToRadian(30));
	light[0].exponent = 3.f;
	light[0].spotDirection.Set(0.f, 1.f, 0.f);


	//Make sure you pass uniform parameters after glUseProgram()
	glUniform3fv(m_parameters[U_LIGHT0_COLOR], 1, &light[0].color.r);
	glUniform1f(m_parameters[U_LIGHT0_POWER], light[0].power);
	glUniform1f(m_parameters[U_LIGHT0_KC], light[0].kC);
	glUniform1f(m_parameters[U_LIGHT0_KL], light[0].kL);
	glUniform1f(m_parameters[U_LIGHT0_KQ], light[0].kQ);

	glUniform1i(m_parameters[U_NUMLIGHTS], 1);
	glUniform1i(m_parameters[U_LIGHT0_TYPE], light[0].type);
	glUniform3fv(m_parameters[U_LIGHT0_COLOR], 1, &light[0].color.r);
	glUniform1f(m_parameters[U_LIGHT0_POWER], light[0].power);
	glUniform1f(m_parameters[U_LIGHT0_KC], light[0].kC);
	glUniform1f(m_parameters[U_LIGHT0_KL], light[0].kL);
	glUniform1f(m_parameters[U_LIGHT0_KQ], light[0].kQ);
	glUniform1f(m_parameters[U_LIGHT0_COSCUTOFF], light[0].cosCutoff);
	glUniform1f(m_parameters[U_LIGHT0_COSINNER], light[0].cosInner);
	glUniform1f(m_parameters[U_LIGHT0_EXPONENT], light[0].exponent);


	Mtx44 projection;
	projection.SetToPerspective(45.0f, 4.0f / 3.0f, 0.1f, 1000.0f);
	projectionStack.LoadMatrix(projection);
}
void ChuanXu::GenerateObj()
{
	meshList[GEO_BOSSHEAD] = MeshBuilder::GenerateOBJ("Boss head", "OBJ//Enemy//Boss_head.obj");
	meshList[GEO_BOSSHEAD]->textureID = LoadTGA("Image//Enemy//Boss_Head.tga");
	meshList[GEO_BOSSBODY] = MeshBuilder::GenerateOBJ("Boss body", "OBJ//Enemy//Boss_body.obj");
	meshList[GEO_BOSSBODY]->textureID = LoadTGA("Image//Enemy//Boss_body.tga");
	meshList[GEO_TURRETBODY] = MeshBuilder::GenerateOBJ("TurretBody", "OBJ//Enemy//Turret_body.obj");
	meshList[GEO_TURRETBODY]->textureID = LoadTGA("Image//Enemy//Turret_Body.tga");
	meshList[GEO_TURRETHEAD] = MeshBuilder::GenerateOBJ("TurretHead", "OBJ//Enemy//Turret_head.obj");
	meshList[GEO_TURRETHEAD]->textureID = LoadTGA("Image//Enemy//Turret_Head.tga");
	turret[1] = new Enemy("Turret_1", Vector3(10, 0, 10));
	turret[1]->setCollider(5, 5);
	turret[1]->updateCurPos();
	
	turret[2] = new Enemy("Turret_2", Vector3(20, 0.20));
	turret[2]->setCollider(5, 5);
	turret[2]->updateCurPos();

	//Bullet
	meshList[GEO_BULLET] = MeshBuilder::GenerateOBJ("Bullet", "OBJ//Enemy//Bullet.obj");
	meshList[GEO_BULLET]->textureID = LoadTGA("Image//Enemy//Bullet.tga");

	bullet[1] = new Bullet("Bullet_1", Vector3(turret[1]->Position.x, turret[1]->Position.y, turret[1]->Position.z));
	bullet[1]->setCollider(10, 10);
	bullet[1]->updateCurPos();

	bullet[2] = new Bullet("Bullet_1", Vector3(turret[2]->Position.x, turret[2]->Position.y, turret[2]->Position.z));
	bullet[2]->setCollider(10, 10);
	bullet[2]->updateCurPos();

	//Robot
	meshList[GEO_ROBOBODY] = MeshBuilder::GenerateOBJ("RoboBody", "OBJ//NPC//Robot_body.obj");
	meshList[GEO_ROBOBODY]->textureID = LoadTGA("Image//NPC//Robot_Body.tga");

	meshList[GEO_ROBOARMS] = MeshBuilder::GenerateOBJ("RoboArms", "OBJ//NPC//Robot_Arm.obj");
	meshList[GEO_ROBOARMS]->textureID = LoadTGA("Image//NPC//Robot_Arms.tga");
	object[1] = new GameObject("Robot", Vector3(10, 0, 10));
	object[1]->setCollider(5, 5);
	object[1]->updateCurPos();
	
	//walls
	meshList[GEO_WALL] = MeshBuilder::GenerateQuad("wall", Color(1, 1, 1), 10, 10);
	WallsObj[1] = new GameObject("Front wall", Vector3(0, 0, -50));
	WallsObj[1]->setCollider(50, 3);
	WallsObj[1]->updateCurPos();

	WallsObj[2] = new GameObject("left wall", Vector3(50, 0, 0));
	WallsObj[2]->setCollider(3, 50);
	WallsObj[2]->updateCurPos();

	WallsObj[3] = new GameObject("back wall", Vector3(0, 0, 50));
	WallsObj[3]->setCollider(50, 3);
	WallsObj[3]->updateCurPos();

	WallsObj[4] = new GameObject("right wall", Vector3(-50, 0, 0));
	WallsObj[4]->setCollider(3, 50);
	WallsObj[4]->updateCurPos();
}
void ChuanXu::Update(double dt)
{		
	static float translateLimit = 1;
	static float time;
	static bool smtHappen = false;
	static Vector3 prevpos;
	static Vector3 prevposTarget;
	//Robot.Set(Robot.x, Robot.y, Robot.z);
	deltaTime = "Health" + std::to_string(player->getHealth()); //"FPS:" + std::to_string(1 / dt);
	if (Application::IsKeyPressed('F'))
	{
		rotateAngle += (float)(100 * dt);
	}
	if (Application::IsKeyPressed('H'))
	{
		rotateAngle -= (float)(100 * dt);
	}
	if (Application::IsKeyPressed('T'))
	{
		object[1]->Position.x -= (float)(5 * sin(Math::DegreeToRadian(rotateAngle))*dt);
		object[1]->Position.z -= (float)(5 * cos(Math::DegreeToRadian(rotateAngle))*dt);
	}
	if (Application::IsKeyPressed('G'))
	{
		object[1]->Position.x += (float)(5 * sin(Math::DegreeToRadian(rotateAngle))*dt);
		object[1]->Position.z += (float)(5 * cos(Math::DegreeToRadian(rotateAngle))*dt);
	}


	static float LSPEED = 10;

	if (Application::IsKeyPressed('V'))
		lightEnable = false;
	if (Application::IsKeyPressed('B'))
		lightEnable = true;

	DelayTimer += (float)dt;





	if (coverOpened)
	{	
		if (translateLimit<-10)
		translateLimit *= -1;
		if (openCover <6)
		openCover += (float)(10 * translateLimit*dt);
	}

	if (Application::IsKeyPressed('I'))
		light[0].position.z -= (float)(LSPEED * dt);
	if (Application::IsKeyPressed('K'))
		light[0].position.z += (float)(LSPEED * dt);
	if (Application::IsKeyPressed('J'))
		light[0].position.x -= (float)(LSPEED * dt);
	if (Application::IsKeyPressed('L'))
		light[0].position.x += (float)(LSPEED * dt);
	if (Application::IsKeyPressed('O'))
		light[0].position.y -= (float)(LSPEED * dt);
	if (Application::IsKeyPressed('P'))
		light[0].position.y += (float)(LSPEED * dt);



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

	/*CamObj->Position = camera.position;
	CamObj->updateCurPos();*/
	player->Position = camera.position;
	player->updateCurPos();

	for (int i = 0; i < NUM_GEOMETRY; ++i)
	{
		if (object[i] && CamObj->trigger(object[i]))
		{
			deltaTime = "Wham bam";
			camera.position = prevpos;
			camera.target = prevposTarget;
			break;
		} 

	}
	for (int i = 0; i < numOfEnemy; ++i)
	{
		if (turret[i] && CamObj->trigger(turret[i]))
		{
			deltaTime = "Wham bam";
			camera.position = prevpos;
			camera.target = prevposTarget;
			break;
		}
		else if (i == (numOfEnemy - 1))
		{
			prevpos = camera.position;
			prevposTarget = camera.target;
		}
	}

	for (int i = 0; i < numOfBullet; ++i)
	{
		if (bullet[i] && player->trigger(bullet[i]))		
		{
			if (!smtHappen)
			{
				player->DmgPlayer(1);
				smtHappen = true;
				camera.position = prevpos;
				camera.target = prevposTarget;
				break;
			}
			bullet[i]->shoot = false;	//stop rendering of bullet after contact with player
		}
	}

	if (DelayTimer > 5)		// to ensure that bullet will stop rendering even if it doesnt hit player
	{
		DelayTimer = 0;
		for (int i = 1; i <= 2; ++i)
				bullet[i]->shoot = false;
	}
	if (DelayTimer > 0.5)	//shoot bullet after updating pos and rotation after 0.5s
		for (int i = 1; i <= 2; ++i)
		{
			bullet[i]->bulletUpdate(dt);
			bullet[i]->updateCurPos();
		}
	else	//if DelayTimer !> (0.5 || 5), update bullet post and rotation 
	{	
		for (int i = 1; i <= 2; ++i)
			bullet[i]->shootBullet(turret[i]->RotateToPlayer(player->Position), turret[i]->Position);
	}
	if (smtHappen)
	{
		time += dt;
		if (time > 5)
		{
			time = 0;
			smtHappen = false;
		}
	}

	camera.Update(dt);
}

void ChuanXu::RenderWalls()
{	
	//side 1
	modelStack.PushMatrix();
	modelStack.Translate(0, 10, -50);
	modelStack.Rotate(90, 0, 0, 1);
	modelStack.Scale(2, 2, 2);
	RenderMesh(meshList[GEO_WALL], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(20, 10, -50);
	modelStack.Rotate(90, 0, 0, 1);
	modelStack.Scale(2, 2, 2);
	RenderMesh(meshList[GEO_WALL], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(40, 10, -50);
	modelStack.Rotate(90, 0, 0, 1);
	modelStack.Scale(2, 2, 2);
	RenderMesh(meshList[GEO_WALL], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-20, 10,-50);
	modelStack.Rotate(90, 0, 0, 1);
	modelStack.Scale(2, 2, 2);
	RenderMesh(meshList[GEO_WALL], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-40, 10, -50);
	modelStack.Rotate(90, 0, 0, 1);
	modelStack.Scale(2, 2, 2);
	RenderMesh(meshList[GEO_WALL], true);
	modelStack.PopMatrix();

	//side 2
	modelStack.PushMatrix();
	modelStack.Translate(-50, 10, 0);
	modelStack.Rotate(90, 0, 1 , 0);
	modelStack.Rotate(90, 0, 0, 1);
	modelStack.Scale(2, 2, 2);
	RenderMesh(meshList[GEO_WALL], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-50, 10, 20);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Rotate(90, 0, 0, 1);
	modelStack.Scale(2, 2, 2);
	RenderMesh(meshList[GEO_WALL], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-50, 10, 40);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Rotate(90, 0, 0, 1);
	modelStack.Scale(2, 2, 2);
	RenderMesh(meshList[GEO_WALL], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-50, 10, -20);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Rotate(90, 0, 0, 1);
	modelStack.Scale(2, 2, 2);
	RenderMesh(meshList[GEO_WALL], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-50, 10, -40);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Rotate(90, 0, 0, 1);
	modelStack.Scale(2, 2, 2);
	RenderMesh(meshList[GEO_WALL], true);
	modelStack.PopMatrix();

	//side 3
	modelStack.PushMatrix();
	modelStack.Translate(0, 10, 50);
	modelStack.Rotate(90, 0, 0, -1);
	modelStack.Rotate(180, 0, 1, 0);
	modelStack.Scale(2, 2, 2);
	RenderMesh(meshList[GEO_WALL], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(20, 10, 50);
	modelStack.Rotate(90, 0, 0, -1);
	modelStack.Rotate(180, 0, 1, 0);
	modelStack.Scale(2, 2, 2);
	RenderMesh(meshList[GEO_WALL], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(40, 10, 50);
	modelStack.Rotate(90, 0, 0, -1);
	modelStack.Rotate(180, 0, 1, 0);
	modelStack.Scale(2, 2, 2);
	RenderMesh(meshList[GEO_WALL], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-20, 10,50);
	modelStack.Rotate(90, 0, 0, -1);
	modelStack.Rotate(180, 0, 1, 0);
	modelStack.Scale(2, 2, 2);
	RenderMesh(meshList[GEO_WALL], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-40, 10, 50);
	modelStack.Rotate(90, 0, 0,-1);
	modelStack.Rotate(180, 0, 1, 0);
	modelStack.Scale(2, 2, 2);
	RenderMesh(meshList[GEO_WALL], true);
	modelStack.PopMatrix();

	//side 4
	modelStack.PushMatrix();
	modelStack.Translate(50, 10, 0);
	modelStack.Rotate(90, 0, -1, 0);
	modelStack.Rotate(90, 0, 0, 1);
	modelStack.Scale(2, 2, 2);
	RenderMesh(meshList[GEO_WALL], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(50, 10, 20);
	modelStack.Rotate(90, 0,-1, 0);
	modelStack.Rotate(90, 0, 0, 1);
	modelStack.Scale(2, 2, 2);
	RenderMesh(meshList[GEO_WALL], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(50, 10, 40);
	modelStack.Rotate(90, 0, -1, 0);
	modelStack.Rotate(90, 0, 0, 1);
	modelStack.Scale(2, 2, 2);
	RenderMesh(meshList[GEO_WALL], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(50, 10, -20);
	modelStack.Rotate(90, 0, -1, 0);
	modelStack.Rotate(90, 0, 0, 1);
	modelStack.Scale(2, 2, 2);
	RenderMesh(meshList[GEO_WALL], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(50, 10, -40);
	modelStack.Rotate(90, 0, -1, 0);
	modelStack.Rotate(90, 0, 0, 1);
	modelStack.Scale(2, 2, 2);
	RenderMesh(meshList[GEO_WALL], true);
	modelStack.PopMatrix();

}

void ChuanXu::skyBox()
{
	RenderMesh(meshList[GEO_AXES], false);

	modelStack.PushMatrix();
	modelStack.Translate(light[0].position.x, light[0].position.y, light[0].position.z);
	RenderMesh(meshList[GEO_LIGHTBALL], false);
	modelStack.PopMatrix();

	//Bottom
	modelStack.PushMatrix();
	//to do: transformation code here
	modelStack.Translate(camera.position.x, camera.position.y - 498.f, camera.position.z);
	modelStack.Rotate(90, 0, -1, 0);
	modelStack.Rotate(90, -1, 0, 0);
	modelStack.Scale(1000.f, 1000.f, 1000.f);
	RenderMesh(meshList[GEO_BOTTOM], false);
	modelStack.PopMatrix();

	//Top
	modelStack.PushMatrix();
	//to do: transformation code here
	modelStack.Translate(camera.position.x, camera.position.y + 498.f, camera.position.z);
	modelStack.Rotate(90, 0, -1, 0);
	modelStack.Rotate(90, 1, 0, 0);
	modelStack.Scale(1000.f, 1000.f, 1000.f);
	RenderMesh(meshList[GEO_TOP], false);
	modelStack.PopMatrix();

	//Front
	modelStack.PushMatrix();
	//to do: transformation code here
	modelStack.Translate(camera.position.x, camera.position.y, camera.position.z - 498.f);
	//modelStack.Rotate(90, -1, 0, 0);
	modelStack.Scale(1000.f, 1000.f, 1000.f);
	RenderMesh(meshList[GEO_FRONT], false);
	modelStack.PopMatrix();

	//Right
	modelStack.PushMatrix();
	//to do: transformation code here
	modelStack.Translate(camera.position.x + 498.f, camera.position.y, camera.position.z);
	modelStack.Rotate(90, 0, -1, 0);
	modelStack.Scale(1000.f, 1000.f, 1000.f);
	RenderMesh(meshList[GEO_RIGHT], false);
	modelStack.PopMatrix();

	//Left
	modelStack.PushMatrix();
	//to do: transformation code here
	modelStack.Translate(camera.position.x - 498.f, camera.position.y, camera.position.z);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Scale(1000.f, 1000.f, 1000.f);
	RenderMesh(meshList[GEO_LEFT], false);
	modelStack.PopMatrix();

	//Back
	modelStack.PushMatrix();
	//to do: transformation code here
	modelStack.Translate(camera.position.x, camera.position.y, camera.position.z + 498.f);
	modelStack.Rotate(180, 0, 1, 0);
	modelStack.Scale(1000.f, 1000.f, 1000.f);
	RenderMesh(meshList[GEO_BACK], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Rotate(90, -1, 0, 0);
	modelStack.Scale(100, 100, 100);
	RenderMesh(meshList[GEO_FLOOR], true);
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
	else if (light[0].type == Light::LIGHT_SPOT)
	{
		Position lightPosition_cameraspace = viewStack.Top() * light[0].position;
		glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightPosition_cameraspace.x);
		Vector3 spotDirection_cameraspace = viewStack.Top() * light[0].spotDirection;
		glUniform3fv(m_parameters[U_LIGHT0_SPOTDIRECTION], 1, &spotDirection_cameraspace.x);
	}
	else
	{
		Position lightPosition_cameraspace = viewStack.Top() * light[0].position;
		glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightPosition_cameraspace.x);
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

	//-------------------------------------------------------------------------------------
	 skyBox();
	 RenderWalls();

	modelStack.PushMatrix();
	modelStack.Translate(object[1]->Position.x, object[1]->Position.y, object[1]->Position.z);
	modelStack.Rotate(rotateAngle, 0, 1, 0);
	RenderMesh(meshList[GEO_ROBOBODY], true);
	modelStack.PushMatrix();
	RenderMesh(meshList[GEO_ROBOARMS], true);
	modelStack.PopMatrix();
	modelStack.PopMatrix();

	// Turret 1 
	modelStack.PushMatrix();
	modelStack.Translate(turret[1]->Position.x, turret[1]->Position.y, turret[1]->Position.z);
	RenderMesh(meshList[GEO_TURRETBODY], true);
	modelStack.PushMatrix();
	modelStack.Rotate(turret[1]->RotateToPlayer(player->Position), 0, 1, 0);
	RenderMesh(meshList[GEO_TURRETHEAD], true);
	modelStack.PopMatrix();
	modelStack.PopMatrix();
	if (bullet[1]->shoot)
	{
		modelStack.PushMatrix();
		modelStack.Translate(bullet[1]->Position.x, bullet[1]->Position.y, bullet[1]->Position.z);
		modelStack.Rotate(bullet[1]->rotation, 0, 1, 0);
		RenderMesh(meshList[GEO_BULLET], false);
		modelStack.PopMatrix();
	}

	//Turret 2
	modelStack.PushMatrix();
	modelStack.Translate(turret[2]->Position.x, turret[2]->Position.y, turret[2]->Position.z);
		RenderMesh(meshList[GEO_TURRETBODY], true);
		modelStack.PushMatrix();
		modelStack.Rotate(turret[2]->RotateToPlayer(player->Position), 0, 1, 0);
		RenderMesh(meshList[GEO_TURRETHEAD], true);
		modelStack.PopMatrix();
		modelStack.PopMatrix();

		if (bullet[2]->shoot)
		{
			modelStack.PushMatrix();
			modelStack.Translate(bullet[2]->Position.x, bullet[2]->Position.y , bullet[2]->Position.z);
			modelStack.Rotate(bullet[2]->rotation, 0, 1, 0);
			RenderMesh(meshList[GEO_BULLET], false);
			modelStack.PopMatrix();
		}

	RenderTextOnScreen(meshList[GEO_TEXT], deltaTime, Color(0, 1, 0), 5, 0, 0);

	////No transform needed
	//RenderMeshOnScreen(meshList[GEO_QUAD], 10, 10, 10, 10);
	//-------------------------------------------------------------------------------------

	/*modelStack.PushMatrix();
	modelStack.Translate(-20, 0, -20);
	RenderMesh(meshList[GEO_VENDINGBODY], true);
	modelStack.PushMatrix();
	modelStack.Translate(0, openCover, 0);
	RenderMesh(meshList[GEO_VENDINGCOVER], true);
	modelStack.PopMatrix();
	modelStack.PopMatrix();*/




	RenderTextOnScreen(meshList[GEO_TEXT], std::to_string(bullet[1]->Position.x), Color(1, 1, 1), 4, 1, 2);
	RenderTextOnScreen(meshList[GEO_TEXT], std::to_string(bullet[1]->Position.z), Color(1, 1, 1), 4, 1, 1);
	RenderTextOnScreen(meshList[GEO_TEXT], std::to_string(DelayTimer), Color(1, 1, 1), 4, 1, 3);
	//RenderMeshOnScreen(meshList[GEO_CROSSHAIR], CrossHair.x, CrossHair.z, 3, 3);
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

void ChuanXu::RenderMeshOnScreen(Mesh* mesh, int x, int y, int sizex, int sizey)
{
	glDisable(GL_DEPTH_TEST);
	Mtx44 ortho;
	ortho.SetToOrtho(0, 80, 0, 60, -10, 10); //size of screen UI
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity(); //No need camera for ortho mode
	modelStack.PushMatrix();
	modelStack.LoadIdentity();
	//to do: scale and translate accordingly
	modelStack.Translate(x, y, 0);
	modelStack.Scale(sizex, sizey, 1);
	RenderMesh(mesh, false); //UI should not have light
	projectionStack.PopMatrix();
	viewStack.PopMatrix();
	modelStack.PopMatrix();
	glEnable(GL_DEPTH_TEST);

}


void ChuanXu::Exit()
{
	for (int i = 0; i < NUM_GEOMETRY; ++i)
	{
		if (meshList[i] != NULL)
			delete meshList[i];
	}
	// Cleanup VBO here
	glDeleteVertexArrays(1, &m_vertexArrayID);

	//Cleanup color
	glDeleteProgram(m_programID);
}
