#pragma once

class CDataProvider;
class CNavMesh;

struct TNavMeshLoader {
    bool load(CDataProvider& dp);
};

CNavMesh* loadNavMesh(const char* filename);
