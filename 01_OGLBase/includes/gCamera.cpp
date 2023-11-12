#include <iostream>
#include "gCamera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <math.h>

#include "../headers/MyApp.h"

/// <summary>
/// Initializes a new instance of the <see cref="gCamera"/> class.
/// </summary>
gCamera::gCamera(void) : m_eye(0.0f, 20.0f, 20.0f), m_at(0.0f), m_up(0.0f, 1.0f, 0.0f), m_speed(16.0f), m_goFw(0), m_goRight(0), m_slow(false)
{
	SetView( glm::vec3(0,20,20), glm::vec3(0,0,0), glm::vec3(0,1,0));

	m_dist = glm::length( m_at - m_eye );	

	SetProj(glm::radians(60.0f), 640/480.0f, 1.0f, 8000.0f);
}

gCamera::gCamera(glm::vec3 _eye, glm::vec3 _at, glm::vec3 _up) : m_speed(16.0f), m_goFw(0), m_goRight(0), m_dist(10), m_slow(false)
{
	SetView(_eye, _at, _up);
}

gCamera::~gCamera(void)
{
}

void gCamera::LinkToApp(CMyApp* app)
{
	m_app = app;
}

void gCamera::SetView(glm::vec3 _eye, glm::vec3 _at, glm::vec3 _up)
{
	m_eye	= _eye;
	m_at	= _at;
	m_up	= _up;

	m_fw  = glm::normalize( m_at - m_eye  );
	m_st = glm::normalize( glm::cross( m_fw, m_up ) );

	m_dist = glm::length( m_at - m_eye );	

	m_u = atan2f( m_fw.z, m_fw.x );
	m_v = acosf( m_fw.y );
}

void gCamera::SetProj(float _angle, float _aspect, float _zn, float _zf)
{
	m_matProj = glm::perspective( _angle, _aspect, _zn, _zf);
	m_matViewProj = m_matProj * m_viewMatrix;
}

glm::mat4 gCamera::GetViewMatrix()
{
	return m_viewMatrix;
}

void gCamera::Update(float _deltaTime)
{
	GameState state = m_app->m_GameState;

	if (state.menu)
	{
		if (m_eye != glm::vec3(0, 0, 0))
		{
			m_eye = glm::vec3(0, 0, 0);
			m_at = glm::vec3(0, 0, 3);
			m_up = glm::vec3(0, 1, 0);
		}
		else
		{
			glm::vec4 cam_at = glm::vec4(m_at, 1);
			cam_at = glm::rotate(_deltaTime * 0.2f, glm::vec3(0.f, 1.f, 0.f)) * cam_at;

			m_at = glm::vec3(cam_at.x, cam_at.y, cam_at.z);
		}
	}

	if (state.hangar)
	{
		if (m_at != m_app->m_player.GetPos())
		{
			FocusOnPosition(glm::vec3(0.f, 0.f, 0.f));
		}
	}

	if (state.play && !state.pause)
	{
		Player* player = &m_app->m_player;
		m_at = player->GetPos() + (m_app->m_cursor_diff_vec * 2000.f);
		m_eye = player->GetPos() - m_app->m_cursor_diff_vec * 40.f + player->GetUpVec() * 5.f;
		m_up = player->GetUpVec();

		if (!m_app->m_lookAround)
		{
			m_eye = player->GetPos() - player->GetForwardVec() * 40.f + player->GetUpVec() * 5.f;
			m_at = player->GetPos() + player->GetForwardVec() * 1000.f;
		}

		if (m_app->m_backward_camera)
		{
			m_eye = player->GetPos() + player->GetForwardVec() * 40.f + player->GetUpVec() * 5.f;
			m_at = player->GetPos() - player->GetForwardVec() * 30.f;
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
	}
	
	//m_eye += (m_goFw*m_fw + m_goRight*m_st)*m_speed*_deltaTime;
	//m_at  += (m_goFw*m_fw + m_goRight*m_st)*m_speed*_deltaTime;

	m_viewMatrix = glm::lookAt( m_eye, m_at, m_up);
	m_matViewProj = m_matProj * m_viewMatrix;
}

void gCamera::UpdateUV(float du, float dv)
{

	m_u += du;
	m_v = glm::clamp<float>(m_v - dv, 0.1f, 3.1f);

	m_eye = m_at + 25.f * glm::vec3(cosf(m_u) * sinf(m_v),
		cosf(m_v),
		sinf(m_u) * sinf(m_v));

	m_fw = glm::normalize(m_at - m_eye);
	m_st = glm::normalize(glm::cross(m_fw, m_up));
}

void gCamera::FocusOnPosition(glm::vec3 pos)
{
	m_u = 0.55f;
	m_v = 1.25f;

	m_at = pos;
	m_eye = m_at + 25.f * glm::vec3(cosf(m_u) * sinf(m_v), cosf(m_v), sinf(m_u) * sinf(m_v));
	m_up = glm::vec3(0.f, 1.f, 0.f);

	m_fw = glm::normalize(m_at - m_eye);
	m_st = glm::normalize(glm::cross(m_fw, m_up));
}

void gCamera::SetSpeed(float _val)
{
	m_speed = _val;
}

void gCamera::Resize(int _w, int _h)
{
	SetProj(glm::radians(60.0f), _w/(float)_h, 1.f, 8000.0f);
}

void gCamera::KeyboardDown(SDL_KeyboardEvent& key)
{
	switch ( key.keysym.sym )
	{
	case SDLK_LSHIFT:
	case SDLK_RSHIFT:
		if ( !m_slow )
		{
			m_slow = true;
			m_speed /= 4.0f;
		}
		break;
	case SDLK_w:
			m_goFw = 1;
		break;
	case SDLK_s:
			m_goFw = -1;
		break;
	case SDLK_a:
			m_goRight = -1;
		break;
	case SDLK_d:
			m_goRight = 1;
		break;
	}
}

void gCamera::KeyboardUp(SDL_KeyboardEvent& key)
{
	float current_speed = m_speed;
	switch ( key.keysym.sym )
	{
	case SDLK_LSHIFT:
	case SDLK_RSHIFT:
		if ( m_slow )
		{
			m_slow = false;
			m_speed *= 4.0f;
		}
		break;
	case SDLK_w:
	case SDLK_s:
			m_goFw = 0;
		break;
	case SDLK_a:
	case SDLK_d:
			m_goRight = 0;
		break;
	}
}

void gCamera::MouseMove(SDL_MouseMotionEvent& mouse)
{
	if ( mouse.state & SDL_BUTTON_LMASK )
	{
		UpdateUV(mouse.xrel/100.0f, mouse.yrel/100.0f);
	}
}

void gCamera::LookAt(glm::vec3 _at)
{
	SetView(m_eye, _at, m_up);
}

