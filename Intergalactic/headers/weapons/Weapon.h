#pragma once

#include "../Entity.h"
#include "Projectile.h"
#include <chrono> 

class Player;

class Weapon : public Entity
{
protected:
	int m_ID;
	bool m_active;
	Player* m_parent;
	glm::vec3 m_shootDir;
	float m_coolDownTime;
	float m_currentCoolDown;
	Texture2D m_projectileImage;

public:
	Weapon();
	int ID();
	glm::vec3 GetShootDir() const;
	float GetCoolDownTime();
	float GetCurrentCooldown();
	void SetPosition(const glm::vec3);
	void SetShootDir(const glm::vec3);
	void SetCooldown(float);
	void SetParent(Player*);
	void SetActive(bool);

	bool IsActive();

	virtual void Shoot(std::vector<std::unique_ptr<Projectile>>&);
	virtual void Shoot(std::vector<std::unique_ptr<Projectile>>&, int);
	virtual Texture2D& GetProjectileImage();
	virtual void Update(const float);
	virtual bool requireTarget();
};
