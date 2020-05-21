#include "types.h"

const Level levels[3] = {
        // Level 1
        {
                .field = {
                        {P_WALL, P_WALL, P_WALL, P_WALL,            P_WALL,        P_WALL,   P_WALL, P_WALL,            P_WALL},
                        {P_WALL, P_FREE, P_WALL, P_FREE,            P_FREE,        P_WALL,   P_FREE, P_TARGET,          P_WALL},
                        {P_WALL, P_FREE, P_WALL, P_FREE,            P_DOOR_SWITCH, P_WALL,   P_FREE, P_OBJECT_TRIANGLE, P_WALL},
                        {P_WALL, P_BOX,  P_WALL, P_FREE,            P_BOX,         P_DOOR,   P_FREE, P_FREE,            P_WALL},
                        {P_WALL, P_FREE, P_FREE, P_FREE,            P_FREE,        P_WALL,   P_FREE, P_FREE,            P_WALL},
                        {P_WALL, P_FREE, P_FREE, P_OBJECT_TRIANGLE, P_FREE,        P_WALL,   P_WALL, P_WALL,            P_WALL},
                        {P_WALL, P_FREE, P_FREE, P_FREE,            P_WALL,        P_FREE,   P_FREE, P_FREE,            P_WALL},
                        {P_WALL, P_BOX,  P_FREE, P_PORTAL,          P_WALL,        P_PORTAL, P_FREE, P_HOUSE,           P_WALL},
                        {P_WALL, P_WALL, P_WALL, P_WALL,            P_WALL,        P_WALL,   P_WALL, P_WALL,            P_WALL}
                },
                .startPos = {1, 1},
                .time = 25,
                .doors = {{5, 3}},
                .doorSwitch = {
                        {4,  2},
                        {-1, -1},
                        {-1, -1}
                }
        },
        // Level 2
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
                .startPos = {4, 5},
                .time = 30,
                .doors = {
                        {2, 6}
                },
                .doorSwitch = {
                        {2, 1}
                }
        },
        // Level 3
        {
                .field = {
                        {P_WALL,  P_WALL,  P_WALL, P_WALL,            P_WALL, P_WALL,        P_WALL,            P_WALL,        P_WALL},
                        {P_WALL,  P_FREE,  P_FREE, P_FREE,            P_BOX,  P_DOOR_SWITCH, P_DOOR_SWITCH,     P_DOOR_SWITCH, P_WALL},
                        {P_WALL,  P_FREE,  P_FREE, P_OBJECT_TRIANGLE, P_BOX,  P_FREE,        P_OBJECT_TRIANGLE, P_FREE,        P_WALL},
                        {P_WALL,  P_FREE,  P_FREE, P_FREE,            P_BOX,  P_FREE,        P_OBJECT_TRIANGLE, P_FREE,        P_WALL},
                        {P_WALL,  P_FREE,  P_FREE, P_OBJECT_TRIANGLE, P_BOX,  P_FREE,        P_OBJECT_TRIANGLE, P_FREE,        P_WALL},
                        {P_WALL,  P_FREE,  P_FREE, P_FREE,            P_BOX,  P_FREE,        P_FREE,            P_TARGET,      P_WALL},
                        {P_WALL,  P_WALL,  P_FREE, P_FREE,            P_WALL, P_WALL,        P_WALL,            P_WALL,        P_WALL},
                        {P_OUTER, P_WALL,  P_WALL, P_FREE,            P_DOOR, P_DOOR,        P_DOOR,            P_HOUSE,       P_WALL},
                        {P_OUTER, P_OUTER, P_WALL, P_WALL,            P_WALL, P_WALL,        P_WALL,            P_WALL,        P_WALL}
                },
                .startPos = {1, 1},
                .time = 120,
                .doors = {
                        {4, 7},
                        {5, 7},
                        {6, 7}
                },
                .doorSwitch = {
                        {5, 1},
                        {6, 1},
                        {7, 1}
                }

        }
};