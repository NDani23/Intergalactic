#pragma once

#include <functional>

#include "../weapons/LaserGun.h"
#include "../weapons/RocketLauncher.h"
#include "../weapons/MinePlacer.h"
#include "../weapons/MachineGun.h"
#include "../weapons/TurretWeapon.h"

class player;

struct WeaponItem
{
	std::function<std::unique_ptr<Weapon>(Player*, int)> InsertWeapon;
	Texture2D Image;
	std::string Text;
	int Price;
	bool Owned = false;
};

class WeaponDataStorage
{
private:
	std::map<int, WeaponItem> m_Weapons;
public:
	WeaponDataStorage();
	std::map<int, WeaponItem>& GetStorage();
};