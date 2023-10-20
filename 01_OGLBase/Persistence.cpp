#include "Persistence.h"
#include "MyApp.h"

Persistence::Persistence()
{
	m_app = nullptr;
}


Persistence::Persistence(CMyApp* app)
{
	m_app = app;
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
	ss << "c " << m_app->m_player.m_credit << "\n";

	//upgrade points
	ss << "u " << m_app->m_player.m_upgradePoints << "\n";

	//upgrade stats
	ss << "s ";
	ss << m_app->m_player.m_stats.damage << " ";
	ss << m_app->m_player.m_stats.fire_rate << " ";
	ss << m_app->m_player.m_stats.health << " ";
	ss << m_app->m_player.m_stats.mobility << " ";
	ss << m_app->m_player.m_stats.speed << "\n";

	//equiped weapons
	ss << "w ";
	m_app->m_player.m_guns[0] == nullptr ? ss << "-1 " : ss << m_app->m_player.m_guns[0]->ID() << " ";
	m_app->m_player.m_guns[2] == nullptr ? ss << "-1 " : ss << m_app->m_player.m_guns[2]->ID() << "\n";

	//equiped Upgrade
	ss << "eu ";
	m_app->m_player.m_Upgrade == nullptr ? ss << "-1 " : ss << m_app->m_player.m_Upgrade->ID() << "\n";


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
			myfile >> m_app->m_player.m_credit;
		}
		else if (line_id == "u")
		{
			myfile >> m_app->m_player.m_upgradePoints;
		}
		else if (line_id == "s")
		{
			myfile >> m_app->m_player.m_stats.damage;
			myfile >> m_app->m_player.m_stats.fire_rate;
			myfile >> m_app->m_player.m_stats.health;
			myfile >> m_app->m_player.m_stats.mobility;
			myfile >> m_app->m_player.m_stats.speed;
		}
		else if (line_id == "w")
		{
			int id;

			myfile >> id;
			if(id != -1) m_app->m_player.m_guns[0] = m_app->m_weaponStorage.GetStorage().at(id).InsertWeapon(&m_app->m_player, -1);

			myfile >> id;
			if (id != -1) m_app->m_player.m_guns[2] = m_app->m_weaponStorage.GetStorage().at(id).InsertWeapon(&m_app->m_player, 1);
		}
		else if (line_id == "eu")
		{
			int id;

			myfile >> id;
			if(id != -1) m_app->m_player.GetUpgrade() = m_app->m_upgradeStorage.GetStorage().at(id).InsertUpgrade(&m_app->m_player);
		}
	}
	
	myfile.close();
	return true;
}