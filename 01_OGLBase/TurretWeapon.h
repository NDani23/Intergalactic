#pragma once

#include "Weapon.h"
#include "Laser.h"

class Enemy;
class Map;

class TurretWeapon : public Weapon
{
private:
	int m_side;
	bool m_shooting;
	float m_fireRateCooldown;
	float m_durationTime;
	std::chrono::time_point<std::chrono::system_clock> m_lastActivateTime;
	Enemy* m_target;
	void FindClosestEnemy();
	void SetShootDir();
public:
	TurretWeapon();
	TurretWeapon(Player*, int);
	void Shoot(std::vector<std::unique_ptr<Projectile>>&) override;
	void Update() override;
};
