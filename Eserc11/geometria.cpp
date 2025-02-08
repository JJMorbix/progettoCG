#include "strutture.h"
#include "Lib.h"
#include "inizializzazioni.h"

extern mat4 Projection, View;

void crea_bounding_box_obj(Object* obj) {
	vec3 minWorld = vec3(obj->mesh[0].Model * vec4(obj->mesh[0].vertices[0], 1.0f));
	vec3 maxWorld = vec3(obj->mesh[0].Model * vec4(obj->mesh[0].vertices[0], 1.0f));
		
	for (const auto& mesh : obj->mesh) {
		for (const auto& vertex : mesh.vertices) {
			vec3 vert = vec3(mesh.Model * vec4(vertex, 1.0f));
			minWorld.x = std::min(minWorld.x, vert.x);
			minWorld.y = std::min(minWorld.y, vert.y);
			minWorld.z = std::min(minWorld.z, vert.z);

			maxWorld.x = std::max(maxWorld.x, vert.x);
			maxWorld.y = std::max(maxWorld.y, vert.y);
			maxWorld.z = std::max(maxWorld.z, vert.z);
		}
	}

	obj->bbox.max = maxWorld;
	obj->bbox.min = minWorld;
	obj->ancora_world = vec4((maxWorld.x + minWorld.x) / 2, (maxWorld.y + minWorld.y) / 2, (maxWorld.z + minWorld.z) / 2, 1.0);
}


void crea_bounding_box(Mesh* mesh) {
	vec3 minWorld = vec3(mesh->Model * vec4(mesh->vertices[0], 1.0f));
	vec3 maxWorld = vec3(mesh->Model * vec4(mesh->vertices[0], 1.0f));

	vec3 min = mesh->vertices[0];
	vec3 max = mesh->vertices[0];

	for (const auto& vertex : mesh->vertices) {
		vec3 vert = vec3(mesh->Model * vec4(vertex, 1.0f));
		minWorld.x = std::min(minWorld.x, vert.x);
		minWorld.y = std::min(minWorld.y, vert.y);
		minWorld.z = std::min(minWorld.z, vert.z);

		maxWorld.x = std::max(maxWorld.x, vert.x);
		maxWorld.y = std::max(maxWorld.y, vert.y);
		maxWorld.z = std::max(maxWorld.z, vert.z);

		min.x = std::min(min.x, vertex.x);
		min.y = std::min(min.y, vertex.y);
		min.z = std::min(min.z, vertex.z);

		max.x = std::max(max.x, vertex.x);
		max.y = std::max(max.y, vertex.y);
		max.z = std::max(max.z, vertex.z);
	}

	vector<vec3> bboxVertices = {
		vec3(min.x, min.y, min.z),
		vec3(max.x, min.y, min.z),
		vec3(max.x, max.y, min.z),
		vec3(min.x, max.y, min.z),
		vec3(min.x, min.y, max.z),
		vec3(max.x, min.y, max.z),
		vec3(max.x, max.y, max.z),
		vec3(min.x, max.y, max.z)
	};

	mesh->vertices.insert(mesh->vertices.end(), bboxVertices.begin(), bboxVertices.end());

	// Calcola l'offset degli indici della bounding box
	GLuint bboxStartIndex = mesh->vertices.size() - bboxVertices.size();

	vector<GLuint> bboxIndices = {
		bboxStartIndex + 0, bboxStartIndex + 1, bboxStartIndex + 1, bboxStartIndex + 2,
		bboxStartIndex + 2, bboxStartIndex + 3, bboxStartIndex + 3, bboxStartIndex + 0,
		bboxStartIndex + 4, bboxStartIndex + 5, bboxStartIndex + 5, bboxStartIndex + 6,
		bboxStartIndex + 6, bboxStartIndex + 7, bboxStartIndex + 7, bboxStartIndex + 4,
		bboxStartIndex + 0, bboxStartIndex + 4, bboxStartIndex + 1, bboxStartIndex + 5,
		bboxStartIndex + 2, bboxStartIndex + 6, bboxStartIndex + 3, bboxStartIndex + 7
	};

	// Aggiungi gli indici della bounding box al VBO degli indici della mesh
	mesh->indices.insert(mesh->indices.end(), bboxIndices.begin(), bboxIndices.end());

	mesh->bbox.max = maxWorld;
	mesh->bbox.min = minWorld;

}

void crea_cubo(Mesh* mesh)
{

	std::vector<glm::vec2> texCoords = {
	   {0.0f, 0.0f}, // Bottom-left
	   {1.0f, 0.0f}, // Bottom-right
	   {1.0f, 1.0f}, // Top-right
	   {0.0f, 1.0f}  // Top-left
	};

	mesh->vertices.push_back(vec3(-1.0, -1.0, 1.0));
	mesh->texCoords.push_back(texCoords[0]);
	mesh->colors.push_back(vec4(1.0, 0.0, 0.0, 0.5));
	mesh->vertices.push_back(vec3(1.0, -1.0, 1.0));
	mesh->texCoords.push_back(texCoords[1]);
	mesh->colors.push_back(vec4(0.0, 1.0, 0.0, 0.5));
	mesh->vertices.push_back(vec3(1.0, 1.0, 1.0));
	mesh->texCoords.push_back(texCoords[2]);
	mesh->colors.push_back(vec4(0.0, 0.0, 1.0, 0.5));
	mesh->vertices.push_back(vec3(-1.0, 1.0, 1.0));
	mesh->texCoords.push_back(texCoords[3]);
	mesh->colors.push_back(vec4(1.0, 0.0, 1.0, 0.5));
	
	// back
	mesh->vertices.push_back(vec3(-1.0, -1.0, -1.0));
	mesh->texCoords.push_back(texCoords[0]);
	mesh->colors.push_back(vec4(1.0, 1.0, 1.0, 0.5));
	mesh->vertices.push_back(vec3(1.0, -1.0, -1.0));
	mesh->texCoords.push_back(texCoords[1]);
	mesh->colors.push_back(vec4(1.0, 1.0, 1.0, 0.5));
	mesh->vertices.push_back(vec3(1.0, 1.0, -1.0));
	mesh->texCoords.push_back(texCoords[2]);
	mesh->colors.push_back(vec4(1.0, 1.0, 1.0, 0.5));
	mesh->vertices.push_back(vec3(-1.0, 1.0, -1.0));
	mesh->texCoords.push_back(texCoords[3]);
	mesh->colors.push_back(vec4(1.0, 1.0, 1.0, 0.5));

	
	 

	mesh->vertices.push_back(vec3(0.0, 0.0, 0.0));
	mesh->colors.push_back(vec4(0.0, 1.0, 0.0, 1.0));
	mesh->ancora_obj = (vec4(0.0, 0.0, 0.0, 1.0));

	mesh->indices.push_back(0); mesh->indices.push_back(1); mesh->indices.push_back(2);
	mesh->indices.push_back(2); mesh->indices.push_back(3); mesh->indices.push_back(0);
	mesh->indices.push_back(1); mesh->indices.push_back(5); mesh->indices.push_back(6);
	mesh->indices.push_back(6); mesh->indices.push_back(2); mesh->indices.push_back(1);
	mesh->indices.push_back(7); mesh->indices.push_back(6); mesh->indices.push_back(5);
	mesh->indices.push_back(5); mesh->indices.push_back(4); mesh->indices.push_back(7);
	mesh->indices.push_back(4); mesh->indices.push_back(0); mesh->indices.push_back(3);
	mesh->indices.push_back(3); mesh->indices.push_back(7); mesh->indices.push_back(4);
	mesh->indices.push_back(4); mesh->indices.push_back(5); mesh->indices.push_back(1);
	mesh->indices.push_back(1); mesh->indices.push_back(0); mesh->indices.push_back(4);
	mesh->indices.push_back(3); mesh->indices.push_back(2); mesh->indices.push_back(6);
	mesh->indices.push_back(6); mesh->indices.push_back(7); mesh->indices.push_back(3);


	

	mesh->normals.push_back(normalize(vec3(0, 0, 1)));
	mesh->normals.push_back(normalize(vec3(0, 0, 1)));
	mesh->normals.push_back(normalize(vec3(0, 0, 1)));
	mesh->normals.push_back(normalize(vec3(0, 0, 1)));
	mesh->normals.push_back(normalize(vec3(0, 0, -1)));
	mesh->normals.push_back(normalize(vec3(0, 0, -1)));
	mesh->normals.push_back(normalize(vec3(0, 0, -1)));
	mesh->normals.push_back(normalize(vec3(0, 0, -1)));

 
	int nv = mesh->vertices.size();
 
	mesh->indices.push_back(nv-1);

}
 
void crea_piramide(Mesh* mesh)
{
	mesh->vertices.push_back(vec3(-1.0, 0.0, 1.0));
	mesh->colors.push_back(vec4(1.0, 0.0, 0.0, 0.5));
	mesh->vertices.push_back(vec3(1.0, 0.0,  1.0));
	mesh->colors.push_back(vec4(0.0, 1.0, 0.0, 0.5));
	mesh->vertices.push_back(vec3(1.0, 0.0, -1.0));
	mesh->colors.push_back(vec4(0.0, 0.0, 1.0, 0.5));
	mesh->vertices.push_back(vec3(-1.0,  0.0,  -1.0));
	mesh->colors.push_back(vec4(1.0, 1.0, 0.0, 0.5));
	// Apice piramide
	mesh->vertices.push_back(vec3(0.0,1.0,0.0));
	mesh->colors.push_back(vec4(1.0, 1.0, 1.0, 1.0));


	

	mesh->indices.push_back(0); mesh->indices.push_back(1); mesh->indices.push_back(2);
	mesh->indices.push_back(0); mesh->indices.push_back(2); mesh->indices.push_back(3);

	mesh->indices.push_back(0); mesh->indices.push_back(4); mesh->indices.push_back(3);
	mesh->indices.push_back(0); mesh->indices.push_back(1); mesh->indices.push_back(4);

	mesh->indices.push_back(3); mesh->indices.push_back(2); mesh->indices.push_back(4);
	mesh->indices.push_back(1); mesh->indices.push_back(2); mesh->indices.push_back(4);

	mesh->vertices.push_back(vec3(0.0,0.3, 0.0));
	mesh->colors.push_back(vec4(0.0, 1.0, 0.0, 1.0));
	mesh->ancora_obj = (vec4(0.0, 0.3, 0.0, 1.0));


	for (int i = 0; i < mesh->vertices.size(); i++)
		mesh->normals.push_back(vec3(0.0));

	int nv = mesh->vertices.size();
	mesh->indices.push_back(nv-1);

}
void crea_piano(Mesh* mesh, vec4 colore)
{

	mesh->vertices.push_back(vec3(-0.5, 0.0, 0.5));
	mesh->colors.push_back(colore);
	mesh->vertices.push_back(vec3(0.5, 0.0, 0.5));
	mesh->colors.push_back(colore);
	mesh->vertices.push_back(vec3(0.5, 0.0, -0.5));
	mesh->colors.push_back(colore);
	mesh->vertices.push_back(vec3(-0.5, 0.0, -0.5));
	mesh->colors.push_back(colore);

	mesh->indices.push_back(0); mesh->indices.push_back(1); mesh->indices.push_back(2);
	mesh->indices.push_back(0); mesh->indices.push_back(2); mesh->indices.push_back(3);

	mesh->texCoords.push_back(vec2(0.0, 1.0));
	mesh->texCoords.push_back(vec2(1.0, 1.0));
	mesh->texCoords.push_back(vec2(1.0, 0.0));
	mesh->texCoords.push_back(vec2(0.0, 0.0));

	mesh->normals.push_back(vec3(0.0, 1.0, 0.0));
	mesh->normals.push_back(vec3(0.0, 1.0, 0.0));
	mesh->normals.push_back(vec3(0.0, 1.0, 0.0));
	mesh->normals.push_back(vec3(0.0, 1.0, 0.0));

	mesh->vertices.push_back(vec3(0.0, 0.0, 0.0));  //Memorizzo come ultimo vertice l'ancora per poterla visualizzare
	mesh->colors.push_back(vec4(1.0, 0.0, 0.0, 1.0));
	mesh->ancora_obj = (vec4(0.0, 0.0, 0.0, 1.0));  //memorizzo l'ancora nel campo ancora_obj
	 
	int nv = mesh->vertices.size();
	mesh->indices.push_back(nv-1);
}

void crea_piano_suddiviso(Mesh* mesh, vec4 colore)
{

	int N = 1024;

	int i, j;
	float x, y,s,t;
	for (i = 0; i < N; i++)
	{
		x =   (float)i / N;
		for (j = 0; j < N; j++)
			
		{
			y =   (float)j / N;
			mesh->vertices.push_back(vec3(x, 0.0,y));
			mesh->colors.push_back(colore);
			mesh->normals.push_back(vec3(0.0, 1.0, 0.0));
			//Coordinate di texture
			s = x;
			t = y;
			mesh->texCoords.push_back(vec2(s, t));

		}
	}
	int cont = -1;


	

	for (i = 0; i <= pow(N, 2) - (N + 1); i++) {

		j = i % (N);

		if (j != N - 1)
		{
			mesh->indices.push_back(i);
			mesh->indices.push_back(i + 1);
			mesh->indices.push_back(i + N );

			mesh->indices.push_back(i + N + 1);
			mesh->indices.push_back(i+1);
			mesh->indices.push_back(i + N);
		}
		

	}
 

	
	mesh->vertices.push_back(vec3(0.0, 0.0, 0.0));
	mesh->colors.push_back(vec4(0.0, 1.0, 0.0, 1.0));
	mesh->ancora_obj = (vec4(0.0, 0.0, 0.0, 1.0));
	mesh->colors.push_back(vec4(0.0, 1.0, 0.0, 1.0));
	int nv = mesh->vertices.size();
	mesh->indices.push_back(nv-1);

}

void crea_sfera(Mesh* mesh, vec4 colore)
{
	float s, t;
	vec3 centro = vec3(0.0, 0.0, 0.0);
	vec3 raggio = vec3(1.0, 1.0, 1.0);

	int Stacks = 30;  //numero di suddivisioni sull'asse y
	int Slices = 30;  // numero di suddivisioni sull'asse x

	//Calc The Vertices

	//Stacks= suddivisioni lungo l'asse y
	for (int i = 0; i <= Stacks; ++i) {

		float V = i / (float)Stacks;
		float phi = V * pi <float>();

		// Loop Through Slices suddivisioni lungo l'asse x
		for (int j = 0; j <= Slices; ++j) {

			float U = j / (float)Slices;
			float theta = U * (pi <float>() * 2);

			// Calc The Vertex Positions
			float x = centro.x + raggio.x * (cosf(theta) * sinf(phi));
			float y = centro.y + raggio.y * cosf(phi);
			float z = centro.z + raggio.z * sinf(theta) * sinf(phi);

			mesh->vertices.push_back(vec3(x, y, z)),
			mesh->colors.push_back(colore);

			//Normale nel vertice
			mesh->normals.push_back(vec3(x, y, z));
			//coordinata di Texture
			s = U;
			t = V;
			mesh->texCoords.push_back(vec2(s, t));

		}
	}

	// Calc The Index Positions
	for (int i = 0; i < Slices * Stacks + Slices; ++i) {

		mesh->indices.push_back(i);
		mesh->indices.push_back(i + Slices + 1);
		mesh->indices.push_back(i + Slices);


		mesh->indices.push_back(i + Slices + 1);
		mesh->indices.push_back(i);
		mesh->indices.push_back(i + 1);
	}

	mesh->vertices.push_back(vec3(0.0, 0.0, 0.0));
	mesh->colors.push_back(vec4(0.0, 1.0, 0.0, 1.0));

	mesh->ancora_obj = (vec4(0.0, 0.0, 0.0, 1.0));

	int nv = mesh->vertices.size();
	mesh->indices.push_back(nv-1);
}

void crea_toro(Mesh* mesh, vec4 colore)
{
	int Stacks = 30;  //numero di suddivisioni sull'asse x
	int Slices = 30;  // numero di suddivisioni sull'asse y
	float R = 1, r = 0.5;
	float s, t;


	//Calc The Vertices
	for (int i = 0; i <= Stacks; ++i) {

		float V = i / (float)Stacks;
		float phi = V * glm::pi <float>() * 2;

		// Loop Through Slices
		for (int j = 0; j <= Slices; ++j) {

			float U = j / (float)Slices;
			float theta = U * (glm::pi <float>() * 2);

			// Calc The Vertex Positions
			float x = (R + r * cosf(phi)) * cosf(theta);
			float y = r * sinf(phi);
			float z = (R + r * cosf(phi)) * sinf(theta);


			// Push Back Vertex Data
			mesh->vertices.push_back(vec3(x, y, z));
			mesh->colors.push_back(colore);
			//Normale nel vertice
			mesh->normals.push_back(vec3(normalize(vec3(sin(phi) * cos(theta), cos(phi), sin(theta) * sin(phi)))));
			s = U;
			t = V;
			mesh->texCoords.push_back(vec2(s, t));


		}
	}

	// Calc The Index Positions
	for (int i = 0; i < Slices * Stacks + Slices; ++i) {

		mesh->indices.push_back(i);
		mesh->indices.push_back(i + Slices + 1);
		mesh->indices.push_back(i + Slices);


		mesh->indices.push_back(i + Slices + 1);
		mesh->indices.push_back(i);
		mesh->indices.push_back(i + 1);
	}
	mesh->vertices.push_back(vec3(0.0, 0.0, 0.0));
	mesh->colors.push_back(vec4(0.0, 1.0, 0.0, 1.0));

	mesh->ancora_obj = (vec4(0.0, 0.0, 0.0, 1.0));

	int nv = mesh->vertices.size();
	mesh->indices.push_back(nv - 1);

}

void crea_cono(Mesh* mesh, vec4 colore)
{
	int Stacks = 30;  //numero di suddivisioni sull'asse x
	int Slices = 30;  // numero di suddivisioni sull'asse y
	float s, t;


	//Calc The Vertices
	for (int i = 0; i <= Stacks; ++i) {

		float V = i / (float)Stacks;
		float h = V;

		// Loop Through Slices
		for (int j = 0; j <= Slices; ++j) {

			float U = j / (float)Slices;
			float theta = U * (glm::pi <float>() * 2);

			// Calc The Vertex Positions
			float x = h * cosf(theta);
			float y = h;
			float z = h * sinf(theta);


			// Push Back Vertex Data
			mesh->vertices.push_back(vec3(x, y, z));
			mesh->colors.push_back(colore);
			//Normale nel vertice
			mesh->normals.push_back(normalize(vec3(cos(theta) / sqrt(2.0f), -1 / sqrt(2.0f), sin(theta) / sqrt(2.0f))));
			//coordinata di texture
			s = U;
			t = V;
			mesh->texCoords.push_back(vec2(s, t));

		}
	}

	// Calc The Index Positions
	for (int i = 0; i < Slices * Stacks + Slices; ++i) {

		mesh->indices.push_back(i);
		mesh->indices.push_back(i + Slices + 1);
		mesh->indices.push_back(i + Slices);


		mesh->indices.push_back(i + Slices + 1);
		mesh->indices.push_back(i);
		mesh->indices.push_back(i + 1);
	}

	mesh->ancora_obj = (vec4(0.0, 0.0, 0.0, 1.0));

}
void crea_cilindro(Mesh* mesh, vec4 colore)
{
	int Stacks = 30;  //numero di suddivisioni sull'asse x
	int Slices = 30;  // numero di suddivisioni sull'asse y

	float s, t;
	//Calc The Vertices
	for (int i = 0; i <= Stacks; ++i) {

		float V = i / (float)Stacks;
		float h = V;

		// Loop Through Slices
		for (int j = 0; j <= Slices; ++j) {

			float U = j / (float)Slices;
			float theta = U * (glm::pi <float>() * 2);

			// Calc The Vertex Positions
			float x = cosf(theta);
			float y = h;
			float z = sinf(theta);


			// Push Back Vertex Data
			mesh->vertices.push_back(vec3(x, y, z));
			mesh->colors.push_back(colore);
			mesh->normals.push_back(vec3(normalize(vec3(cos(theta), 0, sin(theta)))));
			//Coordinata di texture
			s = U;
			t = V;
			mesh->texCoords.push_back(vec2(s, t));

		}
	}

	// Calc The Index Positions
	for (int i = 0; i < Slices * Stacks + Slices; ++i) {

		mesh->indices.push_back(i);
		mesh->indices.push_back(i + Slices + 1);
		mesh->indices.push_back(i + Slices);


		mesh->indices.push_back(i + Slices + 1);
		mesh->indices.push_back(i);
		mesh->indices.push_back(i + 1);
	}

	mesh->ancora_obj = (vec4(0.0, 0.0, 0.0, 1.0));

}


