#version 330 core

/**
 * Interpolierte Textur-Koordinate des Fragments.
 */
in vec2 fTexCoord;

/**
 * Farbe des Fragments, die auf den Framebuffer geschrieben wird.
 */
out vec4 FragColor;

/**
 * Texture-Sampler, um auf die Textur zuzugreifen. Die Einstellungen
 * des Samplers (Interpolation und Randbehandlung) werden beim Erstellen
 * der Textur gesetzt.
 */
uniform sampler2D Texture;

/**
 * Hauptprogramm des Fragment-Shaders.
 * Diese Funktion wird für jedes Fragment ausgeführt!
 */
void main(void)
{
    FragColor = texture(Texture, fTexCoord);
}
