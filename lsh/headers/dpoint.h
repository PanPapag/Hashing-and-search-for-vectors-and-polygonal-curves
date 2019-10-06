#ifndef DPOINT
#define DPOINT

#include <string>
#include <vector>

template <class T>
class dpoint {
  private:
    std::string item; //vector's id
    std::vector<T> feature_vector; //points' container
  public:
    dpoint(std::string,std::vector<T>&);
    void print_vector(); //auxiliary func
    ~dpoint();
};

#endif
