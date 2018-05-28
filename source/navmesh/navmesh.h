#pragma once

#include <Recast.h>
#include <RecastDebugDraw.h>
#include <DetourDebugDraw.h>
#include <DetourNavMesh.h>
#include <DetourNavMeshQuery.h>
#include <DetourCrowd.h>

#include "resources/resource.h"
#include "navmesh_render.h"

class CNavMesh : public IResource {

public:
    static const int NAVMESHSET_MAGIC = 'M' << 24 | 'S' << 16 | 'E' << 8 | 'T'; //'MSET';
    static const int NAVMESHSET_VERSION = 1;

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

	enum EDrawMode {
		NAVMESH_DRAW_NONE = 0
		, NAVMESH_DRAW_TRANS
		, NAVMESH_DRAW_BVTREE
		, NAVMESH_DRAW_NODES
		, NAVMESH_DRAW_INVIS
		, NAVMESH_DRAW_MESH
		//, NAVMESH_DRAW_VOXELS
		//, NAVMESH_DRAW_VOXELS_WALKABLE
		//, NAVMESH_DRAW_COMPACT
		//, NAVMESH_DRAW_COMPACT_DISTANCE
		//, NAVMESH_DRAW_COMPACT_REGIONS
		//, NAVMESH_DRAW_REGION_CONNECTIONS
		//, NAVMESH_DRAW_RAW_CONTOURS
		//, NAVMESH_DRAW_BOTH_CONTOURS
		, NAVMESH_DRAW_COUNTOURS
		, NAVMESH_DRAW_POLYMESH
		, NAVMESH_DRAW_POLYMESH_DETAILS
		, NAVMESH_DRAW_TYPE_COUNT
	};

    CNavMesh();

    void debugInMenu();

    bool create(const std::string& binFilePath);
    void destroy() override;
	void render(bool use_z_test);

    bool isValid() const;

    dtNavMesh*      getNavMesh() const { return navMesh; }
    dtNavMeshQuery* getNavMeshQuery() const { return navQuery; }
    dtCrowd*        getCrowd() const { return crowd; }

private:
	dtNavMesh*				navMesh = nullptr;
	dtNavMeshQuery*			navQuery = nullptr;
	dtCrowd*				crowd = nullptr;
	NavMeshDebugDrawDX		draw;

	EDrawMode				drawMode;

	rcHeightfield*			solid = nullptr;
	rcCompactHeightfield*	chf = nullptr;
	rcContourSet*			cset = nullptr;
	rcPolyMesh*				pmesh = nullptr;
	rcConfig				cfg;
	rcPolyMeshDetail*       dmesh = nullptr;
	unsigned char*          triareas;
};
