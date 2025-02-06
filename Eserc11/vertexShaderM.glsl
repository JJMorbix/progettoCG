#version 330 core
layout (location = 0) in vec3 aPos;   // the position variable has attribute position 0
layout (location = 1) in vec4 aColor; // the color variable has attribute position 1
layout(location = 2) in vec3 vertexNormal;  //Attributo normale 2
layout (location =3 ) in vec2 coord_st; // Attributo texture
  
out vec4 ourColor; // output a color to the fragment shader
uniform mat4 Projection;  //vARIABILE DI TIPO uniform, rimane la stessa per ogni vertice della primitiva e viene passata dall'esterno
uniform mat4 Model;
uniform mat4 View;
uniform vec3 ViewPos;
uniform float time;
//Struttura per la gestione di un punto luce
struct PointLight {
	vec3 position;
	vec3 color;
	float power;
};
//definizione di una variabile uniform che ha la struttura PointLight
uniform PointLight light;

//Struttura per la gestione di un materiale

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

uniform int sceltaShader;

//Variabile uniorme
uniform Material material;

float strenght = 0.1;
out vec2 frag_coord_st;

void main()
{
//Trasformazione dei vertici dalle coordinate nel sistema di riferimento dell'oggetto (apos), al sistema di riferimento del mondo (premoltiplicazione 
// per Model) e successivamente proiettate nel cubo di centro l'origine e lato lungo 2, con x,y,z che variano tra -1 ed 1- (premoltiplicazione 
//per la matrice Projection)
    ourColor = aColor; // set ourColor to the input color we got from the vertex data

    if (sceltaShader==0)
        gl_Position = Projection*View*Model*vec4(aPos, 1.0);
    
    if (sceltaShader == 1 || sceltaShader == 4 ) //Interpolative Shading Phong
    {
        gl_Position = Projection * View * Model * vec4(aPos, 1.0);

        //Trasformare le coordinate del vertice da elaborare (aPos) in coordinate di vista
        vec4 eyePosition = View * Model * vec4(aPos, 1.0);

        //Trasformiamo la posizione della luce nelle coordinate di vista

        vec4 eyeLightPos = View * vec4(light.position, 1.0);

        //trasformare le normali nel vertice in esame nel sistema di coordinate di vista

        vec3 N = normalize(transpose(inverse(mat3(View * Model))) * vertexNormal);
        //Calcoliamo la direzione della luce L, la direzione riflessione R e di vista

        vec3 V = normalize(ViewPos - eyePosition.xyz);
        vec3 L = normalize((eyeLightPos - eyePosition).xyz);
        vec3 R = reflect(-L, N);  //Costruisce la direzione riflessa di L rispesso alla normale

        //ambientale
        vec3 ambient = strenght * light.power * material.ambient;

        //diffuse
        float coseno_angolo_theta = max(dot(L, N), 0);

        vec3 diffuse = light.power * light.color * coseno_angolo_theta * material.diffuse;

        //speculare
        float coseno_angolo_alfa = pow(max(dot(V, R), 0), material.shininess);

        vec3 specular = light.power * light.color * coseno_angolo_alfa * material.specular;

        ourColor = vec4(ambient + diffuse + specular, 1.0);

         frag_coord_st=coord_st;
    }

    if (sceltaShader == 2) //Interpolative Shading Phong
    {
        gl_Position = Projection * View * Model * vec4(aPos, 1.0);

        //Trasformare le coordinate del vertice da elaborare (aPos) in coordinate di vista
        vec4 eyePosition = View * Model * vec4(aPos, 1.0);

        //Trasformiamo la posizione della luce nelle coordinate di vista

        vec4 eyeLightPos = View * vec4(light.position, 1.0);

        //trasformare le normali nel vertice in esame nel sistema di coordinate di vista

        vec3 N = normalize(transpose(inverse(mat3(View * Model))) * vertexNormal);
        //Calcoliamo la direzione della luce L, la direzione riflessione R e di vista

        vec3 V = normalize(ViewPos - eyePosition.xyz);
        vec3 L = normalize((eyeLightPos - eyePosition).xyz);
        vec3 H = normalize(L+V);  //Costruisce la direzione riflessa di L rispesso alla normale

        //ambientale
        vec3 ambient = strenght * light.power * material.ambient;

        //diffuse
        float coseno_angolo_theta = max(dot(L, N), 0);

        vec3 diffuse = light.power * light.color * coseno_angolo_theta * material.diffuse;

        //speculare
        float coseno_angolo_beta = pow(max(dot(H, N), 0), material.shininess);

        vec3 specular = light.power * light.color * coseno_angolo_beta * material.specular;

        ourColor = vec4(ambient + diffuse + specular, 1.0);


         frag_coord_st=coord_st;

    }

    if (sceltaShader == 5) // Wave
    {
        frag_coord_st = coord_st;
        vec4 v = vec4(aPos, 1);
        v.y = (sin(80.0 * v.x + time) * cos(8.5 * v.y + time) * 0.1);

        //Modello di illuminazione di  Phong con shading interpolativo

        gl_Position = Projection * View * Model * v;
        frag_coord_st = coord_st;
    }

    if (sceltaShader == 6) // Bandiera
    {
        vec4 v = vec4(aPos, 1);
        float offset = 0.3 * sin(0.5 * time + 5.0 * v.x);
        v.z = v.z + offset;
        gl_Position = Projection * View * Model * v;

        frag_coord_st = coord_st;
    }
    

}  

