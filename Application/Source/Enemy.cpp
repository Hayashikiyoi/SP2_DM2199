#include "Enemy.h"
#include "MyMath.h"
Enemy::Enemy()
{
}
Enemy::~Enemy()
{
}

Enemy::Enemy(string Name, Vector3 Position) : GameObject(Name, Position)
{
}

float Enemy::RotateToPlayer(Vector3 player)
{
	rotation = Math::RadianToDegree(atan2f((Position.x - player.x), (Position.z - player.z)));
	return rotation;
}