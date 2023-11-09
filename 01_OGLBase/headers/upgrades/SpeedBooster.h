#pragma once

#include "Upgrade.h"

class SpeedBooster : public Upgrade
{
private:
	bool m_active;
	float m_durationTime;
public:
	SpeedBooster();
	SpeedBooster(Player*);
	void Update() override;
	void Activate() override;
};
