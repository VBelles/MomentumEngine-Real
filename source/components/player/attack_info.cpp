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