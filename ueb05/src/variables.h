#ifndef variables_h
#define variables_h

#define GRID_LENGTH 50
#define MAX_ELEVATION 0.2
#define MIN_ELEVATION 0

/** Wird verwendet, um u.g. Einstellungen zu togglen */
enum e_ToggleTypes {
    FULLSCREEN,
    NORMALS,
    WORLDLIGHT,
    TEXTURES,
    WIREFRAME,
    BREAK
};

typedef struct {
    GLboolean showFullscreen;
    GLboolean showNormals;
    GLboolean showWorldlight;
    GLboolean showTextures;
    GLboolean showWireframe;
    GLboolean showBreak;
} Settings;

#endif
