#ifndef ENEMY_H
#define ENEMY_H

#include "MatrixStack.h"

class Enemy
{
public:
	Enemy();
	~Enemy();
	float RotateToPlayer(Vector3 player);
	Vector3 position;
};

#endif