#pragma once

#include "camera/camera.h"

class CCameraOrthographic : public CCamera {
protected:
	float ortographicWidth = 50.f;
	float ortographicHeight = 50.f;

public:
	void debugInMenu();

	float getOrtographicWidth() const { return ortographicWidth; }
	float getOrtographicHeight() const { return ortographicHeight; }

	void setPerspective(float new_fov_vertical, float new_z_near, float new_z_far) override;
	virtual void setPerspective(float new_z_near, float new_z_far, float new_width, float new_height);
};
