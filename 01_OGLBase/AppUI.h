#pragma once

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

#include "GameState.h"

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
