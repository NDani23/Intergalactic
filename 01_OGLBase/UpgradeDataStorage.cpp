#include "UpgradeDataStorage.h"
#include "Player.h"

UpgradeDataStorage::UpgradeDataStorage()
{
	m_Upgrades.insert
	(
		{
			0,
			std::move(
				UpgradeItem({[](Player* player) { return std::make_unique<SpeedBooster>(player); },
				Texture2D("assets/booster.png"),
				"Speed booster",
				1000})
			)
		}
	);
}

std::map<int, UpgradeItem>& UpgradeDataStorage::GetStorage()
{
	return m_Upgrades;
}