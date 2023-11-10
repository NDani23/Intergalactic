#pragma once

#include "Upgrade.h"

class StealthCoat : public Upgrade
{
private:
	bool m_active;
	float m_durationTime;
	float m_activeTime;
	ProgramObject m_transparentProgram;
public:
	StealthCoat();
	StealthCoat(Player*);
	void Update(const float) override;
	void Activate() override;
	void DrawMesh(ProgramObject&, glm::mat4&) override;
};
