#pragma once

class CTimerFrames {
private:
	static CTimerFrames* instance;
	bool paused = false;
	float time = 0;
	int frames = 0;
public:
	CTimerFrames();
	~CTimerFrames();
	static CTimerFrames& get();
	void update(float delta);
	float getTime() const;
	int getFrames() const;
	void reset();
	void setPaused(bool paused);
};
