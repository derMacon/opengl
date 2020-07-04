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
    elevatedPosition.x = vPosition.x;
    elevatedPosition.y = Elevation*vPosition.x*vPosition.x;
    elevatedPosition.z = vPosition.z;
    elevatedPosition.w = vPosition.w;
    
    /* Die Textur-Koordinate wird untransformiert an den Fragment-
     * Shader weitergereicht. Bei der Rasterization wird dieser Wert
     * jedoch interpoliert. */
    fTexCoord = vTexCoord;
    
    /* Setzen der Vertex-Position im Device-Koordinatensystem.
     * Nachfolgend findet das Clipping und die Rasterization statt. */
    gl_Position = Projection * ModelView * elevatedPosition;
}
