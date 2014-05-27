#include "utils.h"



/*
 * SWAP
 */
// universal invertion of a and b. They must use the same memory size. Use the XOR swap 
//      algorithm
void swap(const void* a, const void* b, const long unsigned int size) {
	// if a == b, algorithm is useless
	if(a != b) {
		// divided target into bytes
		unsigned char *A = (unsigned char*) a; 
		unsigned char *B = (unsigned char*) b;
		long unsigned int i = 0;
		// for each byte apply three XOR
		for(i = 0; i < size; i++) {
			A[i] ^= B[i];
			B[i] ^= A[i];
			A[i] ^= B[i];
		}
		// a and b are inverted !
	}
}




/*
 * STR AFTER
 */
// return the string contains by mainStr after subStr
std::string strAfter(const std::string subStr, /*in*/ const std::string mainStr) {
        std::string ret = "";
        char* p = (char*)strstr(mainStr.c_str(), subStr.c_str()); // p point the first caracter of subStr, or NULL
        if(p != NULL) { // if subStr found in mainStr
                p += subStr.size(); // first character after subStr in mainStr
                while(*p != '\0') {
                        ret += *p; // add the readed character to return string
                        p += 1; // next character
                }
        }
        return ret;
}




/*
 * RAND STR
 */
// Return a rand string contain given caracs
std::string randstr(size_t size, char* caracs) {
// INITIALIZATION
        unsigned int nb_caracs = strlen(caracs);
        std::string ret = "";
// TREATMENT
        for(unsigned int i = 0; i < size; i++) {
                ret += caracs[randN(nb_caracs)];
        }
        return ret;
}





/*
 * LOGS
 */
// Print given printf-like formated string in stderr.
//void flogs(short level, const char* str, ...) {
        //if(level < VERBOSE_LEVEL) {
                //va_list args;
                //va_start(args, str);
                //FILE* f = fopen(FILE_LOGS, "wa");
                //if(f != NULL) {
                        //vfprintf(f, str, args);
                        //fclose(f);
                //} else
                        //vfprintf(stderr, str, args);

                //va_end(args);
        //}
//}
//void ologs(short level, const char* str, ...) {
        //if(level < VERBOSE_LEVEL) {
                //va_list args;
                //va_start(args, str);
                //vfprintf(stderr, str, args);
                //va_end(args);
        //}
//}
void logs(const char* str, ...) {
        va_list args;
        va_start(args, str);
        vfprintf(stderr, str, args);
        va_end(args);
}





/*
 * ROUND FLOAT
 */
// Return float value, equal to given float value rounding about precision.
float round_float(const float f, const float precision) {
        unsigned int nb_digit = 1./precision - 1.;
        return ((float)((int)(f * nb_digit))) / (float)nb_digit;
}


