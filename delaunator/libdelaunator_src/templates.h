#include "utils.h"



/**
 * @param number an object that can be smaller than zero and multiply by -1
 * @return absolute value of given object
 */
template <class T> T valAbs(T number) {
	if(number < (T)0)
		number *= -1;
	return number;
}



/**
 * @param tab a reference to the table
 * @param sizeOfTab an int equals to number of element in the table
 * @param numberOfMix an int that increase mix quality but decrease performance with bigger values
 * Mix given tab
 */
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




/**
 * @param value a thing that can be added to another
 * @param delta a thing that can be cast in int.
 * @return a thing that is equals to value +- delta
 */
template <class T> T withDelta(const T value, const T delta) {
	return value + randN((int)delta*2) - delta;
}




/**
 * @param queue a STL Queue object
 * @return the first item of queue, and delete it
 */
template<class T> T getItemOf(std::queue<T> &queue) {
	T returnValue = queue.front();
	queue.pop();
	return returnValue;
}



/**
 * @param matrix two dimensions table of something
 * @param sizeOfMatrix unsigned int equals to matrix size in all dimensions
 * @return pointer to new matrix, copy of given one. 
 */
template <class T> T** cpyMatrix(const T** const matrix, const unsigned int sizeOfMatrix) {
	T** newMatrix = new T*[sizeOfMatrix];
	for(unsigned int i = 0; i < sizeOfMatrix; i++) {
		newMatrix[i] = new T[sizeOfMatrix];
		for(unsigned int j = 0; j < sizeOfMatrix; j++) {
			newMatrix[i][j] = matrix[i][j];
		}
	}
	return newMatrix;
}


/**
 * @param a is something that defined > operator
 * @param b is something like a
 * @return a if a > b
 */
template <class T> T max(const T a, const T b) {
        if(a > b)       return a;
        else            return b;
}


/**
 * @param a is something that defined < operator
 * @param b is something like a
 * @return a if a < b
 */
template <class T> T min(const T a, const T b) {
        if(a < b)       return a;
        else            return b;
}



