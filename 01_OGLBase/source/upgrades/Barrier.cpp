#include "../../headers/upgrades/Barrier.h"

Barrier::Barrier()
{
	m_transparentProgram.AttachShaders({
		{ GL_VERTEX_SHADER, "shaders/myVert.vert"},
		{ GL_FRAGMENT_SHADER, "shaders/transparentFrag.frag"}
		});

	m_transparentProgram.BindAttribLocations({
		{ 0, "vs_in_pos" },
		{ 1, "vs_in_norm" },
		{ 2, "vs_in_tex" },
		});

	m_transparentProgram.LinkProgram();

	m_position = glm::vec3(0.f, 0.f, 0.f);

	HitBox hitbox = { m_position, {11.f, 11.f, 11.f} };

	m_hitboxes.emplace_back(hitbox);

	m_mesh = std::unique_ptr<Mesh>(ObjParser::parse("assets/Upgrades/barrier.obj"));
	m_mesh->initBuffers();

	m_texture.FromFile("assets/Upgrades/barrier_tex.png");

	m_transforms = glm::translate(m_position);
}

Barrier::Barrier(glm::vec3 pos)
{

	m_transparentProgram.AttachShaders({
		{ GL_VERTEX_SHADER, "shaders/myVert.vert"},
		{ GL_FRAGMENT_SHADER, "shaders/transparentFrag.frag"}
		});

	m_transparentProgram.BindAttribLocations({
		{ 0, "vs_in_pos" },
		{ 1, "vs_in_norm" },
		{ 2, "vs_in_tex" },
		});

	m_transparentProgram.LinkProgram();

	m_position = pos;

	HitBox hitbox = { m_position, {11.f, 11.f, 11.f} };

	m_hitboxes.emplace_back(hitbox);

	m_mesh = std::unique_ptr<Mesh>(ObjParser::parse("assets/Upgrades/barrier.obj"));
	m_mesh->initBuffers();

	m_texture.FromFile("assets/Upgrades/barrier_tex.png");

	m_transforms = glm::translate(m_position);
}

void Barrier::SetPos(glm::vec3 pos)
{
	m_position = pos;

	m_hitboxes[0].Position = m_position;

	m_transforms = glm::translate(m_position);
}

bool Barrier::CanCollide()
{
	return false;
}

void Barrier::DrawMesh(ProgramObject& program, glm::mat4& viewProj)
{
	program.Unuse();
	glEnable(GL_BLEND);
	m_transparentProgram.Use();
	m_transparentProgram.SetTexture("texImage", 0, m_texture);
	m_transparentProgram.SetUniform("MVP", viewProj * m_transforms);
	m_transparentProgram.SetUniform("world", m_transforms);
	m_transparentProgram.SetUniform("worldIT", glm::inverse(glm::transpose(m_transforms)));

	m_mesh->draw();
	glDisable(GL_BLEND);
	m_transparentProgram.Unuse();
	program.Use();
}