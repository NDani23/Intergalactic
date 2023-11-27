#pragma once

#include "Weapon.h"
#include "Rocket.h"

class RocketLauncher : public Weapon
{
private:
	int m_side;
public:
	RocketLauncher();
	RocketLauncher(Player*, int);
	void Shoot(std::vector<std::unique_ptr<Projectile>>&) override;
	void Update(const float) override;
	bool requireTarget() override;
};