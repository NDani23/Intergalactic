#pragma once

#include "Weapon.h"
#include "Laser.h"

class Enemy;
class Scene;

class TurretWeapon : public Weapon
{
private:
	int m_side;
	bool m_shooting;
	float m_currentfireRateCooldown;
	float m_fireRateCooldown;
	float m_durationTime;
	float m_activeTime;
	Enemy* m_target;
	void FindClosestEnemy();
	void SetShootDir();
public:
	TurretWeapon();
	~TurretWeapon();
	TurretWeapon(Player*, int);
	void Shoot(std::vector<std::unique_ptr<Projectile>>&) override;
	void Update(const float) override;
};
