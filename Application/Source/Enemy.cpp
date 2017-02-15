#include "Enemy.h"
#include "MyMath.h"
Enemy::Enemy() : position(0)
{
}

Enemy::~Enemy()
{
}

float Enemy::RotateToPlayer(Vector3 player)
{
	float rotation = Math::RadianToDegree(atan2f((position.x - player.x), (position.z - player.z)));
	return rotation;
}