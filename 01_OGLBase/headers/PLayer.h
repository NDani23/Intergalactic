#pragma once

#include "Directions.h"
#include "Entity.h"
#include "data_storage/WeaponDataStorage.h"
#include "data_storage/UpgradeDataStorage.h"

#include <algorithm>

class Persistence;
class Scene;

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
	int m_record;
	int m_health;
	int m_max_health;
	glm::vec3 m_forward_vec;
	glm::vec3 m_up_vec;
	glm::vec3 m_cross_vec;
	float m_current_speed;
	float m_speed;
	int m_max_speed;
	bool m_slowing;
	bool m_stealth;
	bool m_flyStraight;
	bool m_lookBack;
	int m_damage;
	horizontal::direction roll_dir = horizontal::none;
	vertical::direction pitch_dir = vertical::none;
	std::vector<std::unique_ptr<Projectile>> m_projectiles;

	glm::vec3 m_fakePos;
	glm::vec3 m_cursorVec;
	Scene* m_scene;

	Entity* m_target;

	std::unique_ptr<Weapon> m_guns[3];
	std::unique_ptr<Upgrade> m_Upgrade;

	ProgramObject m_transparentProgram;

	int m_points;
	int m_upgradePoints;

	int m_credit;

	int m_activeWeaponInd;

	Stats m_stats;

	friend class Persistence;

public:
	Player();
	void Reset(Scene*);

	void Move(float, const glm::vec3&);
	void Shoot();
	void ActivateUpgrade();
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
	void setSpeed(float);
	void setCredit(int);
	void setRecord(int);
	void setStealth(bool);
	void setFakePos(glm::vec3&);
	void FlyStraight(bool);
	void LookBack(bool);

	void SlowDown(bool);
	void Decelerate();
	void Accelerate();
	bool Hit(int) override;

	int GetHealth();
	int GetPoints();
	int GetSpeed();
	int GetMaxSpeed();
	int GetMaxHealth();
	int GetUpgradePoints();
	int GetActiveWeaponInd();
	glm::vec3& GetCursorVec();
	int GetCredit();
	int GetRecord();
	bool IsStealth();
	bool IsLookingBack();
	glm::vec3 GetFakePos();
	Scene* GetMapPtr();
	Stats& GetStats();
	Entity* GetTarget();

	int GetUpgradePointsSum();

	glm::vec3 GetForwardVec();
	glm::vec3 GetUpVec();
	glm::vec3 GetCrossVec();
	std::vector<std::unique_ptr<Projectile>>& GetProjectiles();
	std::unique_ptr<Weapon>* GetWeapons();
	Weapon& GetActiveWeapon1();
	std::unique_ptr<Upgrade>& GetUpgrade();

	void DrawMesh(ProgramObject&, glm::mat4&) override;

private:
	void Roll(const int&, const float&);
	void Pitch(const int&, const float&);
	void updateDimensions();
};
