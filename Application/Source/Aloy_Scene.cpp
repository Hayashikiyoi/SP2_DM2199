#include "Aloy_Scene.h"
#include "GL\glew.h"
#include "Mtx44.h"
#include "Application.h"

#include "shader.hpp"

#include "MeshBuilder.h"
#include "LoadTGA.h"

#include "MyMath.h"
using namespace Math;

Aloy_Scene::Aloy_Scene()
{
}

Aloy_Scene::~Aloy_Scene()
{
}

void Aloy_Scene::Init()
{
	// Init VBO here
	rotateAngle = 0;
	translateX[0] = 0;
	translateX[1] = 0;
	translateX[2] = 0;
	scaleAll = 1;
	TextSize = 3;
	TextSize_2 = 0;
	TextSize_3 = 0;
	MenuSelect = 0;
	Translating[0] = 0;
	Translating[1] = 0;
	TitleCheck = false;
	TextChecking = true;
	TextSwitching = false;

	lightEnable = true;

	//Emable depth test
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE); //Deletes the backface
	glEnable(GL_BLEND);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//Initialise camera
	camera.Init(Vector3(0,0,0), Vector3(0,0,-10),Vector3(0,1,0));
	//Set background color to dark blue
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	//Generate a default VAO for now
	glGenVertexArrays(1, &m_vertexArrayID);
	glBindVertexArray(m_vertexArrayID);

	for (int i = 0; i < NUM_GEOMETRY; ++i)
		meshList[i] = NULL;

	meshList[GEO_AXES] = MeshBuilder::GenerateAxes("reference", 1000, 1000, 1000);
	meshList[GEO_TITLE] = MeshBuilder::GenerateQuad("title", Color(1, 1, 1), 1, 1);
	meshList[GEO_TITLE]->textureID = LoadTGA("Image//Text//Title.tga");

	meshList[GEO_FRONT] = MeshBuilder::GenerateQuad("front", Color(1, 1, 1), 1.f, 1.f);
	meshList[GEO_FRONT]->textureID = LoadTGA("Image//skybox//city_bk.tga");
	
	meshList[GEO_CINAMATIC] = MeshBuilder::GenerateOBJ("Object", "OBJ//Objects//menu.obj");
	meshList[GEO_CINAMATIC]->textureID = LoadTGA("Image//model//Menutransition.tga");

	meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[GEO_TEXT]->textureID = LoadTGA("Image//Text//gothiclight.tga");

	meshList[GEO_TEXT_1] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[GEO_TEXT_1]->textureID = LoadTGA("Image//Text//ocrastd.tga");

	meshList[GEO_TEXT_2] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[GEO_TEXT_2]->textureID = LoadTGA("Image//Text//ocrastd.tga");

	//Load vertex and fragment shaders
	m_programID = LoadShaders("Shader//Texture.vertexshader", "Shader//Text.fragmentshader");
	m_parameters[U_MVP] = glGetUniformLocation(m_programID, "MVP");

	// Get a handle for our "colorTexture" uniform
	m_parameters[U_COLOR_TEXTURE_ENABLED] = glGetUniformLocation(m_programID, "colorTextureEnabled");
	m_parameters[U_COLOR_TEXTURE] = glGetUniformLocation(m_programID, "colorTexture");

	// Get a handle for our "TextColor" uniform
	m_parameters[U_TEXT_ENABLED] = glGetUniformLocation(m_programID, "textEnabled");
	m_parameters[U_TEXT_COLOR] = glGetUniformLocation(m_programID, "textColor");

	glUseProgram(m_programID);

	Mtx44 projection;
	projection.SetToPerspective(45.0f, 4.0f / 3.0f, 0.1f, 4000.0f);
	projectionStack.LoadMatrix(projection);
}

void Aloy_Scene::Update(double dt)
{
	Delaytimer[1] += (float)dt;

	if (Delaytimer[1] < 12)
	{
		Translating[0] -= (float)dt;
	}
	if (Delaytimer[1] > 12)
	{
		Translating[1] -= (float)dt;
		Translating[0] = 0;
	}
	if (Delaytimer[1] > 23)
	{
		Translating[1] = 0;
		Delaytimer[1] = 0;
	}
	// Title will appeared after 10 seconds (Check with DelayTimer at the else statement)
	if (TitleCheck == true)
	{
		titlescale = 50;
		if (TextChecking == true)
		{
			TextSize = 3;
			//Pressing Enter will change to Start and Quit game option
			if (Application::IsKeyPressed(VK_RETURN))
			{
				TextSize = 0;
				Delaytimer[0] = 0;
				TextChecking = false;
			}
		}
		else if (TextChecking == false)
		{
			bSomethingHappen = false;
			switch (MenuSelect)
			{
			case 0:
				bSomethingHappen = true;
				TextSize_2 = 3;
				if (Delaytimer[0] > 0.125)
				{
					if (Application::IsKeyPressed(VK_RETURN))
					{
						SceneManager::instance()->changeScene(2);
						return;
					}
					else if (Application::IsKeyPressed(VK_LEFT) || Application::IsKeyPressed(VK_RIGHT))
					{

						Delaytimer[0] = 0;
						TextSize_2 = 0;
						TextSize_3 = 3;
						MenuSelect = 1;
					}
				}
				break;
			case 1:
				bSomethingHappen = true;
				if (Delaytimer[0] > 0.125)
				{
					if (Application::IsKeyPressed(VK_RETURN))
					{
						SceneManager::instance()->EndGame(true);
					}
					else if (Application::IsKeyPressed(VK_LEFT) || Application::IsKeyPressed(VK_RIGHT))
					{
						Delaytimer[0] = 0;
						TextSize_3 = 0;
						MenuSelect = 0;
					}
				}
				break;
			default:
				break;
			}
			if (bSomethingHappen)
			{
				Delaytimer[0] += (float)dt;
			}
		}
	}
	else
	{
		Delaytimer[0] += (float)dt;
		TextSize = 0;
		if (Delaytimer[0] > 3)
		{
			TitleCheck = true;
		}
	}
	if (Application::IsKeyPressed('1'))
		glEnable(GL_CULL_FACE);
	if (Application::IsKeyPressed('2'))
		glDisable(GL_CULL_FACE);

	//Wireframe / not wireframe
	if (Application::IsKeyPressed('3'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	if (Application::IsKeyPressed('4'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	camera.Update(dt);
}

void Aloy_Scene::Render()
{
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
	//Front
	modelStack.PushMatrix();
	//to do: transformation code here
	modelStack.Translate(camera.position.x, camera.position.y, camera.position.z - 498.f);
	modelStack.Scale(1000.f, 1000.f, 1000.f);
	RenderMesh(meshList[GEO_FRONT], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	//to do: transformation code here
	modelStack.Translate(0,0, -700);
	modelStack.Translate(0,0,-70 * Translating[0]);
	modelStack.Rotate(90,0,0,1);
	modelStack.Scale(50, 50, 50);
	RenderMesh(meshList[GEO_CINAMATIC], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	//to do: transformation code here
	modelStack.Translate(0, 0, -700);
	modelStack.Translate(0, 0, -70 * Translating[1]);
	modelStack.Scale(50, 50, 50);
	RenderMesh(meshList[GEO_CINAMATIC], false);
	modelStack.PopMatrix();
	
	RenderTextOnScreen(meshList[GEO_TEXT], deltaTime, Color(0, 1, 0), 3, 0, 0);

	//No transform needed
	RenderMeshOnScreen(meshList[GEO_TITLE], 40, 30, titlescale, titlescale);

	modelStack.PushMatrix();
	//Text Enter and Start Game
	RenderTextOnScreen(meshList[GEO_TEXT_1], "ENTER TO START", Color(0, 1, 0), TextSize, 6, 4);
	RenderTextOnScreen(meshList[GEO_TEXT_1], "Start Game" , Color(0, 1, 0), TextSize_2, 8, 4);
	RenderTextOnScreen(meshList[GEO_TEXT_2], "Quit Game", Color(0,1,0), TextSize_3 ,8.5,4);
	RenderTextOnScreen(meshList[GEO_TEXT], deltaTime, Color(0, 1, 0), 5, 0, 0);
	modelStack.PopMatrix();
	//-------------------------------------------------------------------------------------

}

void Aloy_Scene::RenderMesh(Mesh *mesh, bool enableLight)
{
	Mtx44 MVP, modelView, modelView_inverse_transpose;

	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	modelView = viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MODELVIEW], 1, GL_FALSE, &modelView.a[0]);

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

void Aloy_Scene::RenderText(Mesh* mesh, std::string text, Color color)
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

void Aloy_Scene::RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y)
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

void Aloy_Scene::RenderMeshOnScreen(Mesh* mesh, int x, int y, int sizex, int sizey)
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


void Aloy_Scene::Exit()
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