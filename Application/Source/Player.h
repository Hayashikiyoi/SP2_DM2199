#ifndef PLAYER_H
#define PLAYER_H

#include "GameObject.h"

class Player : public GameObject
{
public:
	Player();
	Player(string name, Vector3 position);
	virtual ~Player();

	bool isdead(void); // Check player is dead or not
	void DmgPlayer(unsigned Damage); //Damage current player
	void HealPlayer(unsigned HealAmt); //Heal amount of hp of player

	unsigned getHealth(void); //Returns health of player

private:
	unsigned health, maxHealth;
};

#endif