#include "CppUnitTest.h"

#include <GL/glew.h>
#include <SDL.h>

#include <iostream>
#include <string>
#include <cstdint>
#include <cassert>

#include "../includes/GLDebugMessageCallback.h"

#include "../01_OGLBase/headers/Player.h"
#include "../01_OGLBase/headers/enemy/SaR.h"
#include "../01_OGLBase/headers/scenes/Scene.h"
#include "../01_OGLBase/headers/collision_detection/AAB.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

static SDL_Window* win;
static SDL_GLContext context;

bool TestVectorEqual(glm::vec3 vector1, glm::vec3 vector2)
{
	return vector1 == vector2;
}

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
			Assert::AreEqual(true,TestVectorEqual(player.GetPos(), glm::vec3(0.f, 0.f, 0.f)));

			Assert::AreEqual(player.GetHitboxes().size(), (size_t)1);
			Assert::AreEqual(true,TestVectorEqual(player.GetHitboxes().at(0).Position, glm::vec3(0.f, 0.f, 0.f)));

			Assert::AreEqual(player.GetHealth(), 100);
			Assert::AreEqual(player.GetProjectiles().size(), (size_t)0);
		}

		TEST_METHOD(ResetPlayer)
		{
			Scene m_dummyScene;
			Player player;
			
			player.SetPos(glm::vec3(10.f, 10.f, 10.f));
			player.Hit(10);
			player.Shoot();

			player.Reset(&m_dummyScene);

			Assert::AreEqual(true,TestVectorEqual(player.GetPos(), glm::vec3(0.f, 0.f, 0.f)));
			Assert::AreEqual(player.GetHealth(), 100);
			Assert::AreEqual(player.GetProjectiles().size(), (size_t)0);

		}

		TEST_METHOD(MovePlayer)
		{
			Scene m_dummyScene;
			Player player;

			player.setMap(&m_dummyScene);
			player.Move(1, player.GetForwardVec());

			Assert::AreEqual(true,TestVectorEqual(player.GetPos(), glm::vec3(0.f, 0.f, 90.f)));

			Assert::AreEqual(player.GetHitboxes().size(), (size_t)1);
			Assert::AreEqual(true,TestVectorEqual(player.GetHitboxes().at(0).Position, glm::vec3(0.f, 0.f, 90.f)));

			Assert::AreEqual(true,TestVectorEqual(player.GetPos(), player.GetWeapons()[1].get()->GetPos()));
		}

		TEST_METHOD(PlayerShoot)
		{
			Player player;

			Assert::AreEqual(player.GetProjectiles().size(), (size_t)0);

			player.Shoot();

			Assert::AreEqual(player.GetProjectiles().size(), (size_t)2);
		}

		TEST_METHOD(HitPlayer)
		{
			Player player;
			int max_health = player.GetMaxHealth();

			Assert::AreEqual(max_health, player.GetHealth());
			
			bool dead = player.Hit(10);
			Assert::AreEqual(max_health - 10, player.GetHealth());
			Assert::AreEqual(dead, false);

			player.setHealth(max_health);
			dead = player.Hit(500);
			Assert::AreEqual(max_health - 500, player.GetHealth());
			Assert::AreEqual(dead, true);
		}

		TEST_METHOD(ModifyPlayerSpeed)
		{
			Player player;
			
			Assert::AreEqual(player.GetSpeed(), player.GetMaxSpeed());

			player.Decelerate();
			Assert::AreEqual(player.GetSpeed(), player.GetMaxSpeed() - 2);

			player.Accelerate();
			Assert::AreEqual(player.GetSpeed(), player.GetMaxSpeed());

			player.Accelerate();
			Assert::AreEqual(player.GetSpeed(), player.GetMaxSpeed());

			for (int i = 0; i < 100; i++)
			{
				player.Decelerate();
			}

			Assert::AreEqual(player.GetSpeed(), 80);
		}
	};

	TEST_CLASS(WeaponsTest)
	{
	private:

	public:
		TEST_METHOD(General)
		{
			Player player;

			Assert::AreEqual((void*)player.GetWeapons()[0].get(), (void*)nullptr);
			Assert::AreNotEqual((void*)player.GetWeapons()[1].get(), (void*)nullptr);
			Assert::AreEqual((void*)player.GetWeapons()[2].get(), (void*)nullptr);

			Assert::AreEqual(player.GetActiveWeaponInd(), 1);

			player.setActiveWeapon(-1);
			Assert::AreEqual(player.GetActiveWeaponInd(), 1);

			player.setActiveWeapon(2);
			Assert::AreEqual(player.GetActiveWeaponInd(), 1);

			player.setActiveWeapon(0);
			Assert::AreEqual(player.GetActiveWeaponInd(), 1);

			player.setActiveWeapon(5);
			Assert::AreEqual(player.GetActiveWeaponInd(), 1);

			player.GetWeapons()[0] = std::make_unique<RocketLauncher>(&player, -1);
			player.setActiveWeapon(0);
			Assert::AreEqual(player.GetActiveWeaponInd(), 0);

			player.GetWeapons()[2] = std::make_unique<RocketLauncher>(&player, 1);
			player.setActiveWeapon(2);
			Assert::AreEqual(player.GetActiveWeaponInd(), 2);
		}

		TEST_METHOD(RocketLauncherTest)
		{
			Player player;
			Scene m_dummyScene(&player);
			GameState GameState = { false, true, false, false };

			player.Reset(&m_dummyScene);

			player.GetWeapons()[0] = std::make_unique<RocketLauncher>(&player, -1);
			player.setActiveWeapon(0);
			Assert::AreEqual(player.GetActiveWeaponInd(), 0);

			player.Shoot();

			Assert::AreEqual(player.GetProjectiles().size(), (size_t)0);

			m_dummyScene.AddEnemy(glm::vec3(0, 0, 50));

			m_dummyScene.Update(0, GameState);
			player.Shoot();
			Assert::AreEqual(player.GetProjectiles().size(), (size_t)1);

			player.Shoot();
			Assert::AreEqual(player.GetProjectiles().size(), (size_t)1);

			player.GetWeapons()[0]->Update(60);
			player.Shoot();
			Assert::AreEqual(player.GetProjectiles().size(), (size_t)2);
		}

		TEST_METHOD(MachineGunTest)
		{
			Player player;

			player.GetWeapons()[0] = std::make_unique<MachineGun>(&player, -1);
			player.setActiveWeapon(0);
			Assert::AreEqual(player.GetActiveWeaponInd(), 0);

			player.Shoot();
			Assert::AreEqual(player.GetProjectiles().size(), (size_t)1);

			player.Shoot();
			Assert::AreEqual(player.GetProjectiles().size(), (size_t)1);

			player.GetWeapons()[0]->Update(0.26f);
			player.Shoot();
			Assert::AreEqual(player.GetProjectiles().size(), (size_t)2);
		}

		TEST_METHOD(MinePlacesTest)
		{
			Player player;

			player.GetWeapons()[0] = std::make_unique<MinePlacer>(&player, -1);
			player.setActiveWeapon(0);
			Assert::AreEqual(player.GetActiveWeaponInd(), 0);

			player.Shoot();
			Assert::AreEqual(player.GetProjectiles().size(), (size_t)1);

			player.Shoot();
			Assert::AreEqual(player.GetProjectiles().size(), (size_t)1);

			player.GetWeapons()[0]->Update(16.f);
			player.Shoot();
			Assert::AreEqual(player.GetProjectiles().size(), (size_t)2);
		}

		TEST_METHOD(MinePlacerTest)
		{
			Player player;
			Scene m_dummyScene(&player);
			GameState GameState = { false, true, false, false };

			player.Reset(&m_dummyScene);

			player.GetWeapons()[0] = std::make_unique<TurretWeapon>(&player, -1);
			player.setActiveWeapon(0);
			Assert::AreEqual(player.GetActiveWeaponInd(), 0);

			player.Shoot();

			Assert::AreEqual(player.GetProjectiles().size(), (size_t)0);

			m_dummyScene.AddEnemy(glm::vec3(0, 0, 50));

			m_dummyScene.Update(0, GameState);
			player.GetWeapons()[0]->Update(1);
			Assert::AreEqual(player.GetProjectiles().size(), (size_t)1);

		}
	
	};

	TEST_CLASS(UpgadesTest)
	{
	private:

	public:
		TEST_METHOD(General)
		{
			Player player;

			Assert::AreEqual((void*)player.GetUpgrade().get(), (void*)nullptr);

			player.GetUpgrade() = std::make_unique<SpeedBooster>(&player);

			Assert::AreNotEqual((void*)player.GetUpgrade().get(), (void*)nullptr);
		}

		TEST_METHOD(SpeedBoosterTest)
		{
			Player player;
			Scene DummyScene(&player);
			player.Reset(&DummyScene);

			player.GetUpgrade() = std::make_unique<SpeedBooster>(&player);

			Assert::AreEqual(player.GetSpeed(), player.GetMaxSpeed());

			player.ActivateUpgrade();
			Assert::AreEqual(player.GetSpeed(), player.GetMaxSpeed() * 2);

			player.ActivateUpgrade();
			Assert::AreEqual(player.GetSpeed(), player.GetMaxSpeed() * 2);

			player.GetUpgrade()->Update(20.f);
			Assert::AreEqual(player.GetSpeed(), player.GetMaxSpeed());
		}

		TEST_METHOD(ShieldTest)
		{
			Player player;

			player.GetUpgrade() = std::make_unique<Shield>(&player);

			player.Hit(10);
			Assert::AreEqual(player.GetHealth(), player.GetMaxHealth()-10);

			player.ActivateUpgrade();
			player.Hit(10);
			Assert::AreEqual(player.GetHealth(), player.GetMaxHealth() - 10);

			player.GetUpgrade()->Update(30.f);
			player.Hit(10);
			Assert::AreEqual(player.GetHealth(), player.GetMaxHealth() - 20);
		}

		TEST_METHOD(StealthCoatTest)
		{
			Player player;

			player.GetUpgrade() = std::make_unique<StealthCoat>(&player);

			Assert::AreEqual(player.IsStealth(), false);
			Assert::AreEqual(true, TestVectorEqual(player.GetFakePos(), glm::vec3(0, 0, 0)));

			player.ActivateUpgrade();

			Assert::AreEqual(player.IsStealth(), true);
			Assert::AreEqual(true, TestVectorEqual(player.GetFakePos(), glm::vec3(0, 0, 5000.f)));

			player.GetUpgrade()->Update(30.f);

			Assert::AreEqual(player.IsStealth(), false);
		}
	};

	TEST_CLASS(SceneTest)
	{
	private:
	public:
		TEST_METHOD(EnemySpawnTest)
		{
			Player player;
			Scene DummyScene(&player);

			Assert::AreEqual(DummyScene.GetEntities().size(), (size_t)0);
			DummyScene.GetSpawnPoints().emplace_back(std::make_unique<EnemySpawnPoint>(glm::vec3(0, 30, 1900), &player, &DummyScene.GetProjectiles(), &DummyScene));

			Assert::AreEqual(DummyScene.GetEntities().size(), (size_t)1);

			DummyScene.GetSpawnPoints().at(0)->Update(10.f);
			Assert::AreEqual(DummyScene.GetEntities().size(), (size_t)1);

			DummyScene.GetSpawnPoints().at(0)->Update(10.f);
			Assert::AreEqual(DummyScene.GetEntities().size(), (size_t)2);

			DummyScene.GetSpawnPoints().at(0)->Update(19.f);
			Assert::AreEqual(DummyScene.GetEntities().size(), (size_t)3);
		}

		TEST_METHOD(ObjectsCollide)
		{
			Player player;
			Scene DummyScene(&player);
			GameState GameState = { false, true, false, false };

			DummyScene.AddEntity(std::make_shared<Entity>("assets/DeepSpace/meteor.obj", glm::vec3(0, 0, 30), "assets/DeepSpace/meteor_tex.jpg", Dimensions{ 27.0f, 27.0f, 27.0f }));

			DummyScene.AddEnemy(glm::vec3(0, 0, 30));

			Assert::AreEqual(DummyScene.GetEntities().size(), (size_t)2);

			DummyScene.Update(0.1f, GameState);

			Assert::AreEqual(DummyScene.GetEntities().size(), (size_t)1);
		}

		TEST_METHOD(CheckHitPlayer)
		{
			Player player;
			Scene DummyScene(&player);
			GameState GameState = { false, true, false, false };


			Assert::AreEqual(player.GetHealth(), player.GetMaxHealth());

			DummyScene.GetProjectiles().emplace_back(std::make_unique<Laser>(glm::vec3(0, 0, 5), glm::vec3(0, 0, -1), 10));
			Assert::AreEqual(DummyScene.GetProjectiles().size(), (size_t)1);

			DummyScene.Update(0.0001f, GameState);

			Assert::AreEqual(player.GetHealth(), player.GetMaxHealth() - 10);
			Assert::AreEqual(DummyScene.GetProjectiles().size(), (size_t)0);
		}

		TEST_METHOD(CheckHitEnemy)
		{
			Player player;
			Scene DummyScene(&player);
			GameState GameState = { false, true, false, false };

			DummyScene.AddEnemy(glm::vec3(0, 0, 30));

			Assert::AreEqual(DummyScene.GetEntities().size(), (size_t)1);

			player.GetProjectiles().emplace_back(std::make_unique<Laser>(glm::vec3(0, 0, 25), glm::vec3(0, 0, 1), 150));

			DummyScene.Update(0.0001f, GameState);

			Assert::AreEqual(DummyScene.GetEntities().size(), (size_t)0);
			Assert::AreEqual(player.GetProjectiles().size(), (size_t)0);
		}

		TEST_METHOD(GameOverByCollision)
		{
			Player player;
			Scene DummyScene(&player);
			GameState GameState = { false, true, false, false };
			
			DummyScene.AddEntity(std::make_shared<Entity>("assets/DeepSpace/meteor.obj", glm::vec3(0, 0, 100), "assets/DeepSpace/meteor_tex.jpg", Dimensions{ 27.0f, 27.0f, 27.0f }));
			bool isGameOver = DummyScene.Update(0.001f, GameState);
			Assert::AreEqual(false, isGameOver);

			DummyScene.AddEntity(std::make_shared<Entity>("assets/DeepSpace/meteor.obj", glm::vec3(0, 0, 0), "assets/DeepSpace/meteor_tex.jpg", Dimensions{ 27.0f, 27.0f, 27.0f }));
			isGameOver = DummyScene.Update(0.001f, GameState);
			Assert::AreEqual(true, isGameOver);
		}

		TEST_METHOD(GameOverByHit)
		{
			Player player;
			Scene DummyScene(&player);
			GameState GameState = { false, true, false, false };

			DummyScene.GetProjectiles().emplace_back(std::make_unique<Laser>(glm::vec3(0, 0, 5), glm::vec3(0, 0, -1), 150));

			bool isGameOver = DummyScene.Update(0.001f, GameState);

			Assert::AreEqual(true, isGameOver);
		}
	};

	TEST_CLASS(EnemyTest)
	{
	private:
	public:
		TEST_METHOD(EnemyShootingTest)
		{
			Player player;
			Scene DummyScene(&player);
			GameState GameState = { false, true, false, false };

			DummyScene.AddEnemy(glm::vec3(0, 0, 30));

			Assert::AreEqual(DummyScene.GetEntities().size(), (size_t)1);

			Assert::AreEqual(DummyScene.GetProjectiles().size(), (size_t)0);

			DummyScene.Update(0.1f, GameState);

			Assert::AreEqual(DummyScene.GetProjectiles().size(), (size_t)1);
		}

		TEST_METHOD(EnemyCanFollowPlayer)
		{
			Player player;
			Scene DummyScene(&player);
			GameState GameState = { false, true, false, false };

			DummyScene.AddEnemy(glm::vec3(0, 0, 300));
			Enemy* enemy = (SaR*)(DummyScene.GetEntities().at(0).get());

			Assert::AreEqual(DummyScene.GetEntities().size(), (size_t)1);


			DummyScene.Update(0.1f, GameState);

			glm::vec3 to_enemy = glm::normalize(player.GetPos() - enemy->GetPos());

			Assert::AreEqual(true, TestVectorEqual(enemy->GetForwardVec(), to_enemy));

			player.SetPos(glm::vec3(0, 10, 10));

			DummyScene.Update(0.1f, GameState);

			to_enemy = glm::normalize(player.GetPos() - enemy->GetPos());

			Assert::AreEqual(true,TestVectorEqual(enemy->GetForwardVec(), to_enemy));

		}

		TEST_METHOD(EnemyCantFollowPlayer)
		{
			Player player;
			Scene DummyScene(&player);
			GameState GameState = { false, true, false, false };

			DummyScene.AddEnemy(glm::vec3(0, 0, 0));
			player.SetPos(glm::vec3(0, 500, 0));
			Enemy* enemy = (SaR*)(DummyScene.GetEntities().at(0).get());


			DummyScene.Update(0.1f, GameState);

			glm::vec3 to_enemy = glm::normalize(player.GetPos() - enemy->GetPos());

			Assert::AreEqual(false, TestVectorEqual(enemy->GetForwardVec(), to_enemy));

		}

		TEST_METHOD(EnemyTryAvoidPlayer)
		{
			Player player;
			Scene DummyScene(&player);
			GameState GameState = { false, true, false, false };

			DummyScene.AddEnemy(glm::vec3(0, 0, 100));
			Enemy* enemy = (SaR*)(DummyScene.GetEntities().at(0).get());


			DummyScene.Update(0.1f, GameState);

			glm::vec3 to_enemy = glm::normalize(player.GetPos() - enemy->GetPos());

			Assert::AreEqual(false, TestVectorEqual(enemy->GetForwardVec(), to_enemy));

		}

		TEST_METHOD(EnemyTryAvoidObject)
		{
			Player player;
			Scene DummyScene(&player);
			GameState GameState = { false, true, false, false };

			DummyScene.AddEnemy(glm::vec3(0, 0, 950));
			Enemy* enemy = (SaR*)(DummyScene.GetEntities().at(0).get());

			DummyScene.AddEntity(std::make_shared<Entity>("assets/DeepSpace/meteor.obj", glm::vec3(0, 10, 920), "assets/DeepSpace/meteor_tex.jpg", Dimensions{ 27.0f, 27.0f, 27.0f }));

			DummyScene.Update(0.1f, GameState);

			glm::vec3 to_enemy = glm::normalize(player.GetPos() - enemy->GetPos());

			Assert::AreEqual(false, TestVectorEqual(enemy->GetForwardVec(), to_enemy));

		}
	};

	TEST_CLASS(CollisionDetection)
	{
	private:
	public:
		TEST_METHOD(AABShouldNotCollide)
		{
			Entity entity1("assets/DeepSpace/meteor.obj", glm::vec3(0, 0, 0), "assets/DeepSpace/meteor_tex.jpg", Dimensions{ 30.0f, 30.0f, 30.0f });
			Entity entity2("assets/DeepSpace/meteor.obj", glm::vec3(0, 100, 0), "assets/DeepSpace/meteor_tex.jpg", Dimensions{ 30.0f, 30.0f, 30.0f });

			Assert::AreEqual(false, AAB::Collide(entity1, entity2));

			entity2.SetPos(glm::vec3(0, 61, 0));
			Assert::AreEqual(false, AAB::Collide(entity1, entity2));

			entity2.SetPos(glm::vec3(0, 50 , 50));
			Assert::AreEqual(false, AAB::Collide(entity1, entity2));
		}

		TEST_METHOD(AABShouldCollide)
		{
			Entity entity1("assets/DeepSpace/meteor.obj", glm::vec3(0, 0, 0), "assets/DeepSpace/meteor_tex.jpg", Dimensions{ 30.0f, 30.0f, 30.0f });
			Entity entity2("assets/DeepSpace/meteor.obj", glm::vec3(0, 0, 0), "assets/DeepSpace/meteor_tex.jpg", Dimensions{ 30.0f, 30.0f, 30.0f });

			Assert::AreEqual(true, AAB::Collide(entity1, entity2));

			entity2.SetPos(glm::vec3(0, 60, 0));
			Assert::AreEqual(true, AAB::Collide(entity1, entity2));

			entity2.SetPos(glm::vec3(0, 15, 15));
			Assert::AreEqual(true, AAB::Collide(entity1, entity2));
		}

		TEST_METHOD(AABWithPoint)
		{
			Entity entity1("assets/DeepSpace/meteor.obj", glm::vec3(0, 0, 0), "assets/DeepSpace/meteor_tex.jpg", Dimensions{ 30.0f, 30.0f, 30.0f });

			Assert::AreEqual(true, AAB::Collide(entity1, glm::vec3(0, 0, 0)));
			Assert::AreEqual(true, AAB::Collide(entity1, glm::vec3(0, 14, 0)));
			Assert::AreEqual(true, AAB::Collide(entity1, glm::vec3(0, 0, 14.9f)));
			Assert::AreEqual(false, AAB::Collide(entity1, glm::vec3(30, 0, 0)));
			Assert::AreEqual(false, AAB::Collide(entity1, glm::vec3(15, 15, 15)));

		}
	};
}
