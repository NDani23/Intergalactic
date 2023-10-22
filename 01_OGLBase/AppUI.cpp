#include "AppUI.h"
#include "MyApp.h"

AppUI::AppUI(CMyApp* app)
{
	m_currentMapIndex = 0;
	m_app = app;
	m_ItemIdentifier = std::make_pair("", -1);
}

AppUI::AppUI()
{
	m_currentMapIndex = 0;
	m_app = nullptr;
	m_ItemIdentifier = std::make_pair("", -1);
}


void AppUI::Render()
{
	if (m_app == nullptr) return;

	static bool opt_fullscreen = true;
	static bool opt_padding = false;
	static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

	// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
	// because it would be confusing to have two docking targets within each others.
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking;
	if (opt_fullscreen)
	{
		const ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->WorkPos);
		ImGui::SetNextWindowSize(viewport->WorkSize);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
	}
	else
	{
		dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
	}

	// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
	// and handle the pass-thru hole, so we ask Begin() to not render a background.
	if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
		window_flags |= ImGuiWindowFlags_NoBackground;


	if (!opt_padding)
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("DockSpace Demo", nullptr, window_flags);
	if (!opt_padding)
		ImGui::PopStyleVar();

	if (opt_fullscreen)
		ImGui::PopStyleVar(2);

	// Submit the DockSpace
	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
	}

	dockspace_flags = ImGuiDockNodeFlags_PassthruCentralNode ^ ImGuiDockNodeFlags_NoTabBar ^ ImGuiDockNodeFlags_NoResize ^ ImGuiDockNodeFlags_NoUndocking;

	//ImGui::ShowDemoWindow();

	if (m_app->m_GameState.menu)
		RenderMenu();

	if (m_app->m_GameState.play)
		RenderPlayWindow();

	if (m_app->m_GameState.gameover)
		RenderGameOverWindow();

	if (m_app->m_GameState.pause)
		RenderPauseWindow();

	if (m_app->m_GameState.hangar)
		RenderHangarWindow();

	if (m_ItemIdentifier.second != -1)
		RenderBuyWindow();

	ImGui::End();
}

void AppUI::RenderMenu() 
{
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar;

	ImGuiViewport* main_viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x + m_app->m_screenWidth / 3, main_viewport->WorkPos.y + m_app->m_screenHeight / 4));
	ImGui::SetNextWindowSize(ImVec2(m_app->m_screenWidth / 3, m_app->m_screenHeight / 2));

	ImGui::Begin("Menu", nullptr, window_flags);

	ImVec2 windowSize = ImGui::GetWindowSize();
	ImVec2 windowPos = ImGui::GetWindowPos();


	ImGui::Indent(windowSize.x / 8.f);

	ImGui::Text("Record: %d", m_app->m_player.GetRecord());

	if (ImGui::Button("PLAY", ImVec2(windowSize.x * (3.f / 4), windowSize.y / 10)))
	{
		m_app->m_GameState.play = true;
		m_app->m_GameState.menu = false;

		m_app->Reset();
	}

	if (ImGui::Button("<", ImVec2(windowSize.x * 0.10, windowSize.y / 10)))
	{
		m_currentMapIndex = std::abs((m_currentMapIndex - 1) % 2);
		m_app->m_map = m_app->m_maps[m_currentMapIndex].get();
		m_app->m_skyboxTexture = m_app->m_map->GetSkyBox();
	}
	ImGui::SameLine(0.f, 1.f);

	ImGui::Button(m_app->m_map->getName().c_str(), ImVec2(windowSize.x * 0.545, windowSize.y / 10));

	ImGui::SameLine(0.f, 1.f);

	if (ImGui::Button(">", ImVec2(windowSize.x * 0.10, windowSize.y / 10)))
	{
		m_currentMapIndex = (m_currentMapIndex + 1) % 2;
		m_app->m_map = m_app->m_maps[m_currentMapIndex].get();
		m_app->m_skyboxTexture = m_app->m_map->GetSkyBox();
	}


	if (ImGui::Button("HANGAR", ImVec2(windowSize.x * (3.f / 4), windowSize.y / 10)))
	{
		m_app->m_GameState.hangar = true;
		m_app->m_GameState.menu = false;
	}

	if (ImGui::Button("EXIT", ImVec2(windowSize.x * (3.f / 4), windowSize.y / 10)))
	{
		*(m_app->m_quit) = true;
		m_app->m_Persistence.Save();
	}
	ImGui::End();
}

void AppUI::RenderPlayWindow()
{

	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10.f, 10.f));
	if (ImGui::BeginMainMenuBar())
	{
		std::stringstream ss;
		ss << std::setw(2) << std::setfill('0') << (int)m_app->m_PlayTime / 60 << ":" << std::setw(2) << std::setfill('0') << (int)m_app->m_PlayTime % 60;
		ImGui::Text(ss.str().c_str());

		ImGui::Indent(m_app->m_screenWidth - 80.f);
		ImGui::Text("Score: %i", m_app->m_player.GetPoints());

		ImGui::EndMainMenuBar();
	}
	ImGui::PopStyleVar();

	ImGui::Begin("Viewport");
	ImGui::Indent(m_app->m_screenWidth / 3.f);
	ImGui::PushStyleColor(ImGuiCol_PlotHistogram, (ImVec4)ImColor::HSV(0, 255, 235, 255));
	ImGui::ProgressBar(m_app->m_player.GetHealth() / (float)m_app->m_player.GetMaxHealth(), ImVec2(m_app->m_screenWidth / 3.f, 15.0f));
	ImGui::PopStyleColor();


	//Example of drawing a texture into ImGUI::image
	ImVec2 uv_min = ImVec2(0.0f, 0.0f);                 // Top-left
	ImVec2 uv_max = ImVec2(1.0f, 1.0f);                 // Lower-right
	ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);   // No tint
	//ImVec4 border_col = ImGui::GetStyleColorVec4(ImGuiCol_Border);
	ImVec4 border_col = ImVec4(0.5f, 0.5f, 0.5f, 1.0f);
	ImVec2 image_size = ImVec2(m_app->m_screenWidth / 30.f, m_app->m_screenWidth / 30.f);

	ImGui::Indent(m_app->m_screenWidth / 10.f);
	for (int i = 0; i < 3; i++)
	{
		int TexId = m_app->m_player.GetWeapons()[i] == nullptr ? -1 : m_app->m_player.GetWeapons()[i]->GetProjectileImage().GetId();
		if (i == m_app->m_player.GetActiveWeaponInd())
		{
			border_col = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
			image_size = ImVec2(m_app->m_screenWidth / 25.f, m_app->m_screenWidth / 25.f);

			ImGui::Image((ImTextureID)TexId, image_size, uv_min, uv_max, tint_col, border_col);
			ImGui::SameLine();

			border_col = ImVec4(0.5f, 0.5f, 0.5f, 1.0f);
			image_size = ImVec2(m_app->m_screenWidth / 30.f, m_app->m_screenWidth / 30.f);
		}
		else
		{
			ImGui::Image((ImTextureID)TexId, image_size, uv_min, uv_max, tint_col, border_col);
			ImGui::SameLine();
		}
	}

	image_size = ImVec2(m_app->m_screenWidth / 25.f, m_app->m_screenWidth / 25.f);
	if(m_app->m_useUpgrade) border_col = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
	ImGui::Indent(m_app->m_screenWidth / 2.f);

	int TexId = m_app->m_player.GetUpgrade() == nullptr ? -1 : m_app->m_player.GetUpgrade()->GetImage().GetId();
	ImGui::Image((ImTextureID)TexId, image_size, uv_min, uv_max, tint_col, border_col);


	ImGui::End();
}

void AppUI::RenderGameOverWindow()
{
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar;

	ImGuiViewport* main_viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x + m_app->m_screenWidth / 3, main_viewport->WorkPos.y + m_app->m_screenHeight / 5 * 2));
	ImGui::SetNextWindowSize(ImVec2(m_app->m_screenWidth / 3, m_app->m_screenHeight / 5));

	ImGui::Begin("Game Over", nullptr, window_flags);

	ImVec2 windowSize = ImGui::GetWindowSize();
	ImVec2 windowPos = ImGui::GetWindowPos();


	ImGui::Indent(windowSize.x / 8.f);

	ImGui::Text("Final score: %i", m_app->m_player.GetPoints());

	if (ImGui::Button("BACK TO MENU", ImVec2(windowSize.x * (3.f / 4), windowSize.y / 10)))
	{
		m_app->m_GameState.gameover = false;
		m_app->m_GameState.play = false;
		m_app->m_GameState.menu = true;
	}

	ImGui::End();
}

void AppUI::RenderPauseWindow()
{
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar;

	ImGuiViewport* main_viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x + m_app->m_screenWidth / 3, main_viewport->WorkPos.y + m_app->m_screenHeight / 5 * 2));
	ImGui::SetNextWindowSize(ImVec2(m_app->m_screenWidth / 3, m_app->m_screenHeight / 5));

	ImGui::Begin("Paused", nullptr, window_flags);

	ImVec2 windowSize = ImGui::GetWindowSize();
	ImVec2 windowPos = ImGui::GetWindowPos();


	ImGui::Indent(windowSize.x / 8.f);

	if (ImGui::Button("Continue", ImVec2(windowSize.x * (3.f / 4), windowSize.y / 10)))
	{
		m_app->m_GameState.pause = false;
	}

	if (ImGui::Button("BACK TO MENU", ImVec2(windowSize.x * (3.f / 4), windowSize.y / 10)))
	{
		m_app->m_GameState.play = false;
		m_app->m_GameState.pause = false;
		m_app->m_GameState.menu = true;
	}

	ImGui::End();
}

void AppUI::RenderHangarWindow()
{
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10.f, 10.f));
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::Button("BACK TO MENU"))
		{
			m_app->m_GameState.hangar = false;
			m_app->m_GameState.menu = true;
		}


		ImGui::Indent(m_app->m_screenWidth - 100.f);
		ImGui::Text("Credit: %d", m_app->m_player.GetCredit());

		ImGui::EndMainMenuBar();
	}
	ImGui::PopStyleVar();

	ImGui::Begin("HangarBottom");
	{
		ImVec2 uv_min = ImVec2(0.0f, 0.0f);                 // Top-left
		ImVec2 uv_max = ImVec2(1.0f, 1.0f);                 // Lower-right
		ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);   // No tint
		//ImVec4 border_col = ImGui::GetStyleColorVec4(ImGuiCol_Border);
		ImVec4 border_col = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
		ImVec2 image_size = ImVec2(m_app->m_screenWidth / 20.f, m_app->m_screenWidth / 20.f);

		ImGui::Indent(m_app->m_screenWidth / 2.5f);
		for (int i = 0; i < 3; i++)
		{

			int TexId = m_app->m_player.GetWeapons()[i] == nullptr ? -1 : m_app->m_player.GetWeapons()[i]->GetProjectileImage().GetId();

			if (i == 1)
			{
				ImGui::Image((ImTextureID)TexId, image_size, uv_min, uv_max, tint_col, border_col);
			}
			else
			{
				std::string name = std::to_string(i);
				if (ImGui::ImageButton(name.c_str(), (ImTextureID)TexId, image_size, uv_min, uv_max, border_col, tint_col))
				{
					m_app->m_player.GetWeapons()[i] = nullptr;
				}
			}

			if (i != 1)
			{
				if (ImGui::BeginDragDropTarget())
				{
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Weapon"))
					{
						int payload_n = *(const int*)payload->Data;
						m_app->m_player.GetWeapons()[i] = m_app->m_weaponStorage.GetStorage().at(payload_n).InsertWeapon(&m_app->m_player, i - 1);
					}

					ImGui::EndDragDropTarget();
				}
			}
			ImGui::SameLine();
			
		}

		ImGui::Indent(m_app->m_screenWidth / 2.f);
		int TexId = m_app->m_player.GetUpgrade() == nullptr ? -1 : m_app->m_player.GetUpgrade()->GetImage().GetId();
		if (ImGui::ImageButton("Upgrade", (ImTextureID)TexId, image_size, uv_min, uv_max, border_col, tint_col))
		{
			m_app->m_player.GetUpgrade() = nullptr;
		}

		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Upgrade"))
			{
				int payload_n = *(const int*)payload->Data;
				m_app->m_player.GetUpgrade() = m_app->m_upgradeStorage.GetStorage().at(payload_n).InsertUpgrade(&m_app->m_player);
			}

			ImGui::EndDragDropTarget();
		}
	}
	ImGui::End();

	ImGui::Begin("HangarLeft");


	int player_upgrade_points = m_app->m_player.GetUpgradePoints();


	ImVec2 windowSize = ImGui::GetWindowSize();

	ImGui::Text("Upgrade points: %d", player_upgrade_points);

	if (m_app->m_player.GetUpgradePointsSum() != 20)
	{

		ImGui::SameLine();
		if (ImGui::Button("+", ImVec2(20.f, 20.f)))
		{
			if (m_app->m_player.GetCredit() >= 200)
			{
				m_app->m_player.setUpgradePoints(m_app->m_player.GetUpgradePoints() + 1);
				m_app->m_player.setCredit(m_app->m_player.GetCredit() - 200);
			}
		}
		ImGui::SameLine();
		ImGui::Text("(200$)");
	}

	//Example input for stat modifiers
	int step = 1;
	Stats player_stats = m_app->m_player.GetStats();

	{
		int speed = player_stats.speed;
		if (ImGui::InputScalar("SPEED", ImGuiDataType_S8, &speed, &step, NULL, "%d"))
		{
			if (speed > player_stats.speed && player_upgrade_points != 0 && speed < 11)
			{
				player_upgrade_points--;
				m_app->m_player.setUpgradePoints(player_upgrade_points);
				player_stats.speed++;
				m_app->m_player.setStats(player_stats);
			}
			else if (speed < player_stats.speed && speed >= 0)
			{
				player_upgrade_points++;
				m_app->m_player.setUpgradePoints(player_upgrade_points);
				player_stats.speed--;
				m_app->m_player.setStats(player_stats);
			}
		}

		int mobility = player_stats.mobility;
		if (ImGui::InputScalar("MOBILITY", ImGuiDataType_S8, &mobility, &step, NULL, "%d"))
		{
			if (mobility > player_stats.mobility && player_upgrade_points != 0 && mobility < 11)
			{
				player_upgrade_points--;
				m_app->m_player.setUpgradePoints(player_upgrade_points);
				player_stats.mobility++;
				m_app->m_player.setStats(player_stats);
			}
			else if (mobility < player_stats.mobility && mobility >= 0)
			{
				player_upgrade_points++;
				m_app->m_player.setUpgradePoints(player_upgrade_points);
				player_stats.mobility--;
				m_app->m_player.setStats(player_stats);
			}
		}

		int health = player_stats.health;
		if (ImGui::InputScalar("HEALTH", ImGuiDataType_S8, &health, &step, NULL, "%d"))
		{
			if (health > player_stats.health && player_upgrade_points != 0 && health < 11)
			{
				player_upgrade_points--;
				m_app->m_player.setUpgradePoints(player_upgrade_points);
				player_stats.health++;
				m_app->m_player.setStats(player_stats);
			}
			else if (health < player_stats.health && health >= 0)
			{
				player_upgrade_points++;
				m_app->m_player.setUpgradePoints(player_upgrade_points);
				player_stats.health--;
				m_app->m_player.setStats(player_stats);
			}
		}

		int damage = player_stats.damage;
		if (ImGui::InputScalar("DAMAGE", ImGuiDataType_S8, &damage, &step, NULL, "%d"))
		{
			if (damage > player_stats.damage && player_upgrade_points != 0 && damage < 11)
			{
				player_upgrade_points--;
				m_app->m_player.setUpgradePoints(player_upgrade_points);
				player_stats.damage++;
				m_app->m_player.setStats(player_stats);
			}
			else if (damage < player_stats.damage && damage >= 0)
			{
				player_upgrade_points++;
				m_app->m_player.setUpgradePoints(player_upgrade_points);
				player_stats.damage--;
				m_app->m_player.setStats(player_stats);
			}
		}

		int fire_rate = player_stats.fire_rate;
		if (ImGui::InputScalar("FIRE RATE", ImGuiDataType_S8, &fire_rate, &step, NULL, "%d"))
		{
			if (fire_rate > player_stats.fire_rate && player_upgrade_points != 0 && fire_rate < 11)
			{
				player_upgrade_points--;
				m_app->m_player.setUpgradePoints(player_upgrade_points);
				player_stats.fire_rate++;
				m_app->m_player.setStats(player_stats);
			}
			else if (fire_rate < player_stats.fire_rate && fire_rate >= 0)
			{
				player_upgrade_points++;
				m_app->m_player.setUpgradePoints(player_upgrade_points);
				player_stats.fire_rate--;
				m_app->m_player.setStats(player_stats);
			}
		}

	}
	ImGui::End();

	ImGui::Begin("HangarRight");
	{
		ImVec2 uv_min = ImVec2(0.0f, 0.0f);                 // Top-left
		ImVec2 uv_max = ImVec2(1.0f, 1.0f);                 // Lower-right
		ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);   // No tint
		ImVec4 border_col = ImVec4(0.5f, 0.5f, 0.5f, 1.0f);
		ImVec2 image_size = ImVec2(m_app->m_screenWidth / 20.f, m_app->m_screenWidth / 20.f);

		if (ImGui::TreeNode("Weapons"))
		{	
			std::map<int, WeaponItem>& Weapons = m_app->m_weaponStorage.GetStorage();
			for (int n = 0; n < Weapons.size(); n++)
			{
				ImGui::PushID(n);
				Weapons.at(n).Owned ? tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f) : tint_col = ImVec4(0.3f, 0.3f, 0.3f, 1.0f);
				if (ImGui::ImageButton("", (ImTextureID)Weapons.at(n).Image.GetId(), image_size, uv_min, uv_max, border_col, tint_col))
				{
					if(!Weapons.at(n).Owned) m_ItemIdentifier = std::make_pair("Weapon", n);
				}
				
				if (Weapons.at(n).Owned)
				{
					if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
					{
						ImGui::SetDragDropPayload("Weapon", &n, sizeof(int));
						ImGui::Text(Weapons.at(n).Text.c_str());
						ImGui::EndDragDropSource();
					}
				}
				
				ImGui::PopID();
			}
			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Upgrades"))
		{

			std::map<int, UpgradeItem>& Upgrades = m_app->m_upgradeStorage.GetStorage();
			for (int n = 0; n < Upgrades.size(); n++)
			{
				ImGui::PushID(n);
				Upgrades.at(n).Owned ? tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f) : tint_col = ImVec4(0.3f, 0.3f, 0.3f, 1.0f);
				if (ImGui::ImageButton("", (ImTextureID)Upgrades.at(n).Image.GetId(), image_size, uv_min, uv_max, border_col, tint_col))
				{
					if (!Upgrades.at(n).Owned)m_ItemIdentifier = std::make_pair("Upgrade", n);
				}

				if (Upgrades.at(n).Owned)
				{
					if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
					{
						ImGui::SetDragDropPayload("Upgrade", &n, sizeof(int));
						ImGui::Text(Upgrades.at(n).Text.c_str());
						ImGui::EndDragDropSource();
					}
				}

				ImGui::PopID();
			}
			ImGui::TreePop();
		}
	}
	ImGui::End();
}

void AppUI::RenderBuyWindow()
{
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize;

	ImGuiViewport* main_viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x + m_app->m_screenWidth / 3, main_viewport->WorkPos.y + m_app->m_screenHeight / 5 * 2));
	ImGui::SetNextWindowSize(ImVec2(m_app->m_screenWidth / 3, m_app->m_screenHeight / 5));

	ImGui::Begin("Buy", nullptr, window_flags);

	ImVec2 windowSize = ImGui::GetWindowSize();
	ImVec2 windowPos = ImGui::GetWindowPos();


	ImGui::Indent(windowSize.x / 8.f);

	UpgradeItem* upgrade = nullptr;
	WeaponItem* weapon = nullptr;

	if (m_ItemIdentifier.first == "Weapon")
	{
		weapon = &m_app->m_weaponStorage.GetStorage().at(m_ItemIdentifier.second);
		ImGui::Text(weapon->Text.c_str());
		ImGui::SameLine();
		ImGui::Text("(%d$)", weapon->Price);
	}
	else
	{
		upgrade = &m_app->m_upgradeStorage.GetStorage().at(m_ItemIdentifier.second);
		ImGui::Text(upgrade->Text.c_str());
		ImGui::SameLine();
		ImGui::Text("(%d$)", upgrade->Price);
	}


	if (ImGui::Button("BUY", ImVec2(windowSize.x * (3.f / 4), windowSize.y / 10)))
	{
		int price = upgrade == nullptr ? weapon->Price : upgrade->Price;
		upgrade == nullptr ? weapon->Owned = true : upgrade->Owned = true;
		m_app->m_player.setCredit(m_app->m_player.GetCredit() - price);
		m_ItemIdentifier = std::make_pair("", -1);
	}

	if (ImGui::Button("CANCEL", ImVec2(windowSize.x * (3.f / 4), windowSize.y / 10)))
	{
		m_ItemIdentifier = std::make_pair("", -1);
	}

	ImGui::End();
}