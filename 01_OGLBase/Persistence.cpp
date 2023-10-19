#include "Persistence.h"
#include "Player.h"

Persistence::Persistence()
{
	m_player = nullptr;
}


Persistence::Persistence(Player* player)
{
	m_player = player;
}

void Persistence::Save()
{
	std::ofstream myfile;
	myfile.open("data/player_data.txt", std::fstream::out);

	if (!myfile)
	{
		std::cerr << "Could not open file!\n";
		myfile.close();
	}

	std::stringstream ss;
	
	//credit
	ss << "c " << m_player->m_credit << "\n";

	//upgrade points
	ss << "u " << m_player->m_upgradePoints << "\n";

	//upgrade stats
	ss << "s ";
	ss << m_player->m_stats.damage << " ";
	ss << m_player->m_stats.fire_rate << " ";
	ss << m_player->m_stats.health << " ";
	ss << m_player->m_stats.mobility << " ";
	ss << m_player->m_stats.speed << "\n";



	myfile << ss.str();

	myfile.close();
}

bool Persistence::Load()
{
	std::ifstream myfile;

	myfile.open("data/player_data.txt", std::fstream::in);

	if (!myfile)
	{
		myfile.close();
		return false;
	}

	std::string line_id;

	while (myfile >> line_id)
	{
		
		if (line_id == "c")
		{
			myfile >> m_player->m_credit;
		}
		else if (line_id == "u")
		{
			myfile >> m_player->m_upgradePoints;
		}
		else if (line_id == "s")
		{
			myfile >> m_player->m_stats.damage;
			myfile >> m_player->m_stats.fire_rate;
			myfile >> m_player->m_stats.health;
			myfile >> m_player->m_stats.mobility;
			myfile >> m_player->m_stats.speed;
		}
	}
	
	myfile.close();
	return true;
}