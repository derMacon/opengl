#version 330 core

/** 
 * Position des Vertex. 
 * Wird von der Host-Anwendung über den Attribut-Pointer 0 
 * bereitgestellt. 
 */
layout (location = 0) in vec4 vPosition;
/**
 * Textur-Koordinate des Vertex. 
 * Wird von der Host-Anwendung über den Attribut-Pointer 1
 * bereitgestellt. 
 */
layout (location = 1) in vec2 vTexCoord;

/** 
 * Ausgabe des Vertex-Shaders. 
 * Die Textur-Koordinate wird interpoliert vom Fragment-Shader 
 * entgegengenommen.
 */
out vec2 fTexCoord;

/** 
 * Projektions-Matrix.
 * Wird dem Shader mit einem Aufruf von glUniformMatrix4fv übergeben.
 */
uniform mat4 Projection;
/**
 * View-Matrix. 
 * Wird dem Shader mit einem Aufruf von glUniformMatrix4fv übergeben.
 */
uniform mat4 ModelView;

/**
 * Anhebung der Vertizes.
 * Wird dem Shader mit einem Aufruf von glUniform1f übergeben.
 */
uniform float Elevation;

#define M_PI (3.14159265358979323846)

uniform sampler2D Texture;

float modifySphere(float val, float height){
    float modifier = height * Elevation;

    return val > 0 ? val + modifier : val - modifier;
}

/**
 * Hauptprogramm des Vertex-Shaders.
 * Diese Funktion wird für jeden Vertex ausgeführt!
 */
void main(void)
{
    /* Neue Vertex-Position berechnen auf Grundlage der übergebenen
     * Anhebung. vPosition ist die Vertex-Position, die von der Host-
     * Anwendung im Buffer gespeichert wurde. Die w-Komponente wird
     * von der OpenGL-Implementierung automatisch auf 1 gesetzt. */
    vec4 elevatedPosition;
    vec4 sphere;

    vec4 height = texture(Texture, vTexCoord);
    elevatedPosition.x  = vTexCoord.t * M_PI;
    elevatedPosition.y = vPosition.y;
    elevatedPosition.z = vTexCoord.s * M_PI *  2;
    elevatedPosition.w = vPosition.w;

    float angleA = elevatedPosition.x;
    float angleB = elevatedPosition.z;

    // Die Erde ist eine Scheibe!!!! (c) FlatEarthers
    sphere.x = sin(angleA) * cos(angleB);
    sphere.y = sin(angleA) * sin(angleB);
    sphere.z = cos(angleA);
    sphere.w = vPosition.w;

    // Heightmap
    sphere.x = modifySphere(sphere.x, height.x);
    sphere.y = modifySphere(sphere.y, height.y);
    sphere.z = modifySphere(sphere.z, height.z);

    /* Die Textur-Koordinate wird untransformiert an den Fragment-
     * Shader weitergereicht. Bei der Rasterization wird dieser Wert
     * jedoch interpoliert. */
    fTexCoord = vTexCoord;

    /* Setzen der Vertex-Position im Device-Koordinatensystem.
     * Nachfolgend findet das Clipping und die Rasterization statt. */
    gl_Position = Projection * ModelView * sphere;
}
