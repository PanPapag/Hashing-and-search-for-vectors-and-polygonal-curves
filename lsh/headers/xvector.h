#ifndef XVECTOR
#define XVECTOR

#include <string>
#include <vector>

template <class T>
class xVector {
  private:
    std::string item; //vector's id
    std::vector<T> feature_vector; //points' container
  public:
    xVector(std::string,std::vector<T>);
    void PrintVector(); //auxiliary func
    int getDimensions();
    std::vector<T>& getFeatureVector();
    ~xVector();
};

#endif
