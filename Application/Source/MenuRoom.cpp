#include "MenuRoom.h"
#include "GL\glew.h"
#include "Mtx44.h"
#include "Application.h"

#include "shader.hpp"

#include "MeshBuilder.h"
#include "LoadTGA.h"

#include "MyMath.h"
using namespace Math;

Menu_Room::Menu_Room()
{
	/*m_programID = (SceneManager::instance()->programID);*/
}

Menu_Room::~Menu_Room()
{
}

void Menu_Room::Init()
{
	// Init VBO here
	lightEnable = true;

	//Emable depth test
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE); //Deletes the backface
	glEnable(GL_BLEND);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	//translator
	TranslateCode = 100;

	//Initialise camera
	camera.Init(Vector3(0, 10, -400), Vector3(0, 0, -10), Vector3(0, 1, 0));
	player = new Player("camera", Vector3(0, 0, -400));
	lasergun = new Weapon("Blaster", Vector3(-0.12f, -0.750, 2), 100);

	//Set background color to dark blue
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	//Generate a default VAO for now
	glGenVertexArrays(1, &m_vertexArrayID);
	glBindVertexArray(m_vertexArrayID);

	//They are moved to 
	initializeObjects();
	GenerateSkybox();
	GenerateGEOMESH();
	GenerateOBJ();

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

	light[0].type = Light::LIGHT_DIRECTIONAL;
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
	projection.SetToPerspective(45.0f, 4.0f / 3.0f, 0.1f, 2000.0f);
	projectionStack.LoadMatrix(projection);
}

void Menu_Room::Update(double dt)
{
	static float translateLimit = 1;

	//deltaTime = "FPS:" + std::to_string(1 / dt);

	cordx = "X: " + std::to_string(camera.position.x);
	cordz = "Z: " + std::to_string(camera.position.z);
	static float LSPEED = 10;

	static float change = 1;
	TranslateCode += 25 * (float)dt * change;
	if (TranslateCode > 100)
	{
		change = -1;
		//TranslateCode = 0;
		
	}
	else if (TranslateCode < -100)
	{
		change = 1;
	}

	if (Application::IsKeyPressed('V'))
		lightEnable = false;
	if (Application::IsKeyPressed('B'))
		lightEnable = true;

	//if (Application::IsKeyPressed('Y'))
	//	coverOpened = true;

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

//----------------------------------------------------------------------------
	player->Position = camera.position;
	player->updateCurPos();

	static Vector3 prevpos;
	static Vector3 prevposTarget;
	//Run checker
	for (int i = 0; i < NUM_GEOMETRY; ++i)
	{
		if (object[i] && player->trigger(object[i]))
		{
			camera.position = prevpos;
			camera.target = prevposTarget;
			break;
		}
	}

	for (int i = 0; i < numOfEnemy; ++i)
	{
		if (turret[i] && player->trigger(turret[i]))
		{
			//if you need to get pushed out of the collider
			deltaTime = "Wham bam";
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
//----------------------------------------------------------------------------
	object[GEO_BOTTOMVOTEX]->rotation += (float)dt;
	object[GEO_VOTEX]->rotation -= (float)dt;
	deltaTime = "";
	static bool openDoor = false;
	for (int i = 0; i < 6; ++i)
	{
		if (TriggerBox[i] && player->trigger(TriggerBox[i]))
		{
			if (i>=0 && i<=3 && i == SceneManager::instance()->levelCompleted)
				deltaTime = ("Press E: Level " + std::to_string(i+3));
			if (Application::IsKeyPressed('E') && i >= 0 && i <= 3 && i == SceneManager::instance()->levelCompleted)
			{
				SceneManager::instance()->changeScene(i + 3);
				return;
			}
			/*if (i == 3)
				deltaTime = "Press E: Level_3";
			if (Application::IsKeyPressed('E') && i == 3)
				SceneManager::instance()->changeScene(6);*/
			if (i == 4)
				deltaTime = "Press E: Unlock";
			if (Application::IsKeyPressed('E') && i == 4 && SceneManager::instance()->levelCompleted  == 4)
			{
				//turret[14]->Position.z = 1000;
				//turret[14]->Position.y = -100;
				//turret[16]->Position.z = 1000;
				//turret[16]->Position.y = -100;
				openDoor = true;
			}
			std::cout << deltaTime  << std::endl;
			break;
		}
	}

	if (openDoor && turret[14]->Position.y >= -100)
	{
		turret[14]->Position.y -= 10 * dt;
		turret[16]->Position.y -= 10 * dt;
	}
	if (turret[14]->Position.y <= -100)
	{
		turret[14]->Position.z = 1000;
		turret[14]->Position.y = -100;
		turret[14]->updateCurPos();
		turret[16]->Position.z = 1000;
		turret[16]->Position.y = -100;
		turret[16]->updateCurPos();
	}
	//----------------------------------------------------------------------------
	//Gun Stuffs------------------------------------------------------------------
	static float ROF = 0.125f;
	static float TP = 0;
	static bool shoot = false;
	if (Application::IsKeyPressed(VK_LBUTTON) && !shoot)
	{
		lasergun->shoot(&camera);
		SceneManager::instance()->levelCompleted += 1; //Put here to test working drawBridge
		shoot = true;
	}
	if (Application::IsKeyPressed('T'))
	{
		SceneManager::instance()->levelCompleted = 0;
		lasergun->reload();
	}
	if (Application::IsKeyPressed('F'))
	{
		lasergun->pickupClip();
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
	ammoLeft = std::to_string(lasergun->bulletLeft()) + "/45";
	clipCount = std::to_string(lasergun->canisterLeft());
	//----------------------------------------------------------------------------
	//----------------------------------------------------------------------------
	camera.Update(dt);
	lasergun->updateBullet(dt);
}

void Menu_Room::Render()
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
	//RenderSkybox();
	Walls();
	EnemyField();
	DrawBridge();
	BridgeGate();

	for (size_t i = 0; i < clipSize; i++)
	{
		if (lasergun->pBullet[i]->shot() == true)
		{
			modelStack.PushMatrix();
			modelStack.Translate(lasergun->pBullet[i]->Position.x, lasergun->pBullet[i]->Position.y, lasergun->pBullet[i]->Position.z); //Forward trnaslate
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

	//No transform needed
	RenderTextOnScreen(meshList[GEO_TEXT], deltaTime, Color(0, 1, 0), 3, 1, 7);
	
	//RenderTextOnScreen(meshList[GEO_TEXT], cordx, Color(0, 1, 0), 3, 0, 3);
	//RenderTextOnScreen(meshList[GEO_TEXT], cordz, Color(0, 1, 0), 3, 0, 5);
	//RenderMeshOnScreen(meshList[GEO_QUAD], 0, 0, 1, 1, true);
	RenderMeshOnScreen(meshList[GEO_HEALTHBG], 15, 5, 30, 30, false);
	RenderMeshOnScreen(meshList[GEO_HEALTH], 7.7f, 5, HPsizeX, 30, true);
	RenderMeshOnScreen(meshList[GEO_STAMINA], 7.7f, 5, camera.test2, 30, true);

	RenderMeshOnScreen(meshList[GEO_AMMOBG], 65, 10, 30, 30, false);
	RenderTextOnScreen(meshList[GEO_TEXT], clipCount, Color(0, 1, 0), 5, 58, 5);
	RenderTextOnScreen(meshList[GEO_TEXT], ammoLeft, Color(0, 1, 0), 3, 65.5f, 0.5f);
	/*RenderMeshOnScreen(meshList[GEO_BOSSTESTBG], 40, 55, test2, 30, false);	
	RenderMeshOnScreen(meshList[GEO_BOSSTEST], 40, 55, test3, 30, false);*/
	//-------------------------------------------------------------------------------------
}

void Menu_Room::GenerateGEOMESH()
{
	HPsizeX = 21.5f;
	//Quad set to size 1 only
	meshList[GEO_HEALTHBG] = MeshBuilder::GenerateQuad("HealthBG", Color(1, 1, 1), 1, 1);
	meshList[GEO_HEALTHBG]->textureID = LoadTGA("Image//UI//healthBG.tga");
	meshList[GEO_HEALTH] = MeshBuilder::GenerateQuad("Health", Color(1, 1, 1), 1, 1);
	meshList[GEO_HEALTH]->textureID = LoadTGA("Image//UI//healthBar.tga");
	meshList[GEO_STAMINA] = MeshBuilder::GenerateQuad("stamina", Color(1, 1, 1), 1, 1);
	meshList[GEO_STAMINA]->textureID = LoadTGA("Image//UI//staminaBar.tga");

	meshList[GEO_AMMOBG] = MeshBuilder::GenerateQuad("ammoBG", Color(1, 1, 1), 1, 1);
	meshList[GEO_AMMOBG]->textureID = LoadTGA("Image//UI//ammoBG.tga");

	/*meshList[GEO_BOSSTESTBG] = MeshBuilder::GenerateQuad("ammoBG", Color(1, 1, 1), 1, 1);
	meshList[GEO_BOSSTESTBG]->textureID = LoadTGA("Image//UI//Boss//bossBG.tga");
	meshList[GEO_BOSSTEST] = MeshBuilder::GenerateQuad("ammoBG", Color(1, 1, 1), 1, 1);
	meshList[GEO_BOSSTEST]->textureID = LoadTGA("Image//UI//Boss//bossHP.tga");*/
	//------------------------------------------------------------------------------------
	meshList[GEO_AXES] = MeshBuilder::GenerateAxes("reference", 1000, 1000, 1000);

	meshList[GEO_QUAD] = MeshBuilder::GenerateQuad("quad", Color(1, 1, 1), 1, 1);
	//meshList[GEO_QUAD]->textureID = LoadTGA("Image//Text//zelda.tga");

	meshList[GEO_VOTEX] = MeshBuilder::GenerateQuad("Votex", Color(1, 1, 1), 1.f, 1.f);
	//meshList[GEO_VOTEX]->textureID = LoadTGA("Image//skybox//Portal.tga");
	meshList[GEO_VOTEX]->textureID = LoadTGA("Image//skybox//testTop1.tga");
	object[GEO_VOTEX] = new GameObject("Top Portal", Vector3(0, 0, 0));

	meshList[GEO_BOTTOMVOTEX] = MeshBuilder::GenerateQuad("VoxtexBottom", Color(1, 1, 1), 1.f, 1.f);
	//meshList[GEO_BOTTOMVOTEX]->textureID = LoadTGA("Image//skybox//Portal2.tga");
	meshList[GEO_BOTTOMVOTEX]->textureID = LoadTGA("Image//skybox//testBot1.tga");
	object[GEO_BOTTOMVOTEX] = new GameObject("Bottom Portal", Vector3(0, 0, 0));

	meshList[GEO_FLOOR] = MeshBuilder::GenerateQuad("Floor", Color(1, 1, 1), 1.f, 1.f);
	meshList[GEO_FLOOR]->textureID = LoadTGA("Image//floor//floor.tga");

	meshList[GEO_FLOOR_2] = MeshBuilder::GenerateQuad("Floor_2", Color(1, 1, 1), 1.f, 1.f);
	meshList[GEO_FLOOR_2]->textureID = LoadTGA("Image//skybox3//Floor2.tga");

	meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[GEO_TEXT]->textureID = LoadTGA("Image//Text//gothiclight.tga");

	meshList[GEO_DEBUGBOX] = MeshBuilder::GenerateCube("Debug", Color(1, 1, 1), 1.f, 1.f, 1.f);

	meshList[GEO_LIGHTBALL] = MeshBuilder::GenerateSphere("LSphere", Color(1, 1, 1), 12, 12, 1);
}
void Menu_Room::GenerateSkybox()
{
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
}
void Menu_Room::GenerateOBJ()
{
	meshList[GEO_BLASTER] = MeshBuilder::GenerateOBJ("Blaster", "OBJ//Player//blaster.obj");
	meshList[GEO_BLASTER]->textureID = LoadTGA("Image//Player//blaster.tga");
	meshList[GEO_PBULLET] = MeshBuilder::GenerateOBJ("Blaster", "OBJ//Player//Player_Bullet.obj");
	meshList[GEO_PBULLET]->textureID = LoadTGA("Image//Player//Player_Bullet.tga");

	//WALLS
	meshList[GEO_WALL] = MeshBuilder::GenerateOBJ("Wall", "OBJ//Wall//Wall.obj");
	meshList[GEO_WALL]->textureID = LoadTGA("Image//Wall//Wall.tga");
	turret[1] = new Enemy("Wall", Vector3(0, 0, -450));
	turret[1]->setCollider(1000, 15);
	turret[1]->updateCurPos();
	turret[2] = new Enemy("Wall", Vector3(0, 0, 450));
	turret[2]->setCollider(1000, 15);
	turret[2]->updateCurPos();
	turret[3] = new Enemy("Wall", Vector3(-450, 0, 0));
	turret[3]->setCollider(15, 1000);
	turret[3]->updateCurPos();
	turret[4] = new Enemy("Wall", Vector3(450, 0, 0));
	turret[4]->setCollider(15, 1000);
	turret[4]->updateCurPos();

	//ROBOT'S
	meshList[GEO_ROBOLOCKED] = MeshBuilder::GenerateOBJ("RobotLock", "OBJ//NPC//Robot_body.obj");
	meshList[GEO_ROBOLOCKED]->textureID = LoadTGA("Image//NPC//Robot_Body_Locked.tga");
	meshList[GEO_ROBOARMSLOCKED] = MeshBuilder::GenerateOBJ("RobotLock", "OBJ//NPC//Robot_Arm.obj");
	meshList[GEO_ROBOARMSLOCKED]->textureID = LoadTGA("Image/NPC//Robot_Locked.tga");
	meshList[GEO_ROBOBODY] = MeshBuilder::GenerateOBJ("Robot", "OBJ//NPC//Robot_body.obj");
	meshList[GEO_ROBOBODY]->textureID = LoadTGA("Image//NPC//Robot_Body.tga");
	meshList[GEO_ROBOARMS] = MeshBuilder::GenerateOBJ("Robot", "OBJ//NPC//Robot_Arm.obj");
	meshList[GEO_ROBOARMS]->textureID = LoadTGA("Image/NPC//Robot_Arms.tga");
	turret[5] = new Enemy("Robot", Vector3(100, 0, -300));
	turret[5]->setCollider(10, 10);
	turret[5]->updateCurPos();
	TriggerBox[0] = new GameObject("Trigger", Vector3(100, 0, -300));
	TriggerBox[0]->setCollider(20, 20);
	TriggerBox[0]->updateCurPos();
	turret[6] = new Enemy("Robot", Vector3(100, 0, -300));
	turret[6]->setCollider(10, 10);
	turret[6]->updateCurPos();
	//Robot_2
	turret[7] = new Enemy("RobotLock", Vector3(100, 0, -400));
	turret[7]->setCollider(10, 10);
	turret[7]->updateCurPos();
	TriggerBox[1] = new GameObject("Trigger", Vector3(100, 0, -400));
	TriggerBox[1]->setCollider(20, 20);
	TriggerBox[1]->updateCurPos();
	turret[8] = new Enemy("RobotLock", Vector3(100, 0, -400));
	turret[8]->setCollider(10, 10);
	turret[8]->updateCurPos();
	//Robot_3
	turret[9] = new Enemy("Robot", Vector3(-100, 0, -300));
	turret[9]->setCollider(10, 10);
	turret[9]->updateCurPos();
	TriggerBox[2] = new GameObject("Trigger", Vector3(-100, 0, -300));
	TriggerBox[2]->setCollider(20, 20);
	TriggerBox[2]->updateCurPos();
	turret[10] = new Enemy("Robot", Vector3(-100, 0, -300));
	turret[10]->setCollider(10, 10);
	turret[10]->updateCurPos();
	//Robot_4
	turret[11] = new Enemy("Robot", Vector3(-100, 0, -400));
	turret[11]->setCollider(10, 10);
	turret[11]->updateCurPos();
	TriggerBox[3] = new GameObject("Trigger", Vector3(-100, 0, -400));
	TriggerBox[3]->setCollider(20, 20);
	TriggerBox[3]->updateCurPos();
	turret[12] = new Enemy("Robot", Vector3(-100, 0, -400));
	turret[12]->setCollider(10, 10);
	turret[12]->updateCurPos();

	//Computer
	meshList[GEO_COMPUTER] = MeshBuilder::GenerateOBJ("RobotLock", "OBJ//Objects//computer.obj");
	meshList[GEO_COMPUTER]->textureID = LoadTGA("Image//model//computer.tga");
	turret[13] = new Enemy("Robot", Vector3(0, 0, 400));
	turret[13]->setCollider(10, 10);
	turret[13]->updateCurPos();

	//Doors
	meshList[GEO_DOOR] = MeshBuilder::GenerateOBJ("Door", "OBJ//Door//Door_Test_4.obj");
	//meshList[GEO_DOOR]->textureID = LoadTGA("Image//Wall//Wall.tga");
	meshList[GEO_DOOR_FRAME] = MeshBuilder::GenerateOBJ("Door_Frame", "OBJ//Door//doorframe_2.obj");
	//meshList[GEO_DOOR_FRAME]->textureID = LoadTGA("Image//Wall//Wall.tga");
	turret[14] = new Enemy("Door", Vector3(0, 0, 100));
	turret[14]->setCollider(100, 15);
	turret[14]->updateCurPos();
	turret[15] = new Enemy("DoorFrame", Vector3(0, 0, 100));
	//Door 2
	turret[16] = new Enemy("Door", Vector3(0, 0, -100));
	turret[16]->setCollider(100, 15); //Plz Change
	turret[16]->updateCurPos();
	turret[17] = new Enemy("DoorFrame", Vector3(0, 0, -100));
	TriggerBox[4] = new GameObject("Door trigger", Vector3(0, 0, -100));
	TriggerBox[4]->setCollider(100, 20);
	TriggerBox[4]->updateCurPos();

	meshList[GEO_DEBUGBOX] = MeshBuilder::GenerateCube("Box", Color(1,1,1),1,1,1);
	turret[18] = new Enemy("Door trigger", Vector3(-300, 0, 0));
	turret[18]->setCollider(500, 200);
	turret[18]->updateCurPos();

	turret[19] = new Enemy("Door trigger", Vector3(300, 0, 0));
	turret[19]->setCollider(500, 200);
	turret[19]->updateCurPos();

	meshList[GEO_CODEBLOCK] = MeshBuilder::GenerateOBJ("Code", "OBJ//Objects//codeBlock.obj");
	meshList[GEO_CODEBLOCK]->textureID = LoadTGA("Image//model//codeBlock.tga");
}

void Menu_Room::BridgeGate()
{
	modelStack.PushMatrix();
	modelStack.Translate(turret[14]->Position.x + 4, turret[14]->Position.y, turret[14]->Position.z);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Scale(5.5, 5.5, 5.5);
	RenderMesh(meshList[GEO_DOOR], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(turret[15]->Position.x + 4, turret[15]->Position.y, turret[15]->Position.z);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Scale(5.5, 5.5, 5.5);
	RenderMesh(meshList[GEO_DOOR_FRAME], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(turret[16]->Position.x + 4, turret[16]->Position.y, turret[16]->Position.z);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Scale(5, 5, 5);
	RenderMesh(meshList[GEO_DOOR], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(turret[17]->Position.x + 4, turret[17]->Position.y, turret[17]->Position.z);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Scale(5, 5, 5);
	RenderMesh(meshList[GEO_DOOR_FRAME], false);
	modelStack.PopMatrix();
}
void Menu_Room::DrawBridge()
{
	int counter = SceneManager::instance()->levelCompleted;

	for (int i = 0; i <= counter; i++)
	{
		modelStack.PushMatrix();
		modelStack.Translate(0, -1, (-100 + ((float)(i-1)*50)) );
		modelStack.Rotate(90, -1, 0, 0);
		modelStack.Scale(100.f, 100.f, 100.f);
		RenderMesh(meshList[GEO_FLOOR_2], false);
		modelStack.PopMatrix();
	}
}
void Menu_Room::initializeObjects()
{
	//Initialise ur objects here
	for (int i = 0; i < NUM_GEOMETRY; ++i)
	{
		meshList[i] = NULL;
		object[i] = 0;
	}
	for (int i = 0; i < 6; ++i)
	{
		TriggerBox[i] = 0;
	}
	for (int i = 0; i < 4; ++i)
	{
		Locked[i] = 0;
	}

	for (int i = 0; i < numOfEnemy; ++i)
	{
		turret[i] = NULL;
	}
}

void Menu_Room::RenderSkybox()
{
	RenderMesh(meshList[GEO_AXES], false);

	modelStack.PushMatrix();
	modelStack.Translate(light[0].position.x, light[0].position.y, light[0].position.z);
	RenderMesh(meshList[GEO_LIGHTBALL], false);
	modelStack.PopMatrix();

	//Bottom
	modelStack.PushMatrix();
	//to do: transformation code here
	modelStack.Translate(0, -498.f, 0);
	modelStack.Rotate(90, 0, -1, 0);
	modelStack.Rotate(90, -1, 0, 0);
	modelStack.Scale(1000.f, 1000.f, 1000.f);
	RenderMesh(meshList[GEO_BOTTOM], false);
	modelStack.PopMatrix();

	//Top
	modelStack.PushMatrix();
	//to do: transformation code here
	modelStack.Translate(0, 498.f, 0);
	modelStack.Rotate(90, 0, -1, 0);
	modelStack.Rotate(90, 1, 0, 0);
	modelStack.Scale(1000.f, 1000.f, 1000.f);
	RenderMesh(meshList[GEO_TOP], false);
	modelStack.PopMatrix();

	//Front
	modelStack.PushMatrix();
	//to do: transformation code here
	modelStack.Translate(0, 0, -498.f);
	//modelStack.Rotate(90, -1, 0, 0);
	modelStack.Scale(1000.f, 1000.f, 1000.f);
	RenderMesh(meshList[GEO_FRONT], false);
	modelStack.PopMatrix();

	//Right
	modelStack.PushMatrix();
	//to do: transformation code here
	modelStack.Translate(498.f, 0, 0);
	modelStack.Rotate(90, 0, -1, 0);
	modelStack.Scale(1000.f, 1000.f, 1000.f);
	RenderMesh(meshList[GEO_RIGHT], false);
	modelStack.PopMatrix();

	//Left
	modelStack.PushMatrix();
	//to do: transformation code here
	modelStack.Translate(-498.f, 0, 0);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Scale(1000.f, 1000.f, 1000.f);
	RenderMesh(meshList[GEO_LEFT], false);
	modelStack.PopMatrix();

	//Back
	modelStack.PushMatrix();
	//to do: transformation code here
	modelStack.Translate(0, 0, 498.f);
	modelStack.Rotate(180, 0, 1, 0);
	modelStack.Scale(1000.f, 1000.f, 1000.f);
	RenderMesh(meshList[GEO_BACK], false);
	modelStack.PopMatrix();
}

void Menu_Room::Walls()
{
	//Plane
	modelStack.PushMatrix();
	//to do: transformation code here
	modelStack.Translate(0, 0, -600);
	modelStack.Rotate(90, 0, -1, 0);
	modelStack.Rotate(90, -1, 0, 0);
	modelStack.Scale(1000.f, 1000.f, 1000.f);
	RenderMesh(meshList[GEO_FLOOR], false);
	modelStack.PopMatrix();

	//Plane 
	modelStack.PushMatrix();
	//to do: transformation code here
	modelStack.Translate(0, 0, 600);
	modelStack.Rotate(90, 0, -1, 0);
	modelStack.Rotate(90, -1, 0, 0);
	modelStack.Scale(1000.f, 1000.f, 1000.f);
	RenderMesh(meshList[GEO_FLOOR], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(turret[1]->Position.x, turret[1]->Position.y, turret[1]->Position.z);
	modelStack.Scale(10, 10, 10);
	RenderMesh(meshList[GEO_WALL], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(turret[2]->Position.x, turret[2]->Position.y, turret[2]->Position.z);
	modelStack.Scale(10, 10, 10);
	RenderMesh(meshList[GEO_WALL], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Translate(turret[3]->Position.x + 450, turret[3]->Position.y, turret[3]->Position.z - 450);
	modelStack.Scale(10, 10, 10);
	RenderMesh(meshList[GEO_WALL], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Translate(turret[4]->Position.x - 450, turret[4]->Position.y, -turret[4]->Position.z + 450);
	modelStack.Scale(10, 10, 10);
	RenderMesh(meshList[GEO_WALL], false);
	modelStack.PopMatrix();

	//TOP portal
	modelStack.PushMatrix();
	//to do: transformation code here
	modelStack.Translate(0, 300, 0);
	modelStack.Rotate(90 * (object[GEO_VOTEX]->rotation), 0, -1, 0);
	modelStack.Rotate(90, 1, 0, 0);
	modelStack.Scale(500.f, 500.f, 500.f);
	RenderMesh(meshList[GEO_VOTEX], false);
	modelStack.PopMatrix();

	//BOTtom portal
	modelStack.PushMatrix();
	//to do: transformation code here
	modelStack.Translate(0, -300.f, 0);
	modelStack.Rotate(90 * (object[GEO_BOTTOMVOTEX]->rotation), 0, -1, 0);
	modelStack.Rotate(90, -1, 0, 0);
	modelStack.Scale(1000.f, 1000.f, 1000.f);
	RenderMesh(meshList[GEO_BOTTOMVOTEX], false);
	modelStack.PopMatrix();
}

void Menu_Room::EnemyField()
{
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

	modelStack.PushMatrix();
	modelStack.Translate(turret[13]->Position.x, turret[13]->Position.y, turret[13]->Position.z);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Scale(5, 5, 5);
	RenderMesh(meshList[GEO_COMPUTER], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();

	modelStack.Translate(0,TranslateCode, 0);
	modelStack.Rotate(90,0,1,0);
	modelStack.Scale(10,10,10);
	RenderMesh(meshList[GEO_CODEBLOCK], false);
	//modelStack.Translate(turret[18]->getCollider().getmin().x, turret[18]->Position.y, turret[18]->getCollider().getmin().z);
	//modelStack.Scale(1, 100, 1);
	//RenderMesh(meshList[GEO_DEBUGBOX], false);
	//modelStack.PopMatrix();
	//modelStack.PushMatrix();
	//modelStack.Translate(turret[18]->getCollider().getmax().x, turret[18]->Position.y, turret[18]->getCollider().getmax().z);
	//modelStack.Scale(1, 100, 1);
	//RenderMesh(meshList[GEO_DEBUGBOX], false);
	modelStack.PopMatrix();
}

void Menu_Room::RenderMesh(Mesh *mesh, bool enableLight)
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
void Menu_Room::RenderText(Mesh* mesh, std::string text, Color color)
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
void Menu_Room::RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y)
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
void Menu_Room::RenderMeshOnScreen(Mesh* mesh, float x, float y, float sizex, float sizey, bool isHealth)
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
void Menu_Room::Exit()
{
	for (int i = 0; i < NUM_GEOMETRY; ++i)
	{
		if (meshList[i] != NULL)
			delete meshList[i];

		if (object[i] != 0)
			delete object[i];
	}
	for (int i = 0; i < 6; ++i)
	{
		if (TriggerBox[i] != 0)
			delete TriggerBox[i];
	}
	for (int i = 0; i < 4; ++i)
	{
		if (Locked[i] != 0)
			delete Locked[i];
	}
	delete player;
	delete lasergun;
	// Cleanup VBO here
	glDeleteVertexArrays(1, &m_vertexArrayID);

	//Cleanup color
	glDeleteProgram(m_programID);
}