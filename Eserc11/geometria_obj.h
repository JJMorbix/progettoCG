#pragma once
#include "lib.h"
#include "strutture.h"

void add_obj(const char* name, vec3 translateVec, vec3 scaleVec, float ancolo, vec3 rotation_axis, vector<MeshObj>& Model3D);
void normalizeModel(vector<MeshObj>&Model3D);