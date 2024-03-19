#ifndef GALAXIAN_UTILITIES_H
#define GALAXIAN_UTILITIES_H

#include <string>
#include <iostream>
#include "cocos2d.h"

#define TO_STR(TO_CONV) #TO_CONV
#define TO_STR_COV(TO_CONV) TO_STR(TO_CONV)

#define GENERATE_ERROR_MESSAGE(ERROR_CAUSER) \
    (ERROR_CAUSER, "Error: " __FILE__ "(" TO_STR_COV(__LINE__) ")" ": " #ERROR_CAUSER)

#define BODY_SPRITE             "PlayerShip.png"
#define PROJECTILE_SPRITE       "Projectile.png"

#define ENEMY_BLUE_SPRITE       "Enemies/Blue/Idle%i.png"
#define ENEMY_BLUE_COUNT        2

#define ENEMY_PURPLE_SPRITE     "Enemies/Purple/Idle%i.png"
#define ENEMY_PURPLE_COUNT      2

#define ENEMY_RED_SPRITE        "Enemies/Red/Idle%i.png"
#define ENEMY_RED_COUNT         2

#define ENEMY_FLAGSHIP_SPRITE   "Enemies/Flagship/Idle%i.png"
#define ENEMY_FLAGSHIP_COUNT    1

#define START_BUTTON_SPRITE     "Menu/StartButton.png"
#define EXIT_BUTTON_SPRITE      "Menu/ExitButton.png"
#define REPEAT_BUTTON_SPRITE    "Menu/RepeatButton.png"

#endif //GALAXIAN_UTILITIES_H
