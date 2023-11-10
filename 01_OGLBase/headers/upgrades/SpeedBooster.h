#pragma once

#include "Upgrade.h"

class SpeedBooster : public Upgrade
{
private:
	bool m_active;
	float m_durationTime;
	float m_activeTime;
public:
	SpeedBooster();
	SpeedBooster(Player*);
	void Update(const float) override;
	void Activate() override;
};
