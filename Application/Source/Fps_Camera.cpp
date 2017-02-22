#include "Fps_Camera.h"
#include "Application.h"
#include "Mtx44.h"
#include "GLFW\glfw3.h"
#include "MyMath.h"
extern GLFWwindow* m_window;

Fps_Camera::Fps_Camera()
{
}

Fps_Camera::~Fps_Camera()
{
}

void Fps_Camera::Init(const Vector3& pos, const Vector3& target, const Vector3& up)
{
	glfwSetInputMode(m_window, GLFW_CURSOR,GLFW_CURSOR_HIDDEN);//hide the cursor
	this->position = defaultPosition = pos;
	this->target = defaultTarget = target;
	Vector3 view = (target - position).Normalized();
	Vector3 right = view.Cross(up);
	right.y = 0;
	right.Normalize();
	this->up = defaultUp = right.Cross(view).Normalized();
	this->rotate.SetToIdentity(); //I added this
}

void Fps_Camera::Update(double dt)
{
	static const float CAMERA_SPEED = 50.f;
	Vector3 view = (target - position).Normalized();
	Vector3 right = view.Cross(up);
	right.y = 0;
	right.Normalize();
	up = right.Cross(view).Normalized();

	//Checking you walking position
	position.y = 10;

	//For movement
	if (Application::IsKeyPressed('A'))
	{
		position -= right * (float)(CAMERA_SPEED * dt);
		target = position + view * (float)(CAMERA_SPEED * dt);
	}
	if (Application::IsKeyPressed('D'))
	{
		position += right * (float)(CAMERA_SPEED * dt);
		target = position + view * (float)(CAMERA_SPEED * dt);
	}
	if (Application::IsKeyPressed('W'))
	{
		position += view * (float)(CAMERA_SPEED * dt);
		target = position + view * (float)(CAMERA_SPEED * dt);
	}
	if (Application::IsKeyPressed('S'))
	{
		position -= view * (float)(CAMERA_SPEED * dt);
		target = position + view * (float)(CAMERA_SPEED * dt);
	}
	
	glfwGetCursorPos(m_window, &xpos, &ypos);
	
	int mid_x = 800 / 2;
	int mid_y = 600 / 2;

	glfwSetCursorPos(m_window, mid_x, mid_y);

	float horizMovement = Math::DegreeToRadian((float)(mid_x - xpos) * 10);
	float vertMovement = Math::DegreeToRadian((float)(mid_y - ypos) * 10);

	Mtx44 camYaw, camPitch;

	camYaw.SetToIdentity();
	camPitch.SetToIdentity();

	float angleofElevation = (view.y < 0 ? -1.0f : 1.0f)*Math::RadianToDegree(acos(Vector3(view.x, 0, view.z).Normalized().Dot(view)));

	//looking up to 45 degreed and look down to 45 degree not more not less
	if (angleofElevation + vertMovement < 45.0f && angleofElevation + vertMovement>-45.0f)
	{
		camPitch.SetToRotation(vertMovement, right.x, right.y, right.z);
	}
	camYaw.SetToRotation(horizMovement, 0, 1, 0);
	
	rotate = camPitch*camYaw;
	view = (rotate)*view;
	target = position + view;
	up = (rotate)* up;

	if (Application::IsKeyPressed('R'))
	{
		Reset();
	}
}
void Fps_Camera::Reset()
{
	position = defaultPosition;
	target = defaultTarget;
	up = defaultUp;
}
