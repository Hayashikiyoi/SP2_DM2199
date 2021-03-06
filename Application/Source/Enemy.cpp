#include "Enemy.h"
#include "MyMath.h"
Enemy::Enemy()
{
}
Enemy::~Enemy()
{
}

Enemy::Enemy(string Name, Vector3 Position) : GameObject(Name, Position),
health(100)
{
}

float Enemy::RotateToPlayer(Vector3 player)
{
	rotation = Math::RadianToDegree(atan2f((Position.x - player.x), (Position.z - player.z)));
	return rotation;
}

bool Enemy::isdead()
{
	return (health <= 0);
}

void Enemy::dmgToEnemy(float dmg)
{
	if (this->health > 0)
	{
		if (dmg > (this->health))
		{
			this->health = 0;
			return;
		}
		this->health -= dmg;
	}
}

float Enemy::showHP()
{
	return (this->health);
}