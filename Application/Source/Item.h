#ifndef ITEM_H
#define ITEM_H

#include "GameObject.h"

class Item : public GameObject
{
public:
	Item();
	Item(string name, Vector3 pos, string description);
	virtual ~Item();

	string showDescription(void);
	bool isCollected(void);
	void SetCollected(bool Collected);

private:
	bool collected;
	string description;
};

#endif