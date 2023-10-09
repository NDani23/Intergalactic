#include "AppUI.h"

AppUI::AppUI()
{
	show_menu = true;
	show_play_window = false;
	show_hangar_window = false;
	show_pause_window = false;

	m_state = nullptr;
}

AppUI::AppUI(GameState* state)
{
	show_menu = true;
	show_play_window = false;
	show_hangar_window = false;
	show_pause_window = false;

	m_state = state;
}

void AppUI::RenderUI()
{
	
}