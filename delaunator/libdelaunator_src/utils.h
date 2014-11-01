#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED



/*
 * LIBRARIES
 */
// STD LIB
#include <iostream>	/* cerr, endl */
#include <fstream>	/* ofstream type */
#include <sstream>	/* streams types */
#include <ctime>	/* t_clock */
#include <cstdlib>	/* sleep (WIN32) */
#include <cstdarg>	/* var-len args */
#include <string>	/* string type */
#include <cstring>	/* string function */
#include <queue>	/* queue type */
#include <cfloat>       /* FLT_MAX */
#ifdef __GNUC__
#include <unistd.h> 	/* sleep (GNUC) */
#endif




/*
 * DEFINES
 */
#define FILE_LOGS "logs"
//extern unsigned short VERBOSE_LEVEL = 0; // greater it is, more logs there is.



/*
 * PROTOTYPES
 */
void swap(const void*, const void*, const long unsigned int); // universal invertion of a and b. They must use the same memory size. Use the XOR swap algorithm
std::string strAfter(const std::string, const std::string); // return the string contains by mainStr after subStr
std::string randstr(size_t size, char* caracs); // Return a rand string contain given caracs
//void flogs(short, const char* const, ...);
//void ologs(short, const char* const, ...);
void logs(const char* const, ...);
float round_float(const float, const float);

// explicit prototypes
#include "inlines.h"
#include "templates.h"








#endif
