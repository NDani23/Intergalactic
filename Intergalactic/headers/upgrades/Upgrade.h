#pragma once

#include <chrono>

#include "headers/Entity.h"

class Player;

class Upgrade : public Entity
{
protected:
	int m_ID;
	Player* m_parent;
	float m_coolDownTime;
	float m_currentCoolDown;
	Texture2D m_Image;

public:
	Upgrade();
	int ID();
	float GetCoolDownTime();
	float GetCurrentCooldown();
	void SetPosition(const glm::vec3&);
	void SetCooldown(float);
	void SetParent(Player*);
	Player* GetParent();

	virtual Texture2D& GetImage();
	virtual void Update(const float delta) {};
	virtual void Activate() {};
	virtual void Reset() {};
};
