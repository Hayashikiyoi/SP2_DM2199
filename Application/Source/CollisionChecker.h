#ifndef COLLISION_CHECKER_H
#define COLLISION_CHECKER_H

#include "MatrixStack.h"

class boxCollider
{
public:
	boxCollider();
	~boxCollider();

	bool colidecheck(Vector3 objMin, Vector3 objMax); //Strictly to detect where player is at

	Vector3 colliderBoxMin;
	Vector3 colliderBoxMax;
private:
	
};

boxCollider::boxCollider() : colliderBoxMin(0), colliderBoxMax(0)
{
}

boxCollider::~boxCollider()
{
}

bool boxCollider::colidecheck(Vector3 objMin, Vector3 objMax)
{
	return ((colliderBoxMin.x <= objMax.x && colliderBoxMax.x >= objMin.x) &&
		(colliderBoxMin.z <= objMax.z && colliderBoxMax.z >= objMin.z));
}

#endif