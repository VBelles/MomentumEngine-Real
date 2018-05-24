#pragma once

class CDataProvider;
class CNavMesh;

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

struct TNavMeshLoader {

    NavMeshSetHeader  header;
    NavMeshTileHeader tileHeader;

    bool load(CDataProvider& dp);
};

CNavMesh* loadNavMesh(const char* filename);