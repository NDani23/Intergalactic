#pragma once

#include <functional>

#include "SpeedBooster.h"
#include "Shield.h"

class player;

struct UpgradeItem
{
	std::function<std::unique_ptr<Upgrade>(Player*)> InsertUpgrade;
	Texture2D Image;
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
