#include "WeaponDataStorage.h"
#include "Player.h"

WeaponDataStorage::WeaponDataStorage()
{
	m_Weapons.insert
	(
		{
			0,
			std::move(
				WeaponItem(
				{[](Player* player, int side) { return std::make_unique<RocketLauncher>(player, side); },
				Texture2D("assets/rocket.png"),
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
				Texture2D("assets/mine.png"),
				"Mine Placer",
				1500})
			)
		}
	);

}

std::map<int, WeaponItem>& WeaponDataStorage::GetStorage()
{
	return m_Weapons;
}