#pragma once

class TCompDoor;

namespace SLB {
	class Manager;
}

class ScriptingDoor {
private:
	static TCompDoor* getDoor(std::string doorName);

public:
	static void bind(SLB::Manager* manager);
	static bool open(std::string doorName, float time);
	static bool close(std::string doorName, float time);
};

