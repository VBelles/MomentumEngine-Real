#pragma once

#include "navmesh.h"

class CNavMeshQuery {

public:
	const CNavMesh* data;

	CNavMeshQuery(const CNavMesh* aNavMesh);

	void findPath(VEC3 start, VEC3 end);
	void wallDistance(VEC3 pos);
	void raycast(VEC3 start, VEC3 end);

private:
	dtQueryFilter m_filter;

	dtStatus m_pathFindStatus;

	static const int MAX_POLYS = 256;
	static const int MAX_SMOOTH = 2048;

	dtPolyRef m_startRef;
	dtPolyRef m_endRef;
	dtPolyRef m_polys[MAX_POLYS]; // pathfind result.
	dtPolyRef m_parent[MAX_POLYS];
	int m_npolys;
	int m_nsmoothPath;

	float m_smoothPath[MAX_SMOOTH * 3];

	float m_polyPickExt[3];
	
	int m_pathIterNum;
	
	float m_hitPos[3];
	float m_hitNormal[3];
};
