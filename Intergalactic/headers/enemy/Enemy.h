#pragma once

#include "headers/weapons/Weapon.h"
#include "headers/weapons/Laser.h"
#include "headers/scenes/TailFire.h"
#include "headers/Player.h"

#include <stdlib.h> 
#include <algorithm>

class Scene;

class Enemy : public Weapon
{
protected:
	int m_damage;
	int m_mobility;
	int m_health;
	float m_speed;
	float m_shootAngle;
	int m_shootDistance;
	glm::vec3 m_forward_vec;
	glm::vec3 m_up_vec;
	TailFire m_tailFire;
	Player* m_target;
	std::vector<std::unique_ptr<Projectile>>* m_projectiles;
	Scene* m_Scene;

public:
	Enemy();
	bool Hit(int) override;
	void SetTexture();
	bool IsTargetable() override;
	bool IsStatic() override;
	glm::vec3& GetForwardVec();

	virtual void Shoot() {};

	bool Update(const float&) override;

protected:
	void AvoidObject(Entity&, glm::vec3&);
	void CalcBaseDir(glm::vec3&);
	bool CalcAvoidObjectsVec(glm::vec3&);
	bool CalcAvoidFloorVec(glm::vec3&);
	void RegulateTurnDegree(glm::vec3&, const float);
	void CheckIfShoot();
	virtual void UpdateDimensions();
};
