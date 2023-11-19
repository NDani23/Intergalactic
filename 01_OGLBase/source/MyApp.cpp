#include "../headers/MyApp.h"

#include <math.h>
#include <vector>

#include <array>
#include <list>
#include <tuple>
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include "../includes/GLUtils.hpp"

#include "../headers/Timer.h"


CMyApp::CMyApp(void)
{
	m_Persistence = Persistence(this);
	UI = AppUI(this);
	m_camera.SetView(glm::vec3(0, 0, 0), glm::vec3(0, 0, 3), glm::vec3(0, 1, 0));
	m_quit = nullptr;

	m_scenes[0] = std::make_unique<DeepSpace>(&m_player);
	m_scenes[1] = std::make_unique<PlanetEarth>(&m_player);
	m_scene = m_scenes[0].get();
}

CMyApp::~CMyApp(void)
{
}

void CMyApp::InitShaders()
{

	m_axesProgram.Init({
		{GL_VERTEX_SHADER, "shaders/axes.vert"},
		{GL_FRAGMENT_SHADER, "shaders/axes.frag"}
		});
}

bool CMyApp::Init(bool* quit)
{
	m_quit = quit;

	m_Persistence.Load();

	m_camera.LinkToApp(this);

	m_player.Reset(m_scene);

	glClearColor(0.125f, 0.25f, 0.5f, 1.0f);

	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glLineWidth(4.0f);

	InitShaders();

	m_camera.SetProj(glm::radians(60.0f), 640.0f / 480.0f, 1.f, 8000.0f);

	return true;
}

void CMyApp::Clean()
{
}

void CMyApp::Update()
{
	//Timer timer;
	float delta_time = ImGui::GetIO().DeltaTime;

	m_cursor_diff_vec = glm::normalize((m_player.GetUpVec() * -1.0f * m_mouseY) + (m_player.GetCrossVec() * -1.0f * m_mouseX) + m_player.GetForwardVec());

	if (m_GameState.play && !m_GameState.pause)
	{
		if (!m_GameState.gameover)
		{
			m_PlayTime += delta_time;

			m_player.Move(delta_time, m_cursor_diff_vec);
			m_player.UpdateProjectiles(delta_time);

			if (m_shooting) m_player.Shoot();
			if (m_useUpgrade && m_player.GetUpgrade() != nullptr) m_player.ActivateUpgrade();
		}

		if (m_scene->Update(delta_time, m_GameState))
		{
			GameOver();
		}	
	}

	m_camera.Update(delta_time);
}

void CMyApp::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glm::mat4 viewProj = m_camera.GetViewProj();

	//Objects
	ProgramObject& BaseProgram = m_scene->getProgram();
	
	glm::vec3 eye_pos = m_camera.GetEye();
	BaseProgram.SetUniform("eye_pos", eye_pos);

	m_scene->DrawScene(viewProj, m_GameState, m_camera.GetEye(), m_axesProgram);

	BaseProgram.Use();
	if ((m_GameState.play && !m_GameState.gameover) || m_GameState.hangar)
	{
		m_player.DrawMesh(BaseProgram, viewProj);	
	}
	BaseProgram.Unuse();

	//DrawHitBoxes(m_axesProgram, viewProj);

	//ImGui
	UI.Render();
}

void CMyApp::Reset()
{
	m_PlayTime = 0.f;
	m_player.Reset(m_scene);
}

void CMyApp::GameOver()
{
	m_player.setHealth(0);
	m_player.setCredit(m_player.GetCredit() + m_player.GetPoints());
	if (m_player.GetPoints() > m_player.GetRecord()) m_player.setRecord(m_player.GetPoints());
	m_GameState.gameover = true;
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
		m_player.SlowDown(true);
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
		m_scene->AddEnemy();
		break;
	case SDLK_LCTRL:
		m_player.SlowDown(false);
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
	if (m_GameState.hangar) m_camera.MouseMove(mouse);

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
	if (wheel.y > 0) // scroll up
	{
		m_player.Accelerate();
	}
	else if (wheel.y < 0) // scroll down
	{
		m_player.Decelerate();
	}
}

void CMyApp::Resize(int _w, int _h)
{
	m_screenWidth = _w;
	m_screenHeight = _h;

	glViewport(0, 0, _w, _h );

	m_camera.Resize(_w, _h);
}

void CMyApp::DrawHitBoxes(ProgramObject& program, glm::mat4& viewProj)
{
	std::vector<glm::vec3> Points;

	for (std::shared_ptr<Entity>& entity : m_scene->GetEntities())
	{
		for (HitBox& hitbox : entity->GetHitboxes())
		{
			hitbox.Draw(program, viewProj);
		}
	}


	m_player.GetHitboxes()[0].Draw(program, viewProj);
}

void CMyApp::Exit()
{
	m_Persistence.Save();
}