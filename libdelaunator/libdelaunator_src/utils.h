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








/*
 * PREDECLARATIONS
 */
// structure of dual, two int (for coordonate, sizes,...)
struct dual {
	// CONSTRUCTOR
		dual(const int a = 0, const int b = 0) : x(a), y(b) {}
	// OPERATORS
                dual operator+(const dual &d) const     {return dual(x+d.x, y+d.y);}
                dual operator+=(const dual &d)          {x += d.x; y+= d.y; return *this;}
                dual operator-(const dual &d) const     {return dual(x-d.x, y-d.y);}
                bool operator==(const dual &d) const    {return (x==d.x && y==d.y);}
	// ATTRIBUTES
		int x, y;
};

// structure of integer percentage
struct percentage {
	public:
	// CONSTRUCTOR
		percentage(const int p_ = 0) {p = valAbs(p_) % 101; }
		percentage(const percentage &p_) {p = p_.p;}
	// METHODS
                int of(const int nb) { return (nb * p) / 100; }
                float of(const float nb) { return (nb * (float)p) / 100.; }
                int value() { return p; }
	// OPERATORS
		int operator==(const percentage &p_)	{return (p_.p == p);}
		int operator==(const int p_) 		{return (p_ == p);}
		int operator=(const percentage p_) 	{p = p_.p; return p;}
		int operator=(const int p_) 		{p = valAbs(p_)%101; return p;}
		int operator+(const percentage &p_) 	{return (p + p_.p)%101;}
		int operator+(const int p_) 		{return (p + p_)%101;}
		int operator-(const percentage &p_) 	{return valAbs(p - p_.p)%101;}
		int operator-(const int p_) 		{return valAbs(p - p_)%101;}
	private:
	// ATTRIBUTES
		int p; // value of percentage
};




#endif
