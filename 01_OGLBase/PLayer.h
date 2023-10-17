#pragma once

#include "Directions.h"
#include "Entity.h"
#include "Laser.h"
#include "LaserGun.h"
#include "RocketLauncher.h"
#include "MinePlacer.h"

#include <algorithm>

struct Stats
{
	int speed;
	int mobility;
	int health;
	int fire_rate;
	int damage;
};

class Player : public Entity
{
private:

	int m_health;
	int m_max_health;
	glm::vec3 m_forward_vec;
	glm::vec3 m_up_vec;
	glm::vec3 m_cross_vec;
	float m_speed;
	int m_max_speed;
	bool m_slowing;
	int m_damage;
	horizontal::direction roll_dir = horizontal::none;
	vertical::direction pitch_dir = vertical::none;
	std::vector<std::unique_ptr<Projectile>> m_projectiles;

	Entity* m_target;

	std::unique_ptr<Weapon> m_guns[3];

	int m_points;
	int m_upgradePoints;

	int m_activeWeaponInd;

	Stats m_stats;

public:
	Player();
	void Reset();

	void Move(const float&, const glm::vec3&);
	void Shoot();
	void RemoveProjectile(std::unique_ptr<Projectile>&);
	void UpdateProjectiles(const float&);

	void setRollDir(const horizontal::direction&);
	void setPitchDir(const vertical::direction&);
	void setHealth(int);
	void setPoints(int);
	void setUpgradePoints(int);
	void setStats(Stats);
	void setActiveWeapon(int);
	void setTarget(Entity*);

	void Decelerate(bool);
	bool Hit(int) override;

	int GetHealth();
	int GetPoints();
	int GetSpeed();
	int GetMaxSpeed();
	int GetMaxHealth();
	int GetUpgradePoints();
	int GetActiveWeaponInd();
	Stats& GetStats();
	Entity* GetTarget();

	glm::vec3 GetForwardVec();
	glm::vec3 GetUpVec();
	glm::vec3 GetCrossVec();
	std::vector<std::unique_ptr<Projectile>>& GetProjectiles();
	std::unique_ptr<Weapon>* GetWeapons();
	Weapon& GetActiveWeapon1();

private:
	void Roll(const int&, const float&);
	void Pitch(const int&, const float&);
	void updateDimensions();
};
