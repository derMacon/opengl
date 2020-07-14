#ifndef variables_h
#define variables_h

#define GRID_LENGTH 50
#define MAX_ELEVATION 0.2
#define MIN_ELEVATION 0
#define TIMER_CALLS_PS 144

#define GRID_LENGTH_X (GLfloat) (2.0)
#define GRID_LENGTH_Z (GLfloat) (M_PI)

#define TILE_WIDTH  (GLfloat) (GRID_LENGTH_Z / (GRID_LENGTH ));
#define TILE_HEIGHT (GLfloat) (GRID_LENGTH_X / (GRID_LENGTH ));

#define OFFSET_X (GLfloat) (GRID_LENGTH_Z / 2)
#define OFFSET_Z (GLfloat) (GRID_LENGTH_X / 2)
#define STB_IMAGE_STATIC
#define STB_IMAGE_IMPLEMENTATION
/**
 * Alle Daten eines Vertexes.
 */
typedef struct {
    float x, y, z; /**< Position */
    float s, t;    /**< Textur-Koordinate */
} Vertex;

typedef enum {
    worldMap,
    heightMap
} TexName;

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

typedef struct {
    GLboolean showFullscreen;
    GLboolean showNormals;
    GLboolean showPhong;
    GLboolean showTextures;
    GLboolean showWireframe;
    GLboolean showBreak;
    enum ColorType color;
} Settings;

#endif
