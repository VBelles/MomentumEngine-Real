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

#endif

