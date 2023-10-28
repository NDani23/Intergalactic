#pragma once

#include "Weapon.h"
#include "Laser.h"
#include "Player.h"

#include <stdlib.h> 
#include <algorithm>

class Map;

class Enemy : public Weapon
{
protected:
	int m_damage;
	int m_mobility;
	int m_health;
	float m_speed;
	glm::vec3 m_forward_vec;
	glm::vec3 m_up_vec;

	Player* m_target;
	std::vector<std::unique_ptr<Projectile>>* m_projectiles;
	Map* m_Map;

public:
	Enemy();
	virtual void Shoot() {};
	bool Hit(int) override;
	void SetTexture();
	bool IsTargetable() override;

	virtual bool Update(const float&) override;

protected:
	virtual HitBox UpdateDimensions();
	virtual void CalcBaseDir(glm::vec3&);
	virtual bool CalcAvoidObjectsVec(glm::vec3&);
	virtual bool CalcAvoidFloorVec(glm::vec3&);
	virtual void RegulateTurnDegree(glm::vec3&);
	virtual void CheckIfShoot();
};
