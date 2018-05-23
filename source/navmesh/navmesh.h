#pragma once

#include "resources/resource.h"

#include "Recast/Include/Recast.h"
//#include "DebugUtils/Include/RecastDebugDraw.h"
//#include "DebugUtils/Include/DetourDebugDraw.h"
#include "Detour/Include/DetourNavMesh.h"
#include "Detour/Include/DetourNavMeshQuery.h"
#include "DetourCrowd/Include/DetourCrowd.h"

static const int NAVMESHSET_MAGIC = 'M' << 24 | 'S' << 16 | 'E' << 8 | 'T'; //'MSET';
static const int NAVMESHSET_VERSION = 1;

class CNavMesh : public IResource {

private:
	dtNavMesh*		navMesh  = nullptr;
	dtNavMeshQuery*	navQuery = nullptr;
    dtCrowd*        crowd    = nullptr;

	rcHeightfield*			solid = nullptr;
	rcCompactHeightfield*	chf   = nullptr;
	rcContourSet*			cset  = nullptr;
	rcPolyMesh*				pmesh = nullptr;
	rcConfig				cfg;
	rcPolyMeshDetail*       dmesh = nullptr;
	unsigned char*          triareas;

public:
    struct NavMeshSetHeader {
        int magic;
        int version;
        int numTiles;
        dtNavMeshParams params;
    };

    struct NavMeshTileHeader {
        dtTileRef tileRef;
        int dataSize;
    };

    CNavMesh();

    void debugInMenu();

    bool loadNavmesh(const std::string& binFilePath);
    void destroy() override;

    bool isValid() const;

    dtNavMesh*      getNavMesh() { return navMesh; }
    dtNavMeshQuery* getNavMeshQuery() { return navQuery; }
    dtCrowd*        getCrowd() { return crowd; }
};
