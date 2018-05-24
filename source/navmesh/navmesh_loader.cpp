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

	// TODO: Hacerlo con el loader y no pasando filename aquí.
    CNavMesh* navmesh = new CNavMesh();
    if (!navmesh->create(filename)) return nullptr;

    return navmesh;
}
