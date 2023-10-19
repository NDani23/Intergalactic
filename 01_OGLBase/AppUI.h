#pragma once

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <functional>

#include "GameState.h"
#include "WeaponDataStorage.h"
#include "UpgradeDataStorage.h"
#include "Weapon.h"
#include "Upgrade.h"

class CMyApp;

class AppUI
{
private:
	CMyApp* m_app;

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
};
