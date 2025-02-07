#pragma once
#include "strutture.h"
#include "lib.h"
#include "utilities.h"
extern float clear_color[3];
extern mat4 Projection;

extern bool flagWf;
extern bool flagAncora;
extern bool flagBbox;
extern ViewSetup SetupTelecamera;
extern PerspectiveSetup SetupProspettiva;
extern mat4 View, projection;
extern vector<Mesh> Scena;
extern vector<Object> ScenaObj;
extern Mesh sky;
extern vector<MaterialObj> materials;
extern vector<point_light> lights;
 
extern unsigned int programId, programId1, programIdr;
 
#define BUFFER_OFFSET(i) ((char *)NULL + (i))

void rendering(float currentFrame,Uniform uniform, LightShaderUniform light_unif, vector<int>texture,  int cubemapTexture)
{
   
    int i, k;
    glClearColor(clear_color[0], clear_color[1], clear_color[2], 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //Imposto la matrice di proiezione per la scena da disegnare
    Projection = perspective(radians(SetupProspettiva.fovY), SetupProspettiva.aspect, SetupProspettiva.near_plane, SetupProspettiva.far_plane);

    //Costruisco la matrice di Vista che applicata ai vertici in coordinate del mondo li trasforma nel sistema di riferimento della camera.
    View = lookAt(vec3(SetupTelecamera.position), vec3(SetupTelecamera.target), vec3(SetupTelecamera.upVector));

    // Disegno Sky box
    //Disabilita il test di profondità. 
    // Disabilitando il depth test, si assicura che le facce del cubo siano sempre visibili, anche se si sovrappongono ad altri oggetti.
    // Le matrici di proiezione e vista vengono utilizzate per proiettare 
    // il cubo sulla schermata e creare l'effetto di un ambiente infinito.
    glDepthMask(GL_FALSE); //Riabilita il test di profondità per il rendering degli altri oggetti della scena.

    glUseProgram(programId1);
    glUniform1i(glGetUniformLocation(programId1, "skybox"), 0);
    glUniformMatrix4fv(uniform.MatProjS, 1, GL_FALSE, value_ptr(Projection));
    glUniformMatrix4fv(uniform.MatViewS, 1, GL_FALSE, value_ptr(View));
    // skybox cube
    glBindVertexArray(sky.VAO);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
    glDrawElements(GL_TRIANGLES, sky.indices.size() * sizeof(GLuint), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    glDepthMask(GL_TRUE);
     
    glUseProgram(programIdr);
    //Utilizzo il program shader per il disegno
    glUniformMatrix4fv(uniform.MatProjR, 1, GL_FALSE, value_ptr(Projection));
    glUniformMatrix4fv(uniform.MatModelR, 1, GL_FALSE, value_ptr(Scena[0].Model));
    glUniformMatrix4fv(uniform.MatViewR, 1, GL_FALSE, value_ptr(View));
    glUniform3f(uniform.loc_view_posR, SetupTelecamera.position.x, SetupTelecamera.position.y, SetupTelecamera.position.z);
    glBindVertexArray(Scena[0].VAO);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
    glDrawElements(GL_TRIANGLES, (Scena[0].indices.size() - 1) * sizeof(GLuint), GL_UNSIGNED_INT, 0);
    
    glUseProgram(programId);
    
    //Passo al Vertex Shader il puntatore alla matrice View, che sarà associata alla variabile Uniform mat4 Projection
   //all'interno del Vertex shader. Uso l'identificatio MatView
    glUniformMatrix4fv(uniform.MatView, 1, GL_FALSE, value_ptr(View));
    //La matrice di proiezione che mappa il volume di vista in coordinate NDC
    glUniformMatrix4fv(uniform.MatProj, 1, GL_FALSE, value_ptr(Projection));
    glUniform1f(uniform.loc_time, currentFrame);

    //flagWf: se true viene fatta la visualizzazione wireframe
    if (flagWf == true)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    
    for (const auto& light : lights)
    {
        glUniform3f(light_unif.light_position_pointer, light.position.x, light.position.y, light.position.z);
        glUniform3f(light_unif.light_color_pointer, light.color.r, light.color.g, light.color.b);
        glUniform1f(light_unif.light_power_pointer, light.power);
    }

    //Passo allo shader il puntatore alla posizione della camera
    glUniform3f(uniform.loc_view_pos, SetupTelecamera.position.x, SetupTelecamera.position.y, SetupTelecamera.position.z);

    for (i = 1; i < Scena.size(); i++)
    {
        glUniformMatrix4fv(uniform.MatModel, 1, GL_FALSE, value_ptr(Scena[i].Model));
        glBindVertexArray(Scena[i].VAO);

        update_ancora(&Scena[i]);
        ///La funzione glDrawElements è uno strumento fondamentale in OpenGL per rendere primitive grafiche (come triangoli, linee o punti) 
        // utilizzando dati di vertici memorizzati in buffer di vertici (VBO) e indicizzati da un buffer di elementi (EBO).
        //Utilizzando gli indici, possiamo ottimizzare il processo di rendering e creare geometrie complesse con meno dati di vertici.
        //GL_TRIANGLES: Questo parametro specifica il tipo di primitiva da rendere.In questo caso, stiamo rendendo dei triangoli.
        //(cubo.indices.size() - 1) * sizeof(GLuint) : Questo calcola il numero di indici da disegnare.Il termine cubo.indices.size() indica il numero totale di indici, ma sottraiamo 1 perché gli indici sono spesso memorizzati a coppie o triple per definire i triangoli.Moltiplicando per sizeof(GLuint) otteniamo la dimensione in byte.
        //GL_UNSIGNED_INT : Questo specifica il tipo di dato degli indici nel buffer di elementi.In questo caso, sono interi senza segno.
        //0 : Questo è l'offset in byte all'interno del buffer di elementi.In questo caso, iniziamo dall'inizio.
        //Utilizzando gli indici, possiamo ottimizzare il processo di rendering e creare geometrie complesse con meno dati di vertici

        glUniform1i(uniform.loc_sceltaShader, Scena[i].sceltaShader);

        glUniform3fv(light_unif.material_ambient, 1, glm::value_ptr(materials[Scena[i].material].ambient));
        glUniform3fv(light_unif.material_diffuse, 1, glm::value_ptr(materials[Scena[i].material].diffuse));
        glUniform3fv(light_unif.material_specular, 1, glm::value_ptr(materials[Scena[i].material].specular));
        glUniform1f(light_unif.material_shininess, materials[Scena[i].material].shininess);
        glUniformMatrix4fv(uniform.MatModel, 1, GL_FALSE, value_ptr(Scena[i].Model));
        glBindVertexArray(Scena[i].VAO);
      
        if (i==Piano)
        {
            glUniform1i(uniform.loc_texture1, 0);
            glBindTexture(GL_TEXTURE_2D, texture[mare]);
        }

        else if (i ==Drappo)
        {
            glUniform1i(uniform.loc_texture1, 0);
            glBindTexture(GL_TEXTURE_2D, texture[bandiera_ita]);
        }

        else if (i==Asta)
        {
            glUniform1i(uniform.loc_texture1, 0);
          
           glBindTexture(GL_TEXTURE_2D, texture[cemento]);
        }
        else
        {
            glUniform1i(uniform.loc_texture1, 0);
            glBindTexture(GL_TEXTURE_2D, texture[cemento]);
        }

        // Disegna la mesh principale
        glDrawElements(GL_TRIANGLES, Scena[i].indices.size() - 24, GL_UNSIGNED_INT, 0);

        if (flagBbox) {
            // Disegna la bounding box con GL_LINES
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            glDrawElements(GL_LINES, 24, GL_UNSIGNED_INT, (void*)(sizeof(GLuint) * (Scena[i].indices.size() - 24)));

            // Ripristina lo stato precedente
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }

        if (flagAncora)
        {
            glPointSize(15.0);
            int ind = Scena[i].indices.size() - 1;
            glDrawElements(GL_POINTS, 1, GL_UNSIGNED_INT, BUFFER_OFFSET(ind * sizeof(GLuint)));
        }
        glBindVertexArray(0);
    }

   //Rendering della ScenaObj

    for (const auto& obj : ScenaObj) {
        for (size_t i = 0; i < obj.mesh.size(); i++)
        {
            const MeshObj* mesh = &obj.mesh[i];

            glUniformMatrix4fv(uniform.MatModel, 1, GL_FALSE, value_ptr(mesh->Model));
            glUniform1i(uniform.loc_sceltaShader, mesh->sceltaShader);

            //Passo allo shader il puntatore ai materiali
            glUniform3fv(light_unif.material_ambient, 1, value_ptr(mesh->materiale.ambient));
            glUniform3fv(light_unif.material_diffuse, 1, value_ptr(mesh->materiale.diffuse));
            glUniform3fv(light_unif.material_specular, 1, value_ptr(mesh->materiale.specular));
            glUniform1f(light_unif.material_shininess, mesh->materiale.shininess);
            glBindVertexArray(mesh->VAO);


            glDrawElements(GL_TRIANGLES, (mesh->indices.size() - (i == 0 ? 24 : 0)) * sizeof(GLuint), GL_UNSIGNED_INT, 0);

            if (i == 0) { // flagBbox &&
                // Disegna la bounding box con GL_LINES
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                glDrawElements(GL_LINES, 24, GL_UNSIGNED_INT, (void*)(sizeof(GLuint) * (mesh->indices.size() - 24)));

                // Ripristina lo stato precedente
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            }

            glBindVertexArray(0);
        }
    }
}

