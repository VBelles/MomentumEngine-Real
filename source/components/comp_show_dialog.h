#pragma once

struct TMsgAllScenesCreated;
struct TMsgTriggerEnter;
struct TMsgTriggerExit;
struct TMsgColliderDestroyed;

class TCompShowDialog : public TCompBase {
private:
	//json
	std::vector<std::string> dialogsToShow;
	float timeToShowMessage = 3.f;
	float timeToRefreshMessage = 10.f;
	float timeToAllowCancel = 0.5f;

	int dialogIndex = 0;

	CTimer2 cancelTimer;
	bool isCancelable = false;
	CTimer2 showMessageTimer;
	bool firstMessageShown = false;
	CTimer2 refreshMessageTimer;
	bool allMessagesShown = false;

	bool isPlayerInside = false;

public:
	DECL_SIBLING_ACCESS();

	void debugInMenu();
	static void registerMsgs();
	void load(const json& j, TEntityParseContext& ctx);
	void update(float delta);

	void onAllScenesCreated(const TMsgAllScenesCreated& msg);
	void onTriggerEnter(const TMsgTriggerEnter& msg);
	void onColliderDestroyed(const TMsgColliderDestroyed & msg);
	void onTriggerExit(const TMsgTriggerExit& msg);
};

