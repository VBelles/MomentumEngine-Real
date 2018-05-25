#pragma once

struct UniqueThing {
	bool done;
	VEC3 position;
	std::string level;
};

class CModuleUniques : public IModule {
private:
	//maps
	std::map<std::string, UniqueThing> coins;
	std::map<std::string, UniqueThing> chrysalis;
	std::map<std::string, UniqueThing> altars;
	std::map<std::string, UniqueThing> events;

public:
	CModuleUniques(const std::string& aname) : IModule(aname) {}
	bool start() override;
	bool stop() override;
	void update(float delta) override;

	UniqueThing* getUniqueCoin(std::string id);
	UniqueThing* getUniqueChrysalis(std::string id);
	UniqueThing* getUniqueAltar(std::string id);
	UniqueThing* getUniqueEvent(std::string id);

	void setCoinTaken(std::string id, bool isTaken);
	void setChrysalisTaken(std::string id, bool isTaken);
	void setAltarBroken(std::string id, bool isBroken);
	void setEventTriggered(std::string id, bool isTriggered);
};
