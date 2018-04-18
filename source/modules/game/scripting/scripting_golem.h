#pragma once

class TCompTransform;
namespace SLB{
	class Manager;
}

class ScriptingGolem {

private:
	CHandle playerTransformHandle;

public:
	ScriptingGolem();
	~ScriptingGolem();
	static void bind(SLB::Manager* manager);
	void spawnAt(float x, float y, float z);
	void spawn();
	TCompTransform* getPlayerTransform();
};

