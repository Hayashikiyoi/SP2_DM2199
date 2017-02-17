#ifndef ENEMY_H
#define ENEMY_H

#include "MatrixStack.h"

class Enemy
{
public:
	Enemy();
	Enemy(string Name, Vector3 Position);
	virtual ~Enemy();
	float RotateToPlayer(Vector3 player);
};

#endif
