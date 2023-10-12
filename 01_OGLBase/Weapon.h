#pragma once

#include "Entity.h"
#include "Projectile.h"
#include <chrono> 

class Player;

class Weapon : public Entity
{
protected:
	Player* m_parent;
	glm::vec3 m_shootDir;
	float m_coolDownTime;
	std::chrono::time_point<std::chrono::system_clock> m_lastShootTime;
	Texture2D m_projectileImage;

public:
	Weapon();
	glm::vec3 GetShootDir() const;
	float GetCoolDownTime();
	void SetPosition(const glm::vec3);
	void SetShootDir(const glm::vec3);
	void SetCooldown(float);
	void SetParent(Player*);

	virtual void Shoot(std::vector<Projectile>&);
	virtual void Shoot(std::vector<Projectile>&, int);
	virtual Texture2D& GetProjectileImage();
	virtual void Update();
	virtual bool requireTarget();
};
