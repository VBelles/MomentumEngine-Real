#include "mcv_platform.h"
#include "attack_info.h"

AttackInfo::AttackInfo() {
}

AttackInfo::AttackInfo(const AttackInfo& other) {
	AttackInfo::operator=(other);
}

AttackInfo& AttackInfo::operator=(const AttackInfo& other) {
	damage = other.damage;
	invulnerabilityTime = other.invulnerabilityTime;
	givesPower = other.givesPower;
	activatesMechanism = other.activatesMechanism;
	SAFE_DELETE(stun);
	if (other.stun) stun = new Stun(*other.stun);
	SAFE_DELETE(verticalLauncher);
	if (other.verticalLauncher) verticalLauncher = new VerticalLauncher(*other.verticalLauncher);
	SAFE_DELETE(horizontalLauncher);
	if (other.horizontalLauncher) horizontalLauncher = new HorizontalLauncher(*other.horizontalLauncher);
	SAFE_DELETE(grab);
	if (other.grab) grab = new Grab(*other.grab);
	SAFE_DELETE(propel);
	if (other.propel) propel = new Propel(*other.propel);
	return *this;
}

AttackInfo::~AttackInfo() {
	SAFE_DELETE(stun);
	SAFE_DELETE(verticalLauncher);
	SAFE_DELETE(horizontalLauncher);
	SAFE_DELETE(grab);
	SAFE_DELETE(propel);
}

void AttackInfo::load(const json& j) {
	SAFE_DELETE(stun);
	SAFE_DELETE(verticalLauncher);
	SAFE_DELETE(horizontalLauncher);
	SAFE_DELETE(grab);
	SAFE_DELETE(propel);

	damage = j.value("damage", 0.0f);
	invulnerabilityTime = j.value("invulnerabilityTime", 0.f);
	givesPower = j.value("givesPower", false);
	activatesMechanism = j.value("activatesMechanism", false);

	if (j.count("stun")) {
		stun = new AttackInfo::Stun{ j.value("stun", 1.0f) };
	}
	if (j.count("verticalLauncher")) {
		const json& jVerticalLauncher = j["verticalLauncher"];
		verticalLauncher = new AttackInfo::VerticalLauncher{ jVerticalLauncher.value("suspensionDuration", 1.0f),
			loadVEC3(jVerticalLauncher["velocity"]) };
	}
	if (j.count("horizontalLauncher")) {
		const json& jHorizontalLauncher = j["horizontalLauncher"];
		verticalLauncher = new AttackInfo::VerticalLauncher{ jHorizontalLauncher.value("suspensionDuration", 1.0f),
			loadVEC3(jHorizontalLauncher["velocity"]) };
	}
	if (j.count("grab")) {
		grab = new AttackInfo::Grab{ j.value("grab", 1.0f) };
	}
	if (j.count("propel")) {
		propel = new AttackInfo::Propel{ loadVEC3(j["propel"]) };
	}
}
