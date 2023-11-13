#pragma once

// C++ includes
#include <memory>

// GLEW
#include <GL/glew.h>

// SDL
#include <SDL.h>
#include <SDL_opengl.h>
#include <string>
#include <sstream>
#include <iomanip>

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>

#include "../includes/gCamera.h"
		  
#include "../includes/ProgramObject.h"
#include "../includes/BufferObject.h"
#include "../includes/VertexArrayObject.h"
#include "../includes/TextureObject.h"

// mesh
#include "../includes/ObjParser_OGL3.h"

#include "Directions.h"
#include "scenes/DeepSpace.h"
#include "scenes/PlanetEarth.h"
#include "weapons/Projectile.h"
#include "Player.h"
#include "weapons/Turret.h"
#include "GameState.h"
#include "AppUI.h"
#include "persistence/Persistence.h"
#include "collision_detection/GJK.h"


class CMyApp
{
public:
	CMyApp();
	~CMyApp();

	bool Init(bool*);
	void Clean();

	void Reset();
	void LoadScene();

	void Update();
	void Render();

	void KeyboardDown(SDL_KeyboardEvent&);
	void KeyboardUp(SDL_KeyboardEvent&);
	void MouseMove(SDL_MouseMotionEvent&);
	void MouseDown(SDL_MouseButtonEvent&);
	void MouseUp(SDL_MouseButtonEvent&);
	void MouseWheel(SDL_MouseWheelEvent&);
	void Resize(int, int);
	void DrawHitBoxes(ProgramObject&, glm::mat4&);
	void GameOver();
	void Exit();

	friend class AppUI;
	friend class Persistence;
	friend class gCamera;

protected:
	// shaderekhez szükséges változók
	ProgramObject		m_axesProgram;		

	gCamera				m_camera;

	GameState m_GameState = {true, false, false, false};

	WeaponDataStorage m_weaponStorage;
	UpgradeDataStorage m_upgradeStorage;

	Persistence m_Persistence;
	AppUI UI;
	Player m_player;

	std::array<std::unique_ptr<Scene>, 2> m_scenes;
	Scene* m_scene;

	float m_PlayTime = 0;

	bool m_shooting = false;
	bool m_useUpgrade = false;
	bool m_lookAround = false;

	bool m_backward_camera = false;
	glm::vec3 m_cursor_diff_vec = { 0.0f, 0.0f, 0.0f };

	float m_mouseX = 320.0f;
	float m_mouseY = 240.0f;

	int m_screenWidth = 640;
	int m_screenHeight = 480;

	struct Vertex
	{
		glm::vec3 p;
		glm::vec3 n;
		glm::vec2 t;
	};

	bool* m_quit;

	// a jobb olvashatóság kedvéért
	void InitShaders();
};

