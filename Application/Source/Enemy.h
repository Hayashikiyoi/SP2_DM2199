#ifndef ENEMY_H
#define ENEMY_H

#include "MatrixStack.h"
#include "GameObject.h"
class Enemy : public GameObject
{
public:
	Enemy();
	~Enemy();
	float RotateToPlayer(Vector3 player);
	Vector3 position;
};

#endif