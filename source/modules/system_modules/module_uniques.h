#pragma once

enum EnemyType {
	DREIDEL, KIPPAH, GOLEM, DREIDEL_DUMMY, KIPPAH_DUMMY, GOLEM_DUMMY
};

enum ElementType {
	UNDEFINED,
	COIN,
	CHRYSALIS,
	ALTAR,
	EVENT,
	ENEMY,
	LIFEPIECE,
	POWERUP
};

struct UniqueElement {
	bool done;
	VEC3 position;
	std::string level;
};

struct UniqueEnemy : public UniqueElement {
	EnemyType type;
};

struct UniquePowerUp : public UniqueElement {
	std::string stateToUnlock;
};

class CModuleUniques : public IModule {
private:

	//maps
	std::map<std::string, UniqueElement> coins;
	std::map<std::string, UniqueElement> lifePieces;
	std::map<std::string, UniqueElement> chrysalides;
	std::map<std::string, UniqueElement> altars;
	std::map<std::string, UniqueElement> events;
	std::map<std::string, UniqueEnemy> enemies;
	std::map<std::string, UniquePowerUp> powerUps;

	void parseChunk(const json& j, ElementType type);

	void parseEnemy(const json & j, std::string id);
	void parsePowerUp(const json & j, std::string id);

public:
	CModuleUniques(const std::string& aname) : IModule(aname) {}
	bool start() override;
	bool stop() override;
	void reset();
	void update(float delta) override;

	UniqueElement* getUniqueElement(ElementType type, std::string id);
	UniqueElement* getUniqueCoin(std::string id);
	UniqueElement* getUniqueLifePiece(std::string id);
	UniqueElement* getUniqueChrysalis(std::string id);
	UniqueElement* getUniqueAltar(std::string id);
	UniqueElement* getUniqueEvent(std::string id);
	UniqueEnemy* getUniqueEnemy(std::string id);
	UniquePowerUp* getUniquePowerUp(std::string id);

	int getNumUniqueElement(ElementType type);

	bool setCoinTaken(std::string id, bool isTaken);
	bool setLifePieceTaken(std::string id, bool isTaken);
	bool setChrysalisTaken(std::string id, bool isTaken);
	bool setAltarBroken(std::string id, bool isBroken);
	bool setEventTriggered(std::string id, bool isTriggered);
	bool setEnemyDead(std::string id, bool isDead);
	bool setPowerUpTaken(std::string id, bool isTaken);
};
