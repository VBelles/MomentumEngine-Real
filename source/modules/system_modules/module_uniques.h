#pragma once

enum EnemyType {
	DREIDEL, KIPPAH
};

struct UniqueElement {
	bool done;
	VEC3 position;
	std::string level;
};

struct UniqueEnemy {
	bool done;
	VEC3 position;
	std::string level;
	EnemyType type;
};

class CModuleUniques : public IModule {
private:
	enum ElementType {
		COIN,
		CHRYSALIS,
		ALTAR,
		EVENT,
		ENEMY
	};

	//maps
	std::map<std::string, UniqueElement> coins;
	std::map<std::string, UniqueElement> chrysalides;
	std::map<std::string, UniqueElement> altars;
	std::map<std::string, UniqueElement> events;
	std::map<std::string, UniqueEnemy> enemies;

	void parseChunk(const json& j, ElementType type);

	void parseEnemy(const json & j, std::string id);

public:
	CModuleUniques(const std::string& aname) : IModule(aname) {}
	bool start() override;
	bool stop() override;
	void update(float delta) override;

	UniqueElement* getUniqueCoin(std::string id);
	UniqueElement* getUniqueChrysalis(std::string id);
	UniqueElement* getUniqueAltar(std::string id);
	UniqueElement* getUniqueEvent(std::string id);
	UniqueEnemy* getUniqueEnemy(std::string id);

	bool setCoinTaken(std::string id, bool isTaken);
	bool setChrysalisTaken(std::string id, bool isTaken);
	bool setAltarBroken(std::string id, bool isBroken);
	bool setEventTriggered(std::string id, bool isTriggered);
	bool setEnemyDead(std::string id, bool isDead);
};
