#pragma once

#include "Recast/Include/Recast.h"
#include "Detour/Include/DetourCommon.h"
#include "Detour/Include/DetourNavMesh.h"
#include "Detour/Include/DetourNavMeshQuery.h"
#include "DetourCrowd/Include/DetourCrowd.h"

#include "navmesh.h"

class CNavmeshQuery {

public:
	struct TPos {
		VEC3 p;
		bool   set;
		TPos() : p(0, 0, 0), set(false) {}
	};

	CNavMesh* data;
	TPos    p1;
	TPos    p2;

	void findPath(TPos& start, TPos& end);
	void wallDistance(TPos& pos);
	void raycast(TPos& start, TPos& end);

private:
	
	dtQueryFilter m_filter;

	dtStatus m_pathFindStatus;

	static const int MAX_POLYS = 256;
	static const int MAX_SMOOTH = 2048;

	dtPolyRef m_startRef;
	dtPolyRef m_endRef;
	dtPolyRef m_polys[MAX_POLYS];
	dtPolyRef m_parent[MAX_POLYS];
	int m_npolys;
	int m_nsmoothPath;

	float m_smoothPath[MAX_SMOOTH * 3];

	int m_pathIterNum;
	
	float m_hitPos[3];
	float m_hitNormal[3];
};
