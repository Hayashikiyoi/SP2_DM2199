#include "Scene4_Boss.h"
#include "GL\glew.h"
#include "Mtx44.h"
#include "Application.h"

#include "shader.hpp"

#include "MeshBuilder.h"
#include "LoadTGA.h"

#include "MyMath.h"
using namespace Math;

Scene4_Boss::Scene4_Boss()
{
	m_programID = SceneManager::instance()->returnProg();
}

Scene4_Boss::~Scene4_Boss()
{
}

void Scene4_Boss::Init()
{
	// Init VBO here
	rotateAngle = 0;
	scaleAll = 2;
	lightEnable = true;

	//Emable depth test
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE); //Deletes the backface
	glEnable(GL_BLEND);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//Initialise camera
	camera.Init(Vector3(400, 0, 0), Vector3(0, 0, 0), Vector3(0, 1, 0));
	player = new Player("camera", Vector3(400, 0, 0));
	lasergun = new Weapon("Blaster", Vector3(-0.12f, -0.750, 2), 100);

	//Set background color to dark blue
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	//Generate a default VAO for now
	glGenVertexArrays(1, &m_vertexArrayID);
	glBindVertexArray(m_vertexArrayID);

	//Initialise ur objects here
	for (int i = 0; i < NUM_GEOMETRY; ++i)
	{
		meshList[i] = NULL;
		object[i] = 0;
	}
	for (int i = 0; i < numOfWalls; ++i)
	{
		WallsObj[i] = NULL;
	}
	for (int i = 0; i < numOfEnemy; ++i)
	{
		turret[i] = NULL;
	}

	for (int i = 0; i < numOfBullets; ++i)
	{
		bullet[i] = NULL;
	}
	Keys[1] = 0;
	TriggerBox[0] = 0;
	meshList[GEO_AXES] = MeshBuilder::GenerateAxes("reference", 1000, 1000, 1000);

	meshList[GEO_FRONT] = MeshBuilder::GenerateQuad("front", Color(1, 1, 1), 1.f, 1.f);
	meshList[GEO_FRONT]->textureID = LoadTGA("Image//skybox3//space2__right.tga");

	meshList[GEO_BACK] = MeshBuilder::GenerateQuad("back", Color(1, 1, 1), 1.f, 1.f);
	meshList[GEO_BACK]->textureID = LoadTGA("Image//skybox3//space2__left.tga");

	meshList[GEO_BOTTOM] = MeshBuilder::GenerateQuad("bottom", Color(1, 1, 1), 1.f, 1.f);
	meshList[GEO_BOTTOM]->textureID = LoadTGA("Image//skybox3//space2__down.tga");

	meshList[GEO_TOP] = MeshBuilder::GenerateQuad("top", Color(1, 1, 1), 1.f, 1.f);
	meshList[GEO_TOP]->textureID = LoadTGA("Image//skybox3//space2__up.tga");

	meshList[GEO_RIGHT] = MeshBuilder::GenerateQuad("right", Color(1, 1, 1), 1.f, 1.f);
	meshList[GEO_RIGHT]->textureID = LoadTGA("Image//skybox3//space2__front.tga");

	meshList[GEO_LEFT] = MeshBuilder::GenerateQuad("left", Color(1, 1, 1), 1.f, 1.f);
	meshList[GEO_LEFT]->textureID = LoadTGA("Image//skybox3//space2__back.tga");

	meshList[GEO_FLOOR] = MeshBuilder::GenerateQuad("Floor", Color(1, 1, 1), 1.f, 1.f);
	meshList[GEO_FLOOR]->textureID = LoadTGA("Image//floor//floor.tga");

	meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[GEO_TEXT]->textureID = LoadTGA("Image//Text//gothiclight.tga");


	//meshList[GEO_LIGHTBALL] = MeshBuilder::GenerateSphere("LSphere", Color(1, 1, 1), 12, 12, 1);

	GUI();
	GenerateOBJ();

	//Load vertex and fragment shaders
	//m_programID = LoadShaders("Shader//Texture.vertexshader", "Shader//Text.fragmentshader");
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
	moveBullet = 0;
	Mtx44 projection;
	projection.SetToPerspective(45.0f, 4.0f / 3.0f, 0.1f, 4000.0f);
	projectionStack.LoadMatrix(projection);
}

void Scene4_Boss::lightingfunc()
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

void Scene4_Boss::Update(double dt)
{
	static float translateLimit = 1;
	static Vector3 prevpos;
	static Vector3 prevposTarget;
	static bool smtHappen = false;
	static bool smtHappen2 = false;
	static bool smtHappen3 = false;
	static float time;
	static float LSPEED = 10;

	DelayTimer += (float)dt;
	rotateAngle += (float)dt;
	HPsizeX = (float)player->getHealth()*0.215f;
	test3 = turret[9]->showHP()*0.3f;

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
	
	//---------------------------------------------------------------------------------------------------------------------------------------------------------
	//Player Shoot
	static float ROF = 0.125f;
	static float TP = 0;
	static bool shoot = false;
	if (Application::IsKeyPressed('T'))
	{
		lasergun->reload();
	}
	
	if (Application::IsKeyPressed(VK_LBUTTON) && !shoot)
	{
		lasergun->shoot(&camera);
		shoot = true;
	}
	if (shoot)
	{
		TP += (float)dt;
		if (TP > ROF)
		{
			TP = 0; 
			shoot = false;
		}
	}
	AmmoLeft = std::to_string(lasergun->bulletLeft())+ "/45";
	clipCount = std::to_string(lasergun->canisterLeft());

	for (int i = 0; i < clipSize; ++i)
	{
		if (lasergun->pBullet[i] && turret[9] && turret[9]->trigger(lasergun->pBullet[i]) && !smtHappen2)
		{
			turret[9]->dmgToEnemy(1);
			smtHappen2 = true;
			break;
		}
	}
	if (turret[9]->showHP() == 0)
	{
		turret[9]->isdead();
		turret[9]->Position.z = 1000;
		turret[9]->Position.x = 1000;
		turret[9]->updateCurPos();
		turret[10]->Position.z = 1000;
		turret[10]->Position.x = 1000;
		turret[10]->updateCurPos();
	}
	if (DelayTimer > 5)
	{
		DelayTimer = 0;
		bullet[1]->shoot = false;
		bullet[2]->shoot = false;
		bullet[3]->shoot = false;
		bullet[4]->shoot = false;
		bullet[5]->shoot = false;
	}
	else if (DelayTimer < 0.5)
	{
		bullet[1]->shoot = true;
		bullet[2]->shoot = true;
		bullet[3]->shoot = true;
		bullet[4]->shoot = true;
		bullet[5]->shoot = true;
	}
	//---------------------------------------------------------------------------------------------------------------------------------------------------------
	//Bullet to player Collision
	player->Position = camera.position;
	player->updateCurPos();

	for (int i = 1; i <= 5; ++i)
	{
		if (bullet[i]->shoot)
			bullet[i]->bulletUpdate(dt);
		if (bullet[i]->shoot == false){
			if (i == 5)
				bullet[i]->shootBullet(turret[9]->RotateToPlayer(camera.position), turret[9]->Position);
			else
			bullet[i]->shootBullet(turret[i]->RotateToPlayer(camera.position), turret[i]->Position);
		}
		bullet[i]->updateCurPos();
	}
	for (int i = 0; i < NUM_GEOMETRY; ++i)
	{
		if (object[i] && player->trigger(object[i]))
		{
			//if you need to get pushed out of the collider
			//deltaTime = "Wham bam";
			camera.position = prevpos;
			camera.target = prevposTarget;
			break;
		}
	}
	for (int i = 0; i < numOfBullets; ++i)
	{
		if (bullet[i] && player->trigger(bullet[i]))
		{
			if (!smtHappen)
			{
				if (i == 5)
				player->DmgPlayer(30);
				else 
				player->DmgPlayer(10);

				smtHappen = true;
				bullet[i]->shoot = false;
				DelayTimer = 0;
			}
		}
	}
	if (player->getHealth() == 0)
	{
		SceneManager::instance()->changeScene(5);
		return;
	}
	//---------------------------------------------------------------------------------------------------------------------------------------------------------
	//Getting Key Functions
	if (turret[9]->isdead()){
		Keys[1]->Position.z = 0;
		Keys[1]->updateCurPos();
		if (Keys[1] && player->trigger(Keys[1]))
		{
			if (Application::IsKeyPressed('E'))
			{
				Keys[1]->SetCollected(true);
				WallsObj[8]->Position.z = 100;
				WallsObj[8]->updateCurPos();
				WallsObj[9]->Position.z = -100;
				WallsObj[9]->updateCurPos();
				Keys[1]->Position.z = 1000;
				Keys[1]->Position.x = 1000;
				Keys[1]->updateCurPos();
			}
		}
	}
	else
	{
		Keys[1]->Position.z = 1000;
		Keys[1]->updateCurPos();
	}
	if (Keys[2] && player->trigger(Keys[2]))
	{
		if (!smtHappen)
		{
			if (Application::IsKeyPressed('E'))
			{
				lasergun->pickupClip();
				Keys[2]->Position.x = 1000;
				Keys[2]->Position.z = 1000;
				Keys[2]->updateCurPos();
				smtHappen3 = true;
			}
		}
	}
	if (Keys[3] && player->trigger(Keys[3]))
	{
		if (!smtHappen)
		{
			if (Application::IsKeyPressed('E'))
			{
				lasergun->pickupClip();
				Keys[3]->Position.x = 1000;
				Keys[3]->Position.z = 1000;
				Keys[3]->updateCurPos();
				smtHappen3 = true;
			}
		}
	}
	if (Keys[4] && player->trigger(Keys[4]))
	{
		if (!smtHappen)
		{
			if (Application::IsKeyPressed('E'))
			{
				lasergun->pickupClip();
				Keys[4]->Position.x = 1000;
				Keys[4]->Position.z = 1000;
				Keys[4]->updateCurPos();
				smtHappen3 = true;
			}
		}
	}
	if (Keys[5] && player->trigger(Keys[5]))
	{
		if (!smtHappen)
		{
			if (Application::IsKeyPressed('E'))
			{
				player->HealPlayer(50);
				Keys[5]->Position.x = 1000;
				Keys[5]->Position.z = 1000;
				Keys[5]->updateCurPos();
				smtHappen3 = true;
			}
		}
	}
	//---------------------------------------------------------------------------------------------------------------------------------------------------------
	//Collision on walls
	for (int i = 0; i < numOfWalls; ++i)
	{
		if (WallsObj[i] && player->trigger(WallsObj[i]))
		{
			//if you need to get pushed out of the collider
			//deltaTime = "Why";
			camera.position = prevpos;
			camera.target = prevposTarget;
			break;
		}
	}
	//---------------------------------------------------------------------------------------------------------------------------------------------------------
	//Collision on objects
	for (int i = 0; i < numOfEnemy; ++i)
	{
		if (turret[i] && player->trigger(turret[i]))
		{
			//if you need to get pushed out of the collider
			//deltaTime = "Wham bam";
			camera.position = prevpos;
			camera.target = prevposTarget;
			break;
		}
		else if (i == (numOfEnemy - 1))//Max value thx
		{
			prevpos = camera.position;
			prevposTarget = camera.target;
		}
	}
	//---------------------------------------------------------------------------------------------------------------------------------------------------------
	//Change Scene
	if (TriggerBox[0] && player->trigger(TriggerBox[0]))
	{
		SceneManager::instance()->levelCompleted = 3;
		SceneManager::instance()->changeScene(3);
		return;
	}
	//---------------------------------------------------------------------------------------------------------------------------------------------------------
	//Delay for Bullet attacks
	if (smtHappen)
	{
		time += (float)dt;
		if (time > 2)
		{
			time = 0;
			smtHappen = false;
		}
	}
	if (smtHappen2)
	{
		time += (float)dt;
		if (time > 1)
		{
			time = 0;
			smtHappen2 = false;
		}
	}
	if (smtHappen3)
	{
		time += (float)dt;
		if (time > 10)
		{
			time = 0;
			Keys[2]->Position.x = 300;
			Keys[2]->Position.z = 0;
			Keys[2]->updateCurPos();

			Keys[3]->Position.x = 0;
			Keys[3]->Position.z = 300;
			Keys[3]->updateCurPos();
			
			Keys[4]->Position.x = 0;
			Keys[4]->Position.z = -300;
			Keys[4]->updateCurPos();

			Keys[5]->Position.x = 100;
			Keys[5]->Position.z = -300;
			Keys[5]->updateCurPos();
			smtHappen3 = false;
		}
	}
	if (DelayTimer > 0.125)
	{
		test2 += 20 * (float)dt;
		if (test2 > 30.f)
			test2 = 30.f;
		if (test3 <= 0)
			test2 -= 40*(float)dt;
	}
	camera.Update(dt);
	lasergun->updateBullet(dt);
}

void Scene4_Boss::GUI()
{
	HPsizeX = 21.5f;
	test2 = 0.f;
	test3 = 30.f;
	meshList[GEO_HEALTH] = MeshBuilder::GenerateQuad("HealthBG", Color(1, 1, 1), 1.f, 1.f);
	meshList[GEO_HEALTH]->textureID = LoadTGA("Image//UI//healthBar.tga");

	meshList[GEO_STAMINA] = MeshBuilder::GenerateQuad("stamina", Color(1, 1, 1), 1.f, 1.f);
	meshList[GEO_STAMINA]->textureID = LoadTGA("Image//UI//staminaBar.tga");

	meshList[GEO_HEALTHBG] = MeshBuilder::GenerateQuad("HealthBG", Color(1, 1, 1), 1.f, 1.f);
	meshList[GEO_HEALTHBG]->textureID = LoadTGA("Image//UI//healthBG.tga");

	meshList[GEO_AMMOBG] = MeshBuilder::GenerateQuad("ammoBG", Color(1, 1, 1), 1, 1);
	meshList[GEO_AMMOBG]->textureID = LoadTGA("Image//UI//ammoBG.tga");

	meshList[GEO_BOSSTESTBG] = MeshBuilder::GenerateQuad("ammoBG", Color(1, 1, 1), 1, 1);
	meshList[GEO_BOSSTESTBG]->textureID = LoadTGA("Image//UI//Boss//bossBG.tga");
	meshList[GEO_BOSSTEST] = MeshBuilder::GenerateQuad("ammoBG", Color(1, 1, 1), 1, 1);
	meshList[GEO_BOSSTEST]->textureID = LoadTGA("Image//UI//Boss//bossHP.tga");
}

void Scene4_Boss::skyBox()
{
	RenderMesh(meshList[GEO_AXES], false);

	/*modelStack.PushMatrix();
	modelStack.Translate(light[0].position.x, light[0].position.y, light[0].position.z);
	RenderMesh(meshList[GEO_LIGHTBALL], false);
	modelStack.PopMatrix();*/

	//Bottom
	modelStack.PushMatrix();
	//to do: transformation code here
	modelStack.Translate(0, - 996.f, 0);
	modelStack.Rotate(90, 0, -1, 0);
	modelStack.Rotate(90, -1, 0, 0);
	modelStack.Scale(2000.f, 2000.f, 2000.f);
	RenderMesh(meshList[GEO_BOTTOM], false);
	modelStack.PopMatrix();

	//Top
	modelStack.PushMatrix();
	//to do: transformation code here
	modelStack.Translate(0, 996.f, 0);
	modelStack.Rotate(90, 0, -1, 0);
	modelStack.Rotate(90, 1, 0, 0);
	modelStack.Scale(2000.f, 2000.f, 2000.f);
	RenderMesh(meshList[GEO_TOP], false);
	modelStack.PopMatrix();

	//Front
	modelStack.PushMatrix();
	//to do: transformation code here
	modelStack.Translate(0, 0, - 996.f);
	//modelStack.Rotate(90, -1, 0, 0);
	modelStack.Scale(2000.f, 2000.f, 2000.f);
	RenderMesh(meshList[GEO_FRONT], false);
	modelStack.PopMatrix();

	//Right
	modelStack.PushMatrix();
	//to do: transformation code here
	modelStack.Translate(996.f, 0, 0);
	modelStack.Rotate(90, 0, -1, 0);
	modelStack.Scale(2000.f, 2000.f, 2000.f);
	RenderMesh(meshList[GEO_RIGHT], false);
	modelStack.PopMatrix();

	//Left
	modelStack.PushMatrix();
	//to do: transformation code here
	modelStack.Translate(- 996.f,0,0);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Scale(2000.f, 2000.f, 2000.f);
	RenderMesh(meshList[GEO_LEFT], false);
	modelStack.PopMatrix();

	//Back
	modelStack.PushMatrix();
	//to do: transformation code here
	modelStack.Translate(0,0, 996.f);
	modelStack.Rotate(180, 0, 1, 0);
	modelStack.Scale(2000.f, 2000.f, 2000.f);
	RenderMesh(meshList[GEO_BACK], false);
	modelStack.PopMatrix();

	//Bottom
	modelStack.PushMatrix();
	//to do: transformation code here
	modelStack.Translate(0,0,0);
	modelStack.Rotate(90, 0, -1, 0);
	modelStack.Rotate(90, -1, 0, 0);
	modelStack.Scale(1000.f, 1000.f, 1000.f);
	RenderMesh(meshList[GEO_FLOOR], false);
	modelStack.PopMatrix();

}

void Scene4_Boss::Walls()
{
	modelStack.PushMatrix();
	modelStack.Translate(WallsObj[1]->Position.x, WallsObj[1]->Position.y, WallsObj[1]->Position.z);
	modelStack.Scale(10, 10, 10);
	RenderMesh(meshList[GEO_WALL],false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(WallsObj[2]->Position.x, WallsObj[2]->Position.y, WallsObj[2]->Position.z);
	modelStack.Scale(10, 10, 10);
	RenderMesh(meshList[GEO_WALL], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Translate(WallsObj[3]->Position.x + 450, WallsObj[3]->Position.y, WallsObj[3]->Position.z - 450);
	modelStack.Scale(10, 10, 10);
	RenderMesh(meshList[GEO_WALL], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Translate(WallsObj[4]->Position.x - 450, WallsObj[4]->Position.y,-WallsObj[4]->Position.z + 450);
	modelStack.Scale(10, 10, 10);
	RenderMesh(meshList[GEO_WALL], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(WallsObj[5]->Position.x, WallsObj[5]->Position.y, WallsObj[5]->Position.z);
	modelStack.Scale(10, 10, 10);
	RenderMesh(meshList[GEO_WALL_2], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(WallsObj[6]->Position.x, WallsObj[6]->Position.y, WallsObj[6]->Position.z);
	modelStack.Scale(10, 10, 10);
	RenderMesh(meshList[GEO_WALL_2], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Rotate(90,0,1,0);
	modelStack.Translate(WallsObj[7]->Position.x, WallsObj[7]->Position.y, WallsObj[7]->Position.z);
	modelStack.Scale(15, 15, 15);
	RenderMesh(meshList[GEO_DOOR_FRAME], false);
	modelStack.PopMatrix();

	if (!Keys[1]->isCollected())
	{
		modelStack.PushMatrix();
		modelStack.Translate(WallsObj[8]->Position.x, WallsObj[8]->Position.y, WallsObj[8]->Position.z);
		modelStack.Rotate(90, 0, 1, 0);
		modelStack.Scale(15, 15, 15);
		RenderMesh(meshList[GEO_DOOR], false);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(WallsObj[9]->Position.x, WallsObj[9]->Position.y, WallsObj[9]->Position.z);
		modelStack.Rotate(270, 0, 1, 0);
		modelStack.Scale(15, 15, 15);
		RenderMesh(meshList[GEO_DOOR], false);
		modelStack.PopMatrix();
	}
	else
	{
		modelStack.PushMatrix();
		modelStack.Translate(WallsObj[8]->Position.x, WallsObj[8]->Position.y, WallsObj[8]->Position.z - 60);
		modelStack.Rotate(90, 0, 1, 0);
		modelStack.Scale(15, 15, 15);
		RenderMesh(meshList[GEO_DOOR], false);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(WallsObj[9]->Position.x, WallsObj[9]->Position.y, WallsObj[9]->Position.z + 60);
		modelStack.Rotate(270, 0, 1, 0);
		modelStack.Scale(15, 15, 15);
		RenderMesh(meshList[GEO_DOOR], false);
		modelStack.PopMatrix();
	}

	modelStack.PushMatrix();
	modelStack.Translate(WallsObj[10]->Position.x, 0, WallsObj[10]->Position.z);
	modelStack.Scale(3, 3, 3);
	RenderMesh(meshList[GEO_DEBRIS], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(WallsObj[11]->Position.x, 0, WallsObj[11]->Position.z);
	modelStack.Scale(3, 3, 3);
	RenderMesh(meshList[GEO_DEBRIS], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(WallsObj[12]->Position.x, 0, WallsObj[12]->Position.z);
	modelStack.Scale(3, 3, 3);
	RenderMesh(meshList[GEO_DEBRIS], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(WallsObj[13]->Position.x, 0, WallsObj[13]->Position.z);
	modelStack.Scale(3, 3, 3);
	RenderMesh(meshList[GEO_DEBRIS], false);
	modelStack.PopMatrix();
}

void Scene4_Boss::GenerateOBJ()
{
	//BOSS
	meshList[GEO_TURRETHEAD] = MeshBuilder::GenerateOBJ("Boss_head", "OBJ//Enemy//Boss_Head.obj");
	meshList[GEO_TURRETHEAD]->textureID = LoadTGA("Image//Enemy//Boss_head.tga");
	meshList[GEO_TURRETBODY] = MeshBuilder::GenerateOBJ("Boss_body", "OBJ//Enemy//Boss_body.obj");
	meshList[GEO_TURRETBODY]->textureID = LoadTGA("Image//Enemy//Boss_body.tga");
	turret[9] = new Enemy("Boss_head", Vector3(0,0,0));
	turret[9]->setCollider(100, 100);
	turret[9]->updateCurPos();
	turret[10] = new Enemy("Boss_body", Vector3(0,0,0));

	//WALLS
	meshList[GEO_WALL] = MeshBuilder::GenerateOBJ("Wall", "OBJ//Wall//Wall.obj");
	meshList[GEO_WALL]->textureID = LoadTGA("Image//Wall//Wall.tga");
	WallsObj[1] = new GameObject("Wall", Vector3(0,0,-450));
	WallsObj[1]->setCollider(1000,15);
	WallsObj[1]->updateCurPos();
	WallsObj[2] = new GameObject("Wall", Vector3(0,0,450));
	WallsObj[2]->setCollider(1000,15);
	WallsObj[2]->updateCurPos();
	WallsObj[3] = new GameObject("Wall", Vector3(-450, 0, 0));
	WallsObj[3]->setCollider(15, 1000);
	WallsObj[3]->updateCurPos();
	WallsObj[4] = new GameObject("Wall", Vector3(450, 0, 0));
	WallsObj[4]->setCollider(15, 1000);
	WallsObj[4]->updateCurPos();
	//WALLS_2
	meshList[GEO_WALL_2] = MeshBuilder::GenerateOBJ("Wall2", "OBJ//Wall//Wall2.obj");
	meshList[GEO_WALL_2]->textureID = LoadTGA("Image//Wall//Wall.tga");
	WallsObj[5] = new GameObject("Wall2", Vector3(-390,0,-50));
	WallsObj[5]->setCollider(125,15);
	WallsObj[5]->updateCurPos();
	WallsObj[6] = new GameObject("Wall2", Vector3(-390, 0, 50));
	WallsObj[6]->setCollider(125, 15);
	WallsObj[6]->updateCurPos();
	//TURRETS
	meshList[GEO_TURRETHEAD_2] = MeshBuilder::GenerateOBJ("Turret", "OBJ//Enemy//Turret_head.obj");
	meshList[GEO_TURRETHEAD_2]->textureID = LoadTGA("Image//Enemy//Turret_Head.tga");
	meshList[GEO_TURRETBODY_2] = MeshBuilder::GenerateOBJ("Turret", "OBJ//Enemy//Turret_body.obj");
	meshList[GEO_TURRETBODY_2]->textureID = LoadTGA("Image//Enemy//Turret_Body.tga");
	turret[5] = new Enemy("Turret", Vector3(300, 0, 300));
	turret[5]->setCollider(10,10);
	turret[5]->updateCurPos();
	turret[1] = new Enemy("Turret", Vector3(300,0,300));
	//TURRETS_2
	turret[6] = new Enemy("Turret", Vector3(-300, 0, -300));
	turret[6]->setCollider(10, 10);
	turret[6]->updateCurPos();
	turret[2] = new Enemy("Turret", Vector3(-300, 0, -300));
	//TURRETS_3
	turret[7] = new Enemy("Turret", Vector3(300, 0, -300));
	turret[7]->setCollider(10, 10);
	turret[7]->updateCurPos();
	turret[3] = new Enemy("Turret", Vector3(300, 0, -300));
	//TURRETS_4
	turret[8] = new Enemy("Turret", Vector3(-300, 0, 300));
	turret[8]->setCollider(10, 10);
	turret[8]->updateCurPos();
	turret[4] = new Enemy("Turret", Vector3(-300, 0, 300));

	//Door
	meshList[GEO_DOOR_FRAME] = MeshBuilder::GenerateOBJ("DoorFrame", "OBJ//Door//doorframe.obj");
	meshList[GEO_DOOR_FRAME]->textureID = LoadTGA("Image//Door//Door_Frame.tga");
	WallsObj[7] = new GameObject("DoorFrame", Vector3(0,0,-325));
	meshList[GEO_DOOR] = MeshBuilder::GenerateOBJ("Door", "OBJ//Door//Half_door.obj");
	meshList[GEO_DOOR]->textureID = LoadTGA("Image//Door//Door.tga");
	WallsObj[8] = new GameObject("Door", Vector3(-325, 0, 20));
	WallsObj[8]->setCollider(35, 90);
	WallsObj[8]->updateCurPos();
	WallsObj[9] = new GameObject("Door", Vector3(-325, 0, -20));
	WallsObj[9]->setCollider(35, 90);
	WallsObj[9]->updateCurPos();

	TriggerBox[0] = new Enemy("Exit", Vector3(-400,0,0));
	TriggerBox[0]->setCollider(100, 100);
	TriggerBox[0]->updateCurPos();

	meshList[GEO_CARDKEY] = MeshBuilder::GenerateOBJ("Card", "OBJ//NPC//keycard.obj");
	meshList[GEO_CARDKEY]->textureID = LoadTGA("Image//model//RedKey.tga");
	Keys[1] = new Item("Card", Vector3(-100,0,0), "Getting a Key");
	Keys[1]->setCollider(10, 10);
	Keys[1]->SetCollected(false);
	Keys[1]->updateCurPos();

	//Bullets
	meshList[GEO_BULLET] = MeshBuilder::GenerateOBJ("Bullet", "OBJ//Enemy//Bullet.obj");
	meshList[GEO_BULLET]->textureID = LoadTGA("Image//Enemy//Bullet.tga");
	bullet[1] = new Bullet("Bullet_1", Vector3(turret[1]->Position.x, turret[1]->Position.y, turret[1]->Position.z));
	bullet[1]->setCollider(5, 5);
	bullet[1]->updateCurPos();

	bullet[2] = new Bullet("Bullet_1", Vector3(turret[2]->Position.x, turret[2]->Position.y, turret[2]->Position.z));
	bullet[2]->setCollider(5, 5);
	bullet[2]->updateCurPos();

	bullet[3] = new Bullet("Bullet_1", Vector3(turret[3]->Position.x, turret[3]->Position.y, turret[3]->Position.z));
	bullet[3]->setCollider(5, 5);
	bullet[3]->updateCurPos();

	bullet[4] = new Bullet("Bullet_1", Vector3(turret[4]->Position.x, turret[4]->Position.y, turret[4]->Position.z));
	bullet[4]->setCollider(5, 5);
	bullet[4]->updateCurPos();

	bullet[5] = new Bullet("Bullet_1", Vector3(turret[9]->Position.x, turret[9]->Position.y, turret[9]->Position.z));
	bullet[5]->setCollider(30,30);
	bullet[5]->updateCurPos();

	meshList[GEO_BLASTER] = MeshBuilder::GenerateOBJ("Blaster", "OBJ//Player//blaster.obj");
	meshList[GEO_BLASTER]->textureID = LoadTGA("Image//Player//blaster.tga");
	meshList[GEO_PBULLET] = MeshBuilder::GenerateOBJ("Blaster", "OBJ//Player//Player_Bullet.obj");
	meshList[GEO_PBULLET]->textureID = LoadTGA("Image//Player//Player_Bullet.tga");

	meshList[GEO_BATTERY] = MeshBuilder::GenerateOBJ("Battery", "OBJ//NPC//Battery_Clips.obj");
	meshList[GEO_BATTERY]->textureID = LoadTGA("Image//Player//Battery_Clips.tga");
	Keys[2] = new Item("Battery", Vector3(300, 0, 0), "Getting a Battery");
	Keys[2]->setCollider(10, 10);
	Keys[2]->updateCurPos();

	Keys[3] = new Item("Battery", Vector3(0, 0, 300), "Getting a Battery");
	Keys[3]->setCollider(10, 10);
	Keys[3]->updateCurPos();

	Keys[4] = new Item("Battery", Vector3(0, 0, -300), "Getting a Battery");
	Keys[4]->setCollider(10, 10);
	Keys[4]->updateCurPos();

	meshList[GEO_RECOVERY] = MeshBuilder::GenerateOBJ("Recovery", "OBJ//Player//Can.obj");
	//meshList[GEO_RECOVERY]->textureID = LoadTGA("Image//Player//.tga");
	Keys[5] = new Item("Battery", Vector3(100, 0, -300), "Getting a Battery");
	Keys[5]->setCollider(10, 10);
	Keys[5]->updateCurPos();


	meshList[GEO_DEBRIS] = MeshBuilder::GenerateOBJ("Debris", "OBJ//Objects//Debris.obj");
	meshList[GEO_DEBRIS]->textureID = LoadTGA("Image//floor//floor.tga");
	WallsObj[10] = new GameObject("Debris", Vector3(200,0,200));
	WallsObj[10]->setCollider(50,50);
	WallsObj[10]->updateCurPos();

	WallsObj[11] = new GameObject("Debris", Vector3(-200, 0,-200));
	WallsObj[11]->setCollider(50, 50);
	WallsObj[11]->updateCurPos();

	WallsObj[12] = new GameObject("Debris", Vector3(200, 0,-200));
	WallsObj[12]->setCollider(50, 50);
	WallsObj[12]->updateCurPos();
	
	WallsObj[13] = new GameObject("Debris", Vector3(-200,0, 200));
	WallsObj[13]->setCollider(50, 50);
	WallsObj[13]->updateCurPos();
}

void Scene4_Boss::EnemyField()
{
	//BOSS
	modelStack.PushMatrix();
	modelStack.Translate(turret[10]->Position.x, turret[10]->Position.y, turret[10]->Position.z);
	modelStack.Scale(8, 8, 8);
	RenderMesh(meshList[GEO_TURRETBODY], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(turret[9]->Position.x, turret[9]->Position.y + 1, turret[9]->Position.z);
	modelStack.Rotate(180, 0, 1, 0);
	modelStack.Rotate(turret[9]->RotateToPlayer(camera.position), 0, 1, 0);
	modelStack.Scale(7, 7, 7);
	RenderMesh(meshList[GEO_TURRETHEAD], true);
	modelStack.PopMatrix();
	
	if (bullet[5]->shoot)
	{
		modelStack.PushMatrix();
		modelStack.Translate(bullet[5]->Position.x, bullet[5]->Position.y - 25 , bullet[5]->Position.z - 2);
		modelStack.Rotate(bullet[5]->rotation, 0, 1, 0);
		modelStack.Scale(5,5,5);
		RenderMesh(meshList[GEO_BULLET], false);
		modelStack.PopMatrix();
	}
	//TURRETS_1
	modelStack.PushMatrix();
	modelStack.Translate(turret[1]->Position.x, turret[1]->Position.y, turret[1]->Position.z);
	modelStack.Rotate(turret[1]->RotateToPlayer(camera.position), 0, 1, 0);
	modelStack.Scale(3, 3, 3);
	RenderMesh(meshList[GEO_TURRETHEAD_2], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(turret[5]->Position.x, turret[5]->Position.y, turret[5]->Position.z);
	modelStack.Scale(3, 3, 3);
	RenderMesh(meshList[GEO_TURRETBODY_2], true);
	modelStack.PopMatrix();

	if (bullet[1]->shoot)
	{
		modelStack.PushMatrix();
		modelStack.Translate(bullet[1]->Position.x, bullet[1]->Position.y, bullet[1]->Position.z);
		modelStack.Rotate(bullet[1]->rotation, 0, 1, 0);
		RenderMesh(meshList[GEO_BULLET], false);
		modelStack.PopMatrix();
	}

	//TURRETS_2
	modelStack.PushMatrix();
	modelStack.Translate(turret[2]->Position.x, turret[2]->Position.y, turret[2]->Position.z);
	modelStack.Rotate(turret[2]->RotateToPlayer(camera.position), 0, 1, 0);
	modelStack.Scale(3, 3, 3);
	RenderMesh(meshList[GEO_TURRETHEAD_2], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(turret[6]->Position.x, turret[6]->Position.y, turret[6]->Position.z);
	modelStack.Scale(3, 3, 3);
	RenderMesh(meshList[GEO_TURRETBODY_2], true);
	modelStack.PopMatrix();

	if (bullet[2]->shoot)
	{
		modelStack.PushMatrix();
		modelStack.Translate(bullet[2]->Position.x, bullet[2]->Position.y, bullet[2]->Position.z);
		modelStack.Rotate(bullet[2]->rotation, 0, 1, 0);
		RenderMesh(meshList[GEO_BULLET], false);
		modelStack.PopMatrix();
	}

	//TURRETS_3
	modelStack.PushMatrix();
	modelStack.Translate(turret[3]->Position.x, turret[3]->Position.y, turret[3]->Position.z);
	modelStack.Rotate(turret[3]->RotateToPlayer(camera.position), 0, 1, 0);
	modelStack.Scale(3, 3, 3);
	RenderMesh(meshList[GEO_TURRETHEAD_2], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(turret[7]->Position.x, turret[7]->Position.y, turret[7]->Position.z);
	modelStack.Scale(3, 3, 3);
	RenderMesh(meshList[GEO_TURRETBODY_2], true);
	modelStack.PopMatrix();


	if (bullet[3]->shoot)
	{
		modelStack.PushMatrix();
		modelStack.Translate(bullet[3]->Position.x, bullet[3]->Position.y, bullet[3]->Position.z);
		modelStack.Rotate(bullet[3]->rotation, 0, 1, 0);
		RenderMesh(meshList[GEO_BULLET], false);
		modelStack.PopMatrix();
	}

	//TURRETS_4
	modelStack.PushMatrix();
	modelStack.Translate(turret[4]->Position.x, turret[4]->Position.y, turret[4]->Position.z);
	modelStack.Rotate(turret[4]->RotateToPlayer(camera.position), 0, 1, 0);
	modelStack.Scale(3,3,3);
	RenderMesh(meshList[GEO_TURRETHEAD_2], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(turret[8]->Position.x, turret[8]->Position.y, turret[8]->Position.z);
	modelStack.Scale(3, 3, 3);
	RenderMesh(meshList[GEO_TURRETBODY_2], true);
	modelStack.PopMatrix();
	
	if (bullet[4]->shoot)
	{
		modelStack.PushMatrix();
		modelStack.Translate(bullet[4]->Position.x, bullet[4]->Position.y, bullet[4]->Position.z);
		modelStack.Rotate(bullet[4]->rotation, 0, 1, 0);
		RenderMesh(meshList[GEO_BULLET], false);
		modelStack.PopMatrix();
	}

	if (!Keys[1]->isCollected())
	{
		modelStack.PushMatrix();
		modelStack.Translate(Keys[1]->Position.x, Keys[1]->Position.y, Keys[1]->Position.z);
		modelStack.Rotate(90 * rotateAngle, 0, 1, 0);
		modelStack.Rotate(15,1,0,0);
		modelStack.Scale(scaleAll, scaleAll, scaleAll);
		RenderMesh(meshList[GEO_CARDKEY], true);
		modelStack.PopMatrix();
	}

	modelStack.PushMatrix();
	modelStack.Translate(Keys[2]->Position.x, Keys[2]->Position.y, Keys[2]->Position.z);
	modelStack.Rotate(90 * rotateAngle, 0, 1, 0);
	RenderMesh(meshList[GEO_BATTERY], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(Keys[3]->Position.x, Keys[3]->Position.y, Keys[3]->Position.z);
	modelStack.Rotate(90 * rotateAngle, 0, 1, 0);
	RenderMesh(meshList[GEO_BATTERY], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(Keys[4]->Position.x, Keys[4]->Position.y, Keys[4]->Position.z);
	modelStack.Rotate(90 * rotateAngle, 0, 1, 0);
	RenderMesh(meshList[GEO_BATTERY], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(Keys[5]->Position.x, Keys[5]->Position.y, Keys[5]->Position.z);
	modelStack.Rotate(90 * rotateAngle, 0, 1, 0);
	RenderMesh(meshList[GEO_RECOVERY], true);
	modelStack.PopMatrix();
}

void Scene4_Boss::Render()
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

	//-------------------------------------------------------------------------------------	
	skyBox();
	Walls();
	EnemyField();

	//GUN Bullets
	for (size_t i = 0; i < clipSize; i++)
	{
		if (lasergun->pBullet[i]->shot() == true)
		{
			modelStack.PushMatrix();
			//modelStack.Translate(-2.f, -.8f, 2.f); //Pos of cur gun
			
			modelStack.Translate(lasergun->pBullet[i]->Position.x, lasergun->pBullet[i]->Position.y, lasergun->pBullet[i]->Position.z); //Forward trnaslate
			//modelStack.Translate(lasergun->Position.x, lasergun->Position.y, lasergun->Position.z);
			modelStack.Scale(0.5f, 0.5f, 0.5f);
			RenderMesh(meshList[GEO_PBULLET], true);
			modelStack.PopMatrix();
		}
	}
	//Test gun
	modelStack.PushMatrix();
	modelStack.LoadMatrix(lasergun->rotateGunToCamera(camera.position, camera.up, camera.target)); //Parent to cam
	glDisable(GL_DEPTH_TEST); //Gun forever renders
	glDisable(GL_CULL_FACE);
	modelStack.Translate(lasergun->Position.x, lasergun->Position.y, lasergun->Position.z); //Translate to a proper position
	modelStack.Rotate(180, 0, 1, 0); //Gun is inverted
	RenderMesh(meshList[GEO_BLASTER], true);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	modelStack.PopMatrix();

	RenderMeshOnScreen(meshList[GEO_HEALTHBG], 15, 5, 30, 30, false);
	RenderMeshOnScreen(meshList[GEO_HEALTH], 8, 5, HPsizeX, 30, true);
	RenderMeshOnScreen(meshList[GEO_STAMINA], 8.1f, 5.f, camera.test2, 30.f, true);
	RenderMeshOnScreen(meshList[GEO_AMMOBG], 65, 10, 30, 30, false);

	RenderTextOnScreen(meshList[GEO_TEXT], clipCount, Color(0, 1, 0), 5, 58, 5);
	RenderTextOnScreen(meshList[GEO_TEXT], AmmoLeft, Color(0, 1, 0), 3, 65.5f, 0.5f);
	
	RenderMeshOnScreen(meshList[GEO_BOSSTESTBG], 40, 55, test2, 30, false);
	RenderMeshOnScreen(meshList[GEO_BOSSTEST], 40, 55, test3, 30, false);
	//-------------------------------------------------------------------------------------
}

void Scene4_Boss::RenderMesh(Mesh *mesh, bool enableLight)
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

void Scene4_Boss::RenderText(Mesh* mesh, std::string text, Color color)
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

void Scene4_Boss::RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y)
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
	modelStack.Translate(x, y, 0);
	modelStack.Scale(size, size, size);


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

void Scene4_Boss::RenderMeshOnScreen(Mesh* mesh, float x, float y, float sizex, float sizey, bool isHealth)
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

void Scene4_Boss::Exit()
{
	for (int i = 0; i < NUM_GEOMETRY; ++i)
	{
		if (meshList[i] != NULL)
			delete meshList[i];

		if (object[i] != 0)
			delete object[i];
	}
	//Keys
	if (Keys[1] != 0)
		delete Keys[1];
	//TriggerBox
	if (TriggerBox[0] != 0)
		delete TriggerBox[0];
	
	delete player;
	delete lasergun;
	// Cleanup VBO here
	glDeleteVertexArrays(1, &m_vertexArrayID);

	//Cleanup color
	glDeleteProgram(m_programID);
}