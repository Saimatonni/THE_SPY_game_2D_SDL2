#include "Physics.h"
using namespace std;

void Physics::IncreaseAcceleration(int x, int y)
{
	acceleration.x += x;
	acceleration.y += y;
}

void Physics::IncreaseVelocity(int x, int y)
{
	velocity.x += x;
	velocity.y += y;
}

void Physics::MovePosition(int x, int y)
{
	position.x += x;
	position.y += y;
}

Physics::Physics()
{
	velocity.x = velocity.y = acceleration.x = acceleration.y = position.x = position.y = 0;
}

Physics::~Physics()
{

}

void Physics::Update()
{
	velocity.x += acceleration.x;
	velocity.y += acceleration.y;
	if (velocity.y > 6) velocity.y = 6;
	///cout << velocity.y << endl;
	box.x += velocity.x;
	box.y += velocity.y;
}

