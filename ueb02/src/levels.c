
#include "types.h"

const int levelTimes[3] = {30, 25, 30};

const int startPosLevels[3][2] = {{1, 1},
                                  {4, 5},
                                  {3, 3}};

const pushyLevel level1 = {
        {P_WALL, P_WALL, P_WALL, P_WALL,            P_WALL,        P_WALL,   P_WALL, P_WALL,            P_WALL},
        {P_WALL, P_FREE, P_WALL, P_FREE,            P_FREE,        P_WALL,   P_FREE, P_TARGET,          P_WALL},
        {P_WALL, P_FREE, P_WALL, P_FREE,            P_DOOR_SWITCH, P_WALL,   P_FREE, P_OBJECT_TRIANGLE, P_WALL},
        {P_WALL, P_BOX,  P_WALL, P_FREE,            P_BOX,         P_DOOR,   P_FREE, P_FREE,            P_WALL},
        {P_WALL, P_FREE, P_FREE, P_FREE,            P_FREE,        P_WALL,   P_FREE, P_FREE,            P_WALL},
        {P_WALL, P_FREE, P_FREE, P_OBJECT_TRIANGLE, P_FREE,        P_WALL,   P_WALL, P_WALL,            P_WALL},
        {P_WALL, P_FREE, P_FREE, P_FREE,            P_WALL,        P_FREE,   P_FREE, P_FREE,            P_WALL},
        {P_WALL, P_BOX,  P_FREE, P_PORTAL,          P_WALL,        P_PORTAL, P_FREE, P_HOUSE,           P_WALL},
        {P_WALL, P_WALL, P_WALL, P_WALL,            P_WALL,        P_WALL,   P_WALL, P_WALL,            P_WALL}};

const pushyLevel level2 = {
        {P_OUTER, P_WALL, P_WALL,        P_WALL,            P_WALL, P_WALL,            P_WALL,   P_WALL,   P_OUTER},
        {P_OUTER, P_WALL, P_DOOR_SWITCH, P_HOUSE,           P_BOX,  P_FREE,            P_FREE,   P_WALL,   P_OUTER},
        {P_OUTER, P_WALL, P_FREE,        P_FREE,            P_BOX,  P_FREE,            P_FREE,   P_WALL,   P_OUTER},
        {P_OUTER, P_WALL, P_FREE,        P_FREE,            P_BOX,  P_FREE,            P_PORTAL, P_WALL,   P_OUTER},
        {P_OUTER, P_WALL, P_WALL,        P_WALL,            P_WALL, P_WALL,            P_WALL,   P_WALL,   P_OUTER},
        {P_OUTER, P_WALL, P_FREE,        P_FREE,            P_FREE, P_FREE,            P_FREE,   P_TARGET, P_OUTER},
        {P_OUTER, P_WALL, P_DOOR,        P_OBJECT_TRIANGLE, P_FREE, P_OBJECT_TRIANGLE, P_FREE,   P_WALL,   P_OUTER},
        {P_OUTER, P_WALL, P_BOX,         P_BOX,             P_FREE, P_FREE,            P_PORTAL, P_WALL,   P_OUTER},
        {P_OUTER, P_WALL, P_WALL,        P_WALL,            P_WALL, P_WALL,            P_WALL,   P_WALL,   P_OUTER}
};

const pushyLevel level3 = {
        {P_WALL, P_WALL,  P_WALL, P_WALL, P_WALL, P_WALL, P_WALL,   P_WALL,   P_WALL},
        {P_WALL, P_START, P_FREE, P_FREE, P_FREE, P_BOX,  P_DOOR,   P_BOX,    P_WALL},
        {P_WALL, P_BOX,   P_FREE, P_FREE, P_FREE, P_FREE, P_BOX,    P_FREE,   P_WALL},
        {P_WALL, P_FREE,  P_BOX,  P_FREE, P_FREE, P_BOX,  P_FREE,   P_FREE,   P_WALL},
        {P_WALL, P_BOX,   P_FREE, P_BOX,  P_FREE, P_BOX,  P_PORTAL, P_PORTAL, P_WALL},
        {P_WALL, P_FREE,  P_BOX,  P_FREE, P_BOX,  P_BOX,  P_FREE,   P_FREE,   P_WALL},
        {P_WALL, P_BOX,   P_FREE, P_BOX,  P_FREE, P_BOX,  P_FREE,   P_PORTAL, P_WALL},
        {P_WALL, P_FREE,  P_BOX,  P_FREE, P_BOX,  P_BOX,  P_PORTAL, P_TARGET, P_WALL},
        {P_WALL, P_WALL,  P_WALL, P_WALL, P_WALL, P_WALL, P_WALL,   P_WALL,   P_WALL}
};