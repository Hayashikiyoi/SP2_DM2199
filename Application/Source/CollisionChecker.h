#ifndef COLLISION_CHECKER_H
#define COLLISION_CHECKER_H

#include "MatrixStack.h"

class boxCollider
{
public:
	boxCollider();
	~boxCollider();

	Vector3 getmin(void); //returns the min value of the vector
	Vector3 getmax(void); //returns the max value of the vector
	void updatePos(Vector3 position); //Position of the object
	void setBoxSize(unsigned length, unsigned width); //Size of the collider box
	bool collidecheck(Vector3 objMin, Vector3 objMax); //Detects if a collision have happened return true or false
	
private:
	unsigned length, width;
	Vector3 colliderBoxMin;
	Vector3 colliderBoxMax;
};

#endif