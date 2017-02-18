#include "Player.h"

Player::Player()
{
}
Player::~Player()
{
}
Player::Player(string name, Vector3 position) :
GameObject(name, position), health(100), maxHealth(100)
{
}

bool Player::isdead()
{
	return(health <= 0);
}
void Player::DmgPlayer(unsigned Damage)
{
	if (this->health > 0)
	{
		if (Damage > (this->health))
		{
			this->health = 0;
			return;
		}
		this->health -= Damage;
	}
}
void Player::HealPlayer(unsigned HealAmt)
{
	if (this->health < this->maxHealth)
	{
		if ((HealAmt + this->health) > this->maxHealth)
		{
			this->health = this->maxHealth;
			return;
		}
		this->health += HealAmt;
	}
}

unsigned Player::getHealth(void)
{
	return (this->health);
}