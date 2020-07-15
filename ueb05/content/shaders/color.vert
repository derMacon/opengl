#version 330 core

#define M_PI (3.14159265358979323846)

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
 * Vertex-Positon.
 */
out vec4 fragmentPosition;
/**
 * Vertex-Normale.
 */
out vec3 fragmentNormal;

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

uniform sampler2D Texture;
uniform sampler2D HeightMap;

/*
* Hilft beim erhoehen oder verringern der Heightmap
* Eingebaut, um Codeverdopplung zu vermeiden
*/
float modifySphere(float val, float height){
    float modifier = height * Elevation;
    return val > 0
        ? val + modifier
        : val - modifier;
}

/*
* Errechnet die Elevated Position
*/
vec4 calcNewPos (vec2 offset)
{
    vec4 newPos;
    newPos.x = vPosition.x + offset.x;
    newPos.y = vPosition.y;
    newPos.z = vPosition.z + offset.y;
    newPos.w = vPosition.w;

    return newPos;
}

/**
 * Berechnet die Normalen
 */
vec3 calcNormal()
{
    const float offset = 0.1;
    vec3 left = calcNewPos(vec2(-offset, 0)).xyz;
    vec3 right = calcNewPos(vec2(offset, 0)).xyz;
    vec3 top = calcNewPos(vec2(0, -offset)).xyz;
    vec3 bottom = calcNewPos(vec2(0, offset)).xyz;
    return normalize(cross(bottom - top, right - left));
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
    vec4 tempPosition;
    fragmentNormal = calcNormal();

    vec4 height = texture(HeightMap, vTexCoord);
    elevatedPosition.x  = vTexCoord.t * M_PI;
    elevatedPosition.y = vPosition.y;
    elevatedPosition.z = vTexCoord.s * M_PI *  2;
    elevatedPosition.w = vPosition.w;

    // Scheibe zum Globe wandeln
    float angleA = elevatedPosition.x;
    float angleB = elevatedPosition.z;
    tempPosition.x = sin(angleA) * cos(angleB);
    tempPosition.y = sin(angleA) * sin(angleB);
    tempPosition.z = cos(angleA);
    tempPosition.w = vPosition.w;

    // Heightmap (und Elevation)
    tempPosition.x = modifySphere(tempPosition.x, height.x);
    tempPosition.y = modifySphere(tempPosition.y, height.y);
    tempPosition.z = modifySphere(tempPosition.z, height.z);

    /* Die Textur-Koordinate wird untransformiert an den Fragment-
     * Shader weitergereicht. Bei der Rasterization wird dieser Wert
     * jedoch interpoliert. */
    fTexCoord = vTexCoord;
    fragmentPosition = tempPosition;

    /* Setzen der Vertex-Position im Device-Koordinatensystem.
     * Nachfolgend findet das Clipping und die Rasterization statt. */
    gl_Position = Projection * ModelView * tempPosition;
}
