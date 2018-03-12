#pragma once
#include "components/player/comp_player_model.h"

class PlayerFilterCallback: public PxControllerFilterCallback {

private:
	CHandle playerModelHandle;


public:

	PlayerFilterCallback(CHandle playerModelHandle);
	TCompPlayerModel* getPlayerModel() {
		return playerModelHandle;
	}

	bool filter(const PxController& a, const PxController& b) override;

	
	
};