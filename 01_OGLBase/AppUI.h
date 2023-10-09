#pragma once

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

#include "GameState.h"

class AppUI
{
private:
	bool show_menu;
	bool show_play_window;
	bool show_hangar_window;
	bool show_pause_window;
	GameState* m_state;

public:
	AppUI();
	AppUI(GameState*);

	void RenderUI();
};
