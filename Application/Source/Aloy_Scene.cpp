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
	TextSize = 30;
	MenuSelect = 0;
	TextChecking = true;
	TextSwitching = false;

	lightEnable = true;

	//Emable depth test
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE); //Deletes the backface
	glEnable(GL_BLEND);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//Initialise camera
	camera.Init(Vector3(0, 0, 0), Vector3(0, 0, -10), Vector3(0, 1, 0));

	//Set background color to dark blue
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	//Generate a default VAO for now
	glGenVertexArrays(1, &m_vertexArrayID);
	glBindVertexArray(m_vertexArrayID);

	for (int i = 0; i < NUM_GEOMETRY; ++i)
		meshList[i] = NULL;

	//meshList[GEO_AXES] = MeshBuilder::GenerateAxes("reference", 1000, 1000, 1000);
	meshList[GEO_TITLE] = MeshBuilder::GenerateQuad("title", Color(1, 1, 1), 1, 1);
	meshList[GEO_TITLE]->textureID = LoadTGA("Image//zelda.tga");

	meshList[GEO_TEXT_1] = MeshBuilder::GenerateQuad("Text", Color(1, 1, 1), 1, 1);
	meshList[GEO_TEXT_1]->textureID = LoadTGA("Image//zelda.tga");

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
	meshList[GEO_TEXT]->textureID = LoadTGA("Image//gothiclight.tga");

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

	// Get a handle for our "colorTexture" uniform
	m_parameters[U_COLOR_TEXTURE_ENABLED] = glGetUniformLocation(m_programID, "colorTextureEnabled");
	m_parameters[U_COLOR_TEXTURE] = glGetUniformLocation(m_programID, "colorTexture");

	// Get a handle for our "TextColor" uniform
	m_parameters[U_TEXT_ENABLED] = glGetUniformLocation(m_programID, "textEnabled");
	m_parameters[U_TEXT_COLOR] = glGetUniformLocation(m_programID, "textColor");

	glUseProgram(m_programID);



	Mtx44 projection;
	projection.SetToPerspective(45.0f, 4.0f / 3.0f, 0.1f, 1000.0f);
	projectionStack.LoadMatrix(projection);
}

void Aloy_Scene::Update(double dt)
{
	deltaTime = "FPS:" + std::to_string(1 / dt);

	if (Application::IsKeyPressed(VK_F1))
		SceneManager::instance()->changeScene(1);//Test Scene

	if (TextChecking == true)
	{
		if (Application::IsKeyPressed(VK_RETURN))
		{
			TextSize = 0;
			TextChecking = false;
			Delaytimer = 0;
		}
	}
	else if (TextChecking == false)
	{
		switch (MenuSelect)
		{
		case 0:
			Delaytimer += (float)dt;
			if (Delaytimer > 1.0)
			{
				if (Application::IsKeyPressed(VK_RETURN))
					SceneManager::instance()->changeScene(0);
				else if (Application::IsKeyPressed(VK_LEFT))
					Delaytimer = 0;
					MenuSelect == 1;
			}
		case 1:
			Delaytimer += (float)dt;
			if (Delaytimer > 1.0)
			{
				if (Application::IsKeyPressed(VK_RETURN))
					//Aloy_Scene::Exit();
					MenuSelect == 0;
				else if (Application::IsKeyPressed(VK_LEFT))
					MenuSelect == 0;
			}
		}
	}

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

	RenderTextOnScreen(meshList[GEO_TEXT], deltaTime, Color(0, 1, 0), 3, 0, 0);

	//No transform needed
	RenderMeshOnScreen(meshList[GEO_TITLE], 40, 30, 40, 40);

	modelStack.PushMatrix();
	RenderMeshOnScreen(meshList[GEO_TEXT_1], 30, 30, TextSize, TextSize);
	
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