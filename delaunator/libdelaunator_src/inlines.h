#include "utils.h"




/******************************************************************
 * CONVERTION METHODS
 ******************************************************************/
/**
 * @return int equivalent to given string
 */
inline int str2int(std::string str) {
        std::stringstream ss(str);
        int n;
        ss >> n;
        return n;
}



/**
 * @return unsigned int equivalent to given string
 */
inline unsigned int str2uint(std::string str) {
        std::stringstream ss(str);
        unsigned int n;
        ss >> n;
        return n;
}



/**
 * @param line string of unsigned int separated by ':' character
 * @return a vector of unsigned int where values are write in given string
 */
inline std::vector<unsigned int> str2vec_uint(std::string line) {
// INITIALIZING
        std::vector<unsigned int> values;
        std::string val = "";
// TREATMENT
        for(unsigned int i = 0, count = line.size(); i < count; i++) {
                if(line[i] == ':') {
                        values.push_back(str2uint(val));
                        val = "";
                } else  val += line[i];
        }
// ENDING
        return values;
}


/**
 * @param vec vector of unsigned int
 * @return string equivalent of given vector. format : val:val:[val:,...]
 */
inline std::string vec_uint2str(std::vector<unsigned int> vec) {
        std::string line = "";
        for(auto v : vec)       line += std::to_string(v) + ":";
        return line;
}





/******************************************************************
 * TIME METHODS
 ******************************************************************/
/**
 * @param evaluated clock 
 * @return seconds passes since given clock
 */
inline float timeSince(const clock_t t) {
	return ((float)(clock()-t) / CLOCKS_PER_SEC);
}



/**
 * @param milliseconds is time of waiting.
 * stop the program for given milliseconds
 */
inline void waitFor(const int milliseconds) {
#ifdef __GNUC__
	sleep((float)milliseconds / 1000.);
#elif __WIN32__
	sleep(milliseconds);
#endif
}





/******************************************************************
 * LOGS MANAGEMENT METHODS
 ******************************************************************/
/**
 * add log to FILE_LOGS
 * @param log string to add to logs
 */
inline void clogs(const std::string log) {
	std::ofstream fl(FILE_LOGS, std::ios::app);
	if(fl != NULL) {
		fl << log << std::endl;
		fl.close();
	} else
		std::cerr << "ERROR: FILE_LOGS can't opened !" << std::endl;
}
// modify clogs
/**
 * Modify behaviour of clogs, or apply an operation to existing data.
 * @param order an int that defined a behaviour/an order.
 * orders can be : 
 *      0 for erase all the log file content
 */
inline void clogs(const int order) {
	if(order == 0) { // ERASE ALL THE FILE CONTENT
		std::ofstream fl(FILE_LOGS);
		if(fl != NULL)	fl.close();
		else std::cerr << "ERROR: FILE_LOGS can't opened !" << std::endl;
        }
}






/******************************************************************
 * RANDOM METHODS
 ******************************************************************/
/**
 * @param N the excluded bound 
 * @return random number between 0 included and N exluded.
 */
inline int randN(const int N) {
        return (int) (rand() / (double)RAND_MAX * (N));
}



/**
 * @param n a positive integer
 * @return true one time on n
 */
inline bool aChanceOn(const unsigned int n) {
        return (randN(n) == 0); // true one time on n
}






/******************************************************************
 * TERM METHODS
 ******************************************************************/


/**
 * Modify color of text and background in terminal.
 * Colors :   
 *      0 black   
 *      1 red  
 *      2 green  
 *      3 orange  
 *      4 blue  
 *      5 magenta  
 *      6 cyan  
 *      7 white  
 *  @param textc a color that will be used for letters
 *  @param backc a color that will be used for background
 */
enum termcolor { BLACK=0, RED=1, GREEN=2, ORANGE=3, BLUE=4, MAGENTA=5, CYAN=6, WHITE=7 };
inline void term_color(const int textc = 7, const int backc = 0) {
	std::cout << "\033[" << textc+30 << ";" << backc+40 << "m";
}


/**
 * Reset term colors.
 */
inline void term_reset() {
	std::cout << "\033[0m";
}


/**
 * Clear term.
 */
inline void term_clear() {
	std::cout << "\033[H\033[2J";
}



