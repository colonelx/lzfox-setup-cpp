#ifndef EXCEPTION_H
#define EXCEPTION_H
#include <exception>
#include <string>

using namespace std;

class GeneralException : public std::exception {
  public:
    GeneralException(string msg) throw();
    virtual string what() throw();
  private:
    string message;
};
#endif