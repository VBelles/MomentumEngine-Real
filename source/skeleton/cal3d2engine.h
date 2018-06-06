#ifndef INC_CAL3D_2_ENGINE_H_
#define INC_CAL3D_2_ENGINE_H_

// Cal2DX conversions, VEC3 are the same, QUAT must change the sign of w
CalVector DX2Cal(const VEC3& p);
CalQuaternion DX2Cal(const QUAT& q);
VEC3 Cal2DX(const CalVector& p);
QUAT Cal2DX(const CalQuaternion& q);

#endif



