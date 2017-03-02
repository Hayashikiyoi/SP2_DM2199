#ifndef FPS_CAMERA_H
#define FPS_CAMERA_H

#include "Camera2.h"
#include "Mtx44.h"

class Fps_Camera : public Camera
{
public:
	Vector3 defaultPosition;
	Vector3 defaultTarget;
	Vector3 defaultUp;
	Vector3 view, right;
	Mtx44 rotate;
	
	Fps_Camera();
	~Fps_Camera();
	virtual void Init(const Vector3& pos, const Vector3& target, const Vector3& up);
	virtual void Update(double dt);
	bool open;
	float test2;
	bool sprintcheck;
	double xpos, ypos;
};

#endif