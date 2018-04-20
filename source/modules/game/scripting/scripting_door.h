#pragma once

class TCompTransform;
class TCompDoor;

namespace SLB{
	class Manager;
}

class ScriptingDoor {

private:
	CHandle doorHandle;

public:
	ScriptingDoor(std::string name);
	~ScriptingDoor();
	static void bind(SLB::Manager* manager);
	void open(float time);
	void close(float time);

	TCompDoor* getDoor();
};

