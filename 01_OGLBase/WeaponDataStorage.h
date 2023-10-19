#pragma once

#include <functional>

#include "LaserGun.h"
#include "RocketLauncher.h"
#include "MinePlacer.h"

class player;

struct WeaponItem
{
	std::function<std::unique_ptr<Weapon>(Player*, int)> InsertWeapon;
	Texture2D Image;
	std::string Text;
};

class WeaponDataStorage
{
private:
	std::map<int, WeaponItem> m_Weapons;
public:
	WeaponDataStorage();
	std::map<int, WeaponItem>& GetStorage();
};