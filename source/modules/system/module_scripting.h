#pragma once

//struct lua_State;

class CModuleScripting : public IModule {
public:

	CModuleScripting(const std::string& name);
	bool start() override;
	bool stop() override;
	void update(float delta) override;
	void render() override;

private:
	//lua_State* state;
};
