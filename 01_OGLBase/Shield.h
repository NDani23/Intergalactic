#pragma once

#include "Upgrade.h"
#include "Barrier.h"

class Map;

class Shield : public Upgrade
{
private:
	std::shared_ptr<Barrier> m_barrier;
	bool m_active;
	float m_durationTime;
public:
	Shield();
	Shield(Player*);
	void Update() override;
	void Activate() override;
};
