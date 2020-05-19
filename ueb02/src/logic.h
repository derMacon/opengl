
#ifndef __LOGIC_H__
#define __LOGIC_H__

#include "types.h"

void setPlayerMovement(enum e_Direction direction);

Game *getGame();

void decreaseTime();

void initLevel(int levelId);

#endif