#pragma once

class CModulePause : public IModule {
private:
	bool pause = false;

	void onPausePressed();

public:
	CModulePause(const std::string& aname) : IModule(aname) {}
    bool start() override;
    bool stop() override;
    void update(float delta) override;
    void render() override;
};