#version 330 core

#define ZERO 0
#define ONE  1

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
in vec4 fragmentPosition;

/**
 * Normale des Fragments.
 */
in vec3 fragmentNormal;

uniform float showPhong;
uniform float showSepia;
uniform float showTexture;
uniform float showNormals;
uniform float camPos[3];

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
vec3 lightPosition = vec3(camPos[0], camPos[1], camPos[2]);

// Material
const vec3 materialAmbient = vec3(0.2, 0.2, 0.2);
const vec3 materialDiffuse = vec3(0.85, 0.85, 0.85);
const vec3 materialSpecular = vec3(0.5, 0.5, 0.5);
const float shinyness = 3;

// Lichter
vec3 lightAmbient = vec3(ONE, ONE, ONE);
vec3 lightDiffuse = vec3(ONE, ONE, ONE);
vec3 lightSpecular = vec3(ONE, ONE, ONE);

/**
    Berechnet Phong
    return: Die Intensitaet
*/
vec4 phong()
{
    vec3 normal = normalize(fragmentNormal);
    vec3 light = normalize(lightPosition - fragmentPosition.xyz);
    vec3 reflection = reflect(-light, normal);
    vec3 view = normalize(CameraPos - fragmentPosition.xyz);

    // https://intern.fh-wedel.de/fileadmin/mitarbeiter/ne/CG/Beleuchtung.pdf Seite 16
    vec3 iAmb = materialAmbient * lightAmbient;
    vec3 iDiff = max(dot(normal, light), 0) * materialDiffuse * lightDiffuse;
    vec3 iSpec = pow(max(dot(view, reflection), 0), shinyness) * materialSpecular * lightSpecular;

    return vec4(iAmb + iDiff + iSpec, 1);
}

vec4 setColor(vec4 FragColor){

    // Temp var, damit wir korrekt rechnen koennen
    float r = FragColor.r;
    float g = FragColor.g;
    float b = FragColor.b;

    // 0 : grayscale
    // 1 : sepia
    // 2 : normal

    // R
    FragColor.r = mix(
        r * 1 + 0 * g + 0 * b,                 // Normal
        mix(
            r * 0.393 + 0.769 * g + 0.189 * b, // Sepia
            0.299 * r + 0.587 * g + 0.114 * b, // GrayScale
            step(colorType, ZERO)
        ),
        step(colorType, ONE)
    );

    // G
    FragColor.g = mix(
        r * 0 + 1 * g + 0 * b,                 // Normal
        mix(
            r * 0.343 + 0.786 * g + 0.168 * b, // Sepia
            0.299 * r + 0.587 * g + 0.114 * b, // GrayScale
            step(colorType, ZERO)
        ), step(colorType, ONE)
    );

    // B
    FragColor.b = mix(
        r * 0 + g * 0 + b * 1,                 // Normal
        mix(
            r * 0.272 + g * 0.534 + b * 0.131, // Sepia
            0.299 * r + 0.587 * g + 0.114 * b, // GrayScale
            step(colorType, ZERO)
        ), step(colorType, ONE)
    );

    return FragColor;
}

/**
 * Hauptprogramm des Fragment-Shaders.
 * Diese Funktion wird für jedes Fragment ausgeführt!
 */
void main(void)
{
    vec4 textures;
    vec4 colorChange;

    // Zum ueberpruefen, ob Farbe schwarz ist
    vec3 zero = vec3(ZERO, ZERO, ZERO);

    // Farbwechsel (mix, um schwarz auszuschliessen)
    vec3 color = mix (
        lightPosition,
        vec3 (0.2f, 0.2f, 0.2f),
        step(
            lightPosition, zero
        )
    );

    // mix geht nicht, deshalb muessen wir if nutzen :(
    if (showNormals == 1.0){
        // Normalen anzeigen
        colorChange = fragmentPosition;
    } else {
        // Farbverlauf
        colorChange = vec4(color, 1);
    }

    // Geht leider nicht!
    //    vec4 colorChange = mix(
    //        vec4(color, 1),
    //        fragmentPosition,
    //        step(
    //            showNormals, zero
    //        )
    //    );

    // Textur an- oder ausschalten
    textures = mix (
        texture(Texture, fTexCoord),
        colorChange,
        step(
            showTexture, 0
        )
    );

    // Phong an- oder ausschalten
    FragColor = mix(
        textures * phong(),
        textures,
        (
            step(showPhong, 0)
        )
    );

    FragColor = setColor(FragColor);
}
