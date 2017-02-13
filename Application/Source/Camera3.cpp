#include "Camera3.h"
#include "Application.h"
#include "Mtx44.h"

Camera3::Camera3()
{
}

Camera3::~Camera3()
{
}

void Camera3::Init(const Vector3& pos, const Vector3& target, const Vector3& up)
{
	this->position = defaultPosition = pos;
	this->target = defaultTarget = target;
	Vector3 view = (target - position).Normalized();
	Vector3 right = view.Cross(up);
	right.y = 0;
	right.Normalize();
	this->up = defaultUp = right.Cross(view).Normalized();
}

void Camera3::Update(double dt)
{
	static const float CAMERA_SPEED = 50.f;
	Vector3 view = (target - position).Normalized();
	Vector3 right = view.Cross(up);
	right.y = 0;
	right.Normalize();
	up = right.Cross(view).Normalized();
	if (Application::IsKeyPressed('A'))
	{
		position -= right * (float)(50.f * dt);
		target = position + view * (float)(50.f * dt);
	}
	if (Application::IsKeyPressed('D'))
	{
		position += right * (float)(50.f * dt);
		target = position + view * (float)(50.f * dt);
	}
	if (Application::IsKeyPressed('W'))
	{
		position += view * (float)(50.f * dt);
		target = position + view * (float)(50.f * dt);
	}
	if (Application::IsKeyPressed('S'))
	{
		position -= view * (float)(50.f * dt);
		target = position + view * (float)(50.f * dt);
	}


	if (Application::IsKeyPressed(VK_UP))
	{
		float pitch = (float)(CAMERA_SPEED * dt);
		Mtx44 rotation;
		rotation.SetToRotation(pitch, right.x, right.y, right.z);
		view = (rotation) * view;
		target = position + view;
		up = (rotation)* up;
	}
	if (Application::IsKeyPressed(VK_DOWN))
	{
		float pitch = (float)(-CAMERA_SPEED * dt);
		Mtx44 rotation;
		rotation.SetToRotation(pitch, right.x, right.y, right.z);
		view = (rotation)* view;
		target = position + view;
		up = (rotation)* up;
	}
	if (Application::IsKeyPressed(VK_LEFT))
	{
		float yaw = (float)(CAMERA_SPEED * dt);
		Mtx44 rotation;
		rotation.SetToRotation(yaw, 0, 1, 0);
		view = (rotation)* view;
		target = position + view;
		up = (rotation) * up;
	}
	if (Application::IsKeyPressed(VK_RIGHT))
	{
		float yaw = (float)(-CAMERA_SPEED * dt);
		Mtx44 rotation;
		rotation.SetToRotation(yaw, 0, 1, 0);
		view = (rotation)* view;
		target = position + view;
		up = (rotation) * up;
	}
	if (Application::IsKeyPressed('R'))
	{
		Reset();
	}
}

void Camera3::Reset()
{
	position = defaultPosition;
	target = defaultTarget;
	up = defaultUp;
}