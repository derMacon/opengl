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
 * Interpolierte Fragment-Koordinate des Fragments.
 */
in vec4 fFragPos;

/**
 * Normale des Fragments.
 */
in vec3 fNormal;

/**
 * Binormale des Fragments.
 */
in vec3 fBinormal;

/**
 * Tangente des Fragments.
 */
in vec3 fTangent;

uniform float showPhong;
uniform float showSepia;

/**
 * Position der Kamera.
 */
uniform vec3 CameraPos;

/**
 * Texture-Sampler, um auf die Textur zuzugreifen. Die Einstellungen
 * des Samplers (Interpolation und Randbehandlung) werden beim Erstellen
 * der Textur gesetzt.
 */
uniform sampler2D Texture;

uniform float colors[3];

const vec3 materialAmbi = vec3(0.1, 0.1, 0.1);
const vec3 materialDiff = vec3(0.85, 0.85, 0.85);
const vec3 materialSpec = vec3(0.5, 0.5, 0.5);
const float materialShi = 3;

const vec3 lightPosition = vec3(1, -5, 5);

vec3 lightAmbi = vec3(colors[0], colors[1], colors[2]);
vec3 lightDiff = vec3(colors[0], colors[1], colors[2]);
vec3 lightSpec = vec3(colors[0], colors[1], colors[2]);

/**
    Berechnet Phong
    return: Die Intensitaet
*/
vec4 phong()
{
    vec3 normal = normalize(fNormal);
    vec3 lightVec = normalize(lightPosition - fFragPos.xyz);
    vec3 reflectVec = reflect(-lightVec, normal);
    vec3 viewVec = normalize(CameraPos - fFragPos.xyz);

    vec3 iAmbi = materialAmbi * lightAmbi;
    vec3 iDiff = max(dot(normal, lightVec), 0) * materialDiff * lightDiff;
    vec3 iSpec = pow(max(dot(viewVec, reflectVec), 0), materialShi) * materialSpec * lightSpec;

    return vec4(iAmbi + iDiff + iSpec, 1);
}

/**
 * Hauptprogramm des Fragment-Shaders.
 * Diese Funktion wird für jedes Fragment ausgeführt!
 */
void main(void)
{
    if (showPhong == 1.0f){
        FragColor = texture(Texture, fTexCoord) * phong();
    } else {
        FragColor = texture(Texture, fTexCoord);
    }
}
