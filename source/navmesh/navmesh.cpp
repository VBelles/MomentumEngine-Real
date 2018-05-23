#include "mcv_platform.h"
#include "navmesh.h"

class CNavMeshResourceClass : public CResourceClass {
public:
	CNavMeshResourceClass() {
        class_name = "navMeshes";
        extensions = { ".bin" };
    }
    IResource* create(const std::string& name) const override {
        CNavMesh* res = nullptr;
        /*if (name.find(".bin") != std::string::npos)
            res = load(name);*/
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

    cfg.tileSize = 32;
    cfg.cs = 0.3f;
    cfg.ch = 0.2f;
    cfg.walkableSlopeAngle = 45.0f;
    cfg.walkableHeight = 2.0;
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
    ImGui::DragInt("tileSize", &cfg.tileSize, 1, 0, 1024);
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
    ImGui::DragFloat("detailSampleMaxError", &cfg.detailSampleMaxError, 0.01f, 0.f, 16);
}

bool CNavMesh::loadNavmesh(const std::string& binFilePath) {
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
