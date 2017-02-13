#ifndef CAMERA4_H
#define CAMERA4_H

#include "Vector3.h"

class Camera4
{
public:
	Vector3 position;
	Vector3 target;
	Vector3 up;

	Camera4();
	~Camera4();
	virtual void Init(const Vector3& pos, const Vector3& target, const Vector3& up);
	virtual void Reset();
	virtual void Update(double dt);
};

#endif