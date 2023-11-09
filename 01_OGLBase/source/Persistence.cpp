#include "../headers/Persistence.h"
#include "../headers/MyApp.h"

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

	ss << "r " << m_app->m_player.GetRecord() << "\n";

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
	m_app->m_player.m_guns[2] == nullptr ? ss << "-1 \n" : ss << m_app->m_player.m_guns[2]->ID() << "\n";

	//equiped Upgrade
	ss << "eu ";
	m_app->m_player.m_Upgrade == nullptr ? ss << "-1 \n" : ss << m_app->m_player.m_Upgrade->ID() << "\n";

	//owned weapons
	ss << "ow ";
	{
		std::vector<int> indexes;
		for (int i = 0; i < m_app->m_weaponStorage.GetStorage().size(); i++)
		{
			if (m_app->m_weaponStorage.GetStorage().at(i).Owned) indexes.push_back(i);
		}
		ss << indexes.size();

		for (int i : indexes)
		{
			ss << " ";
			ss << i;
		}
		ss << "\n";
	}

	//owned upgrades
	ss << "ou ";
	{
		std::vector<int> indexes;
		for (int i = 0; i < m_app->m_upgradeStorage.GetStorage().size(); i++)
		{
			if (m_app->m_upgradeStorage.GetStorage().at(i).Owned) indexes.push_back(i);
		}
		ss << indexes.size();

		for (int i : indexes)
		{
			ss << " ";
			ss << i;
		}
		ss << "\n";
	}

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
		else if (line_id == "r")
		{
			int record;
			myfile >> record;
			m_app->m_player.setRecord(record);
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
		else if (line_id == "ow")
		{
			int count;
			myfile >> count;

			for (int i = 0; i < count; i++)
			{
				int id;
				myfile >> id;
				m_app->m_weaponStorage.GetStorage().at(id).Owned = true;
			}
		}
		else if (line_id == "ou")
		{
			int count;
			myfile >> count;

			for (int i = 0; i < count; i++)
			{
				int id;
				myfile >> id;
				m_app->m_upgradeStorage.GetStorage().at(id).Owned = true;
			}
		}
	}
	
	myfile.close();
	return true;
}