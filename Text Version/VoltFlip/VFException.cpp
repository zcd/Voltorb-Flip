#include "VFException.h"

VFException::VFException(std::string message, Move move)
{
    this->message = message;
    this->move = move;
}

VFException::~VFException(void)
{
}

std::ostream& operator<<(std::ostream &strm, const VFException &v) {
    return strm << "VFException: " << v.move << ". " << v.message << std::endl;
}