#pragma once

#include <string>

struct GunData {
	const std::string name;
	const float damage;
	const float bulletSpeed;

	const bool isAuto;
	const float timeBetweenShots;

	GunData(const std::string& name_, const float damage_, const float bulletSpeed_,
			const bool isAuto_, const float timeBetweenShots_)
			: name(name_), damage(damage_), bulletSpeed(bulletSpeed_), isAuto(isAuto_),
			timeBetweenShots(timeBetweenShots_) {}
};
