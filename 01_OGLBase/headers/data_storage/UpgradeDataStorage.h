#pragma once

#include <functional>

#include "../upgrades/SpeedBooster.h"
#include "../upgrades/shield.h"
#include "../upgrades/StealthCoat.h"

class player;

struct UpgradeItem
{
	std::function<std::unique_ptr<Upgrade>(Player*)> InsertUpgrade;
	Texture2D Image;
	std::string Name;
	std::string Text;
	int Price;
	bool Owned = false;
};

class UpgradeDataStorage
{
private:
	std::map<int, UpgradeItem> m_Upgrades;
public:
	UpgradeDataStorage();
	std::map<int, UpgradeItem>& GetStorage();
};
