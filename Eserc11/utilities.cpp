#pragma once

#include "lib.h"
#include "strutture.h"
extern unsigned int programId, programId1, programIdr;

extern ViewSetup SetupTelecamera;

extern int selected_obj;
extern vector<Mesh> Scena;
extern Uniform uniform;
extern LightShaderUniform light_unif;
void modifyModelMatrix(vec3 translation_vector, vec3 rotation_vector, GLfloat angle, GLfloat scale_factor)
{
	//ricordare che mat4(1) costruisce una matrice identità di ordine 4
	vec3 traslModel;
	mat4 aa;
	aa= Scena[selected_obj].Model;
	
	// Estrazione della traslazione corrente :
	//Viene estratta la componente di traslazione dalla matrice di modellazione corrente dell'oggetto selezionato (l'ultima colonna della matrice di
	// Modellazione dell'oggetto selezionato.
	// Questa informazione è necessaria per centrare l'oggetto prima di applicare le trasformazioni di scala e rotazione.

	traslModel = glm::vec3(aa[3][0], aa[3][1], aa[3][2]);
	
	//Traslazione: Viene creata una matrice di traslazione che sposta l'oggetto in base al vettore di traslazione fornito.

	mat4 traslation = glm::translate(glm::mat4(1), translation_vector);

	

	//Per scalare intorno al centro dell'oggetto, lo traslo nell'origine, lo scalo e poi lo riposizono nella sua posizione memorizzata
	//in traslModel
	//Viene creata una matrice di traslazione temporanea per spostare l'oggetto nell'origine.
	// Viene applicata la scala all'oggetto nell'origine.
	// Viene creata una matrice di traslazione inversa per riportare l'oggetto nella sua posizione originale.

	mat4 scala= glm::translate(glm::mat4(1), traslModel);
	scala =scale(scala, glm::vec3(scale_factor, scale_factor, scale_factor));
	scala = translate(scala, -traslModel);


	//Per ruotare intorno al centro dell'oggetto, lo traslo nell'origine, ruoto e poi lo riposizono nella sua posizione memorizzata
	//in traslModel

	//Analogamente alla scala, viene creata una matrice di traslazione temporanea per centrare l'oggetto, 
	// una matrice di rotazione e infine una matrice di traslazione inversa per riportare l'oggetto nella sua posizione originale.
	
	mat4 rotation= glm::translate(glm::mat4(1), traslModel);
	rotation = glm::rotate(rotation, angle, rotation_vector);
	rotation = glm::translate(rotation, -traslModel);
	 

	//Le matrici di traslazione, rotazione e scala vengono moltiplicate nell'ordine corretto (traslazione, rotazione, scala) 
	// e premoltiplicate alla matrice di modellazione corrente dell'oggetto selezionato.
	// Questo aggiorna la matrice di modellazione dell'oggetto selezionato  con le nuove trasformazioni.
	if (selected_obj < 0) return;
	   Scena[selected_obj].Model = traslation*rotation * scala*Scena[selected_obj].Model;

}

void update_ancora(Mesh* mesh )
{
	mesh->ancora_world = mesh->ancora_obj;
	mesh->ancora_world = mesh->Model * mesh->ancora_world;

}

void clear_objModel(vector<MeshObj> * Model3D){
	int i;
	for (i = 0; i < Model3D->size(); i++)
{
	Model3D->at(i).vertices.clear();
	Model3D->at(i).colors.clear();
	Model3D->at(i).normals.clear();
	Model3D->at(i).indices.clear();
	Model3D->at(i).texCoords.clear();
}
}

void getUniform() {

	uniform.MatProj = glGetUniformLocation(programId, "Projection");
	uniform.MatModel = glGetUniformLocation(programId, "Model");
	uniform.MatView = glGetUniformLocation(programId, "View");
	uniform.loc_time= glGetUniformLocation(programId, "time");
	uniform.loc_sceltaShader = glGetUniformLocation(programId, "sceltaShader");
	uniform.loc_view_pos = glGetUniformLocation(programId, "ViewPos");
	uniform.loc_texture1 = glGetUniformLocation(programId, "id_tex1");
	uniform.loc_texture2 = glGetUniformLocation(programId, "id_tex2");
	//location delle variabili uniformi per la gestione della luce
	light_unif.light_position_pointer = glGetUniformLocation(programId, "light.position");
	light_unif.light_color_pointer = glGetUniformLocation(programId, "light.color");
	light_unif.light_power_pointer = glGetUniformLocation(programId, "light.power");

	//location delle variabili uniformi per la gestione dei materiali
	light_unif.material_ambient = glGetUniformLocation(programId, "material.ambient");
	light_unif.material_diffuse = glGetUniformLocation(programId, "material.diffuse");
	light_unif.material_specular = glGetUniformLocation(programId, "material.specular");
	light_unif.material_shininess = glGetUniformLocation(programId, "material.shininess");

	//location delle variabili uniform per lo skybox
	uniform.MatProjS = glGetUniformLocation(programId1, "Projection");
	uniform.MatViewS = glGetUniformLocation(programId1, "View");
	uniform.loc_skybox = glGetUniformLocation(programId1, "skybox");
	
	uniform.MatModelR = glGetUniformLocation(programIdr, "Model");
	uniform.MatViewR = glGetUniformLocation(programIdr, "View");
	uniform.MatProjR = glGetUniformLocation(programIdr, "Projection");
	uniform.loc_view_posR = glGetUniformLocation(programIdr, "ViewPos");
	uniform.loc_cubemapR = glGetUniformLocation(programIdr, "cubemap");
}

bool checkCollisions() {
	vec3 pos = SetupTelecamera.position;
	bool check = false;
	for (Mesh mesh : Scena) {
		check = (pos.x >= mesh.bbox.min.x && pos.x <= mesh.bbox.max.x) &&
			(pos.y >= mesh.bbox.min.y && pos.y <= mesh.bbox.max.y) &&
			(pos.z >= mesh.bbox.min.z && pos.z <= mesh.bbox.max.z);

		if (check) { break; }
	}

	return check;
}