#include "Scene1tutorial.h"
#include "GL\glew.h"
#include "Mtx44.h"
#include "Application.h"

#include "shader.hpp"

#include "MeshBuilder.h"
#include "LoadTGA.h"

#include "MyMath.h"
using namespace Math;

Scene1tutorial::Scene1tutorial()
{
}

Scene1tutorial::~Scene1tutorial()
{
}

void Scene1tutorial::Init()
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
	camera.Init(Vector3(-30, 30, 40), Vector3(0, 0, -10), Vector3(0, 1, 0));
	player = new Player("camera", Vector3(40, 30, 30));
	lasergun = new Weapon("Blaster", Vector3(-0.12f, -0.750, 2), 100);

	//Set background color to dark blue
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	//Generate a default VAO for now
	glGenVertexArrays(1, &m_vertexArrayID);
	glBindVertexArray(m_vertexArrayID);

	for (int i = 0; i < NUM_GEOMETRY; ++i)
	{
	    meshList[i] = NULL;
		object[i] = NULL;
		VendingMachine[i] = NULL;
	}

	CamObj = new GameObject("camera", Vector3(0, 0, 0));

	for (int i = 0; i < numOfRocks; ++i)
	{
		Rock[i] = NULL;
	}

	for (int i = 0; i < numOfBullets; ++i)
	{
		bullet[i] = NULL;
	}

	for (int i = 0; i < numOfEnemy; ++i)
	{
		turret[i] = NULL;
	}

	for (int i = 0; i < 6; ++i)
	{
		Keys[i] = NULL;
	}

	meshList[GEO_AXES] = MeshBuilder::GenerateAxes("reference", 1000, 1000, 1000);
	meshList[GEO_QUAD] = MeshBuilder::GenerateQuad("quad", Color(1, 1, 1), 1, 1);

	meshList[GEO_FRONT] = MeshBuilder::GenerateQuad("front", Color(1, 1, 1), 1.f, 1.f);
	meshList[GEO_FRONT]->textureID = LoadTGA("Image//skyboxtutorial//front.tga");

	meshList[GEO_BACK] = MeshBuilder::GenerateQuad("back", Color(1, 1, 1), 1.f, 1.f);
	meshList[GEO_BACK]->textureID = LoadTGA("Image//skyboxtutorial//back.tga");

	meshList[GEO_BOTTOM] = MeshBuilder::GenerateQuad("bottom", Color(1, 1, 1), 1.f, 1.f);
	meshList[GEO_BOTTOM]->textureID = LoadTGA("Image//skyboxtutorial//bottom.tga");

	meshList[GEO_TOP] = MeshBuilder::GenerateQuad("top", Color(1, 1, 1), 1.f, 1.f);
	meshList[GEO_TOP]->textureID = LoadTGA("Image//skyboxtutorial//top.tga");

	meshList[GEO_RIGHT] = MeshBuilder::GenerateQuad("right", Color(1, 1, 1), 1.f, 1.f);
	meshList[GEO_RIGHT]->textureID = LoadTGA("Image//skyboxtutorial//right.tga");

	meshList[GEO_LEFT] = MeshBuilder::GenerateQuad("left", Color(1, 1, 1), 1.f, 1.f);
	meshList[GEO_LEFT]->textureID = LoadTGA("Image//skyboxtutorial//left.tga");

	meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[GEO_TEXT]->textureID = LoadTGA("Image//Text//gothiclight.tga");

	meshList[GEO_DEBUGBOX] = MeshBuilder::GenerateCube("Debug", Color(1, 1, 1), 1.f, 1.f, 1.f);

	meshList[GEO_LIGHTBALL] = MeshBuilder::GenerateSphere("LSphere", Color(1, 1, 1), 12, 12, 1);

	GUI();
	GenerateOBJ();

	meshList[GEO_FLOOR] = MeshBuilder::GenerateGround("Ground", Color(1, 1, 1), 1.f, 1.f);
	meshList[GEO_FLOOR]->textureID = LoadTGA("Image//model//ground.tga");

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

	translate_DoorLeft = -11.5;
}
void Scene1tutorial::GenerateOBJ()
{
	meshList[GEO_ROCKWALL] = MeshBuilder::GenerateOBJ("Rock1", "OBJ//Wall//Rock1.obj");
	meshList[GEO_ROCKWALL]->textureID = LoadTGA("Image//Wall//Rock.tga");
	Rock[0] = new GameObject("Rock", Vector3(-1.5, 6, 20));
	Rock[0]->setCollider(10, 10);
	Rock[0]->updateCurPos();
	Rock[1] = new GameObject("Rock", Vector3(20, 6, 20));
	Rock[1]->setCollider(10, 10);
	Rock[1]->updateCurPos();

	meshList[GEO_VENDINGBODY] = MeshBuilder::GenerateOBJ("Vending machine", "OBJ//NPC//Vending_Machine.obj");
	meshList[GEO_VENDINGBODY]->textureID = LoadTGA("Image//NPC//Vending_Machine.tga");
	VendingMachine[0] = new GameObject("Vendingmachine", Vector3(-40, 7, -20));
	VendingMachine[0]->setCollider(7, 7);
	VendingMachine[0]->updateCurPos();

	meshList[GEO_VENDINGCOVER] = MeshBuilder::GenerateOBJ("Vending machine cover", "OBJ//NPC//Vending_Cover.obj");
	meshList[GEO_VENDINGCOVER]->textureID = LoadTGA("Image//NPC//Vending_Cover.tga");

	meshList[GEO_BLUEKEYCARD] = MeshBuilder::GenerateOBJ("keycard", "OBJ//NPC//keycard2.obj");
	meshList[GEO_BLUEKEYCARD]->textureID = LoadTGA("Image//model//bluekeycard.tga");
	Keys[0] = new Item("Card", Vector3(-100, 0, 0), "Getting a Key");
	Keys[0]->setCollider(10, 10);
	Keys[0]->SetCollected(false);
	Keys[0]->updateCurPos();

	meshList[GEO_BLASTER] = MeshBuilder::GenerateOBJ("Blaster", "OBJ//Player//blaster.obj");
	meshList[GEO_BLASTER]->textureID = LoadTGA("Image//Player//blaster.tga");
	meshList[GEO_PBULLET] = MeshBuilder::GenerateOBJ("Blaster", "OBJ//Player//Player_Bullet.obj");
	meshList[GEO_PBULLET]->textureID = LoadTGA("Image//Player//Player_Bullet.tga");

	meshList[GEO_BATTERY] = MeshBuilder::GenerateOBJ("Battery", "OBJ//NPC//Battery_Clips.obj");
	meshList[GEO_BATTERY]->textureID = LoadTGA("Image//Player//Battery_Clips.tga");
	Keys[1] = new Item("Battery", Vector3(300, 0, 0), "Getting a Battery");
	Keys[1]->setCollider(10, 10);
	Keys[1]->updateCurPos();

	Keys[2] = new Item("Battery", Vector3(0, 0, 300), "Getting a Battery");
	Keys[2]->setCollider(10, 10);
	Keys[2]->updateCurPos();

	Keys[3] = new Item("Battery", Vector3(0, 0, -300), "Getting a Battery");
	Keys[3]->setCollider(10, 10);
	Keys[3]->updateCurPos();


	meshList[GEO_RECOVERY] = MeshBuilder::GenerateOBJ("Recovery", "OBJ//Player//Can.obj");
	//meshList[GEO_RECOVERY]->textureID = LoadTGA("Image//Player//.tga");
	Keys[4] = new Item("Battery", Vector3(100, 0, -300), "Getting a Battery");
	Keys[4]->setCollider(10, 10);
	Keys[4]->updateCurPos();

	meshList[GEO_ROBOLOCKED] = MeshBuilder::GenerateOBJ("RobotLock", "OBJ//NPC//Robot_body.obj");
	meshList[GEO_ROBOLOCKED]->textureID = LoadTGA("Image//NPC//Robot_Body_Locked.tga");
	meshList[GEO_ROBOARMSLOCKED] = MeshBuilder::GenerateOBJ("RobotLock", "OBJ//NPC//Robot_Arm.obj");
	meshList[GEO_ROBOARMSLOCKED]->textureID = LoadTGA("Image/NPC//Robot_Locked.tga");
	meshList[GEO_ROBOBODY] = MeshBuilder::GenerateOBJ("Robot", "OBJ//NPC//Robot_body.obj");
	meshList[GEO_ROBOBODY]->textureID = LoadTGA("Image//NPC//Robot_Body.tga");
	meshList[GEO_ROBOARMS] = MeshBuilder::GenerateOBJ("Robot", "OBJ//NPC//Robot_Arm.obj");
	meshList[GEO_ROBOARMS]->textureID = LoadTGA("Image/NPC//Robot_Arms.tga");
	turret[0] = new Enemy("Robot", Vector3(100, 0, -300));
	turret[0]->setCollider(10, 10);
	turret[0]->updateCurPos();
	TriggerBox[0] = new GameObject("Trigger", Vector3(100, 0, -300));
	TriggerBox[0]->setCollider(20, 20);
	TriggerBox[0]->updateCurPos();
	turret[1] = new Enemy("Robot", Vector3(100, 0, -300));
	turret[1]->setCollider(10, 10);
	turret[1]->updateCurPos();

}

void Scene1tutorial::Update(double dt)
{
	//static float translateLimit = 1;
	static float translateDirection = 1;
	static float time;

	static Vector3 prevpos;
	static Vector3 prevposTarget;
	//Robot.Set(Robot.x, Robot.y, Robot.z);
	//deltaTime = "Health" + std::to_string(player->getHealth()); //"FPS:" + std::to_string(1 / dt);
	

	static float LSPEED = 10;

	if (Application::IsKeyPressed('V'))
		lightEnable = false;
	if (Application::IsKeyPressed('B'))
		lightEnable = true;

	DelayTimer += (float)dt;

	//if (coverOpened)
	//{
	//	if (translateLimit<-10)
	//		translateLimit *= -1;
	//	if (openCover <6)
	//		openCover += (float)(10 * translateLimit*dt);
	//}

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

	//CamObj->Position = camera.position;
	//CamObj->updateCurPos();
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
	
	//Run checker
	 

	//for (int i = 0; i < 2; ++i)
	//{
	//	/*if (Keys[i] && player->trigger(Keys[i]))
	//	{
	//		if (i == 0)
	//	     	deltaTime = "Press E";
	//		if (Application::IsKeyPressed('E') && i == 1)
	//		{
	//			Keys[1]->SetCollected(true);
	//			break;
	//		}
	//	}*/
	//}

	for (int i = 0; i < NUM_GEOMETRY; ++i)
	{
		if (object[i] && player->trigger(object[i]))
		{
			camera.position = prevpos;
			camera.target = prevposTarget;
			break;
		}

		if (VendingMachine[i] && player->trigger(VendingMachine[i]))
		{
		    camera.position = prevpos;
			camera.target = prevposTarget;
			break;
		}

	}
	for (int i = 0; i < numOfRocks; ++i)
	{
		if (Rock[i] && player->trigger(Rock[i]))
		{
			camera.position = prevpos;
			camera.target = prevposTarget;
			break;
		}
		else if (i == (numOfRocks - 1))
		{
			prevpos = camera.position;
			prevposTarget = camera.target;
		}
	}

	for (int i = 0; i < numOfEnemy; ++i)
	{
		if (turret[i] && player->trigger(turret[i]))
		{
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

	for (int i = 0; i < numOfBullets; ++i)
	{
		if (bullet[i] && player->trigger(bullet[i]))
		{
			camera.position = prevpos;
			camera.target = prevposTarget;
			break;
		}
		else if (i == (numOfBullets - 1))
		{
			prevpos = camera.position;
			prevposTarget = camera.target;
		}
	}

	for (int i = 0; i < 6; ++i)
	{
		if (Keys[i] && player->trigger(Keys[i]))
		{
			camera.position = prevpos;
			camera.target = prevposTarget;
			break;
		}
		else if (i == (6 - 1))
		{
			prevpos = camera.position;
			prevposTarget = camera.target;
		}
	}
	
	static float key_rotating = 0;


	rotateAngle += (float)(30 *dt);
	 

	coords = "x: " + std::to_string((int)(camera.position.x)) + " z: " + std::to_string((int)(camera.position.z));

	camera.Update(dt);
}

void Scene1tutorial::GUI()
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

}

void Scene1tutorial::RenderWall()
{
	for (int i = 0; i < 90; i += 10)
	{
        modelStack.PushMatrix();
		modelStack.Translate((float)i - 42, 6, 47);
		modelStack.Scale(10.f, 10.f, 10.f);
		RenderMesh(meshList[GEO_ROCKWALL], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate((float)i - 42, 6, -47);
		modelStack.Scale(10.f, 10.f, 10.f);
		RenderMesh(meshList[GEO_ROCKWALL], true);
		modelStack.PopMatrix();
	
		modelStack.PushMatrix();
		modelStack.Translate(-47, 6, (float)i - 42);
		modelStack.Scale(10.f, 10.f, 10.f);
		modelStack.Rotate(90, 0, 1, 0);
		RenderMesh(meshList[GEO_ROCKWALL], true);
		modelStack.PopMatrix();
		
		modelStack.PushMatrix();
		modelStack.Translate(47, 6, (float)i - 42);
		modelStack.Scale(10.f, 10.f, 10.f);
		modelStack.Rotate(90, 0, 1, 0);
		RenderMesh(meshList[GEO_ROCKWALL], true);
		modelStack.PopMatrix();
		
	}
    for (int i = 0; i < 30; i += 10)
	{
      	modelStack.PushMatrix();
		modelStack.Translate((float)i - 37, 6, 20);
	    modelStack.Scale(10.f, 10.f, 10.f);
		RenderMesh(meshList[GEO_ROCKWALL], true);
		modelStack.PopMatrix();
	
		modelStack.PushMatrix();
		modelStack.Translate(10, 6, (float)i + 20);
		modelStack.Scale(10.f, 10.f, 10.f);
		RenderMesh(meshList[GEO_ROCKWALL], true);
		modelStack.PopMatrix();
	}
	
	for (int i = 0; i < 40; i += 10)
	{
		modelStack.PushMatrix();
		modelStack.Translate(30, 6, (float)i - 10);
		modelStack.Scale(10.f, 10.f, 10.f);
		RenderMesh(meshList[GEO_ROCKWALL], true);
		modelStack.PopMatrix();
	}
	
	for (int i = 0; i < 40; i += 10)
	{
    	modelStack.PushMatrix();
		modelStack.Translate((float)i - 10, 6, -10);
		modelStack.Scale(10.f, 10.f, 10.f);
		RenderMesh(meshList[GEO_ROCKWALL], true);
		modelStack.PopMatrix();
	}
	
   	for (int i = 0; i < 30; i += 10)
	{
		modelStack.PushMatrix();
		modelStack.Translate(-10, 6, (float)i - 30);
		modelStack.Scale(10.f, 10.f, 10.f);
		RenderMesh(meshList[GEO_ROCKWALL], true);
		modelStack.PopMatrix();
	}
	
	for (int i = 0; i < 20; i += 10)
	{
		modelStack.PushMatrix();
		modelStack.Translate(28, 6, (float)i - 38);
		modelStack.Scale(10.f, 10.f, 10.f);
		RenderMesh(meshList[GEO_ROCKWALL], true);
		modelStack.PopMatrix();
	}
	
		modelStack.PushMatrix();
		modelStack.Translate(-1.5, 6, 20);
		modelStack.Scale(10.f, 10.f, 10.f);
		RenderMesh(meshList[GEO_ROCKWALL], true);
		modelStack.PopMatrix();
	
		modelStack.PushMatrix();
		modelStack.Translate(20, 6, 20);
		modelStack.Scale(10.f, 10.f, 10.f);
		RenderMesh(meshList[GEO_ROCKWALL], true);
		modelStack.PopMatrix();

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

void Scene1tutorial::skyBox()
{
	modelStack.PushMatrix();
	modelStack.Translate(0, 7.1f, 0);
	RenderMesh(meshList[GEO_AXES], false);
	modelStack.PopMatrix();

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
	modelStack.Translate(0, 7, 0);
	modelStack.Rotate(90, -1, 0, 0);
	modelStack.Scale(1000, 1000, 1000);
	RenderMesh(meshList[GEO_FLOOR], true);
	modelStack.PopMatrix();
}

void Scene1tutorial::RenderObjects()
{
	modelStack.PushMatrix();
	modelStack.Translate(20, 8, 40);;
	modelStack.Scale(2.5, 2.5, 2.5);
	modelStack.Rotate(rotateAngle, 0, 1, 0);
	RenderMesh(meshList[GEO_BLUEKEYCARD], true);
	modelStack.PopMatrix();
	
	modelStack.PushMatrix();
	modelStack.Translate(-40, 7, -20);
	modelStack.Scale(0.5, 0.5, 0.5);
	modelStack.Rotate(270, 0, 1, 0);
	RenderMesh(meshList[GEO_VENDINGBODY], true);
	modelStack.PushMatrix();
	RenderMesh(meshList[GEO_VENDINGCOVER], true);
	modelStack.PopMatrix();
	modelStack.PopMatrix();

	//TURRETS
	meshList[GEO_TURRETHEAD_2] = MeshBuilder::GenerateOBJ("Turret", "OBJ//Enemy//Turret_head.obj");
	meshList[GEO_TURRETHEAD_2]->textureID = LoadTGA("Image//Enemy//Turret_Head.tga");
	meshList[GEO_TURRETBODY_2] = MeshBuilder::GenerateOBJ("Turret", "OBJ//Enemy//Turret_body.obj");
	meshList[GEO_TURRETBODY_2]->textureID = LoadTGA("Image//Enemy//Turret_Body.tga");
	turret[5] = new Enemy("Turret", Vector3(300, 0, 300));
	turret[5]->setCollider(10, 10);
	turret[5]->updateCurPos();
	turret[1] = new Enemy("Turret", Vector3(300, 0, 300));

	//Bullets
	meshList[GEO_BULLET] = MeshBuilder::GenerateOBJ("Bullet", "OBJ//Enemy//Bullet.obj");
	meshList[GEO_BULLET]->textureID = LoadTGA("Image//Enemy//Bullet.tga");
	bullet[1] = new Bullet("Bullet_1", Vector3(turret[1]->Position.x, turret[1]->Position.y, turret[1]->Position.z));
	bullet[1]->setCollider(5, 5);
	bullet[1]->updateCurPos();
}

void Scene1tutorial::Render()
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
	RenderWall();
	RenderObjects();

	RenderTextOnScreen(meshList[GEO_TEXT], deltaTime, Color(0, 1, 0), 5, 0, 0);

	modelStack.PushMatrix();
	RenderTextOnScreen(meshList[GEO_TEXT], coords, Color(1, 0, 0), 2.f, 0.5, 10.f);
	modelStack.PopMatrix();

	////No transform needed
	//RenderMeshOnScreen(meshList[GEO_QUAD], 10, 10, 10, 10);
	//-------------------------------------------------------------------------------------

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
	RenderMeshOnScreen(meshList[GEO_HEALTH], 8.f, 5, HPsizeX, 30, true);
	RenderMeshOnScreen(meshList[GEO_STAMINA], 8.1f, 5, camera.test2, 30, true);
	RenderMeshOnScreen(meshList[GEO_AMMOBG], 65, 10, 30, 30, false);

	RenderTextOnScreen(meshList[GEO_TEXT], clipCount, Color(0, 1, 0), 5, 58, 5);
	RenderTextOnScreen(meshList[GEO_TEXT], ammoLeft, Color(0, 1, 0), 3, 65.5f, 0.5f);

	RenderMeshOnScreen(meshList[GEO_BOSSTESTBG], 40, 55, test2, 30, false);
	RenderMeshOnScreen(meshList[GEO_BOSSTEST], 40, 55, test3, 30, false);

	modelStack.PushMatrix();
	modelStack.Translate(turret[5]->Position.x, turret[5]->Position.y, turret[5]->Position.z);
	modelStack.Rotate(turret[5]->RotateToPlayer(camera.position), 0, 1, 0);
	modelStack.Scale(5, 5, 5);
	RenderMesh(meshList[GEO_ROBOBODY], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(turret[6]->Position.x, turret[6]->Position.y, turret[6]->Position.z);
	modelStack.Rotate(turret[6]->RotateToPlayer(camera.position), 0, 1, 0);
	modelStack.Scale(5, 5, 5);
	RenderMesh(meshList[GEO_ROBOARMS], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(turret[7]->Position.x, turret[7]->Position.y, turret[7]->Position.z);
	modelStack.Rotate(turret[7]->RotateToPlayer(camera.position), 0, 1, 0);
	modelStack.Scale(5, 5, 5);
	RenderMesh(meshList[GEO_ROBOLOCKED], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(turret[8]->Position.x, turret[8]->Position.y, turret[8]->Position.z);
	modelStack.Rotate(turret[8]->RotateToPlayer(camera.position), 0, 1, 0);
	modelStack.Scale(5, 5, 5);
	RenderMesh(meshList[GEO_ROBOARMSLOCKED], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(turret[9]->Position.x, turret[9]->Position.y, turret[9]->Position.z);
	modelStack.Rotate(turret[9]->RotateToPlayer(camera.position), 0, 1, 0);
	modelStack.Scale(5, 5, 5);
	RenderMesh(meshList[GEO_ROBOLOCKED], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(turret[10]->Position.x, turret[10]->Position.y, turret[10]->Position.z);
	modelStack.Rotate(turret[10]->RotateToPlayer(camera.position), 0, 1, 0);
	modelStack.Scale(5, 5, 5);
	RenderMesh(meshList[GEO_ROBOARMSLOCKED], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(turret[11]->Position.x, turret[11]->Position.y, turret[11]->Position.z);
	modelStack.Rotate(turret[11]->RotateToPlayer(camera.position), 0, 1, 0);
	modelStack.Scale(5, 5, 5);
	RenderMesh(meshList[GEO_ROBOLOCKED], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(turret[12]->Position.x, turret[12]->Position.y, turret[12]->Position.z);
	modelStack.Rotate(turret[12]->RotateToPlayer(camera.position), 0, 1, 0);
	modelStack.Scale(5, 5, 5);
	RenderMesh(meshList[GEO_ROBOARMSLOCKED], false);
	modelStack.PopMatrix();
}

void Scene1tutorial::RenderMesh(Mesh *mesh, bool enableLight)
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

void Scene1tutorial::RenderText(Mesh* mesh, std::string text, Color color)
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

void Scene1tutorial::RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y)
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

void Scene1tutorial::RenderMeshOnScreen(Mesh* mesh, int x, int y, int sizex, int sizey, bool isHealth)
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


void Scene1tutorial::Exit()
{
	for (int i = 0; i < NUM_GEOMETRY; ++i)
	{
		if (meshList[i] != NULL)
			delete meshList[i];
		if (VendingMachine[i] != NULL)
			delete VendingMachine[i];
	}

	for (int i = 0; i < 6; ++i)
	{
		if (Keys[i] != NULL)
			delete Keys[i];
	}

	for (int i = 0; i < numOfRocks; ++i)
	{
		if (Rock[i] != NULL)
			delete Rock[i];
	}
	for (int i = 0; i < numOfEnemy; ++i)
	{
		if (turret[i] != NULL)
			delete turret[i];
	}
	for (int i = 0; i < numOfBullets; ++i)
	{
		if (bullet[i] != NULL)
			delete bullet[i];
	}
	// Cleanup VBO here
	glDeleteVertexArrays(1, &m_vertexArrayID);

	//Cleanup color
	glDeleteProgram(m_programID);
}
