#include "CppUnitTest.h"

#include <GL/glew.h>
#include <SDL.h>

#include "../includes/GLDebugMessageCallback.h"

#include "../01_OGLBase/headers/PLayer.h"
#include "../01_OGLBase/headers/enemy/SaR.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

static SDL_Window* win;
static SDL_GLContext context;

namespace OGLtest
{
	TEST_MODULE_INITIALIZE(ModuleInit)
	{

		if (SDL_Init(SDL_INIT_VIDEO) == -1)
		{
			Logger::WriteMessage("Failed to initialize sdl");
			return;
		}

		win = 0;
		win = SDL_CreateWindow("Hello SDL&OpenGL!",
			100,
			100,
			640,
			480,
			SDL_WINDOW_OPENGL | SDL_WINDOW_HIDDEN);

		if (win == 0)
		{
			Logger::WriteMessage("Failed to create window");
			return;
		}

		context = SDL_GL_CreateContext(win);
		if (context == 0)
		{
			Logger::WriteMessage("Failed to create context");
			return;
		}

		GLenum error = glewInit();
		if (error != GLEW_OK)
		{
			Logger::WriteMessage("Failed to initialize glew");
			return;
		}
	}

	TEST_MODULE_CLEANUP(ModuleCleanup)
	{
		SDL_GL_DeleteContext(context);
		SDL_DestroyWindow(win);
		SDL_Quit();
	}

//Unit tests: --------------------------------------------------------------------------------------------------------------------------------------------

	TEST_CLASS(PlayerTest)
	{
	private:

	public:
		TEST_METHOD(InitPlayer)
		{
			Player player;
			Assert::AreEqual(player.GetHealth(), 100);
		}
	};

	TEST_CLASS(EnemyTest)
	{
	private:

	public:
		TEST_METHOD(InitEnemy)
		{
			SaR sar;
			Assert::AreEqual(sar.GetPos().x, 0.f);
			Assert::AreEqual(sar.GetPos().y, 0.f);
			Assert::AreEqual(sar.GetPos().z, 0.f);
		}
	};
}
