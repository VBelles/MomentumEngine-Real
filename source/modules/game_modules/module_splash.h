#pragma once

class CModuleSplash : public IModule {
public:
	CModuleSplash(const std::string& name)
		: IModule(name) {
	}
	bool start() override;
	void update(float delta) override;

private:
	float _timer;
};