#pragma once


class PowerGauge {

public:
	class PowerListener {
	public:
		virtual void OnLevelChange(int powerLevel) = 0;
	};

private:
	PowerListener* powerListener;

public:
	int powerLevel = 1;	//Ex: 1, 2, 3 (ssj)
	float power = 0.f;	//Ex: 2300
	float maxPower = 30000.f;
	float powerPerLevel = 10000.f;
	float dropSpeed = 5.f * 60.f;

	PowerGauge(PowerListener* listener): powerListener(listener) {}
	void Update(float dt);
	void ReleasePower();
	void GainPower();
	void IncreasePower(float increment);
};