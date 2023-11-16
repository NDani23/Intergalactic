#pragma once

#include <glm/glm.hpp>
#include <GL/glew.h>
#include <vector>
#include <utility>
#include <iostream> 
#include <memory>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>

#include "../../includes/ProgramObject.h"
#include "../../includes/BufferObject.h"
#include "../../includes/VertexArrayObject.h"

struct ParticleProps
{
	glm::vec3 Position;
	glm::vec3 Velocity, VelocityVariation;
	glm::vec4 ColorBegin, ColorEnd;
	float SizeBegin, SizeEnd, SizeVariation;
	float LifeTime = 1.0f;
};

class ParticleSystem
{
public:
	ParticleSystem();

	void Update(const float delta);
	void Render(glm::mat4 viewProj);

	void Emit(const ParticleProps& particleProps);
private:
	struct Particle
	{
		glm::vec3 Position;
		glm::vec3 Velocity;
		glm::vec4 ColorBegin, ColorEnd;
		float Rotation = 0.0f;
		float SizeBegin, SizeEnd;

		float LifeTime = 1.0f;
		float LifeRemaining = 0.0f;

		bool Active = false;
	};

	std::vector<Particle> m_ParticlePool;
	uint32_t m_PoolIndex = 999;

	VertexArrayObject	m_CubeVao;			// VAO
	IndexBuffer			m_CubeIndices;		// index buffer
	ArrayBuffer			m_CubeVertexBuffer;	// VBO

	ProgramObject m_ParticleProgram;
};

