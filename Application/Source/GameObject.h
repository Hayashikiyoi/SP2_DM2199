#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "Vector3.h"
#include "CollisionChecker.h"
#include <string>

using std::string;

class GameObject
{
private:
	boxCollider collider;
	string Name;

public:
	Vector3 Position;
	float rotation;

	GameObject();
	GameObject(string Name,Vector3 Position);
	virtual ~GameObject();
	
	virtual string getName();
	virtual void setCollider(float sizeX, float sizeZ);
	virtual void updateCurPos(void);
	virtual boxCollider getCollider();
	virtual bool trigger(GameObject* other);
};
#endif