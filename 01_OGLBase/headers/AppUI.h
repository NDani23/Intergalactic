#pragma once

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <functional>

#include "data_storage/WeaponDataStorage.h"
#include "data_storage/UpgradeDataStorage.h"
#include "weapons/Weapon.h"
#include "upgrades/Upgrade.h"

class CMyApp;

class AppUI
{
private:
	ImFont* small_font;
	ImFont* medium_font;
	ImFont* large_font;
	Texture2D cursor_tex;
	int m_currentMapIndex;
	CMyApp* m_app;
	std::pair<std::string, int> m_ItemIdentifier;

public:
	AppUI(CMyApp* app);
	AppUI();

	void Render();

private:
	void RenderViewPort();
	void RenderMenu();
	void RenderPlayWindow();
	void RenderGameOverWindow();
	void RenderPauseWindow();
	void RenderHangarWindow();
	void RenderBuyWindow();

	void HandleViewportEvents();
};
