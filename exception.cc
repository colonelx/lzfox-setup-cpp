#ifndef EXCEPTION_CC
#define EXCEPTION_CC
#include "exception.h"
#include <exception>
#include <string>

using namespace std;

GeneralException::GeneralException(string msg) throw() : message(msg) {}

string GeneralException::what() throw() {
    return this->message;
}
#endif