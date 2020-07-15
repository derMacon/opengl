#ifndef variables_h
#define variables_h

/** Defines */
/* ==== Allgemeines */
#define TIMER_CALLS_PS 144
#define MAX_ELEVATION 0.2
#define MIN_ELEVATION 0
#define STB_IMAGE_STATIC
#define STB_IMAGE_IMPLEMENTATION

/* ==== Kamera */
#define CAMERA_SPEED 3000;

/* ==== Grid */
#define GRID_LENGTH 50
#define GRID_LENGTH_X (GLfloat) (2.0)
#define GRID_LENGTH_Z (GLfloat) (M_PI)
#define TILE_WIDTH  (GLfloat) (GRID_LENGTH_Z / (GRID_LENGTH ));
#define TILE_HEIGHT (GLfloat) (GRID_LENGTH_X / (GRID_LENGTH ));
#define OFFSET_X (GLfloat) (GRID_LENGTH_Z / 2)
#define OFFSET_Z (GLfloat) (GRID_LENGTH_X / 2)

/** ====  Enums */
/* Diese Texturen gibt es */
typedef enum {
    worldMap,
    heightMap
} TexName;

/* Diese Farb-Typen gibt es */
enum ColorType {
    COLOR_GRAYSCALE,
    COLOR_SEPIA,
    COLOR_NORMAL,
};

/** Wird verwendet, um u.g. Einstellungen zu togglen */
enum e_ToggleTypes {
    FULLSCREEN,
    NORMALS,
    PHONG,
    TEXTURES,
    WIREFRAME,
    BREAK
};

/** ====  Structs */
/* Alle Daten eines Vertexes. */
typedef struct {
    float x, y, z; /**< Position */
    float s, t;    /**< Textur-Koordinate */
} Vertex;

/* Kermaposition */
typedef struct {
    float x, y, z;
} CamPos;

/* Settings */
typedef struct {
    GLboolean showFullscreen;
    GLboolean showNormals;
    GLboolean showPhong;
    GLboolean showTextures;
    GLboolean showWireframe;
    GLboolean showBreak;
    enum ColorType color;
    CamPos camPos;
} Settings;

#endif
