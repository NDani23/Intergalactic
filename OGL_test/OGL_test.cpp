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

		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
#ifdef _DEBUG
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
#endif

		SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
		SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);

		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

		const char* glsl_version = "#version 130";
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

		win = 0;
		win = SDL_CreateWindow("Hello SDL&OpenGL!",
			100,
			100,
			640,
			480,
			SDL_WINDOW_OPENGL | SDL_WINDOW_HIDDEN);

		//SDL_WINDOW_RESIZABLE for debug
		//SDL_WINDOW_FULLSCREEN_DESKTOP for fullscreen

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

		SDL_GL_SetSwapInterval(1);

		GLenum error = glewInit();
		if (error != GLEW_OK)
		{
			Logger::WriteMessage("Failed to initialize glew");
			return;
		}

		int glVersion[2] = { -1, -1 };
		glGetIntegerv(GL_MAJOR_VERSION, &glVersion[0]);
		glGetIntegerv(GL_MINOR_VERSION, &glVersion[1]);
		std::cout << "Running OpenGL " << glVersion[0] << "." << glVersion[1] << std::endl;

		if (glVersion[0] == -1 && glVersion[1] == -1)
		{
			SDL_GL_DeleteContext(context);
			SDL_DestroyWindow(win);

			Logger::WriteMessage("could not create opengl context");

			return;
		}

		std::stringstream window_title;
		window_title << "OpenGL " << glVersion[0] << "." << glVersion[1];
		SDL_SetWindowTitle(win, window_title.str().c_str());

		GLint context_flags;
		glGetIntegerv(GL_CONTEXT_FLAGS, &context_flags);
		if (context_flags & GL_CONTEXT_FLAG_DEBUG_BIT) {
			glEnable(GL_DEBUG_OUTPUT);
			glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
			glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, GL_FALSE);
			glDebugMessageCallback(GLDebugMessageCallback, nullptr);
		}
	}

	TEST_MODULE_CLEANUP(ModuleCleanup)
	{
		SDL_GL_DeleteContext(context);
		SDL_DestroyWindow(win);
		SDL_Quit();
	}



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
