#pragma once

namespace SLB {
	class Manager;
}

class CMusicPlayer;

class ScriptingSound {
private:
	static void bindConstants(SLB::Manager* manager);

	static CMusicPlayer* getMusicPlayer();

public:
	static void bind(SLB::Manager* manager);

	static void startSoundEvent(std::string event);
	static void stopSoundEvent(std::string event, int mode);
	static void emitSoundEvent(std::string event);
	static void stopMusic(float time);
	static void startSong(std::string song);
};

