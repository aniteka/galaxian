#ifndef GALAXIAN_UTILITIES_H
#define GALAXIAN_UTILITIES_H

#include <string>
#include "cocos2d.h"

#define TO_STR(TO_CONV) #TO_CONV
#define TO_STR_COV(TO_CONV) TO_STR(TO_CONV)

#define GENERATE_ERROR_MESSAGE(ERROR_CAUSER) \
    (ERROR_CAUSER, "Error: " __FILE__ "(" TO_STR_COV(__LINE__) ")" ": " #ERROR_CAUSER "\n")

#endif //GALAXIAN_UTILITIES_H
