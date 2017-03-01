#include "tutorialScene.h"
#include "GL\glew.h"
#include "Mtx44.h"
#include "Application.h"

#include "shader.hpp"

#include "MeshBuilder.h"
#include "LoadTGA.h"

#include "MyMath.h"
using namespace Math;

tutorialScene::tutorialScene()
{
	m_programID = SceneManager::instance()->returnProg();
}

tutorialScene::~tutorialScene()
{
}

void tutorialScene::Init()
{
	// Init VBO here
	lightEnable = true;

	//Emable depth test
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE); //Deletes the backface
	glEnable(GL_BLEND);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//Initialise camera
	camera.Init(Vector3(36, 10, 0), Vector3(-10, 10, 0), Vector3(0, 1, 0));
	player = new Player("camera", Vector3(36, 10, 0));
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

void tutorialScene::Update(double dt)
{
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
	for (int i = 0; i < 5; ++i) //Collision with walls
	{
		if (walls[i] && player->trigger(walls[i]))
		{
			camera.position = prevpos;
			camera.target = prevposTarget;
			break;
		}
		else if (i == (5 - 1))//Max value thx
		{
			prevpos = camera.position;
			prevposTarget = camera.target;
		}
	}
	for (int i = 0; i < 3; ++i)
	{
		if (triggerbox[i] && player->trigger(triggerbox[i]))
		{
			//std::cout << "Hit triggerbox " << std::to_string(i) << std::endl;
			switch (i)
			{
			case 0:
				tutorial = true;
				triggerbox[0]->Position = Vector3(1000, 0, 1000); //Move tutorial box away
				triggerbox[0]->updateCurPos();
				break;
			case 1:
				SceneManager::instance()->changeScene(3); //Change to correct scene
				std::cout << "Change scene" << std::endl;
				return;
				break;
			default:
				break;
			}
			break;
		}
	}
	//----------------------------------------------------------------------------
	//Tutorial--------------------------------------------------------------------
	
	//----------------------------------------------------------------------------
	//Gun Stuffs------------------------------------------------------------------
	static float ROF = 0.125f;
	static float TP = 0;
	static bool shoot = false;
	if (Application::IsKeyPressed(VK_LBUTTON) && !shoot)
	{
		lasergun->shoot(&camera);
		shoot = true;
	}
	if (Application::IsKeyPressed('T'))
	{
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
	static float fixedTimer = 0.f;
	static bool tutorialhappen = false;
	if (tutorialhappen)
	{
		fixedTimer += dt;
		if (fixedTimer > 2.f)
		{
			fixedTimer = 0.f;
			tutorialhappen = false;
		}
	}
	if (tutorial)
	{
		//camera.Reset();
		if (Application::IsKeyPressed('K') && !tutorialhappen)
		{
			tutorialNum++;
			tutorialhappen = true;
		}
		if (tutorialNum > 2)
		{
			tutorial = false;
		}
	}
	else
	{
		camera.Update(dt);
	}
	
	lasergun->updateBullet(dt);
}

void tutorialScene::Render()
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
	RenderWalls();
	RenderFloor();
	RenderGameItems();

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
	RenderMeshOnScreen(meshList[GEO_HEALTHBG], 15, 5, 30, 30, false);
	RenderMeshOnScreen(meshList[GEO_HEALTH], 7.7f, 5, HPsizeX, 30, true);
	RenderMeshOnScreen(meshList[GEO_STAMINA], 7.7f, 5, camera.test2, 30, true);
	RenderMeshOnScreen(meshList[GEO_AMMOBG], 65, 10, 30, 30, false);
	RenderTextOnScreen(meshList[GEO_TEXT], clipCount, Color(0, 1, 0), 5, 58, 5);
	RenderTextOnScreen(meshList[GEO_TEXT], ammoLeft, Color(0, 1, 0), 3, 65.5f, 0.5f);

	//Tutorial Instructions
	if (tutorialNum == 0)
	{
		RenderMeshOnScreen(meshList[GEO_TUTORIAL1], 40, 30, 60, 60, false);
	}
	else if (tutorialNum == 1)
	{
		RenderMeshOnScreen(meshList[GEO_TUTORIAL2], 40, 30, 60, 60, false);
	}
	else if (tutorialNum == 2)
	{
		RenderMeshOnScreen(meshList[GEO_TUTORIAL3], 40, 30, 60, 60, false);
	}
	/*RenderTextOnScreen(meshList[GEO_TEXT], tempPlayerposX, Color(0, 1, 0), 3, 65.5f, 8);
	RenderTextOnScreen(meshList[GEO_TEXT], tempPlayerposZ, Color(0, 1, 0), 3, 65.5f, 10);*/
	//-------------------------------------------------------------------------------------
}

void tutorialScene::GenerateGEOMESH()
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
	//------------------------------------------------------------------------------------
	meshList[GEO_AXES] = MeshBuilder::GenerateAxes("reference", 1000, 1000, 1000);

	meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[GEO_TEXT]->textureID = LoadTGA("Image//Text//gothiclight.tga");

	meshList[GEO_DEBUGBOX] = MeshBuilder::GenerateCube("Debug", Color(1, 1, 1), 1.f, 1.f, 1.f);

	meshList[GEO_QUADFLOOR] = MeshBuilder::GenerateQuad("quad", Color(1, 1, 1), 1, 1);
	meshList[GEO_QUADFLOOR]->textureID = LoadTGA("Image//Tutorial//floor.tga");

	meshList[GEO_QUADCEILING] = MeshBuilder::GenerateQuad("quad", Color(1, 1, 1), 1, 1);
	meshList[GEO_QUADCEILING]->textureID = LoadTGA("Image//Tutorial//ceiling.tga");

	meshList[GEO_TUTORIAL1] = MeshBuilder::GenerateQuad("quad", Color(1, 1, 1), 1, 1);
	meshList[GEO_TUTORIAL1]->textureID = LoadTGA("Image//Tutorial//tutorial.tga");

	meshList[GEO_TUTORIAL2] = MeshBuilder::GenerateQuad("quad", Color(1, 1, 1), 1, 1);
	meshList[GEO_TUTORIAL2]->textureID = LoadTGA("Image//Tutorial//tutorial2.tga");

	meshList[GEO_TUTORIAL3] = MeshBuilder::GenerateQuad("quad", Color(1, 1, 1), 1, 1);
	meshList[GEO_TUTORIAL3]->textureID = LoadTGA("Image//Tutorial//tutorial3.tga");
}
void tutorialScene::GenerateSkybox()
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
void tutorialScene::GenerateOBJ()
{
	//Models that no need colliderboxes
	meshList[GEO_BLASTER] = MeshBuilder::GenerateOBJ("Blaster", "OBJ//Player//blaster.obj");
	meshList[GEO_BLASTER]->textureID = LoadTGA("Image//Player//blaster.tga");
	meshList[GEO_PBULLET] = MeshBuilder::GenerateOBJ("Blaster", "OBJ//Player//Player_Bullet.obj");
	meshList[GEO_PBULLET]->textureID = LoadTGA("Image//Player//Player_Bullet.tga");

	//Models need colliderboxes
	//-----------------------------------------------------------------------
	//Walls------------------------------------------------------------------
	meshList[GEO_WALLROW] = MeshBuilder::GenerateOBJ("WallRow", "OBJ//Tutorial//wallRow.obj");
	meshList[GEO_WALLROW]->textureID = LoadTGA("Image//Tutorial//wall.tga");
	for (int i = 0; i < 2; ++i) //0-1
	{
		Vector3 store;
		switch (i)
		{
		case 0:
			store = { 0, 8, -10 };
			break;
		case 1:
			store = { 0, 8, 10 };
			break;
		default:
			break;
		}
		walls[i] = new GameObject("WallRow", store);
		walls[i]->setCollider(84, 5);
		walls[i]->updateCurPos();
	}

	walls[2] = new GameObject("WallRow", Vector3(40,8,0));
	walls[2]->rotation = 90;
	walls[2]->setCollider(5, 21);
	walls[2]->updateCurPos();
	walls[3] = new GameObject("WallRow", Vector3(-40, 8, 0));
	walls[3]->rotation = 90;
	walls[3]->setCollider(5, 21);
	walls[3]->updateCurPos();
	//-----------------------------------------------------------------------
	//Pressure Plate---------------------------------------------------------
	meshList[GEO_BUTTON] = MeshBuilder::GenerateOBJ("Button", "OBJ//Tutorial//button.obj");
	meshList[GEO_BUTTON]->textureID = LoadTGA("Image//Tutorial//button.tga");
	object[GEO_BUTTON] = new GameObject("Button", Vector3(-30, 8, 0));
	object[GEO_BUTTON]->setCollider(2.5f, 2.5f);
	object[GEO_BUTTON]->updateCurPos();

	//OBJ Trigger boxes
	triggerbox[1] = new GameObject("ToNextLevelButton", Vector3(-30, 8, 0)); //Create a if statement to go to next level(near button)
	triggerbox[1]->setCollider(3.5f, 3.5f);
	triggerbox[1]->updateCurPos();
	
	triggerbox[0] = new GameObject("Instructions", Vector3(36, 10, 0));	//Instructions
	triggerbox[0]->setCollider(2, 2);
	triggerbox[0]->updateCurPos();
	tutorial = false;
	tutorialNum = 0;
}

void tutorialScene::initializeObjects()
{
	//Initialise ur objects here
	for (int i = 0; i < NUM_GEOMETRY; ++i)
	{
		meshList[i] = NULL;
		object[i] = 0;
	}
	for (int i = 0; i < 5; ++i)
	{
		walls[i] = 0;
	}
	for (int i = 0; i < 3; ++i)
	{
		triggerbox[i] = 0;
	}
}
void tutorialScene::RenderGameItems()
{
	modelStack.PushMatrix();
	modelStack.Translate(object[GEO_BUTTON]->Position.x, object[GEO_BUTTON]->Position.y, object[GEO_BUTTON]->Position.z);
	modelStack.Scale(0.5f, 0.5f, 0.5f);
	RenderMesh(meshList[GEO_BUTTON], true);
	modelStack.PopMatrix();
}
void tutorialScene::RenderWalls()
{
	for (int i = 0; i < 2; ++i) //First 2 walls
	{
		modelStack.PushMatrix();
		modelStack.Translate(walls[i]->Position.x, walls[i]->Position.y, walls[i]->Position.z);
		modelStack.Scale(4, 2, 1);
		RenderMesh(meshList[GEO_WALLROW], false);
		modelStack.PopMatrix();
	}
	modelStack.PushMatrix();
	modelStack.Translate(walls[2]->Position.x, walls[2]->Position.y, walls[2]->Position.z);
	modelStack.Rotate(walls[2]->rotation, 0, 1, 0);
	modelStack.Scale(1, 2, 1);
	RenderMesh(meshList[GEO_WALLROW], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(walls[3]->Position.x, walls[3]->Position.y, walls[3]->Position.z);
	modelStack.Rotate(walls[3]->rotation, 0, 1, 0);
	modelStack.Scale(1, 2, 1);
	RenderMesh(meshList[GEO_WALLROW], false);
	modelStack.PopMatrix();
}
void tutorialScene::RenderFloor()
{
	for (int i = 0; i < 5; ++i)
	{
		modelStack.PushMatrix();
		modelStack.Translate((-40+(i*20)), 8, 0);
		modelStack.Rotate(90, -1, 0, 0);
		modelStack.Scale(20, 20, 20);
		RenderMesh(meshList[GEO_QUADFLOOR], false);
		modelStack.PopMatrix();
	}

	for (int i = 0; i < 5; ++i)
	{
		modelStack.PushMatrix();
		modelStack.Translate((-40 + (i * 20)), 16, 0);
		modelStack.Rotate(90, 1, 0, 0);
		modelStack.Scale(20, 20, 20);
		RenderMesh(meshList[GEO_QUADCEILING], false);
		modelStack.PopMatrix();
	}
}
void tutorialScene::RenderSkybox()
{
	RenderMesh(meshList[GEO_AXES], false);

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

void tutorialScene::RenderMesh(Mesh *mesh, bool enableLight)
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
void tutorialScene::RenderText(Mesh* mesh, std::string text, Color color)
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
void tutorialScene::RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y)
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
void tutorialScene::RenderMeshOnScreen(Mesh* mesh, float x, float y, float sizex, float sizey, bool isHealth)
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
void tutorialScene::Exit()
{
	for (int i = 0; i < NUM_GEOMETRY; ++i)
	{
		if (meshList[i] != NULL)
			delete meshList[i];

		if (object[i] != 0)
			delete object[i];
	}
	for (int i = 0; i < 5; ++i)
	{
		if (walls[i] != 0)
			delete walls[i];
	}
	for (int i = 0; i < 3; ++i)
	{
		if (triggerbox[i] != 0)
			delete triggerbox[i];
	}
	delete player;
	delete lasergun;
	// Cleanup VBO here
	glDeleteVertexArrays(1, &m_vertexArrayID);

	//Cleanup color
	glDeleteProgram(m_programID);
}