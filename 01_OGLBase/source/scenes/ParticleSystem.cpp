#include "../../headers/scenes/ParticleSystem.h"
#include "../../headers/Random.h"

#include <glm/gtc/constants.hpp>
#include <glm/gtx/compatibility.hpp>

ParticleSystem::ParticleSystem()
{
	m_ParticleProgram.AttachShaders({
		{ GL_VERTEX_SHADER, "shaders/BaseGeometryVert.vert"},
		{ GL_FRAGMENT_SHADER, "shaders/BaseGeometryFrag.frag"}
		});

	m_ParticleProgram.BindAttribLocations({
		{ 0, "vs_in_pos" },
		});

	m_ParticleProgram.LinkProgram();


	std::vector<glm::vec3>vertices;

	//front									 
	vertices.push_back({ glm::vec3(-0.5, -0.5, +0.5) });
	vertices.push_back({ glm::vec3(+0.5, -0.5, +0.5) });
	vertices.push_back({ glm::vec3(-0.5, +0.5, +0.5) });
	vertices.push_back({ glm::vec3(+0.5, +0.5, +0.5) });
	//back
	vertices.push_back({ glm::vec3(+0.5, -0.5, -0.5) });
	vertices.push_back({ glm::vec3(-0.5, -0.5, -0.5) });
	vertices.push_back({ glm::vec3(+0.5, +0.5, -0.5) });
	vertices.push_back({ glm::vec3(-0.5, +0.5, -0.5) });
	//right									 
	vertices.push_back({ glm::vec3(+0.5, -0.5, +0.5) });
	vertices.push_back({ glm::vec3(+0.5, -0.5, -0.5) });
	vertices.push_back({ glm::vec3(+0.5, +0.5, +0.5) });
	vertices.push_back({ glm::vec3(+0.5, +0.5, -0.5) });
	//left									 
	vertices.push_back({ glm::vec3(-0.5, -0.5, -0.5) });
	vertices.push_back({ glm::vec3(-0.5, -0.5, +0.5) });
	vertices.push_back({ glm::vec3(-0.5, +0.5, -0.5) });
	vertices.push_back({ glm::vec3(-0.5, +0.5, +0.5) });
	//top									 		
	vertices.push_back({ glm::vec3(-0.5, +0.5, +0.5) });
	vertices.push_back({ glm::vec3(+0.5, +0.5, +0.5) });
	vertices.push_back({ glm::vec3(-0.5, +0.5, -0.5) });
	vertices.push_back({ glm::vec3(+0.5, +0.5, -0.5) });
	//bottom								 		
	vertices.push_back({ glm::vec3(-0.5, -0.5, -0.5) });
	vertices.push_back({ glm::vec3(+0.5, -0.5, -0.5) });
	vertices.push_back({ glm::vec3(-0.5, -0.5, +0.5) });
	vertices.push_back({ glm::vec3(+0.5, -0.5, +0.5) });

	std::vector<int> indices(36);
	int index = 0;
	for (int i = 0; i < 6 * 4; i += 4)
	{
		indices[index + 0] = i + 0;
		indices[index + 1] = i + 1;
		indices[index + 2] = i + 2;
		indices[index + 3] = i + 1;
		indices[index + 4] = i + 3;
		indices[index + 5] = i + 2;
		index += 6;
	}

	m_CubeVertexBuffer.BufferData(vertices);
	m_CubeIndices.BufferData(indices);

	m_CubeVao.Init(
		{
			{ CreateAttribute<		0,
									glm::vec3,
									0,
									sizeof(glm::vec3)
								>, m_CubeVertexBuffer },
		},
		m_CubeIndices
	);

	m_ParticlePool.resize(1000);
}

void ParticleSystem::Update(const float delta)
{
	for (auto& particle : m_ParticlePool)
	{
		if (!particle.Active) continue;

		if (particle.LifeRemaining <= 0.0f)
		{
			particle.Active = false;
			continue;
		}

		particle.LifeRemaining -= delta;
		particle.Position += particle.Velocity * float(delta);
		particle.Rotation += 0.01f * delta;
	}
}

void ParticleSystem::Render(glm::mat4 viewProj)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	m_ParticleProgram.Use();
	m_ParticleProgram.SetUniform("ViewProj", viewProj);

	for (auto& particle : m_ParticlePool)
	{
		if (!particle.Active) continue;

		float life = particle.LifeRemaining / particle.LifeTime;
		glm::vec4 color = glm::lerp(particle.ColorEnd, particle.ColorBegin, life);

		float size = glm::lerp(particle.SizeEnd, particle.SizeBegin, life);

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), { particle.Position.x, particle.Position.y, particle.Position.z })
			* glm::rotate(glm::mat4(1.0f), particle.Rotation, { 0.0f, 1.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { size, size, size });

		m_ParticleProgram.SetUniform("Transform", transform);
		m_ParticleProgram.SetUniform("Color", color);
		m_CubeVao.Bind();
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);
	}
	m_ParticleProgram.Unuse();

	glDisable(GL_BLEND);
}

void ParticleSystem::Emit(const ParticleProps& particleProps)
{
	Particle& particle = m_ParticlePool[m_PoolIndex];
	particle.Active = true;
	particle.Position = particleProps.Position;
	particle.Rotation = Random::Float() * 2.0f * glm::pi<float>();

	//velocity
	particle.Velocity = particleProps.Velocity;
	particle.Velocity.x += particleProps.VelocityVariation.x * (Random::Float() - 0.5f);
	particle.Velocity.y += particleProps.VelocityVariation.y * (Random::Float() - 0.5f);
	particle.Velocity.z += particleProps.VelocityVariation.z * (Random::Float() - 0.5f);

	//color
	particle.ColorBegin = particleProps.ColorBegin;
	particle.ColorEnd = particleProps.ColorEnd;

	particle.LifeTime = particleProps.LifeTime;
	particle.LifeRemaining = particleProps.LifeTime;
	particle.SizeBegin = particleProps.SizeBegin + particleProps.SizeVariation * (Random::Float() - 0.5f);
	particle.SizeEnd = particleProps.SizeEnd;

	m_PoolIndex = --m_PoolIndex % m_ParticlePool.size();
}
