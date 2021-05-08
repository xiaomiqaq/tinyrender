#ifndef UTIL_H
#define UTIL_H
#include<string>
#include"../macro.h"




class StrUtil
{
public:

	static bool cstr_equals(const char* cs1, const char* cs2)
	{
		return strcmp(cs1,cs2) == 0;
	}

	static int wrap_knob(const char *knob)
	{
		if (cstr_equals(knob, "on")) {
			return 1;
		}
		else {
			assert(cstr_equals(knob, "off"));
			return 0;
		}
	}
	static char *duplicate_string(const char *source) 
	{
		char *target = (char*)malloc(strlen(source) + 1);
		strcpy(target, source);
		return target;
	}
};
class FileUtil {
public:
	static const char *file_get_extension(const char *filename)
	{
		const char *dot_pos = strrchr(filename, '.');
		return dot_pos == NULL ? "" : dot_pos + 1;
	}

	//完整路径
	static const std::string get_absolute_path(const char *filename) {
		std::string path = std::string(PROJECT_ASSETS_DICTORY) + std::string(filename);
		return path;
	}
	//判断路径合法性
	static const char *wrap_path(const char *path) {
		if (StrUtil::cstr_equals(path, "null")) {
			return NULL;
		}
		else {
			return path;
		}
	}
	static int wrap_knob(const char *knob) {
		if (StrUtil::cstr_equals(knob, "on")) {
			return 1;
		}
		else {
			assert(StrUtil::cstr_equals(knob, "off"));
			return 0;
		}
	}

};
#endif