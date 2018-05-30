#ifndef _NAVMESH_INPUT_INC
#define _NAVMESH_INPUT_INC


#include "components\comp_tags.h"
#include "components\comp_physics.h"
#include "components\comp_transform.h"
#include "entity\entity.h"
#include <vector>


class CNavmeshInput {
public:
  struct TInput {
    VEC3 pmin;
    VEC3 pmax;
	CTransform transWorld;
	VEC3 half_size;
	VEC3 v[8];
    TInput( ) : pmin( 0, 0, 0 ), pmax( 0, 0, 0 ) { }
  };
  static const int MAX_INPUTS = 1024;
  typedef std::vector< TInput > VInputs;

public:
  VInputs        inputs;
  VEC3           aabb_min;
  VEC3           aabb_max;
  
  float*                verts;
  int*                  tris;
  int                   nverts;
  int                   ntris;
  int                   nverts_total;
  int                   ntris_total;

public:
  CNavmeshInput( );
  
  void clearInput( );
  void addInput( const VEC3& p0, const VEC3& p1, VEC3 p2[], int size, CTransform& transWorld,const VEC3& half_size);
  void addAllSceneCollidersToInput();
  void addSceneCollidersWithTag(std::string tag_name);
  void addShapes(CEntity* e, VEC3 pmin, VEC3 pmax, TCompPhysics::ShapeConfig var);
  void prepareInput(const TInput& input) ;
  void unprepareInput() ;
  void computeBoundaries(  );
  
};

#endif