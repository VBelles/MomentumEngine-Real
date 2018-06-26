#pragma once
#include "modules/module.h"

enum SlashType {
	LEFT_HAND, RIGHT_HAND, LEFT_FOOT, RIGHT_FOOT,
	LEFT_TENTACLE, RIGHT_TENTACLE, LEFT_TENTACLE_SHORT,
	RIGHT_TENTACLE_SHORT
};

class CModuleSlash : public IModule {
public:
	CModuleSlash(const std::string& name);
	bool start() override;
	bool stop() override;
	void update(float delta) override;
	void render() override;

};

