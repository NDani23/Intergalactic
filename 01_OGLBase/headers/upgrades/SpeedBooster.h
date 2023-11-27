#pragma once

#include "Upgrade.h"
#include "headers/scenes/TailFire.h"

class SpeedBooster : public Upgrade
{
private:
	bool m_active;
	float m_durationTime;
	float m_activeTime;
	TailFire m_TailFireLeft;
	TailFire m_TailFireRight;
public:
	SpeedBooster();
	SpeedBooster(Player*);
	void Update(const float) override;
	void Activate() override;
};
