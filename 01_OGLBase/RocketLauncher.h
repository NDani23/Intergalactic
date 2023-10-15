#pragma once

#include "Weapon.h"
#include "Rocket.h"

class RocketLauncher : public Weapon
{
private:

public:
	RocketLauncher();
	RocketLauncher(Player*);
	void Shoot(std::vector<std::unique_ptr<Projectile>>&) override;
	void Update() override;
	bool requireTarget() override;
};