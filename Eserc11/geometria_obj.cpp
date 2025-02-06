#pragma once
#include "strutture.h"
#include "load_meshes_assimp.h"
#include "inizializzazioni.h"
extern string Meshdir;


void normalizeModel(vector<MeshObj>& Model3D) {
	int i, k;
	int nmeshes = Model3D.size();
	vector<vec3> minimo, massimo;
	float minx, miny, minz, maxx, maxy, maxz;
	vec3 centroid = { 0.0f, 0.0f, 0.0f };

	//Calcolo il centroide della mesh (facendo la medua dei suoi vertici)
	int numVertices = 0;
	for (i = 0; i < Model3D.size(); i++)

		for (k = 0; k < Model3D[i].vertices.size(); k++)
		{
			centroid += Model3D[i].vertices[k];
			numVertices++;
		}


	centroid /= numVertices;
 


	for (i = 0; i < Model3D.size(); i++)
		for (k = 0; k < Model3D[i].vertices.size(); k++)
			Model3D[i].vertices[k] -= centroid;
	 
	 
	// Troviamo i valori minimi e massimi per tutte le coordinate del modello
	float minX = std::numeric_limits<float>::max();
	float maxX = -std::numeric_limits<float>::max();
	float minY = std::numeric_limits<float>::max();
	float maxY = -std::numeric_limits<float>::max();
	float minZ = std::numeric_limits<float>::max();
	float maxZ = -std::numeric_limits<float>::max();


	// Iteriamo su tutte le mesh e troviamo i minimi e massimi globali

	for (i = 0; i < Model3D.size(); i++)
		for (k = 0; k < Model3D[i].vertices.size(); k++)
		{
			minX = std::min(minX, Model3D[i].vertices[k].x);
			maxX = std::max(maxX, Model3D[i].vertices[k].x);
			minY = std::min(minY, Model3D[i].vertices[k].y);
			maxY = std::max(maxY, Model3D[i].vertices[k].y);
			minZ = std::min(minZ, Model3D[i].vertices[k].z);
			maxZ = std::max(maxZ, Model3D[i].vertices[k].z);
		}


	// Calcoliamo if fattore di scala per ogni dimensione (per mantenere le proporzioni
	float rangeX = maxX - minX;
	float rangeY = maxY - minY;
	float rangeZ = maxZ - minZ;

	float maxRange = std::max({ rangeX, rangeY, rangeZ });
	for (i = 0; i < Model3D.size(); i++)
		for (k = 0; k < Model3D[i].vertices.size(); k++)
		{
			Model3D[i].vertices[k].x = 2.0f * (Model3D[i].vertices[k].x - minX) / maxRange - 1.0f;
			Model3D[i].vertices[k].y = 2.0f * (Model3D[i].vertices[k].y - minY) / maxRange- 1.0f;
			Model3D[i].vertices[k].z = 2.0f * (Model3D[i].vertices[k].z - minZ) / maxRange - 1.0f;
		}

		 
}



void add_obj(const char* name, vec3 translateVec, vec3 scaleVec, float angolo, vec3 rotation_axis, vector<MeshObj>& Model3D){
bool obj;
auto path = Meshdir + name;
obj = loadAssImp(path.c_str(), Model3D);
int nmeshes = Model3D.size();

normalizeModel(Model3D);

for (int i = 0; i < nmeshes; i++)
{
    INIT_VAO(&Model3D[i]);
    Model3D[i].Model = mat4(1.0);
    Model3D[i].Model = translate(Model3D[i].Model, translateVec);
	Model3D[i].Model = rotate(Model3D[i].Model, cos(radians(angolo)),rotation_axis);
    Model3D[i].Model = scale(Model3D[i].Model, scaleVec);
    Model3D[i].nome = "Modello";
    Model3D[i].sceltaShader = 4;
	Model3D[i].ancora_obj = vec4(0.0, 0.0, 0.0,1.0);
	 
}


}