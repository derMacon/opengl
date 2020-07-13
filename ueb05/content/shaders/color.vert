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
 * Vertex-Positon.
 */
out vec4 fFragPos;
/**
 * Vertex-Normale.
 */
out vec3 fNormal;

/**
 * Vertex-Binormale.
 */
out vec3 fBinormal;
/**
 * Vertex-Tangente.
 */
out vec3 fTangent;

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
uniform sampler2D HeightMap;


const float normalOffset = 0.1;

float modifySphere(float val, float height){
    float modifier = height * Elevation;
    return val > 0 ? val + modifier : val - modifier;
}

vec4 calcElevatedPosition(vec2 offset)
{
    vec4 elevatedPosition;
    elevatedPosition.x = vPosition.x + offset.x;
    elevatedPosition.y = vPosition.y;
    elevatedPosition.z = vPosition.z + offset.y;
    elevatedPosition.w = vPosition.w;

    return elevatedPosition;
}

/**
 * Berechnet die Normalen an diesem Vertex.
 *
 * @return die approximierte Normale
 */
vec3 calcNormal()
{
    vec3 up = calcElevatedPosition(vec2(0, -normalOffset)).xyz;
    vec3 down = calcElevatedPosition(vec2(0, normalOffset)).xyz;
    vec3 right = calcElevatedPosition(vec2(normalOffset, 0)).xyz;
    vec3 left = calcElevatedPosition(vec2(-normalOffset, 0)).xyz;

    vec3 horizontal = right - left;
    vec3 vertical = down - up;

    fTangent = horizontal;
    fBinormal = vertical;

    vec3 normal = cross(vertical, horizontal);
    return normalize(normal);
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
    fNormal = calcNormal();

    vec4 height = texture(HeightMap, vTexCoord);
    elevatedPosition.x  = vTexCoord.t * M_PI;
    elevatedPosition.y = vPosition.y;
    elevatedPosition.z = vTexCoord.s * M_PI *  2;
    elevatedPosition.w = vPosition.w;

    float angleA = elevatedPosition.x;
    float angleB = elevatedPosition.z;

    // Die Erde ist eine Scheibe!!!! (c) FlatEarthers
    tempPosition.x = sin(angleA) * cos(angleB);
    tempPosition.y = sin(angleA) * sin(angleB);
    tempPosition.z = cos(angleA);
    tempPosition.w = vPosition.w;

    // Heightmap
    tempPosition.x = modifySphere(tempPosition.x, height.x);
    tempPosition.y = modifySphere(tempPosition.y, height.y);
    tempPosition.z = modifySphere(tempPosition.z, height.z);

    /* Die Textur-Koordinate wird untransformiert an den Fragment-
     * Shader weitergereicht. Bei der Rasterization wird dieser Wert
     * jedoch interpoliert. */
    fTexCoord = vTexCoord;
    fFragPos = tempPosition;

    /* Setzen der Vertex-Position im Device-Koordinatensystem.
     * Nachfolgend findet das Clipping und die Rasterization statt. */
    gl_Position = Projection * ModelView * tempPosition;
}
