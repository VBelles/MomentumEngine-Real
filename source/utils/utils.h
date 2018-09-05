#ifndef INC_UTILS_UTILS_H_
#define INC_UTILS_UTILS_H_

// -----------------------------------------

uint32_t getID(const char* txt);
uint32_t getID(const void* buff, size_t nbytes);

// -----------------------------------------
//#ifdef _DEBUG
void dbg(const char* fmt, ...);
//#else
//#define dbg(...)
//#endif

bool fatal(const char* fmt, ...);

bool isPressed(int key);

json loadJson(const std::string& filename);
bool fileExists(const std::string& afilename);

float scale(float A, float A1, float A2, float Min, float Max);


//#define safeDelete(x)  if(x) delete x, x = nullptr;

template <typename T>
inline void safeDelete(T*& t) {
	if (t != nullptr) {
		delete t;
		t = nullptr;
	}
}

constexpr auto fps = 1.f / 60.f;

inline float frames2sec(int frames) {
	return frames * fps;
}

inline float frames2sec(float frames) {
	return frames * fps;
}

//const float FPS = 1.f / 60.f;
//#define frames2sec(frames) (frames) * FPS

#endif

