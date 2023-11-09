#pragma once

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <functional>

#include "GameState.h"
#include "WeaponDataStorage.h"
#include "UpgradeDataStorage.h"
#include "weapons/Weapon.h"
#include "upgrades/Upgrade.h"

class CMyApp;

class AppUI
{
private:
	int m_currentMapIndex;
	CMyApp* m_app;
	std::pair<std::string, int> m_ItemIdentifier;

public:
	AppUI(CMyApp* app);
	AppUI();

	void Render();

private:
	void RenderMenu();
	void RenderPlayWindow();
	void RenderGameOverWindow();
	void RenderPauseWindow();
	void RenderHangarWindow();
	void RenderBuyWindow();
};
