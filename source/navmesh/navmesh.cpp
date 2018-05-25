#include "mcv_platform.h"
#include "navmesh.h"
#include "navmesh_loader.h"

//https://github.com/recastnavigation/recastnavigation

class CNavMeshResourceClass : public CResourceClass {
public:
	CNavMeshResourceClass() {
        class_name = "NavMeshes";
        extensions = { ".bin" };
    }
    IResource* create(const std::string& name) const override {
        CNavMesh* res = nullptr;
        if (name.find(".bin") != std::string::npos)
            res = loadNavMesh(name.c_str());
        return res;
    }
};

// A specialization of the template defined at the top of this file
// If someone class getResourceClassOf<CTexture>, use this function:
template<>
const CResourceClass* getResourceClassOf<CNavMesh>() {
    static CNavMeshResourceClass the_resource_class;
    return &the_resource_class;
}

CNavMesh::CNavMesh() {
    navQuery = dtAllocNavMeshQuery();
    crowd = dtAllocCrowd();

	drawMode = EDrawMode::NAVMESH_DRAW_COUNTOURS;

    cfg.tileSize = 32;
    cfg.cs = 0.3f;
    cfg.ch = 0.2f;
    cfg.walkableSlopeAngle = 45.0f;
    cfg.walkableHeight = 2;
    cfg.walkableClimb = 0;
    cfg.walkableRadius = 1;
    cfg.maxEdgeLen = 12;
    cfg.maxSimplificationError = 1.3f;
    cfg.minRegionArea = 8;
    cfg.mergeRegionArea = 20;
    cfg.maxVertsPerPoly = 6;
    cfg.detailSampleDist = 1.0f;
    cfg.detailSampleMaxError = 1.f;
}

void CNavMesh::debugInMenu() {
	// Todos estos datos habría que guardarlos desde el RecastDemo, no vienen en el .bin.
    /*ImGui::DragInt("tileSize", &cfg.tileSize, 1, 0, 1024);
    ImGui::DragFloat("xz cellsize", &cfg.cs, 0.01f, 0.1f, 1.f);
    ImGui::DragFloat("cellheight", &cfg.ch, 0.01f, 0.1f, 1.f);
    ImGui::DragFloat("walkableSlopeAngle", &cfg.walkableSlopeAngle, 1.f, 0.f, 90.f);
    ImGui::DragInt("walkableHeight", &cfg.walkableHeight, 1, 3, 100);
    ImGui::DragInt("walkableClimb", &cfg.walkableClimb, 1, 0, 100);
    ImGui::DragInt("walkableRadius", &cfg.walkableRadius, 1, 0, 100);
    ImGui::DragInt("maxEdgeLen", &cfg.maxEdgeLen, 1, 0, 100);
    ImGui::DragFloat("maxSimplificationError", &cfg.maxSimplificationError, 0.01f, 0.f, 16.f);
    ImGui::DragInt("minRegionArea", &cfg.minRegionArea, 1, 0, 150);
    ImGui::DragInt("mergeRegionArea", &cfg.mergeRegionArea, 1, 0, 150);
    ImGui::DragInt("maxVertsPerPoly", &cfg.maxVertsPerPoly, 1, 3, 12);
    ImGui::DragFloat("detailSampleDist", &cfg.detailSampleDist, 0.01f, 0.f, 16);
    ImGui::DragFloat("detailSampleMaxError", &cfg.detailSampleMaxError, 0.01f, 0.f, 16);*/
}

bool CNavMesh::create(const std::string& binFilePath) {
    FILE* fp = fopen(binFilePath.c_str(), "rb");
    if (!fp) return false;

    // Read header.
    NavMeshSetHeader header;
    size_t readLen = fread(&header, sizeof(NavMeshSetHeader), 1, fp);
    if (readLen != 1) {
        fclose(fp);
        return false;
    }
    if (header.magic != NAVMESHSET_MAGIC) {
        fclose(fp);
        return false;
    }
    if (header.version != NAVMESHSET_VERSION) {
        fclose(fp);
        return false;
    }

    navMesh = dtAllocNavMesh();
    if (!navMesh) {
        fclose(fp);
        return false;
    }
    dtStatus status = navMesh->init(&header.params);
    if (dtStatusFailed(status)) {
        fclose(fp);
        return false;
    }

    // Read tiles.
    for (int i = 0; i < header.numTiles; ++i) {
        NavMeshTileHeader tileHeader;
        readLen = fread(&tileHeader, sizeof(tileHeader), 1, fp);
        if (readLen != 1) {
            fclose(fp);
            return false;
        }

        if (!tileHeader.tileRef || !tileHeader.dataSize) break;

        unsigned char* data = (unsigned char*)dtAlloc(tileHeader.dataSize, DT_ALLOC_PERM);
        if (!data) break;
        memset(data, 0, tileHeader.dataSize);
        readLen = fread(data, tileHeader.dataSize, 1, fp);
        if (readLen != 1) {
            dtFree(data);
            fclose(fp);
            return false;
        }
        navMesh->addTile(data, tileHeader.dataSize, DT_TILE_FREE_DATA, tileHeader.tileRef, 0);
    }
    fclose(fp);

    return true;
}

bool CNavMesh::isValid() const {
    return navMesh != nullptr;
}

void CNavMesh::destroy() {
    delete[] triareas;
    triareas = nullptr;
    rcFreeHeightField(solid);
    solid = nullptr;
    rcFreeCompactHeightfield(chf);
    chf = nullptr;
    rcFreeContourSet(cset);
    cset = nullptr;
    rcFreePolyMesh(pmesh);
    pmesh = nullptr;
    rcFreePolyMeshDetail(dmesh);
    dmesh = nullptr;
    dtFreeNavMesh(navMesh);
    navMesh = nullptr;
}

void CNavMesh::render(bool use_z_test) {
	/*if( !use_z_test )
	Renderer.disableZTest( );*/
	
	/*TPoint aabb_min, aabb_max;
	input_data.aabb.getMinMax( aabb_min, aabb_max );*/

	// render the bounding box
	/*duDebugDrawBoxWire( &m_draw, aabb_min.x, aabb_min.y, aabb_min.z, aabb_max.x, aabb_max.y, aabb_max.z, duRGBA( 255, 255, 255, 128 ), 1.0f );
	draw.begin( DU_DRAW_POINTS, 5.0f );
	draw.vertex( aabb_min.x, aabb_min.y, aabb_min.z, duRGBA( 255, 255, 255, 128 ) );
	draw.end( );*/

	unsigned char navMeshDrawFlags(DU_DRAWNAVMESH_OFFMESHCONS |
								   DU_DRAWNAVMESH_CLOSEDLIST  |
								   DU_DRAWNAVMESH_COLOR_TILES);
	const int SAMPLE_POLYFLAGS_DISABLED = 0xffff;

	//Renderer.disableZWrite( );

	if (navMesh && navQuery &&
		(drawMode == NAVMESH_DRAW_MESH   ||
		 drawMode == NAVMESH_DRAW_TRANS  ||
		 drawMode == NAVMESH_DRAW_BVTREE ||
		 drawMode == NAVMESH_DRAW_NODES  ||
		 drawMode == NAVMESH_DRAW_INVIS)) {
		if (drawMode != NAVMESH_DRAW_INVIS)
			duDebugDrawNavMeshWithClosedList(&draw, *navMesh, *navQuery, navMeshDrawFlags);
		if (drawMode == NAVMESH_DRAW_BVTREE)
			duDebugDrawNavMeshBVTree(&draw, *navMesh);
		if (drawMode == NAVMESH_DRAW_NODES)
			duDebugDrawNavMeshNodes(&draw, *navQuery);
		duDebugDrawNavMeshPolysWithFlags(&draw, *navMesh, SAMPLE_POLYFLAGS_DISABLED, duRGBA(0, 0, 255, 128));
	}

	//Renderer.enableZWrite( );

	if (cset && drawMode == NAVMESH_DRAW_COUNTOURS) {
		//Renderer.disableZWrite( );
		duDebugDrawContours(&draw, *cset);
		//Renderer.enableZWrite( );
	}

	if (pmesh && drawMode == NAVMESH_DRAW_POLYMESH) {
		//Renderer.disableZWrite( );
		duDebugDrawPolyMesh(&draw, *pmesh);
		//Renderer.enableZWrite( );
	}
	if (dmesh && drawMode == NAVMESH_DRAW_POLYMESH_DETAILS) {
		//Renderer.disableZWrite( );
		duDebugDrawPolyMeshDetail(&draw, *dmesh);
		//Renderer.enableZWrite( );
	}

	/*if( type == NAVMESH_STATIC_MESH )
	static_mesh.render( m_draw, draw_mode );*/

	/*Renderer.enableZWrite( );
	if( !use_z_test )
	Renderer.enableZTest( );*/
}
