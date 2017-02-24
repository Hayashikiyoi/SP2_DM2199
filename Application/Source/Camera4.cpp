#include "Camera4.h"
#include "Application.h"

/******************************************************************************/
/*!
\brief
Default constructor
*/
/******************************************************************************/
Camera4::Camera4()
{
}

/******************************************************************************/
/*!
\brief
Destructor
*/
/******************************************************************************/
Camera4::~Camera4()
{
}

/******************************************************************************/
/*!
\brief
Initialize Camera4

\param pos - position of Camera4
\param target - where the Camera4 is looking at
\param up - up vector of Camera4
*/
/******************************************************************************/
void Camera4::Init(const Vector3& pos, const Vector3& target, const Vector3& up)
{
	view = (target - position).Normalized();
	right = view.Cross(up);

	this->position = pos;
	this->target = target;
	this->up = up;
}

/******************************************************************************/
/*!
\brief
Reset the Camera4 settings
*/
/******************************************************************************/
void Camera4::Reset()
{
}

/******************************************************************************/
/*!
\brief
To be called every frame. Camera4 will get user inputs and update its position and orientation

\param dt - frame time
*/
/******************************************************************************/
void Camera4::Update(double dt)
{
	static const float CAMERA_SPEED = 20.f;

	position += view * (float)(CAMERA_SPEED * dt);
	target = position + view * (float)(CAMERA_SPEED * dt);
}