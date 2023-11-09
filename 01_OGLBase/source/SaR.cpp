#include "../headers/SaR.h"
#include "../headers/scenes/Map.h"

std::unique_ptr<Mesh> SaR::m_static_mesh;
Texture2D SaR::sar_static_tex;

int SaR::FirstInit()
{
	SaR::m_static_mesh = std::unique_ptr<Mesh>(ObjParser::parse("assets/Enemies/SaR.obj"));
	SaR::m_static_mesh->initBuffers();
	SaR::sar_static_tex = Texture2D("assets/Enemies/SaR_tex.png");

	return 1;
}

SaR::SaR()
{
	static int onFirstCall = FirstInit();
	m_collider.setVertices(m_static_mesh->GetVertices());
	m_position = glm::vec3(0.0f, 0.0f, 0.0f);
	m_forward_vec = glm::vec3(1.0f, 0.0f, 0.0f);
	m_shootDir = m_forward_vec;
	m_up_vec = glm::vec3(0.0f, 1.0f, 0.0f);

	m_transforms = glm::inverse(glm::lookAt(m_position, m_position + m_forward_vec, glm::vec3(0.0f, 1.0f, 0.0f)));

	HitBox hitbox = { m_position, {8.0, 2.5, 10.0} };
	m_hitboxes.emplace_back(hitbox);

	m_health = 100;
	m_speed = 90;
	m_damage = 10;
	m_mobility = 8;
	m_shootAngle = 0.1f;
	m_shootDistance = 200.f;

	m_coolDownTime = 0.25f;
	m_lastShootTime = std::chrono::system_clock::now();
}

SaR::SaR(glm::vec3 pos, Player* target, std::vector<std::unique_ptr<Projectile>>* projectiles, Map* map)
{
	static int onFirstCall = FirstInit();
	m_collider.setVertices(m_static_mesh->GetVertices());
	m_position = pos;
	m_target = target;
	m_projectiles = projectiles;
	m_Map = map;

	m_forward_vec = glm::normalize(m_target->GetPos() - m_position);
	m_shootDir = m_forward_vec;
	m_up_vec = glm::vec3(0.0f, 1.0f, 0.0f);

	SetTransforms(glm::inverse(glm::lookAt(m_position, m_position + m_forward_vec, glm::vec3(0.0f, 1.0f, 0.0f))));

	HitBox hitbox = { m_position, {8.0, 2.5, 10.0} };
	m_hitboxes.emplace_back(hitbox);

	m_health = 100;
	m_speed = 100;
	m_damage = 10;
	m_mobility = 8;
	m_shootAngle = 0.1f;
	m_shootDistance = 200.f;

	m_mesh = nullptr;

	m_coolDownTime = 0.25f;
	m_lastShootTime = std::chrono::system_clock::now();
}

void SaR::Shoot()
{
	std::chrono::duration<float> elapsed_seconds = std::chrono::system_clock::now() - m_lastShootTime;

	if (elapsed_seconds.count() >= m_coolDownTime)
	{
		m_projectiles->emplace_back(std::make_unique<Laser>(m_position + m_forward_vec * 10.f, m_shootDir, m_damage));

		m_lastShootTime = std::chrono::system_clock::now();
	}
}

void SaR::DrawMesh(ProgramObject& program, glm::mat4& viewProj)
{
	program.SetTexture("texImage", 0, SaR::sar_static_tex);
	program.SetUniform("MVP", viewProj * m_transforms);
	program.SetUniform("world", m_transforms);
	program.SetUniform("worldIT", glm::inverse(glm::transpose(m_transforms)));

	m_static_mesh->draw();
}

HitBox SaR::UpdateDimensions()
{
	HitBox newHitBox = { m_position, {10.0f, 3.0f, 11.0f} };
	glm::vec3 cross_vec = glm::normalize(glm::cross(m_forward_vec, m_up_vec));

	newHitBox.dimensions.height = 2.5 + ((abs(m_up_vec.y) - 1) * (8.0 - 2.5)) / -1;
	newHitBox.dimensions.height = std::max(2.5 + ((abs(m_forward_vec.y) - 0) * (10.0 - 2.5)) / 1, (double)m_hitboxes[0].dimensions.height);

	newHitBox.dimensions.width = 8.0 + ((abs(cross_vec.x) - 1) * (2.5 - 8.0)) / -1;
	newHitBox.dimensions.width = std::max(2.5 + ((abs(m_forward_vec.x)) * (10.0 - 2.5)) / 1, (double)m_hitboxes[0].dimensions.width);

	newHitBox.dimensions.length = 2.5 + ((abs(m_forward_vec.z)) * (10.0 - 2.5)) / 1;
	newHitBox.dimensions.length = std::max(2.5 + ((abs(cross_vec.z)) * (8.0 - 2.5)) / 1, (double)m_hitboxes[0].dimensions.length);

	return newHitBox;
}

std::unique_ptr<Mesh>& SaR::GetMesh()
{
	return m_static_mesh;
}