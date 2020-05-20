#include "types.h"

const Level levels[3] = {{
                                 .field = {
                                         {P_WALL, P_WALL, P_WALL, P_WALL,            P_WALL,        P_WALL,   P_WALL, P_WALL,            P_WALL},
                                         {P_WALL, P_FREE, P_WALL, P_FREE,            P_FREE,        P_WALL,   P_FREE, P_TARGET,          P_WALL},
                                         {P_WALL, P_FREE, P_WALL, P_FREE,            P_DOOR_SWITCH, P_WALL,   P_FREE, P_OBJECT_TRIANGLE, P_WALL},
                                         {P_WALL, P_BOX,  P_WALL, P_FREE,            P_BOX,         P_DOOR,   P_FREE, P_FREE,            P_WALL},
                                         {P_WALL, P_FREE, P_FREE, P_FREE,            P_FREE,        P_WALL,   P_FREE, P_FREE,            P_WALL},
                                         {P_WALL, P_FREE, P_FREE, P_OBJECT_TRIANGLE, P_FREE,        P_WALL,   P_WALL, P_WALL,            P_WALL},
                                         {P_WALL, P_FREE, P_FREE, P_FREE,            P_WALL,        P_FREE,   P_FREE, P_FREE,            P_WALL},
                                         {P_WALL, P_BOX,  P_FREE, P_PORTAL,          P_WALL,        P_PORTAL, P_FREE, P_HOUSE,           P_WALL},
                                         {P_WALL, P_WALL, P_WALL, P_WALL,            P_WALL,        P_WALL,   P_WALL, P_WALL,            P_WALL}},
                                 .startPos = {1, 1},
                                 .time = 25

                         },
                         {
                                 .field = {
                                         {P_WALL, P_WALL,  P_WALL, P_WALL, P_WALL, P_WALL, P_WALL,   P_WALL,   P_WALL},
                                         {P_WALL, P_START, P_FREE, P_FREE, P_FREE, P_BOX,  P_DOOR,   P_BOX,    P_WALL},
                                         {P_WALL, P_BOX,   P_FREE, P_FREE, P_FREE, P_FREE, P_BOX,    P_FREE,   P_WALL},
                                         {P_WALL, P_FREE,  P_BOX,  P_FREE, P_FREE, P_BOX,  P_FREE,   P_FREE,   P_WALL},
                                         {P_WALL, P_BOX,   P_FREE, P_BOX,  P_FREE, P_BOX,  P_PORTAL, P_PORTAL, P_WALL},
                                         {P_WALL, P_FREE,  P_BOX,  P_FREE, P_BOX,  P_BOX,  P_FREE,   P_FREE,   P_WALL},
                                         {P_WALL, P_BOX,   P_FREE, P_BOX,  P_FREE, P_BOX,  P_FREE,   P_PORTAL, P_WALL},
                                         {P_WALL, P_FREE,  P_BOX,  P_FREE, P_BOX,  P_BOX,  P_PORTAL, P_TARGET, P_WALL},
                                         {P_WALL, P_WALL,  P_WALL, P_WALL, P_WALL, P_WALL, P_WALL,   P_WALL,   P_WALL}},
                                 .startPos = {4, 5},
                                 .time = 27

                         },
                         {
                                 .field = {
                                         {P_OUTER, P_WALL, P_WALL,        P_WALL,            P_WALL, P_WALL,            P_WALL,   P_WALL,   P_OUTER},
                                         {P_OUTER, P_WALL, P_DOOR_SWITCH, P_HOUSE,           P_BOX,  P_FREE,            P_FREE,   P_WALL,   P_OUTER},
                                         {P_OUTER, P_WALL, P_FREE,        P_FREE,            P_BOX,  P_FREE,            P_FREE,   P_WALL,   P_OUTER},
                                         {P_OUTER, P_WALL, P_FREE,        P_FREE,            P_BOX,  P_FREE,            P_PORTAL, P_WALL,   P_OUTER},
                                         {P_OUTER, P_WALL, P_WALL,        P_WALL,            P_WALL, P_WALL,            P_WALL,   P_WALL,   P_OUTER},
                                         {P_OUTER, P_WALL, P_FREE,        P_FREE,            P_FREE, P_FREE,            P_FREE,   P_TARGET, P_OUTER},
                                         {P_OUTER, P_WALL, P_DOOR,        P_OBJECT_TRIANGLE, P_FREE, P_OBJECT_TRIANGLE, P_FREE,   P_WALL,   P_OUTER},
                                         {P_OUTER, P_WALL, P_BOX,         P_BOX,             P_FREE, P_FREE,            P_PORTAL, P_WALL,   P_OUTER},
                                         {P_OUTER, P_WALL, P_WALL,        P_WALL,            P_WALL, P_WALL,            P_WALL,   P_WALL,   P_OUTER}
                                 },
                                 .startPos = {3, 3},
                                 .time = 30
                         }
};