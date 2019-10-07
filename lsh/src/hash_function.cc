#include <iostream>
#include <string>
#include <iterator>
#include <vector>

#include "../headers/xvector.h"
#include "../headers/hash_function.h"

template <class T>
HashFunction<T>::HashFunction(double r, xVector<T>& xi) {
    std::cout << "Hi" << std::endl;
}

template <class T>
HashFunction<T>::~HashFunction() {
    std::cout << "Bye" << std::endl;
}

template class HashFunction<double>;
template class HashFunction<std::vector<double>>;
