#pragma once

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <functional>

#include "GameState.h"
#include "Weapon.h"

class CMyApp;

struct DragDropWeaponItem
{
	std::function<std::unique_ptr<Weapon>(Player*, int)> InsertWeapon;
	Texture2D Image;
	std::string Text;
};

class AppUI
{
private:
	DragDropWeaponItem m_weaponChoices[2];
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
