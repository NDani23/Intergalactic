#pragma once

#include "Entity.h"
#include <chrono> 

class Player;

class Upgrade : public Entity
{
protected:
	Player* m_parent;
	float m_coolDownTime;
	std::chrono::time_point<std::chrono::system_clock> m_lastActiveTime;
	Texture2D m_Image;

public:
	Upgrade();
	float GetCoolDownTime();
	void SetPosition(const glm::vec3&);
	void SetCooldown(float);
	void SetParent(Player*);
	Player* GetParent();

	virtual Texture2D& GetImage();
	virtual void Update();
	virtual void Activate();
};
