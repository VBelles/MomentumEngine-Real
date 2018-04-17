#pragma once

class TCompPlayerModel;

class ScriptingPlayer {
private:
	CHandle playerModelHandle;
	TCompPlayerModel* getPlayerModel();

public:
	ScriptingPlayer();
	~ScriptingPlayer();
	float getHp();
};

