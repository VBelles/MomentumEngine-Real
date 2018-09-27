#pragma once

class CGameState : public VModules {
public:
	CGameState(const std::string& name, const bool& needsResources)
		: _name(name), _needsResources(needsResources) {
	}
	const std::string& getName() const { return _name; }
	const bool& needsResources() const { return _needsResources; }
private:
	std::string _name;
	bool _needsResources = false;
};

using VGameStates = std::vector<CGameState*>;
