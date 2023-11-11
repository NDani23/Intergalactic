#include "../headers/AppUI.h"
#include "../headers/MyApp.h"

AppUI::AppUI(CMyApp* app)
{
	ImGuiIO& io = ImGui::GetIO();
	small_font = io.Fonts->AddFontFromFileTTF("assets/ProggyClean.ttf", 13);
	medium_font = io.Fonts->AddFontFromFileTTF("assets/ProggyClean.ttf", 16);
	large_font = io.Fonts->AddFontFromFileTTF("assets/ProggyClean.ttf", 18);
	cursor_tex = Texture2D("assets/cursor.png");
	m_currentMapIndex = 0;
	m_app = app;
	m_ItemIdentifier = std::make_pair("", -1);
}

AppUI::AppUI()
{
	cursor_tex = Texture2D("assets/cursor.png");
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
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar;
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

	dockspace_flags = ImGuiDockNodeFlags_PassthruCentralNode 
					^ ImGuiDockNodeFlags_NoTabBar 
					^ ImGuiDockNodeFlags_NoResize 
					^ ImGuiDockNodeFlags_NoUndocking;

	
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
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 12.0f);
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar;

	ImGuiViewport* main_viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x + m_app->m_screenWidth / 3, main_viewport->WorkPos.y + m_app->m_screenHeight / 4));
	ImGui::SetNextWindowSize(ImVec2(m_app->m_screenWidth * 0.35f, m_app->m_screenHeight * 0.4f));

	ImGui::Begin("Menu", nullptr, window_flags);

	ImVec2 windowSize = ImGui::GetWindowSize();
	ImVec2 windowPos = ImGui::GetWindowPos();


	ImGui::Indent(windowSize.x / 8.f);

	ImGui::Dummy(ImVec2(0.0f, windowSize.y * 0.05f));
	ImGui::SetCursorPosX((windowSize.x - 110.f) * 0.5f);
	ImGui::PushFont(large_font);
	ImGui::Text("Record: %d", m_app->m_player.GetRecord());
	ImGui::PopFont();
	ImGui::Dummy(ImVec2(0.0f, windowSize.y * 0.1f));

	if (ImGui::Button("PLAY", ImVec2(windowSize.x * (3.f / 4), windowSize.y * 0.13f)))
	{
		m_app->m_GameState.play = true;
		m_app->m_GameState.menu = false;

		m_app->Reset();
	}

	if (ImGui::Button("<", ImVec2(windowSize.x * 0.10, windowSize.y * 0.13f)))
	{
		m_currentMapIndex = std::abs((m_currentMapIndex - 1) % 2);
		m_app->m_map = m_app->m_maps[m_currentMapIndex].get();
		m_app->m_skyboxTexture = m_app->m_map->GetSkyBox();
	}
	ImGui::SameLine(0.f, 1.f);

	ImGui::Button(m_app->m_map->getName().c_str(), ImVec2(windowSize.x * 0.545, windowSize.y * 0.13f));

	ImGui::SameLine(0.f, 1.f);

	if (ImGui::Button(">", ImVec2(windowSize.x * 0.10, windowSize.y * 0.13f)))
	{
		m_currentMapIndex = (m_currentMapIndex + 1) % 2;
		m_app->m_map = m_app->m_maps[m_currentMapIndex].get();
		m_app->m_skyboxTexture = m_app->m_map->GetSkyBox();
	}


	if (ImGui::Button("HANGAR", ImVec2(windowSize.x * (3.f / 4), windowSize.y * 0.13f)))
	{
		m_app->m_GameState.hangar = true;
		m_app->m_GameState.menu = false;
	}

	if (ImGui::Button("EXIT", ImVec2(windowSize.x * (3.f / 4), windowSize.y * 0.13f)))
	{
		*(m_app->m_quit) = true;
		m_app->m_Persistence.Save();
	}
	ImGui::PopStyleVar();
	ImGui::End();
}

void AppUI::RenderPlayWindow()
{
	ImGuiIO& io = ImGui::GetIO();
	ImGui::SetMouseCursor(ImGuiMouseCursor_None);
	ImGui::SetCursorPos(io.MousePos);
	ImGui::Image((ImTextureID)cursor_tex.GetId(), ImVec2(20.f, 20.f), ImVec2(0.f, 0.f), ImVec2(1.0f, 1.0f), ImVec4(1.0f, 1.0f, 1.0f, 1.0f), ImVec4(0.f, 0.f, 0.f, 0.f));

	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10.f, 10.f));
	if (ImGui::BeginMainMenuBar())
	{
		ImGui::Text("FPS: %.2f", ImGui::GetIO().Framerate);

		ImGui::Indent(m_app->m_screenWidth / 2);
		std::stringstream ss;
		ss << std::setw(2) << std::setfill('0') << (int)m_app->m_PlayTime / 60 << ":" << std::setw(2) << std::setfill('0') << (int)m_app->m_PlayTime % 60;
		ImGui::Text(ss.str().c_str());


		ImGui::Indent(m_app->m_screenWidth / 2 - 100.f);
		ImGui::Text("Score: %i", m_app->m_player.GetPoints());

		ImGui::EndMainMenuBar();
	}
	ImGui::PopStyleVar();

	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar;

	ImGui::Begin("Viewport", nullptr, window_flags);
	ImVec2 windowSize = ImGui::GetWindowSize();
	ImGui::SetCursorPosX(10.f);
	ImGui::Text("%d", m_app->m_player.GetSpeed());
	ImGui::SameLine();
	ImGui::Indent(m_app->m_screenWidth / 3.f);
	ImGui::PushStyleColor(ImGuiCol_PlotHistogram, (ImVec4)ImColor::HSV(0, 255, 235, 255));
	ImGui::ProgressBar(m_app->m_player.GetHealth() / (float)m_app->m_player.GetMaxHealth(), ImVec2(m_app->m_screenWidth / 3.f, 15.0f));
	ImGui::PopStyleColor();

	float speed = (float)m_app->m_player.GetSpeed();
	ImGui::SetCursorPosX(10.f);
	ImGui::PushStyleColor(ImGuiCol_FrameBg, (ImVec4)ImColor::HSV(4 / 7.0f, 0.5f, 0.5f));
	ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, (ImVec4)ImColor::HSV(4 / 7.0f, 0.6f, 0.5f));
	ImGui::PushStyleColor(ImGuiCol_FrameBgActive, (ImVec4)ImColor::HSV(4 / 7.0f, 0.7f, 0.5f));
	ImGui::PushStyleColor(ImGuiCol_SliderGrab, (ImVec4)ImColor::HSV(4 / 7.0f, 0.9f, 0.9f));
	ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
	ImGui::VSliderFloat("##v", ImVec2(18, 70), &speed, 80.0f, 190.0f, "");
	ImGui::PopItemFlag();
	ImGui::PopStyleColor(4);
	ImGui::SameLine();
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
		Weapon* weapon = m_app->m_player.GetWeapons()[i].get();
		int TexId = weapon == nullptr ? -1 : weapon->GetProjectileImage().GetId();
		if (i == m_app->m_player.GetActiveWeaponInd())
		{
			border_col = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
			image_size = ImVec2(m_app->m_screenWidth / 25.f, m_app->m_screenWidth / 25.f);

			ImGui::SetCursorPos(ImVec2(windowSize.x * 0.5f - image_size.x * 1.5f + i * (image_size.x * 1.4f), (windowSize.y - image_size.y) * 0.5f));
			ImGui::Image((ImTextureID)TexId, image_size, uv_min, uv_max, tint_col, border_col);
			if (weapon != nullptr)
			{
				ImGui::SetCursorPos(ImVec2(windowSize.x * 0.5f - image_size.x * 1.5f + i * (image_size.x * 1.4f), (windowSize.y * 0.5f) + image_size.y * 0.6f));
				ImGui::ProgressBar((weapon->GetCoolDownTime() - weapon->GetCurrentCooldown()) / weapon->GetCoolDownTime(), ImVec2(image_size.x, 10.f));
			}

			border_col = ImVec4(0.5f, 0.5f, 0.5f, 1.0f);
			image_size = ImVec2(m_app->m_screenWidth / 30.f, m_app->m_screenWidth / 30.f);
		}
		else
		{
			ImGui::SetCursorPos(ImVec2(windowSize.x * 0.5f - image_size.x * 1.5f + i * (image_size.x * 1.5f), (windowSize.y - image_size.y) * 0.5f));
			ImGui::Image((ImTextureID)TexId, image_size, uv_min, uv_max, tint_col, border_col);

			if (weapon == nullptr) continue;
			ImGui::SetCursorPos(ImVec2(windowSize.x * 0.5f - image_size.x * 1.5f + i * (image_size.x * 1.5f), (windowSize.y * 0.5f) + image_size.y * 0.6f));
			ImGui::ProgressBar((weapon->GetCoolDownTime() - weapon->GetCurrentCooldown()) / weapon->GetCoolDownTime(), ImVec2(image_size.x, 10.f));
		}
	}

	Upgrade* upgrade = m_app->m_player.GetUpgrade().get();
	image_size = ImVec2(m_app->m_screenWidth / 25.f, m_app->m_screenWidth / 25.f);
	if(m_app->m_useUpgrade) border_col = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
	
	ImGui::SetCursorPos(ImVec2(windowSize.x - 1.5f * image_size.x, (windowSize.y - image_size.y) * 0.5f));
	int TexId = m_app->m_player.GetUpgrade() == nullptr ? -1 : m_app->m_player.GetUpgrade()->GetImage().GetId();
	ImGui::Image((ImTextureID)TexId, image_size, uv_min, uv_max, tint_col, border_col);
	if (upgrade != nullptr)
	{
		ImGui::SetCursorPos(ImVec2(windowSize.x - 1.5f * image_size.x, (windowSize.y * 0.5f) + image_size.y * 0.6f));
		ImGui::ProgressBar((upgrade->GetCoolDownTime() - upgrade->GetCurrentCooldown()) / upgrade->GetCoolDownTime(), ImVec2(image_size.x, 10.f));
	}
	ImGui::End();
}

void AppUI::RenderGameOverWindow()
{
	ImGui::SetMouseCursor(ImGuiMouseCursor_Arrow);
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar;

	ImGuiViewport* main_viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x + m_app->m_screenWidth / 3, main_viewport->WorkPos.y + m_app->m_screenHeight / 5 * 2));
	ImGui::SetNextWindowSize(ImVec2(m_app->m_screenWidth / 3, m_app->m_screenHeight * 0.1f));

	ImGui::Begin("Game Over", nullptr, window_flags);

	ImVec2 windowSize = ImGui::GetWindowSize();
	ImVec2 windowPos = ImGui::GetWindowPos();


	ImGui::Indent(windowSize.x / 8.f);

	ImGui::PushFont(medium_font);
	ImGui::SetCursorPosX((windowSize.x - 100.f) * 0.5f);
	ImGui::Text("Final score: %i", m_app->m_player.GetPoints());
	ImGui::PopFont();
	ImGui::Dummy(ImVec2(0.f, windowSize.y * 0.1f));


	if (ImGui::Button("BACK TO MENU", ImVec2(windowSize.x * (3.f / 4), windowSize.y * 0.25f)))
	{
		m_app->m_GameState.gameover = false;
		m_app->m_GameState.play = false;
		m_app->m_GameState.menu = true;
	}

	ImGui::End();
}

void AppUI::RenderPauseWindow()
{
	ImGui::SetMouseCursor(ImGuiMouseCursor_Arrow);
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar;

	ImGuiViewport* main_viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x + m_app->m_screenWidth / 3, main_viewport->WorkPos.y + m_app->m_screenHeight / 5 * 2));
	ImGui::SetNextWindowSize(ImVec2(m_app->m_screenWidth / 3, m_app->m_screenHeight * 0.1f));

	ImGui::Begin("Paused", nullptr, window_flags);

	ImVec2 windowSize = ImGui::GetWindowSize();
	ImVec2 windowPos = ImGui::GetWindowPos();

	ImGui::Dummy(ImVec2(0.0f, windowSize.y * 0.1f));
	ImGui::Indent(windowSize.x / 8.f);

	if (ImGui::Button("CONTINUE", ImVec2(windowSize.x * (3.f / 4), windowSize.y * 0.25f)))
	{
		m_app->m_GameState.pause = false;
	}

	if (ImGui::Button("BACK TO MENU", ImVec2(windowSize.x * (3.f / 4), windowSize.y * 0.25f)))
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


		ImGui::Indent(m_app->m_screenWidth - 120.f);
		ImGui::Text("Credit: %d", m_app->m_player.GetCredit());

		ImGui::EndMainMenuBar();
	}
	ImGui::PopStyleVar();

	ImGui::Begin("HangarBottom");
	{

		ImVec2 uv_min = ImVec2(0.0f, 0.0f);                 // Top-left
		ImVec2 uv_max = ImVec2(1.0f, 1.0f);                 // Lower-right
		ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.f);   // No tint
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
				if (ImGui::ImageButton(name.c_str(), (ImTextureID)TexId, image_size, uv_min, uv_max, ImVec4(0.f, 0.f, 0.f, 0.f), tint_col))
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


	ImGui::PushFont(medium_font);
	int player_upgrade_points = m_app->m_player.GetUpgradePoints();


	ImVec2 windowSize = ImGui::GetWindowSize();
	float base_unit = windowSize.y / 5 * 0.1f;

	ImGui::Dummy(ImVec2(0.0f, base_unit));
	ImGui::Text("Upgrade points: %d", player_upgrade_points);

	if (m_app->m_player.GetUpgradePointsSum() != 25)
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

	ImGui::Dummy(ImVec2(0.0f, base_unit * 10.f));
	{
		ImGui::Dummy(ImVec2(0.0f, base_unit));
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

		ImGui::Dummy(ImVec2(0.0f, base_unit * 2));
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

		ImGui::Dummy(ImVec2(0.0f, base_unit * 2));
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

		ImGui::Dummy(ImVec2(0.0f, base_unit * 2));
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

		ImGui::Dummy(ImVec2(0.0f, base_unit * 2));
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
	ImGui::PopFont();
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
				ImGui::SetItemTooltip(Weapons.at(n).Text.c_str());
				
				if (Weapons.at(n).Owned)
				{
					if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
					{
						ImGui::SetDragDropPayload("Weapon", &n, sizeof(int));
						ImGui::Text(Weapons.at(n).Name.c_str());
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
				ImGui::SetItemTooltip(Upgrades.at(n).Text.c_str());

				if (Upgrades.at(n).Owned)
				{
					if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
					{
						ImGui::SetDragDropPayload("Upgrade", &n, sizeof(int));
						ImGui::Text(Upgrades.at(n).Name.c_str());
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
	ImGui::SetNextWindowSize(ImVec2(m_app->m_screenWidth / 3, m_app->m_screenHeight * 0.12f));

	ImGui::Begin("Buy", nullptr, window_flags);

	ImVec2 windowSize = ImGui::GetWindowSize();
	ImVec2 windowPos = ImGui::GetWindowPos();


	ImGui::Indent(windowSize.x / 8.f);

	UpgradeItem* upgrade = nullptr;
	WeaponItem* weapon = nullptr;

	ImGui::PushFont(medium_font);
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
	ImGui::PopFont();

	ImGui::Dummy(ImVec2(0.f, windowSize.y * 0.1f));


	if (ImGui::Button("BUY", ImVec2(windowSize.x * (3.f / 4), windowSize.y * 0.23)))
	{
		int price = upgrade == nullptr ? weapon->Price : upgrade->Price;
		upgrade == nullptr ? weapon->Owned = true : upgrade->Owned = true;
		m_app->m_player.setCredit(m_app->m_player.GetCredit() - price);
		m_ItemIdentifier = std::make_pair("", -1);
	}

	if (ImGui::Button("CANCEL", ImVec2(windowSize.x * (3.f / 4), windowSize.y * 0.23)))
	{
		m_ItemIdentifier = std::make_pair("", -1);
	}

	ImGui::End();
}