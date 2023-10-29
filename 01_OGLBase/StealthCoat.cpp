#include "StealthCoat.h"
#include "Player.h"

StealthCoat::StealthCoat()
{
	m_transparentProgram.AttachShaders({
		{ GL_VERTEX_SHADER, "myVert.vert"},
		{ GL_FRAGMENT_SHADER, "transparentFrag.frag"}
		});

	m_transparentProgram.BindAttribLocations({
		{ 0, "vs_in_pos" },
		{ 1, "vs_in_norm" },
		{ 2, "vs_in_tex" },
		});

	m_transparentProgram.LinkProgram();

	m_ID = 2;
	m_parent = nullptr;
	m_position = glm::vec3(0, 0, 0);
	m_transforms = glm::translate(m_position);
	m_coolDownTime = 25.f;
	m_durationTime = 8.f;
	m_lastActiveTime = std::chrono::system_clock::now();

	m_active = false;

	HitBox hitbox = { m_position, {0.0, 0.0, 0.0} };
	m_hitboxes.emplace_back(hitbox);

	m_mesh = std::unique_ptr<Mesh>(ObjParser::parse("assets/stealthCoat.obj"));
	m_mesh->initBuffers();

	m_texture.FromFile("assets/stealthCoat_tex.png");
	m_Image.FromFile("assets/stealthCoat.png");
}

StealthCoat::StealthCoat(Player* parent)
{
	m_transparentProgram.AttachShaders({
		{ GL_VERTEX_SHADER, "myVert.vert"},
		{ GL_FRAGMENT_SHADER, "transparentFrag.frag"}
		});

	m_transparentProgram.BindAttribLocations({
		{ 0, "vs_in_pos" },
		{ 1, "vs_in_norm" },
		{ 2, "vs_in_tex" },
		});

	m_transparentProgram.LinkProgram();

	m_ID = 2;
	m_parent = parent;
	m_active = false;
	m_position = m_parent->GetPos();
	m_transforms = glm::inverse(glm::lookAt(m_position, m_position - parent->GetForwardVec(), m_parent->GetUpVec()));
	m_coolDownTime = 25.f;
	m_durationTime = 8.f;
	m_lastActiveTime = std::chrono::system_clock::now();

	HitBox hitbox = { m_position, {0.0, 0.0, 0.0} };
	m_hitboxes.emplace_back(hitbox);

	m_mesh = std::unique_ptr<Mesh>(ObjParser::parse("assets/stealthCoat.obj"));
	m_mesh->initBuffers();

	m_texture.FromFile("assets/stealthCoat_tex.png");
	m_Image.FromFile("assets/stealthCoat.png");

}

void StealthCoat::Update()
{
	m_position = m_parent->GetPos();
	m_transforms = glm::inverse(glm::lookAt(m_position, m_position - m_parent->GetForwardVec(), m_parent->GetUpVec()));

	std::chrono::duration<float> elapsed_seconds = std::chrono::system_clock::now() - m_lastActiveTime;
	if (m_active && elapsed_seconds.count() >= m_durationTime)
	{
		m_active = false;
		m_parent->setStealth(false);
	}
}

void StealthCoat::Activate()
{
	std::chrono::duration<float> elapsed_seconds = std::chrono::system_clock::now() - m_lastActiveTime;

	if (elapsed_seconds.count() >= m_coolDownTime)
	{
		m_parent->setStealth(true);
		m_parent->setFakePos(m_parent->GetPos() + m_parent->GetForwardVec() * 1000.f);
		m_lastActiveTime = std::chrono::system_clock::now();
		m_active = true;
	}
}

void StealthCoat::DrawMesh(ProgramObject& program, glm::mat4& viewProj)
{
	program.Unuse();
	glEnable(GL_BLEND);
	m_transparentProgram.Use();
	m_transparentProgram.SetTexture("texImage", 0, m_texture);
	m_transparentProgram.SetUniform("MVP", viewProj * m_transforms);
	m_transparentProgram.SetUniform("world", m_transforms);
	m_transparentProgram.SetUniform("worldIT", glm::inverse(glm::transpose(m_transforms)));
	if (m_active)
	{
		m_transparentProgram.SetUniform("alpha", 0.2f);
	}
	else
	{
		m_transparentProgram.SetUniform("alpha", 0.8f);
	}

	m_mesh->draw();
	glDisable(GL_BLEND);
	m_transparentProgram.Unuse();
	program.Use();
}