#include "MyApp.h"

#include <math.h>
#include <vector>

#include <array>
#include <list>
#include <tuple>
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include "includes/GLUtils.hpp"


CMyApp::CMyApp(void)
{
	m_Persistence = Persistence(this);
	UI = AppUI(this);
	m_camera.SetView(glm::vec3(0, 0, 0), glm::vec3(0, 0, 3), glm::vec3(0, 1, 0));
	m_quit = nullptr;

	m_maps[0] = std::make_unique<DeepSpace>(&m_projectiles, &m_player);
	m_maps[1] = std::make_unique<PlanetEarth>(&m_projectiles, &m_player);
	m_map = m_maps[0].get();
}

CMyApp::~CMyApp(void)
{
}

void CMyApp::InitSkyBox()
{
	m_SkyboxPos.BufferData(
		std::vector<glm::vec3>{

		glm::vec3(-1, -1, -1),
		glm::vec3(1, -1, -1),
		glm::vec3(1, 1, -1),
		glm::vec3(-1, 1, -1),

		glm::vec3(-1, -1, 1),
		glm::vec3(1, -1, 1),
		glm::vec3(1, 1, 1),
		glm::vec3(-1, 1, 1),
	}
	);


	m_SkyboxIndices.BufferData(
		std::vector<int>{
			// hátsó lap
			0, 1, 2,
			2, 3, 0,
			// elülső lap
			4, 6, 5,
			6, 4, 7,
			// bal
			0, 3, 4,
			4, 3, 7,
			// jobb
			1, 5, 2,
			5, 6, 2,
			// alsó
			1, 0, 4,
			1, 4, 5,
			// felső
			3, 2, 6,
			3, 6, 7,
	}
	);

	m_SkyboxVao.Init(
		{
			{ CreateAttribute<0, glm::vec3, 0, sizeof(glm::vec3)>, m_SkyboxPos },
		}, m_SkyboxIndices
	);

	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

	m_skyboxTexture = m_map->GetSkyBox();

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void CMyApp::InitShaders()
{

	m_axesProgram.Init({
		{GL_VERTEX_SHADER, "axes.vert"},
		{GL_FRAGMENT_SHADER, "axes.frag"}
		});
}

bool CMyApp::Init(bool* quit)
{
	m_quit = quit;

	m_Persistence.Load();

	glClearColor(0.125f, 0.25f, 0.5f, 1.0f);

	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glLineWidth(4.0f);

	InitShaders();
	InitSkyBox();

	m_camera.SetProj(glm::radians(60.0f), 640.0f / 480.0f, 1.f, 8000.0f);

	return true;
}

void CMyApp::Clean()
{
}

void CMyApp::Reset()
{
	m_PlayTime = 0.f;
	m_projectiles.clear();
	m_player.Reset(m_map);
	m_map->LoadMap();
}

void CMyApp::Update()
{
	float delta_time = ImGui::GetIO().DeltaTime;

	if (m_GameState.menu)
	{

		if (m_camera.GetEye() != glm::vec3(0, 0, 0))
		{
			m_camera.SetView(glm::vec3(0, 0, 0), glm::vec3(0, 0, 3), glm::vec3(0, 1, 0));
		}
		else
		{
			glm::vec4 cam_at = glm::vec4(m_camera.GetAt(), 1);
			cam_at = glm::rotate(delta_time * 0.2f, glm::vec3(0.f, 1.f, 0.f)) * cam_at;
			m_camera.SetView(glm::vec3(0, 0, 0), glm::vec3(cam_at.x, cam_at.y, cam_at.z), glm::vec3(0, 1, 0));
		}
		m_camera.Update(delta_time);
		return;
	}

	if (m_GameState.hangar)
	{
		if (m_player.GetPos() != glm::vec3(0.0f, 0.0f, 0.0f)) m_player.Reset(m_map);
		glm::vec3 cam_at = glm::vec4(m_player.GetPos(), 1);
		glm::vec3 cam_eye = cam_at + m_player.GetCrossVec() * 20.f + m_player.GetUpVec() * 10.f + m_player.GetForwardVec() * 10.f;
		glm::vec3 cam_up = m_player.GetUpVec();

		m_camera.SetView(cam_eye, cam_at, cam_up);
		m_camera.Update(delta_time);
		return;
	}

	if (m_GameState.pause)
	{
		m_camera.Update(delta_time);
		return;
	}

	m_cursor_diff_vec = glm::normalize((m_player.GetUpVec() * -1.0f * m_mouseY) + (m_player.GetCrossVec() * -1.0f * m_mouseX) + m_player.GetForwardVec());

	if (!m_GameState.gameover)
	{
		m_PlayTime += delta_time;

		m_player.Move(delta_time, m_cursor_diff_vec);		
		m_player.UpdateProjectiles(delta_time);
	}

	UpdateProjectiles(delta_time);
	DetectHit(m_player.GetProjectiles());

	UpdateMap(delta_time);

	if (m_shooting) m_player.Shoot();
	if (m_useUpgrade && m_player.GetUpgrade() != nullptr) m_player.ActivateUpgrade();

	//camera
	glm::vec3 new_at = m_player.GetPos() + (m_cursor_diff_vec * 2000.f);
	glm::vec3 new_eye = m_player.GetPos() - m_cursor_diff_vec * 40.f + m_player.GetUpVec() * 5.f;
	glm::vec3 new_up = m_player.GetUpVec();
	if (!m_lookAround)
	{
		new_eye = m_player.GetPos() - m_player.GetForwardVec() * 40.f + m_player.GetUpVec() * 5.f;
		new_at = m_player.GetPos() + m_player.GetForwardVec() * 1000.f;
	}

	if (m_backward_camera)
	{
		new_eye = m_player.GetPos() + m_player.GetForwardVec() * 40.f + m_player.GetUpVec() * 5.f;
		new_at = m_player.GetPos() - m_player.GetForwardVec() * 30.f;
	}

	//camera teszt up
	/*glm::vec3 new_eye = m_player.GetPos() + m_player.GetUpVec() * 50.f;
	glm::vec3 new_at = m_player.GetPos();
	glm::vec3 new_up = m_player.GetForwardVec();*/

	////camera teszt behind
	/*glm::vec3 new_eye = m_player.GetPos() - m_player.GetForwardVec() * 40.f;
	glm::vec3 new_at = m_player.GetPos();
	glm::vec3 new_up = m_player.GetUpVec();*/

	//camera teszt side
	/*glm::vec3 new_eye = m_player.GetPos() + m_player.GetCrossVec() * 40.f;
	glm::vec3 new_at = m_player.GetPos();
	glm::vec3 new_up = m_player.GetUpVec();*/

	m_camera.SetView(new_eye, new_at, new_up);
	m_camera.Update(delta_time);

	DetectCollisions();
}

void CMyApp::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glm::mat4 viewProj = m_camera.GetViewProj();

	// skybox
	GLint prevDepthFnc;
	glGetIntegerv(GL_DEPTH_FUNC, &prevDepthFnc);

	glDepthFunc(GL_LEQUAL);

	m_SkyboxVao.Bind();
	ProgramObject& SkyBoxProgram = m_map->getSkyBoxProgram();

	SkyBoxProgram.Use();
	SkyBoxProgram.SetUniform("MVP", viewProj * glm::translate(m_camera.GetEye()));

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_skyboxTexture);
	glUniform1i(SkyBoxProgram.GetLocation("skyboxTexture"), 0);

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);
	SkyBoxProgram.Unuse();

	glDepthFunc(prevDepthFnc);


	//Objects
	ProgramObject& BaseProgram = m_map->getProgram();

	BaseProgram.Use();
	if (m_GameState.play || m_GameState.hangar)
	{

		if (!m_GameState.gameover)
		{
			//player
			m_player.DrawMesh(BaseProgram, viewProj);
		}
	}

	m_map->DrawEntities(viewProj, m_GameState);
	if (m_GameState.play)
	{
		glm::vec3 eye_pos = m_camera.GetEye();
		BaseProgram.SetUniform("eye_pos", eye_pos);

		BaseProgram.Unuse();

		DrawProjectiles(m_player.GetProjectiles());

		if (m_player.GetTarget() != nullptr && m_player.GetWeapons()[m_player.GetActiveWeaponInd()]->requireTarget())
		{
			m_player.GetTarget()->GetHitboxes()[0].Draw(m_axesProgram, viewProj);
		}

		//DrawHitBoxes(m_axesProgram, viewProj);
		BaseProgram.Use();
	}
	
	BaseProgram.Unuse();

	//ImGui
	UI.Render();
}

void CMyApp::KeyboardDown(SDL_KeyboardEvent& key)
{
	switch (key.keysym.sym)
	{
	case SDLK_w:
		m_player.setPitchDir(vertical::down);
		break;
	case SDLK_s:
		m_player.setPitchDir(vertical::up);
		break;
	case SDLK_a:
		m_player.setRollDir(horizontal::left);
		break;
	case SDLK_d:
		m_player.setRollDir(horizontal::right);
		break;
	case SDLK_1:
		m_player.setActiveWeapon(0);
		break;
	case SDLK_2:
		m_player.setActiveWeapon(1);
		break;
	case SDLK_3:
		m_player.setActiveWeapon(2);
		break;
	case SDLK_SPACE:
		m_shooting = true;
		break;
	case SDLK_LCTRL:
		m_player.Decelerate(true);
		break;
	case SDLK_q:
		m_player.FlyStraight(true);
		break;
	case SDLK_v:
		m_backward_camera = true;
		m_player.LookBack(true);
		break;
	case SDLK_ESCAPE:
		if(m_GameState.play)
			m_GameState.pause = true;
		break;
	case SDLK_LSHIFT:
		m_useUpgrade = true;
		break;
	}
}

void CMyApp::KeyboardUp(SDL_KeyboardEvent& key)
{
	switch (key.keysym.sym)
	{
	case SDLK_w:
		m_player.setPitchDir(vertical::none);
		break;
	case SDLK_s:
		m_player.setPitchDir(vertical::none);
		break;
	case SDLK_a:
		m_player.setRollDir(horizontal::none);
		break;
	case SDLK_d:
		m_player.setRollDir(horizontal::none);
		break;
	case SDLK_SPACE:
		m_shooting = false;
		break;
	case SDLK_LCTRL:
		m_player.Decelerate(false);
		break;
	case SDLK_q:
		m_player.FlyStraight(false);
		break;
	case SDLK_v:
		m_backward_camera = false;
		m_player.LookBack(false);
		break;
	case SDLK_LSHIFT:
		m_useUpgrade = false;
		break;
	}
}

void CMyApp::MouseMove(SDL_MouseMotionEvent& mouse)
{
	m_mouseX = mouse.x / (float)(m_screenWidth / 2) - 1;
	m_mouseY = mouse.y / (float)(m_screenHeight / 2) - 1;
}

void CMyApp::MouseDown(SDL_MouseButtonEvent& mouse)
{
	if (mouse.button == SDL_BUTTON_LEFT)
	{
		m_shooting = true;
	}
	else if (mouse.button == SDL_BUTTON_RIGHT)
	{
		m_lookAround = true;
	}
}

void CMyApp::MouseUp(SDL_MouseButtonEvent& mouse)
{
	if (mouse.button == SDL_BUTTON_LEFT)
	{
		m_shooting = false;
	}
	else if (mouse.button == SDL_BUTTON_RIGHT)
	{
		m_lookAround = false;
	}
}

void CMyApp::MouseWheel(SDL_MouseWheelEvent& wheel)
{
}

void CMyApp::Resize(int _w, int _h)
{
	m_screenWidth = _w;
	m_screenHeight = _h;

	glViewport(0, 0, _w, _h );

	m_camera.Resize(_w, _h);
}

void CMyApp::DetectCollisions()
{
	glm::vec3 player_pos = m_player.GetPos();
	Dimensions player_dims = m_player.GetHitboxes()[0].dimensions;

	if (m_map->GetFloor() != nullptr)
	{
		if (m_map->GetFloor()->DetectCollision(m_player))
		{
			GameOver();
			return;
		}
	}

	for (std::shared_ptr<Entity>& entity : m_map->GetEntities())
	{
		if (m_GameState.gameover) break;
		if (!entity->CanCollide()) break;

		for (HitBox& hitbox : entity->GetHitboxes())
		{
			glm::vec3 distance_vec = player_pos - hitbox.Position;

			Dimensions hitbox_dims = hitbox.dimensions;

			if (abs(distance_vec.x) < player_dims.width / 2 + hitbox_dims.width / 2
				&& abs(distance_vec.y) < player_dims.height / 2 + hitbox_dims.height / 2
				&& abs(distance_vec.z) < player_dims.length / 2 + hitbox_dims.length / 2)
			{
				if (GJK::Collide(m_player.GetCollider(), entity.get()->GetCollider()))
				{
					GameOver();
					break;
				}
				/*GameOver();
				break;*/
			}
		}
		
	}
}

void CMyApp::DetectHit(std::vector<std::unique_ptr<Projectile>>& projectiles)
{
	for (int i=0; i < projectiles.size(); i++)
	{
		Projectile* proj = projectiles[i].get();
		for (std::shared_ptr<Entity>& entity : m_map->GetEntities())
		{
			if (!entity->CanCollide()) continue;

			if (proj->CheckHit(entity.get()))
			{
				if (entity->Hit(proj->GetDamage()))
				{
					auto position = std::find(m_map->GetEntities().begin(), m_map->GetEntities().end(), entity);
					if (position != m_map->GetEntities().end())
						m_map->GetEntities().erase(position);

					m_player.setPoints(m_player.GetPoints() + 10);
				}

				m_player.RemoveProjectile(projectiles[i]);
				break;
			}
		}
	}

	for (int i = 0; i < m_projectiles.size(); i++)
	{
		bool alive = true;
		Projectile* proj = m_projectiles[i].get();
		for (std::shared_ptr<Entity>& entity : m_map->GetEntities())
		{
			if (proj->CheckHit(entity.get()))
			{
				if (entity->Hit(proj->GetDamage()))
				{
					auto position = std::find(m_map->GetEntities().begin(), m_map->GetEntities().end(), entity);
					if (position != m_map->GetEntities().end())
						m_map->GetEntities().erase(position);

					m_player.setPoints(m_player.GetPoints() + 10);
				}

				m_projectiles.erase(m_projectiles.begin() + i);
				alive = false;
				break;
			}
		}
		if (!alive) break;

		if (proj->CheckHit(&m_player))
		{
			m_projectiles.erase(m_projectiles.begin() + i);

			if (m_player.Hit(proj->GetDamage()))
			{
				GameOver();
				break;
			}
		}

	}

}

void CMyApp::DrawProjectiles(std::vector<std::unique_ptr<Projectile>>& projectiles)
{
	ProgramObject& BaseProgram = m_map->getProgram();
	for (std::unique_ptr<Projectile>& projectile : projectiles)
	{
		if (projectile->GetMesh() == nullptr)
		{
			m_axesProgram.Use();
			projectile->DrawMesh(m_axesProgram, m_camera.GetViewProj());
			m_axesProgram.Unuse();
		}
		else
		{
			BaseProgram.Use();
			projectile->DrawMesh(BaseProgram, m_camera.GetViewProj());
			BaseProgram.Unuse();
		}
	}

	for (std::unique_ptr<Projectile>& projectile : m_projectiles)
	{
		if (projectile->GetMesh() == nullptr)
		{
			m_axesProgram.Use();
			projectile->DrawMesh(m_axesProgram, m_camera.GetViewProj());
			m_axesProgram.Unuse();
		}
		else
		{
			BaseProgram.Use();
			projectile->DrawMesh(BaseProgram, m_camera.GetViewProj());
			BaseProgram.Unuse();
		}
	}
}

void CMyApp::UpdateMap(const float& delta)
{
	m_player.setTarget(nullptr);

	for (std::unique_ptr<EnemySpawnPoint>& spawnPoint : m_map->GetSpawnPoints())
	{
		spawnPoint->Update(delta);
	}

	for (int i = m_map->GetEntities().size() - 1; i >= 0; i--)
	{

		Entity* entity = m_map->GetEntities()[i].get();
		if (entity->Update(delta))
		{
			m_map->GetEntities().erase(m_map->GetEntities().begin() + i);
			m_player.setPoints(m_player.GetPoints() + 10);

			continue;
		}

		if (!entity->IsTargetable()) continue;

		glm::vec3 entityPos = entity->GetPos();
		glm::vec3 from_player = entityPos - m_player.GetPos();
		float angle = glm::acos(glm::dot(glm::normalize(from_player), m_player.GetForwardVec()));

		if (glm::length(from_player) < 500.f && !isnan(angle) && !isinf(angle) && angle < 0.3f)
		{
			m_player.setTarget(entity);
		}
		
	}

}

void CMyApp::UpdateProjectiles(const float& delta)
{
	for (int i = 0; i < m_projectiles.size(); i++)
	{
		if (m_projectiles[i]->Update(delta))
		{
			m_projectiles.erase(m_projectiles.begin() + i);
		}
	}
}

void CMyApp::DrawHitBoxes(ProgramObject& program, glm::mat4& viewProj)
{
	std::vector<glm::vec3> Points;

	for (std::shared_ptr<Entity>& entity : m_map->GetEntities())
	{
		for (HitBox& hitbox : entity->GetHitboxes())
		{
			hitbox.Draw(program, viewProj);
		}
	}


	m_player.GetHitboxes()[0].Draw(program, viewProj);
}

void CMyApp::GameOver()
{
	m_player.setHealth(0);
	m_player.setCredit(m_player.GetCredit() + m_player.GetPoints());
	if (m_player.GetPoints() > m_player.GetRecord()) m_player.setRecord(m_player.GetPoints());
	m_GameState.gameover = true;
}

void CMyApp::Exit()
{
	m_Persistence.Save();
}