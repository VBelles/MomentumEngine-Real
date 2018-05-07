#pragma once

class CCamera {
    VEC3 pos;         // eye
    VEC3 target;
    VEC3 up_aux;

    // 3 orthonormals axis in world space
    VEC3 front;
    VEC3 up;
    VEC3 left;

	bool isOrtographic = false;
	float ortographicWidth = 50.f;
	float ortographicHeight = 50.f;

    // Prepective params
	float fov_vertical = deg2rad( 60.f );     // in radians!!
	float z_near = 1.f;
	float z_far = 1000.f;
	float aspect_ratio = 1.f;

    // Matrix
    MAT44 view;
    MAT44 proj;
    MAT44 view_proj;

    void updateViewProj();

  // viewport
  struct TViewport {
    int x0 = 0;
    int y0 = 0;
    int width = 640;
    int height = 480;
  };
  TViewport viewport;

public:
    //CCamera();

	bool isGuiCamera = false;

    VEC3 getPosition() const { return pos; }
    VEC3 getTarget() const { return target; }

    VEC3 getFront() const { return front; }
    VEC3 getUp() const { return up; }
    VEC3 getLeft() const { return left; }

	void getYawPitchRoll(float* yaw, float* pitch, float* roll) const;

    // -------------------------------------
    MAT44 getView() const { return view; }
    MAT44 getProjection() const { return proj; }
    MAT44 getViewProjection() const { return view_proj; }

    // -------------------------------------
    float getFov() const { return fov_vertical; }
    float getZNear() const { return z_near; }
    float getZFar() const { return z_far; }
    float getAspectRatio() const { return aspect_ratio; }
	bool isCameraOrtographic() const { return isOrtographic; }
	float getOrtographicWidth() const { return ortographicWidth; }
	float getOrtographicHeight() const { return ortographicHeight; }
	void setOrthographic(float width, float height, float new_z_near, float new_z_far);
    // 
    void lookAt(VEC3 new_pos, VEC3 new_target, VEC3 new_up_aux = VEC3(0, 1, 0));
    void setPerspective(float new_fov_vertical, float new_z_near, float new_z_far, bool isOrtographic = false, float new_width = 50.f, float new_height = 50.f);
	void setViewport(int x0, int y0, int width, int height);
    bool getScreenCoordsOfWorldCoord(VEC3 world_pos, VEC3 *screen_coords) const;

    VEC3 TransformToWorld(VEC2 velocityVector);
    VEC3 TransformToWorld(VEC3 velocityVector);
};
