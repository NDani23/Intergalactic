#pragma once

#include "ParticleSystem.h"

class TailFire
{
private:
	ParticleProps m_ParticleProp;
	ParticleSystem* m_particleSystem;
	float m_frequency;
	float m_emitTailFireTime;
public:
	TailFire();
	void SetPosition(glm::vec3&);
	void SetParticleProp(const ParticleProps&);
	void Setfrequency(float);
	void SetPartycleSystem(ParticleSystem*);

	void Update(const float);
};


