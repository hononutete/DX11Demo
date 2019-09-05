#pragma once

#include "Mesh.h"

class PlaneMeshFactory
{
public:
	PlaneMeshFactory();
	~PlaneMeshFactory();

	HRESULT CreateMesh (MeshGroup** ppMeshGroup);

};

