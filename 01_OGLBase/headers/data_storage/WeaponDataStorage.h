#pragma once

#include <functional>

#include "headers/weapons/LaserGun.h"
#include "headers/weapons/RocketLauncher.h"
#include "headers/weapons/MinePlacer.h"
#include "headers/weapons/MachineGun.h"
#include "headers/weapons/TurretWeapon.h"

class player;

struct WeaponItem
{
	std::function<std::unique_ptr<Weapon>(Player*, int)> InsertWeapon;
	Texture2D Image;
	std::string Name;
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