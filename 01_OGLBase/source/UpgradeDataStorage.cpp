#include "../headers/UpgradeDataStorage.h"
#include "../headers/Player.h"

UpgradeDataStorage::UpgradeDataStorage()
{
	m_Upgrades.insert
	(
		{
			0,
			std::move(
				UpgradeItem({[](Player* player) { return std::make_unique<SpeedBooster>(player); },
				Texture2D("assets/Upgrades/booster.png"),
				"Speed booster",
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
				1500})
			)
		}
	);
}

std::map<int, UpgradeItem>& UpgradeDataStorage::GetStorage()
{
	return m_Upgrades;
}