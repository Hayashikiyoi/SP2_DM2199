#include "GameObject.h"


GameObject::GameObject()
{
}

GameObject::~GameObject()
{
}

GameObject::GameObject(string Name, Vector3 Position) : Name(Name), Position(Position),rotation(0)
{

}

string GameObject::getName()
{
	return (this->Name);
}

boxCollider GameObject::getCollider()
{
	return (this->collider);
}

void GameObject::setCollider(float sizeX, float sizeZ)
{
	if (sizeX != 0 && sizeZ != 0)
	{
		collider.setBoxSize(sizeX, sizeZ);
	}
}

bool GameObject::trigger(GameObject* other)
{
	boxCollider others = other->getCollider();
	return (collider.collidecheck(others.getmin(), others.getmax()));
}

void GameObject::updateCurPos(void)
{
	collider.updatePos(Position);
}