#version 330 core
in vec4 ourColor;
out vec4 FragColor;

// Dichiarazione campionatori uniformi
uniform sampler2D id_tex1;
uniform sampler2D id_tex2;

in vec2 frag_coord_st; // Coordinate texture del frammento corrente
in vec3 fragNormal;    // Normale interpolata dal vertex shader
in vec3 fragPosition;  // Posizione interpolata del frammento nello spazio della vista

// Variabili uniformi per illuminazione
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 lightColor;

uniform int sceltaShader;

void main()
{
    if (sceltaShader == 6 || sceltaShader == 5) {
        FragColor = texture(id_tex1, frag_coord_st);
        return;
    }

    if (sceltaShader == 1 || sceltaShader == 2 || sceltaShader == 3) {
        FragColor = mix(ourColor, texture(id_tex1, frag_coord_st), 0.5);
        return;
    }
    
    FragColor = ourColor;
}
