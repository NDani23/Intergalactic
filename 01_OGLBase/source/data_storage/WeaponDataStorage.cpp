#include "../../headers/data_storage/WeaponDataStorage.h"
#include "../../headers/Player.h"

WeaponDataStorage::WeaponDataStorage()
{
	m_Weapons.insert
	(
		{
			0,
			std::move(
				WeaponItem(
				{[](Player* player, int side) { return std::make_unique<RocketLauncher>(player, side); },
				Texture2D("assets/Weapons&Projectiles/rocket.png"),
				"Thermal Rocket Launcher",
				"Thermal Rocket Launcher\n\nShoots a rocket that follows the targeted enemy and explodes upon impact.\n\nDamage: 100\nCooldown: 15s",
				2000})
			)
		}
	);

	m_Weapons.insert
	(
		{
			1,
			std::move(
				WeaponItem({[](Player* player, int side) { return std::make_unique<MinePlacer>(player, side); },
				Texture2D("assets/Weapons&Projectiles/mine.png"),
				"Mine Placer",
				"Mine Placer\n\nLeaves a mine behind that explodes when an enemy get close to it.\n\nDamage: 200\nCooldown: 15s",
				1500})
			)
		}
	);

	m_Weapons.insert
	(
		{
			2,
			std::move(
				WeaponItem({[](Player* player, int side) { return std::make_unique<MachineGun>(player, side); },
				Texture2D("assets/Weapons&Projectiles/machine_gun.png"),
				"Machine Gun",
				"Machine Gun\n\nA laser machine gun with high fire rate and free aiming.\n\nDamage: 10\nCooldown: 0.1s",
				2500})
			)
		}
	);

	m_Weapons.insert
	(
		{
			3,
			std::move(
				WeaponItem({[](Player* player, int side) { return std::make_unique<TurretWeapon>(player, side); },
				Texture2D("assets/Weapons&Projectiles/turret_weapon.png"),
				"Turret",
				"Turret\n\nAn automatic device that shoots the closest enemy for 15 seconds after activated.\n\nDamage: 10\nCooldown: 60s",
				1500})
			)
		}
	);

}

std::map<int, WeaponItem>& WeaponDataStorage::GetStorage()
{
	return m_Weapons;
}