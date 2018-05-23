#pragma once

#include "resources/resource.h"

#include "Recast/Include/Recast.h"
#include "Detour/Include/DetourNavMesh.h"
#include "Detour/Include/DetourNavMeshQuery.h"

class CNavMesh : public IResource {

public:
	dtNavMesh*		navMesh;
	dtNavMeshQuery*	navQuery;

private:
	rcHeightfield*			solid;
	rcCompactHeightfield*	chf;
	rcContourSet*			cset;
	rcPolyMesh*				pmesh;
	rcConfig				cfg;
	rcPolyMeshDetail*       dmesh;
	unsigned char*          triareas;

public:
    bool create();
    void destroy() override;
    void activate() const;

    void debugInMenu();

    bool  isValid() const;
    void  updateFromCPU(const void *new_cpu_data, size_t num_bytes_to_update);
};
