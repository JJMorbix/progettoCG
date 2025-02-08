#pragma once
#include "lib.h"
#include "ShaderMaker.h"
#include "strutture.h"
#include "Materiali_Base.h"
#include "geometria.h"
#include "geometria_obj.h"
#include "utilities.h"
#include "gestioneTexture.h"


extern unsigned int programId, programId1, programIdr;

extern ViewSetup SetupTelecamera;
extern PerspectiveSetup SetupProspettiva;
extern int width, height;
extern string SkyboxDir;
//extern int cubemapTexture;
extern vector<Mesh> Scena;
extern vector<Object> ScenaObj;

extern vector<point_light> lights;
extern vector<MaterialObj> materials;
extern vector<Shader> shaders;
extern GLFWwindow* window;
extern Mesh sky;


int INIT_GLAD_GLFW()
{
    /* Initialize the library */
    if (!glfwInit())   //se glfwInit() dà valore falso, return -1
        return -1;

    //Imposta le proprietà del contesto e del profile
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    //Abilita il double buffering

    glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(height, width, "Hello World", NULL, NULL);
    if (!window)
    {
        std::cout << "Failed to create the window !" << std::endl;
        glfwTerminate(); //Libera le risorse allcoata da glfwInit
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window); //crea il context corrente e lo associa a window. In opengl un rendering context è una macchina astati che memorizza tutte le informazioni necessarie e le risorse per il rendering grafico


    //verifica se la libreria GLAD è riuscita a caricare correttamente tutti i puntatori 
    // alle funzioni OpenGL necessarie.

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failde to load opengl function pointers !" << std::endl;
        glfwTerminate();
        return -1;
    }
}
 
void INIT_SHADER(void)
{
    GLenum ErrorCheckValue = glGetError();

    char* vertexShader = (char*)"vertexshaderM.glsl";
    char* fragmentShader = (char*)"fragmentshaderM.glsl";
    // La funzione successiva crea un programma shader completo a partire da due shader individuali: 
    // uno per la gestione dei vertici e uno per la gestione dei pixel. 
    // Il programma shader risultante viene identificato da un numero univoco (il programId) che verr  utilizzato in seguito per associarlo ad un oggetto grafico e per renderizzarlo.
     //All'interno della funzione ShaderMaker::createProgram
        //Compilazione degli shader : La funzione compila i due shader individuali, verificando che non ci siano errori di sintassi.
        //Linkaggio : Una volta compilati, i due shader vengono collegati insieme per formare un programma shader completo.
        // Creazione dell'identificativo: Viene generato un identificativo univoco per il programma shader e viene restituito alla funzione chiamante.

    programId = ShaderMaker::createProgram(vertexShader, fragmentShader);
    glUseProgram(programId);

    //CCrea un program shader per la renderizzazione della cuemap
    vertexShader = (char*)"vertexShader_cubemap.glsl";
     fragmentShader = (char*)"fragmentShader_cubemap.glsl";
     programId1 = ShaderMaker::createProgram(vertexShader, fragmentShader);

     vertexShader = (char*)"vertexShader_riflessione.glsl";
     fragmentShader = (char*)"fragmentShader_riflessione.glsl";
     programIdr = ShaderMaker::createProgram(vertexShader, fragmentShader);

}

void INIT_VAO(Mesh* mesh)
{

    glGenVertexArrays(1, &mesh->VAO);
    glBindVertexArray(mesh->VAO);
    //Genero , rendo attivo, riempio il VBO della geometria dei vertici
    glGenBuffers(1, &mesh->VBO_vertices);
    glBindBuffer(GL_ARRAY_BUFFER, mesh->VBO_vertices);
    glBufferData(GL_ARRAY_BUFFER, mesh->vertices.size() * sizeof(vec3), mesh->vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0);

    //Genero , rendo attivo, riempio il VBO dei colori
    glGenBuffers(1, &mesh->VBO_colors);
    glBindBuffer(GL_ARRAY_BUFFER, mesh->VBO_colors);
    glBufferData(GL_ARRAY_BUFFER, mesh->colors.size() * sizeof(vec4), mesh->colors.data(), GL_STATIC_DRAW);
    //Adesso carico il VBO dei colori nel layer 2
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(1);

	//Genero , rendo attivo, riempio il VBO delle normali
	glGenBuffers(1, &mesh->VBO_normals);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->VBO_normals);
	glBufferData(GL_ARRAY_BUFFER, mesh->normals.size() * sizeof(vec3), mesh->normals.data(), GL_STATIC_DRAW);
	//Adesso carico il VBO delle NORMALI nel layer 2
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(2);

    //Genero , rendo attivo, riempio il VBO delle texture
    glGenBuffers(1, &mesh->VBO_coord_texture);
    glBindBuffer(GL_ARRAY_BUFFER, mesh->VBO_coord_texture);
    glBufferData(GL_ARRAY_BUFFER, mesh->texCoords.size() * sizeof(vec2), mesh->texCoords.data(), GL_STATIC_DRAW);
    //Adesso carico il VBO delle texture nel layer 3
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(3);

    //EBO di tipo indici
    glGenBuffers(1, &mesh->EBO_indices);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->EBO_indices);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->indices.size() * sizeof(GLuint), mesh->indices.data(), GL_STATIC_DRAW);


}

void INIT_CAMERA_PROJECTION(void)
{
    //Imposto la telecamera

    SetupTelecamera.position = glm::vec3(0.0, 0.5, 30.0 );
    SetupTelecamera.target = glm::vec3(0.0, 0.0, 0.0 );
    SetupTelecamera.direction = SetupTelecamera.target - SetupTelecamera.position;
    SetupTelecamera.upVector = glm::vec3(0.0, 1.0, 0.0 );


    //Imposto la proiezione prospettica

    SetupProspettiva.aspect = (GLfloat)width / (GLfloat)height;
    SetupProspettiva.fovY = 45.0f;
    SetupProspettiva.far_plane = 2000.0f;
    SetupProspettiva.near_plane = 0.1f;
}

void INIT_Illuminazione()
{
    //Setup della luce
    point_light light1;
    light1.position = { 50.0, 2.0, 10.0 };
    light1.color = { 1.0,1.0,1.0 };
    light1.power = 2.f;

    point_light light2;
    light2.position = { -50.0, 2.0, 10.0 };
    light2.color = { 1.0,1.0,1.0 };
    light2.power = 3.f;

    lights.push_back(light1);
    lights.push_back(light2);

    //Setup dei materiali
    // Materials setup
    materials.resize(8);
    materials[MaterialType::RED_PLASTIC].name = "Red Plastic";
    materials[MaterialType::RED_PLASTIC].ambient = red_plastic_ambient;
    materials[MaterialType::RED_PLASTIC].diffuse = red_plastic_diffuse;
    materials[MaterialType::RED_PLASTIC].specular = red_plastic_specular;
    materials[MaterialType::RED_PLASTIC].shininess = red_plastic_shininess;

    materials[MaterialType::EMERALD].name = "Emerald";
    materials[MaterialType::EMERALD].ambient = emerald_ambient;
    materials[MaterialType::EMERALD].diffuse = emerald_diffuse;
    materials[MaterialType::EMERALD].specular = emerald_specular;
    materials[MaterialType::EMERALD].shininess = emerald_shininess;

    materials[MaterialType::BRASS].name = "Brass";
    materials[MaterialType::BRASS].ambient = brass_ambient;
    materials[MaterialType::BRASS].diffuse = brass_diffuse;
    materials[MaterialType::BRASS].specular = brass_specular;
    materials[MaterialType::BRASS].shininess = brass_shininess;

    materials[MaterialType::SNOW_WHITE].name = "Snow_White";
    materials[MaterialType::SNOW_WHITE].ambient = snow_white_ambient;
    materials[MaterialType::SNOW_WHITE].diffuse = snow_white_diffuse;
    materials[MaterialType::SNOW_WHITE].specular = snow_white_specular;
    materials[MaterialType::SNOW_WHITE].shininess = snow_white_shininess;

    materials[MaterialType::YELLOW].name = "Yellow";
    materials[MaterialType::YELLOW].ambient = yellow_ambient;
    materials[MaterialType::YELLOW].diffuse = yellow_diffuse;
    materials[MaterialType::YELLOW].specular = yellow_specular;
    materials[MaterialType::YELLOW].shininess = yellow_shininess;

    materials[MaterialType::PINK].name = "ROSA";
    materials[MaterialType::PINK].ambient = pink_ambient;
    materials[MaterialType::PINK].diffuse = pink_diffuse;
    materials[MaterialType::PINK].specular = pink_specular;
    materials[MaterialType::PINK].shininess = pink_shininess;

    materials[MaterialType::BROWN].name = "MARRONE";
    materials[MaterialType::BROWN].ambient = brown_ambient;
    materials[MaterialType::BROWN].diffuse = brown_diffuse;
    materials[MaterialType::BROWN].specular = brown_specular;
    materials[MaterialType::BROWN].shininess = brown_shininess;

    materials[MaterialType::NO_MATERIAL].name = "NO_MATERIAL";
    materials[MaterialType::NO_MATERIAL].ambient = glm::vec3(1, 1, 1);
    materials[MaterialType::NO_MATERIAL].diffuse = glm::vec3(0, 0, 0);
    materials[MaterialType::NO_MATERIAL].specular = glm::vec3(0, 0, 0);
    materials[MaterialType::NO_MATERIAL].shininess = 1.f;

    //Setup degli shader
    shaders.resize(7);
    shaders[ShaderOption::NONE].value = 0;
    shaders[ShaderOption::NONE].name = "NONE";
    shaders[ShaderOption::GOURAD_SHADING].value = 1;
    shaders[ShaderOption::GOURAD_SHADING].name = "GOURAD SHADING";
    shaders[ShaderOption::BLINNPHONG_SHADING].value = 2;
    shaders[ShaderOption::BLINNPHONG_SHADING].name = "BLINN PHONG SHADING";
    shaders[ShaderOption::PHONG_SHADING].value = 3;
    shaders[ShaderOption::PHONG_SHADING].name = "PHONG SHADING";
    shaders[ShaderOption::NO_TEXTURE].value = 4;
    shaders[ShaderOption::NO_TEXTURE].name = "NO TEXTURE";
    shaders[ShaderOption::WAVE].value = 5;
    shaders[ShaderOption::WAVE].name = "WAVE";
    shaders[ShaderOption::FLAG].value = 6;
    shaders[ShaderOption::FLAG].name = "FLAG";

}

int INIT_TEXTURE(string path, int flip)
{
    int texture1;
    texture1 = loadTexture(path.c_str(), flip);
    return texture1;

}
int INIT_Cubemap(string SkyboxDir)
{
    int cubemapTexture;
    vector<std::string> faces
    {
        /*"right.jpg",
            "left.jpg",
            "top.jpg",
            "bottom.jpg",
            "front.jpg",
            "back.jpg"*/
        SkyboxDir + "posx.jpg",
        SkyboxDir + "negx.jpg",
        SkyboxDir + "posy.jpg",
        SkyboxDir + "negy.jpg",
        SkyboxDir + "posz.jpg",
        SkyboxDir + "negz.jpg"
    };
    return cubemapTexture =loadCubemap(faces, 0);

}
void INIT_Scene(vector<std::string> path_texture, vector<int>*texture, int* cubemapTexture)
{
    int i;
    Mesh cubo = {}, piano = {}, cono = {}, sfera = {}, toro = {}, asta = {}, drappo = {};
    vector<MeshObj> Model3D;

    INIT_SHADER();
    INIT_CAMERA_PROJECTION();
    INIT_Illuminazione();
    for (i=0;i<path_texture.size();i++)
        texture->push_back(INIT_TEXTURE(path_texture[i], 0));
   

    *cubemapTexture = INIT_Cubemap(SkyboxDir);
     
    crea_cubo(&sky);
    INIT_VAO(&sky);
    
    //Inizializza la geometria della sfera, , la sua matrice di modellazione, il suo materiale ed il tipo di shader
    crea_sfera(&sfera, vec4(0.6, 1.0, 0.5, 1.0));
    sfera.Model = mat4(1.0);
    sfera.Model = translate(sfera.Model, vec3(3.5, 5.5, 2.5));
    sfera.Model = scale(sfera.Model, vec3(1.5, 1.5, 1.5));
    sfera.material = MaterialType::SNOW_WHITE;
    sfera.nome = "Sfera";
    sfera.sceltaShader = GOURAD_SHADING;
    crea_bounding_box(&sfera);
    //Genera il VAO della sfera
    INIT_VAO(&sfera);
    Scena.push_back(sfera);

    //Inizializza la geometria del piano, , la sua matrice di modellazione, il suo materiale ed il tipo di shader
    crea_piano_suddiviso(&piano, vec4(0.0, 0.2, 1.0, 1.0));
    piano.Model = mat4(1.0);
    piano.Model = rotate(piano.Model, radians(1.5f), vec3(1.0, 0.0, 0.0));
    piano.Model = translate(piano.Model, vec3(0.5, 0.0, 0.5));
    piano.Model = scale(piano.Model, vec3(300.0f, 1.0f, 300.0f));
    piano.Model = translate(piano.Model, vec3(-0.5, 0.0, -0.5));
    piano.nome = "Piano";
    piano.sceltaShader = WAVE;
    piano.material = MaterialType::BROWN;
    //crea_bounding_box(&piano);
    //Genera il VAO del piano
    INIT_VAO(&piano);
    Scena.push_back(piano);

    //Inizializza la geometria del cubo, la sua matrice di modellazione, il suo materiale ed il tipo di shader
    crea_cubo(&cubo);
    cubo.Model = mat4(1.0);
    cubo.Model = translate(cubo.Model, vec3(3.5, 2.0, 2.5));
    cubo.Model = scale(cubo.Model, vec3(2.0f, 2.0f, 2.0f));
    cubo.Model = rotate(cubo.Model, radians(10.0f), vec3(0.0, 1.0, 0.0));
    cubo.nome = "Cubo";
    cubo.sceltaShader = GOURAD_SHADING;
    cubo.material = MaterialType::EMERALD;
    crea_bounding_box(&cubo);
    //Genera il VAO del cubo
    INIT_VAO(&cubo);
    Scena.push_back(cubo);

  

    //Inizializza la geometria della piramide,la sua matrice di modellazione, il suo materiale ed il tipo di shader

    crea_cono(&cono, vec4(0.0, 1.0, 0.0, 1.0));
    cono.Model = mat4(1.0);
    cono.Model = translate(cono.Model, vec3(-2.5, 1.2, 12.0));
    cono.Model = scale(cono.Model, vec3(1.2, 1.5, 1.2));
    cono.Model = rotate(cono.Model, radians(180.0f), vec3(1.0, 0.0, 0.0));

    cono.nome = "Cono";
    cono.sceltaShader = GOURAD_SHADING;
    cono.material = MaterialType::YELLOW;
    crea_bounding_box(&cono);
    //Genera il VAO del cono
    INIT_VAO(&cono);
    Scena.push_back(cono);

    
    


    //Inizializza la geometria del toro, , la sua matrice di modellazione, il suo materiale ed il tipo di shader
    crea_toro(&toro, vec4(0.0, 0.0, 1.0, 0.8));
    toro.Model = mat4(1.0);
    toro.Model = translate(toro.Model, vec3(-2.0, 2.0, 2.5));
    toro.Model = rotate(toro.Model, radians(90.0f), vec3(1.0, 0.0, 0.0));
    toro.Model = scale(toro.Model, vec3(1.5, 0.5, 1.5));
    //Genera il VAO del toro
    toro.nome = "Toro";
    toro.sceltaShader = GOURAD_SHADING;
    crea_bounding_box(&toro);
    INIT_VAO(&toro);
    Scena.push_back(toro);


    //Inizializza geometria cilindo per costruire asta bandiera
    crea_cilindro(&asta,vec4(1.0,0.0,0.0,1.0));
    asta.Model = mat4(1.0);
    asta.Model = translate(asta.Model, vec3(6.0, 2.0, -0.5));
    asta.Model = scale(asta.Model, vec3(0.5, 12.0, 0.5));
    //Genera il VAO dell'asta
    asta.nome = "Asta";
    asta.sceltaShader = GOURAD_SHADING;
    crea_bounding_box(&asta);
    INIT_VAO(&asta);
    Scena.push_back(asta);


    //Inizializza geometria piano per costruire drappo bandiera

    crea_piano_suddiviso(&drappo, vec4(1.0, 1.0, 0.0, 1.0));
    drappo.Model = mat4(1.0);
    drappo.Model = translate(drappo.Model, vec3(8.0, 10.0, 0.0));
    drappo.Model = rotate(drappo.Model, radians(90.0f), vec3(1.0, 0.0, 0.0));
    drappo.Model = translate(drappo.Model, vec3(0.5, 0.0, 0.5));
    drappo.Model = scale(drappo.Model, vec3(6.0f, 1.0f, 4.0f));
    drappo.Model = translate(drappo.Model, vec3(-0.5,0.0, -0.5));
   
    drappo.nome = "Drappo";
    drappo.sceltaShader = FLAG;
    drappo.material = MaterialType::BROWN;
    //Genera il VAO del piano
    crea_bounding_box(&drappo);
    INIT_VAO(&drappo);
    Scena.push_back(drappo);

    add_obj("Alien Animal.obj", vec3(-4.0, 4.0, 12.0), vec3(4.0, 4.0, 4.0), 0.0f, vec3(0.0, 1.0, 0.0), Model3D);
    Object obj;
    obj.mesh = Model3D;
    obj.nome = "Alieno";
    crea_bounding_box_obj(&obj);
    ScenaObj.push_back(obj);
    clear_objModel(&Model3D);

    //carica modelli memorizzati in file obj e li aggiunge alla struttura ScenaObj
    /*add_obj("piper_pa18.obj", vec3(-18.0, 14.0, 0.0), vec3(6.0, 6.0, 6.0), 180.0f, vec3(0.0, 1.0, 0.0), Model3D);
	Object obj;
	obj.mesh = Model3D;
    crea_bounding_box_obj(&obj);
    ScenaObj.push_back(obj);
    clear_objModel(&Model3D);



    add_obj("Shelby.obj", vec3(-15.0, 8.0, 0.0), vec3(8.0,8.0, 8.0), 0.0f, vec3(0.0, 1.0, 0.0), Model3D);
    ScenaObj.push_back(Model3D);
    clear_objModel(&Model3D);

 
    add_obj("suv.obj", vec3(35.0, 8.0, 12.0), vec3(8.0, 8.0, 8.0), 0.0f, vec3(0.0, 1.0, 0.0), Model3D);
    ScenaObj.push_back(Model3D);
    clear_objModel(&Model3D);

    add_obj("dog.obj", vec3(-4.0, 0.0, 18.0), vec3(2.0, 2.0, 2.0), 0.0f, vec3(0.0, 1.0, 0.0), Model3D);
    ScenaObj.push_back(Model3D);
    clear_objModel(&Model3D);*/

 }