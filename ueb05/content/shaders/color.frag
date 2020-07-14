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

uniform float colorType;
const vec3 lightPosition = vec3(1, -5, 5);

const vec3 ambient_material = vec3(0.2, 0.2, 0.2);
const vec3 diffuse_material = vec3(0.85, 0.85, 0.85);
const vec3 specular_material = vec3(0.5, 0.5, 0.5);
const float shinyness = 3;

float val = 1;
vec3 ambient_light = vec3(val, val, val);
vec3 diffuse_light = vec3(val, val, val);
vec3 spectacular_light = vec3(val, val, val);

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

    vec3 iAmbi = ambient_material * ambient_light;
    vec3 iDiff = max(dot(normal, lightVec), 0) * diffuse_material * diffuse_light;
    vec3 iSpec = pow(max(dot(viewVec, reflectVec), 0), shinyness) * specular_material * spectacular_light;

    return vec4(iAmbi + iDiff + iSpec, 1);
}

/**
 * Hauptprogramm des Fragment-Shaders.
 * Diese Funktion wird für jedes Fragment ausgeführt!
 */
void main(void)
{
    FragColor = mix(texture(Texture, fTexCoord) * phong(), texture(Texture, fTexCoord), (step(showPhong, 0)));

    //    0 : grayscale
    //    1 : sepia
    //    2: normal

    float outer = 1;
    float inner = 0;

    float r = FragColor.r;
    float g = FragColor.g;
    float b = FragColor.b;

    FragColor.r = mix(
        r * 1 + 0 * g + 0 * b,                  // NM
        mix(
            r * 0.393 + 0.769 * g + 0.189 * b, // Sepia
            0.299 * r + 0.587 * g + 0.114 * b, // GS
            (step(colorType, inner))
        ),
        (step(colorType, outer))
    );

    FragColor.g = mix(
    r * 0 + 1 * g + 0 * b,                  // NM
    mix(
        r * 0.343 + 0.786 * g + 0.168 * b, // Sepia
        0.299 * r + 0.587 * g + 0.114 * b, // GS
        (step(colorType, inner))
    ), (step(colorType, outer)));

    FragColor.b = mix(
    r * 0 + g * 0 + b * 1,                  // NM
    mix(
        r * 0.272 + g * 0.534 + b * 0.131, // Sepia
        0.299 * r + 0.587 * g + 0.114 * b, // GS
        (step(colorType, inner))
    ), step(colorType, outer));


    //        Grayscale
    //        FragColor.r = 0.299 * r + 0.587 * g + 0.114 * b;
    //        FragColor.g = FragColor.r;
    //        FragColor.b = FragColor.r;

    // Normal
    //    FragColor.r = r * 1 + 0 * g + 0 * b;
    //    FragColor.g = r * 0 + 1 * g + 0 * b;
    //    FragColor.b = r * 0 + g * 0 + b * 1;

    //        Sepia
    //        FragColor.r = r * 0.393 + 0.769 * g + 0.189 * b;
    //        FragColor.g = r * 0.343 + 0.786 * g + 0.168 * b;
    //        FragColor.b = r * 0.272 + g * 0.534 + b * 0.131;

}
