#ifndef CAMERA4_H
#define CAMERA4_H

#include "Vector3.h"
#include "Camera2.h"

#include "Mtx44.h"

class Camera4: public Camera
{
public:
	Vector3 position;
	Vector3 target;
	Vector3 up;
	Vector3 view, right;
	Mtx44 rotate;

	Camera4();
	~Camera4();
	virtual void Init(const Vector3& pos, const Vector3& target, const Vector3& up);
	virtual void Reset();
	virtual void Update(double dt);
};

#endif