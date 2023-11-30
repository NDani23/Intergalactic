#pragma once

#include "Weapon.h"
#include "Mine.h"

class MinePlacer : public Weapon
{
private:
	int m_side;
public:
	MinePlacer();
	~MinePlacer();
	MinePlacer(Player*, int);
	void Shoot(std::vector<std::unique_ptr<Projectile>>&) override;
	void Update(const float) override;
	void Reset() override;
};
