#include "CollisionChecker.h"

boxCollider::boxCollider() : colliderBoxMin(0), colliderBoxMax(0), length(0), width(0)
{
}

boxCollider::~boxCollider()
{
}

Vector3 boxCollider::getmin(void)
{
	return (this->colliderBoxMin);
}

Vector3 boxCollider::getmax(void)
{
	return (this->colliderBoxMax);
}

bool boxCollider::collidecheck(Vector3 objMin, Vector3 objMax)
{
	return ((colliderBoxMin.x <= objMax.x && colliderBoxMax.x >= objMin.x) &&
		(colliderBoxMin.z <= objMax.z && colliderBoxMax.z >= objMin.z));
}

void boxCollider::setBoxSize(unsigned length, unsigned width)
{
	//Size of box based on origin
	this->colliderBoxMax.x = length * 0.5f;
	this->colliderBoxMax.z = width * 0.5f;
	this->colliderBoxMin.x = length * -0.5f;
	this->colliderBoxMin.z = width * -0.5f;

	//Store length and width for later use in 
	this->length = length;
	this->width = width;
}

void boxCollider::updatePos(Vector3 position)
{
	this->colliderBoxMax.x = (this->length * 0.5f) + position.x;
	this->colliderBoxMax.z = (this->width * 0.5f) + position.z;
	this->colliderBoxMin.x = (this->length * -0.5f) + position.x;
	this->colliderBoxMin.z = (this->width * -0.5f) + position.z;
}