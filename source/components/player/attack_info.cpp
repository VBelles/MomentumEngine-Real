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
	invulnerabilityTime = j.value("invulnerability_time", 0.f);
	givesPower = j.value("gives_power", false);
	activatesMechanism = j.value("activates_mechanism", false);
	ignoresBlock = j.value("ignores_block", false);
	superArmorDamage = j.value("super_armor_damage", 0);

	if (j.count("stun")) {
		stun = new AttackInfo::Stun{ j.value("stun", 1.0f) };
	}
	if (j.count("vertical_launcher")) {
		const json& jVerticalLauncher = j["vertical_launcher"];
		verticalLauncher = new AttackInfo::VerticalLauncher{ jVerticalLauncher.value("suspension_duration", 1.0f),
			loadVEC3(jVerticalLauncher["velocity"]) };
	}
	if (j.count("horizontal_launcher")) {
		const json& jHorizontalLauncher = j["horizontal_launcher"];
		verticalLauncher = new AttackInfo::VerticalLauncher{ jHorizontalLauncher.value("suspension_duration", 1.0f),
			loadVEC3(jHorizontalLauncher["velocity"]) };
	}
	if (j.count("grab")) {
		grab = new AttackInfo::Grab{ j.value("grab", 1.0f) };
	}
	if (j.count("propel")) {
		const json& jPropel = j["propel"];
		propel = new AttackInfo::Propel{jPropel.value("duration", 0.f), loadVEC3(j["propel"])};
	}
}
