#include "mcv_platform.h"
#include "navmesh_loader.h"
#include "utils/data_provider.h"
#include "navmesh.h"

bool TNavMeshLoader::load(CDataProvider& dp) {
    bool eof_found = false;
    //while (!eof_found) { }
    // TODO: Usar el data provider para leer el navmesh del .bin
    // y no hacerlo directamente en loadNavMesh.

    return true;
}

CNavMesh* loadNavMesh(const char* filename) {
    CFileDataProvider fdp(filename);
    assert(fdp.isValid());

    /*TNavMeshLoader loader;
    if (!loader.load(fdp)) return nullptr;*/

    FILE* fp = fopen(filename, "rb");
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
    

    CNavMesh* navmesh = new CNavMesh();
    if (!navmesh->create(
        // TODO: add parameters here
    
    )) return nullptr;

    return navmesh;
}
