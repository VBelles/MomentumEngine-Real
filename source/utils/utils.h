#ifndef INC_UTILS_UTILS_H_
#define INC_UTILS_UTILS_H_

//#ifdef _DEBUG
void dbg(const char* fmt, ...);
//#else
//#define dbg(...)
//#endif

bool fatal(const char* fmt, ...);

bool isPressed(int key);

template <typename T>
T clamp(const T& n, const T& lower, const T& upper) {
	return std::max(lower, std::min(n, upper));
}

json loadJson(const std::string& filename);

#endif

