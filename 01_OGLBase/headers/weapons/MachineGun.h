#pragma once

#include "Weapon.h"
#include "Laser.h"

class MachineGun : public Weapon
{
private:
	int m_side;
public:
	MachineGun();
	MachineGun(Player*, int);
	void Shoot(std::vector<std::unique_ptr<Projectile>>&) override;
	void Update(const float) override;
};
