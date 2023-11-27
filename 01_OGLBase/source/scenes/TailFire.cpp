#include "headers/scenes/TailFire.h"

TailFire::TailFire()
{
	m_particleSystem = nullptr;
	m_frequency = 0.01f;
	m_emitTailFireTime = 0.025f;

	m_ParticleProp.ColorEnd = { 200.f / 255.f, 200.f / 255.f, 200.f / 255.f, 1.f };
	m_ParticleProp.ColorBegin = { 254.f / 255.f, 109.f / 255.f, 41 / 255.f, 1.f };
	m_ParticleProp.SizeBegin = 0.65f;
	m_ParticleProp.SizeVariation = 0.2f;
	m_ParticleProp.SizeEnd = 0.0f;
	m_ParticleProp.LifeTime = 0.08f;
	m_ParticleProp.Velocity = { 0.0f, 0.0f, 0.0f };
	m_ParticleProp.VelocityVariation = { 3.f, 3.f, 3.f };
	m_ParticleProp.Position = { 0.0f, 0.f, 0.0f };
}

void TailFire::SetPartycleSystem(ParticleSystem* particleSystem)
{
	m_particleSystem = particleSystem;
}


void TailFire::SetPosition(glm::vec3& pos)
{
	m_ParticleProp.Position = pos;
}

void TailFire::SetParticleProp(const ParticleProps& particleProp)
{
	m_ParticleProp = particleProp;
}

void TailFire::Setfrequency(float freq)
{
	m_frequency = freq;
}

void TailFire::Update(const float delta)
{
	if (m_particleSystem == nullptr) return;

	if (m_emitTailFireTime >= m_frequency)
	{
		for (int i = 0; i < m_emitTailFireTime / m_frequency; i++)
		{
			m_particleSystem->Emit(m_ParticleProp);
		}

		m_emitTailFireTime = 0.f;
	}
	else
	{
		m_emitTailFireTime += delta;
	}
}