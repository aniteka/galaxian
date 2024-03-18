#ifndef GALAXIAN_UTILITIES_H
#define GALAXIAN_UTILITIES_H

#include <string>
#include <iostream>
#include "cocos2d.h"

#define TO_STR(TO_CONV) #TO_CONV
#define TO_STR_COV(TO_CONV) TO_STR(TO_CONV)

#define GENERATE_ERROR_MESSAGE(ERROR_CAUSER) \
    (ERROR_CAUSER, "Error: " __FILE__ "(" TO_STR_COV(__LINE__) ")" ": " #ERROR_CAUSER)

#define BODY_SPRITE             "DEV/DevA.png"
#define PROJECTILE_SPRITE       "DEV/DevB.png"

#define ENEMY_BLUE_SPRITE       "DEV/DevBlue.png"
#define ENEMY_PURPLE_SPRITE     "DEV/DevPurple.png"
#define ENEMY_RED_SPRITE        "DEV/DevRed.png"
#define ENEMY_FLAGSHIP_SPRITE   "DEV/DevFlagship"

#define BASE_FONT               "fonts/arial.ttf"

#define START_BUTTON_SPRITE     "DEV/DevButton.png"
#define EXIT_BUTTON_SPRITE      "DEV/DevExit.png"

#endif //GALAXIAN_UTILITIES_H