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
	float m_activeTime;
public:
	Shield();
	Shield(Player*);
	void Update(const float delta) override;
	void Activate() override;
};
