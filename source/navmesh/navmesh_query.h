#pragma once

#include "navmesh.h"

class CNavMeshQuery {

public:
	CNavMesh* data;

	CNavMeshQuery(CNavMesh* aNavMesh);

	void findPath(VEC3 start, VEC3 end);
	std::vector<VEC3> getSmoothPath(VEC3 start, VEC3 end);
	void wallDistance(VEC3 fromPos);
	float getWallDistance(VEC3 fromPos);
	void raycast(VEC3 start, VEC3 end);
	// TODO: Añadir un getRaycast si lo queremos usar.

private:
	dtQueryFilter m_filter;

	dtStatus m_pathFindStatus;

	static const int MAX_POLYS = 256;
	static const int MAX_SMOOTH = 2048;

	dtPolyRef m_startRef;
	dtPolyRef m_endRef;
	dtPolyRef m_polys[MAX_POLYS]; // Pathfind result (polygons).
	dtPolyRef m_parent[MAX_POLYS];
	int m_npolys;
	int m_nsmoothPath;
	float m_smoothPath[MAX_SMOOTH * 3]; // Pathfind result.

	float m_polyPickExt[3];
	
	int m_pathIterNum;
	
	float m_distanceToWall;
	float m_hitPos[3];
	float m_hitNormal[3];
};
