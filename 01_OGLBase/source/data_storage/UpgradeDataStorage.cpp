#include "../../headers/data_storage/UpgradeDataStorage.h"
#include "../../headers/Player.h"

UpgradeDataStorage::UpgradeDataStorage()
{
	m_Upgrades.insert
	(
		{
			0,
			std::move(
				UpgradeItem({[](Player* player) { return std::make_unique<SpeedBooster>(player); },
				Texture2D("assets/Upgrades/booster.png"),
				"Speed Booster",
				"Speed Booster\n\nIncreases the speed of the ship to 2x the top speed.\n\nDuration: 5s\nCooldown: 20s",
				1000})
			)
		}
	);

	m_Upgrades.insert
	(
		{
			1,
			std::move(
				UpgradeItem({[](Player* player) { return std::make_unique<Shield>(player); },
				Texture2D("assets/Upgrades/shield.png"),
				"Shield",
				"Shield\n\nPlaces an energy shield around the ship that blocks any incoming projectiles.\n\nDuration: 10s\nCooldown: 30s",
				1500})
			)
		}
	);

	m_Upgrades.insert
	(
		{
			2,
			std::move(
				UpgradeItem({[](Player* player) { return std::make_unique<StealthCoat>(player); },
				Texture2D("assets/Upgrades/stealthCoat.png"),
				"Stealth Coat",
				"Stealth Coat\n\nMakes the ship invisible for enemies.\n\nDuration: 8s\nCooldown: 40s",
				1500})
			)
		}
	);
}

std::map<int, UpgradeItem>& UpgradeDataStorage::GetStorage()
{
	return m_Upgrades;
}