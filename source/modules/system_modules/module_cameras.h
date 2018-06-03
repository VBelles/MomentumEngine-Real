#pragma once

class CModuleCameras : public IModule {
public:
	enum EPriority { DEFAULT = 0, GAMEPLAY, TEMPORARY, DEBUG, NUM_PRIORITIES };

	std::map<std::string, Interpolator::IInterpolator*> interpolators;

	CModuleCameras(const std::string& name);
	bool start() override;
	bool stop() override;
	void update(float delta) override;
	void render() override;

	void setDefaultCamera(CHandle camera);
	void setOutputCamera(CHandle camera);
	void blendInCamera(CHandle camera, float blendTime = 0.f,
                       EPriority priority = EPriority::DEFAULT,
                       Interpolator::IInterpolator* interpolator = nullptr);
	void blendOutCamera(CHandle camera, float blendTime = 0.f);
	bool IsCameraAloneInMix(CHandle camera);

    CHandle getCurrentBlendedCamera() { return _mixedCameras.front().camera; }

	Interpolator::IInterpolator* getInterpolator(std::string interpolator);

private:
	void renderInMenu();

	struct TMixedCamera {
		enum EState { ST_BLENDING_IN, ST_IDLE, ST_BLENDING_OUT };

		CHandle camera;
		EState state = EState::ST_IDLE;
		EPriority type = EPriority::DEFAULT;
		float blendInTime = 0.f; // gets to full ratio (1.f) in n seconds
		float blendOutTime = 0.f; // gets to full ratio (1.f) in n seconds
		float weight = 0.f;  // blend weight ratio
		float time = 0.f; // current blending time
		Interpolator::IInterpolator* interpolator = nullptr;

		void blendIn(float duration);
		void blendOut(float duration);
	};

	using VMixedCameras = std::vector<TMixedCamera>;

	TMixedCamera* getMixedCamera(CHandle camera);
	void blendCameras(const CCamera* camera1,
                      const CCamera* camera2,
                      float ratio, CCamera* output) const;
	void checkDeprecated();

	VMixedCameras _mixedCameras;
	CHandle _defaultCamera;
	CHandle _outputCamera;
};
