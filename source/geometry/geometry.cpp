#include "mcv_platform.h"

VEC2 loadVEC2(const json& j) {
	VEC2 v;
	std::string str = j.get< std::string >();
	int n = sscanf(str.c_str(), "%f %f", &v.x, &v.y);
	assert(n == 2);
	return v;
}

VEC2 loadVEC2(const json& j, const VEC2& defaultValue) {
	if (!j.is_string()) return defaultValue;
	VEC2 v;
	std::string str = j.get< std::string >();
	if (str.empty()) return defaultValue;
	int n = sscanf(str.c_str(), "%f %f", &v.x, &v.y);
	if (n != 2) {
		v = defaultValue;
	}
	return v;
}

VEC3 loadVEC3(const json& j) {
	VEC3 v;
	std::string str = j.get< std::string >();
	int n = sscanf(str.c_str(), "%f %f %f", &v.x, &v.y, &v.z);
	assert(n == 3);
	return v;
}

VEC3 loadVEC3(const json& j, const VEC3& defaultValue) {
	if (!j.is_string()) return defaultValue;
	VEC3 v;
	std::string str = j.get< std::string >();
	if (str.empty()) return defaultValue;
	int n = sscanf(str.c_str(), "%f %f %f", &v.x, &v.y, &v.z);
	if (n != 3) {
		v = defaultValue;
	}
	return v;
}

VEC4 loadVEC4(const json& j) {
	VEC4 v;
	std::string str = j.get< std::string >();
	int n = sscanf(str.c_str(), "%f %f %f %f", &v.x, &v.y, &v.z, &v.w);
	assert(n == 4);
	return v;
}

VEC4 loadVEC4(const json& j, const VEC4& defaultValue) {
	if (!j.is_string()) return defaultValue;
	VEC4 v;
	std::string str = j.get< std::string >();
	if (str.empty()) return defaultValue;
	int n = sscanf(str.c_str(), "%f %f %f %f", &v.x, &v.y, &v.z, &v.w);
	if (n != 4) {
		v = defaultValue;
	}
	return v;
}

// from "0 1 2 3" return QUAT(0,1,2,3)
QUAT loadQUAT(const json& j) {
	auto txt = j.get<std::string>();
	QUAT q;
	int n = sscanf(txt.c_str(), "%f %f %f %f", &q.x, &q.y, &q.z, &q.w);
	assert(n == 4);
	return q;
}

QUAT loadQUAT(const json& j, const QUAT& defaultValue) {
	if (!j.is_string()) return defaultValue;
	QUAT q;
	std::string str = j.get< std::string >();
	if (str.empty()) return defaultValue;
	int n = sscanf(str.c_str(), "%f %f %f %f", &q.x, &q.y, &q.z, &q.w);
	if (n != 4) {
		q = defaultValue;
	}
	return q;
}
