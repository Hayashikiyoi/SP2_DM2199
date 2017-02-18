#include "Item.h"

Item::Item()
{
}
Item::Item(string name, Vector3 pos, string Description) :
GameObject(name, pos), description(Description), collected(false)
{
}
Item::~Item()
{
}

string Item::showDescription(void)
{
	return (this->description);
}
bool Item::isCollected(void)
{
	return (this->collected);
}
void Item::SetCollected(bool Collected)
{
	this->collected = Collected;
}