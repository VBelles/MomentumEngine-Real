#pragma once

struct UniqueElement {
	bool done;
	VEC3 position;
	std::string level;
};


class CModuleUniques : public IModule {
private:
	enum ElementType {
		COIN,
		CHRYSALIS,
		ALTAR,
		EVENT
	};

	//maps
	std::map<std::string, UniqueElement> coins;
	std::map<std::string, UniqueElement> chrysalides;
	std::map<std::string, UniqueElement> altars;
	std::map<std::string, UniqueElement> events;

	void parseChunk(const json& j, ElementType type);

public:
	CModuleUniques(const std::string& aname) : IModule(aname) {}
	bool start() override;
	bool stop() override;
	void update(float delta) override;

	UniqueElement* getUniqueCoin(std::string id);
	UniqueElement* getUniqueChrysalis(std::string id);
	UniqueElement* getUniqueAltar(std::string id);
	UniqueElement* getUniqueEvent(std::string id);

	bool setCoinTaken(std::string id, bool isTaken);
	bool setChrysalisTaken(std::string id, bool isTaken);
	bool setAltarBroken(std::string id, bool isBroken);
	bool setEventTriggered(std::string id, bool isTriggered);
};
