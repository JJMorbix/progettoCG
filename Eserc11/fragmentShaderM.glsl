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
uniform vec3 ViewPos;
uniform vec3 lightColor;

//Struttura per la gestione di un punto luce
struct PointLight {
	vec3 position;
	vec3 color;
	float power;
};

//definizione di una variabile uniform che ha la struttura PointLight
#define LIGHT_NUM 2
uniform PointLight lights[LIGHT_NUM];

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};
uniform Material material;

uniform int sceltaShader;

float strenght = 1;

void main()
{
    if (sceltaShader == 6 || sceltaShader == 5) {
        FragColor = texture(id_tex1, frag_coord_st);
        return;
    }

    if (sceltaShader == 3) { // Phong
        vec3 N = fragNormal;
        vec3 V = normalize(ViewPos - fragPosition);
        
        vec3 lightingResult = vec3(0.0);

        // Ambientale
        vec3 ambient = strenght * material.ambient;

        PointLight light;
        for (int i=0; i < LIGHT_NUM; ++i) {
            light = lights[i];

            vec3 L = normalize(light.position - fragPosition);
            vec3 R = reflect(-L, N);

            float lightDistance = length(light.position - fragPosition);
            // float fAtt = 1 / (lightDistance * lightDistance);

            // Diffuse
            float cosTheta = max(dot(L, N), 0.0);
            vec3 diffuse = light.power * cosTheta * light.color * material.diffuse;

            // Speculare
            float cosAlpha = pow(max(dot(V, R), 0.0), material.shininess);
            vec3 specular = light.power * cosAlpha * light.color * material.specular;

            lightingResult += ambient + diffuse + specular;
        }

        FragColor = mix(vec4(lightingResult, 1.0), texture(id_tex1, frag_coord_st), 0.5);
        return;
    }

    if (sceltaShader == 2) { // BlinnPhong
        vec3 N = normalize(fragNormal);
        vec3 V = normalize(ViewPos - fragPosition);
        
        vec3 lightingResult = vec3(0.0);

        // Ambientale
        vec3 ambient = strenght * material.ambient;

        PointLight light;
        for (int i=0; i < LIGHT_NUM; ++i) {
            light = lights[i];

            vec3 L = normalize(light.position - fragPosition);
            vec3 R = reflect(-L, N);

            float lightDistance = length(light.position - fragPosition);
            // float fAtt = 1 / (lightDistance * lightDistance);

            // Diffuse
            float cosTheta = max(dot(L, N), 0.0);
            vec3 diffuse = light.power * cosTheta * light.color * material.diffuse;

            // Speculare
            vec3 H = normalize(L + V);
            float cosAlpha = pow(max(dot(H, N), 0.0), material.shininess);
            vec3 specular = light.power * cosAlpha * light.color * material.specular;

            lightingResult += ambient + diffuse + specular;
        }

        FragColor = mix(vec4(lightingResult, 1.0), texture(id_tex1, frag_coord_st), 0.5);
        return;
    }

    if (sceltaShader == 1  || sceltaShader == 3) {
        FragColor = mix(ourColor, texture(id_tex1, frag_coord_st), 0.5);
        return;
    }
    
    FragColor = ourColor;
}
