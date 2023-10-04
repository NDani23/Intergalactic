#pragma once

#include "Directions.h"
#include "Entity.h"
#include "Laser.h"
#include "LaserGun.h"

#include <algorithm>

class Player : public Entity
{
private:
	glm::vec3 m_forward_vec;
	glm::vec3 m_up_vec;
	glm::vec3 m_cross_vec;
	float m_speed;
	bool m_slowing;
	horizontal::direction roll_dir = horizontal::none;
	vertical::direction pitch_dir = vertical::none;
	std::vector<Projectile> m_projectiles;
	LaserGun gun1;
	LaserGun gun2;

public:
	Player();
	void Move(const float&, const glm::vec3&);
	void Shoot();
	void RemoveProjectile(Projectile&);
	void UpdateProjectiles(const float&);

	void setRollDir(const horizontal::direction&);
	void setPitchDir(const vertical::direction&);

	void Decelerate(bool);

	glm::vec3 GetForwardVec();
	glm::vec3 GetUpVec();
	glm::vec3 GetCrossVec();
	std::vector<Projectile>& GetProjectiles();
	Weapon& GetActiveWeapon1();
	Weapon& GetActiveWeapon2();

private:
	void Roll(const int&, const float&);
	void Pitch(const int&, const float&);
	void updateDimensions();
};
