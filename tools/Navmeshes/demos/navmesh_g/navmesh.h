#ifndef INC_NAVMESH_H_
#define INC_NAVMESH_H_

#include "mcv_platform.h"
#include "Recast.h"
#include "DetourNavMesh.h"
#include "DetourNavMeshQuery.h"

#include "DebugDraw.h"
#include "RecastDebugDraw.h"

#include "navmesh_builder.h"
#include "navmesh_input.h"
#include "navmesh_render.h"

class CNavmesh {
public:

	struct rcConfigGUI
	{
		/// The width of the field along the x-axis. [Limit: >= 0] [Units: vx]
		int width;
		/// The height of the field along the z-axis. [Limit: >= 0] [Units: vx]
		int height;
		/// The width/height size of tile's on the xz-plane. [Limit: >= 0] [Units: vx]
		int tileSize;
		/// The size of the non-navigable border around the heightfield. [Limit: >=0] [Units: vx]
		int borderSize;
		/// The xz-plane cell size to use for fields. [Limit: > 0] [Units: wu] 
		float cs;
		/// The y-axis cell size to use for fields. [Limit: > 0] [Units: wu]
		float ch;
		/// The minimum bounds of the field's AABB. [(x, y, z)] [Units: wu]
		float bmin[3];
		/// The maximum bounds of the field's AABB. [(x, y, z)] [Units: wu]
		float bmax[3];
		/// The maximum slope that is considered walkable. [Limits: 0 <= value < 90] [Units: Degrees] 
		float walkableSlopeAngle;
		/// Minimum floor to 'ceiling' height that will still allow the floor area to 
		/// be considered walkable. [Limit: >= 3] [Units: vx] 
		float walkableHeight;
		/// Maximum ledge height that is considered to still be traversable. [Limit: >=0] [Units: vx] 
		float walkableClimb;
		/// The distance to erode/shrink the walkable area of the heightfield away from 
		/// obstructions.  [Limit: >=0] [Units: vx] 
		float walkableRadius;
		/// The maximum allowed length for contour edges along the border of the mesh. [Limit: >=0] [Units: vx] 
		float maxEdgeLen;
		/// The maximum distance a simplfied contour's border edges should deviate 
		/// the original raw contour. [Limit: >=0] [Units: vx]
		float maxSimplificationError;
		/// The minimum number of cells allowed to form isolated island areas. [Limit: >=0] [Units: vx] 
		float minRegionArea;
		/// Any regions with a span count smaller than this value will, if possible, 
		/// be merged with larger regions. [Limit: >=0] [Units: vx] 
		float mergeRegionArea;
		/// The maximum number of vertices allowed for polygons generated during the 
		/// contour to polygon conversion process. [Limit: >= 3] 
		int maxVertsPerPoly;
		/// Sets the sampling distance to use when generating the detail mesh.
		/// (For height detail only.) [Limits: 0 or >= 0.9] [Units: wu] 
		float detailSampleDist;
		/// The maximum distance the detail mesh surface should deviate from heightfield
		/// data. (For height detail only.) [Limit: >=0] [Units: wu] 
		float detailSampleMaxError;
	};

  enum {
    FLAG_WALK = 0x01
  , FLAG_SWIM = 0x02
  , FLAG_DISABLED = 0x10
  , ALL_FLAGS = 0xffff
  };


public:

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

  dtNavMesh*            m_navMesh;
  dtNavMeshQuery*       m_navQuery;
  DebugDrawGL           m_draw;

  rcConfigGUI m_rcConfigGUI; // used only on GUI

private:
  rcHeightfield*        m_solid;
  rcCompactHeightfield* m_chf;
  rcContourSet*         m_cset;
  rcPolyMesh*           m_pmesh;
  rcConfig              m_cfg; // used for the creation
  rcPolyMeshDetail*     m_dmesh;
  rcBuildContext*       m_ctx;
  unsigned char*        m_triareas;

  rcBuildContext        m_context;

public:
  CNavmeshInput         m_input;
  EDrawMode             m_draw_mode;

public:
  CNavmesh( );
  void build( );
  dtNavMesh* create( const rcConfigGUI& cfg );
  void prepareQueries( );
  void destroy( );
  void dumpLog( );
  void render( bool use_z_test );
  void resetRecastConfigGUIValues();
};


#endif