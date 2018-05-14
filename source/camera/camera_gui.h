#pragma once

#include "camera/camera_orthographic.h"

class CCameraGui : public CCameraOrthographic {
private:
	MAT44 loadOrtho(float x_min, float x_max, float y_min, float y_max, float z_min, float z_max);

public:
	void setPerspective(float new_fov_vertical, float new_z_near, float new_z_far) override;
	void setPerspective(float new_z_near, float new_z_far, float new_width, float new_height) override;
};
