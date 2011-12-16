#include <list>

class projectile
{
public:
	float age;
	float position[3];
	float velocity[3];

public:
	projectile();
	~projectile();
	void render();
	void move();
}Bullet;

class projectileList
{
public:
	int size;
private:
	std::list<projectile> projectiles;
	
public:
	projectileList();
	~projectileList();
	void render();
}BulletList;