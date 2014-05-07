#include "utils.h"



/*
 * VAL ABS
 */
// return absolute value of given object
template <class T> T valAbs(T number) {
	if(number < (T)0)
		number *= -1;
	return number;
}



/*
 * MIX TAB
 */
// mix given tab
template <class T> void mixTab(T* tab, const int sizeOfTab, const int numberOfMix = 1) {
	// making mix k times
	for(int k = numberOfMix-1; k >= 0; k--) {
		// for one mix :
		for(int i = sizeOfTab-1; i >= 0; i--) {
			// swap the current item of tab with another, choose randomly
			swap(&tab[randN(sizeOfTab)], &tab[i], sizeof(T));
		}
	}
}




/*
 * WITH DELTA
 */
// return the given value modified randomly with delta value
template <class T> T withDelta(const T value, const T delta) {
	return value + randN((int)delta*2) - delta;
}




/*
 * GET ITEM OF
 */
// return the first item and delete it from queue
template<class T> T getItemOf(std::queue<T> &queue) {
	T returnValue = queue.front();
	queue.pop();
	return returnValue;
}



/*
 * COPY TAB
 */
// return pointer to new table, copy of given tab. (tab must have the same size in 
// 	each dimensions)
template <class T> T** cpyTab(const T** const tab, const unsigned int sizeOfTab) {
	T** newTab = new T*[sizeOfTab];
	for(unsigned int i = 0; i < sizeOfTab; i++) {
		newTab[i] = new T[sizeOfTab];
		for(unsigned int j = 0; j < sizeOfTab; j++) {
			newTab[i][j] = tab[i][j];
		}
	}
	return newTab;
}


/*
 * MAX
 */
// return a if a > b
template <class T> T max(const T a, const T b) {
        if(a > b)       return a;
        else            return b;
}


/*
 * MIN
 */
// return a if a < b
template <class T> T min(const T a, const T b) {
        if(a < b)       return a;
        else            return b;
}



