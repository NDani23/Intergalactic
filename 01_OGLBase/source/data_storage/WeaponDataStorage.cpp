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
				"Thermal rocket launcher",
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
				1500})
			)
		}
	);

}

std::map<int, WeaponItem>& WeaponDataStorage::GetStorage()
{
	return m_Weapons;
}