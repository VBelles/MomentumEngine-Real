#include "mcv_platform.h"
#include "navmesh_input.h"


CNavmeshInput::CNavmeshInput( )
  : verts( nullptr )
  , tris( nullptr )
  , nverts( 0 )
  , ntris( 0 )
  , nverts_total( 0 )
  , ntris_total( 0 )
{ }

// ---------------------------------------------------
void CNavmeshInput::clearInput( ) {
  inputs.clear( );
  nverts_total = 0;
  ntris_total = 0;
}



void CNavmeshInput::addInput( const VEC3& p0, const VEC3& p1 , VEC3 p2[], int size,CTransform& transWorld,const VEC3& half_size) {
  nverts_total += 8;
  ntris_total += 10;

  TInput input;

  for (int i = 0; i < size; ++i)
  {
	  input.v[i] = p2[i];
	 // dbg("vec: %f %f %f \n", p2[i].x, p2[i].y, p2[i].z);
  }

  input.pmin = p0;
  input.pmax = p1;
  input.transWorld = transWorld;
  input.half_size = half_size;
  inputs.push_back( input );
}


void CNavmeshInput::addAllSceneCollidersToInput()
{
	// take all the colliders from physics
	getHandleManager<CEntity>()->forEach([this](CEntity* e) {

		TCompPhysics* comp_physics = e->get<TCompPhysics>();
		if (comp_physics)
		{
			VEC3 pmin;
			VEC3 pmax;

			// Find the min point and max point of the collider
			TCompPhysics* comp_physics = e->get<TCompPhysics>();
			TCompPhysics::Config colliders = comp_physics->getConfig();

			for each (TCompPhysics::ShapeConfig var in colliders.shape_configs)
			{
				addShapes(e, pmin, pmax, var);
			}
		}

	});
}

void CNavmeshInput::addSceneCollidersWithTag(std::string tag_name)
{
	// take all the colliders from physics
	uint32_t id_tag = getID(tag_name.c_str());
	auto handles_tag = CTagsManager::get().getAllEntitiesByTag(id_tag);
	if (handles_tag.size() > 0)
	{
		for (auto handle : handles_tag)
		{
			CEntity* e = handle;
			VEC3 pmin;
			VEC3 pmax;

			// Find the min point and max point of the collider
			TCompPhysics* comp_physics = e->get<TCompPhysics>();
			if (!comp_physics)
				return;
			TCompPhysics::Config colliders = comp_physics->getConfig();

			for each (TCompPhysics::ShapeConfig var in colliders.shape_configs)
			{
				if(!var.is_trigger)
					addShapes(e,pmin,pmax,var);
				
			}
		}
	}
}

void CNavmeshInput::addShapes(CEntity* e,VEC3 pmin,VEC3 pmax,TCompPhysics::ShapeConfig var)
{
	//dbg("shape: %s\n", var.name.c_str());
	// transform entity
	TCompTransform* comp_transform = e->get<TCompTransform>();

	// pivot (0,0,0) with local vertex points
 	VEC3 v[8];
	v[0] =  VEC3(-var.half_size.x, -var.half_size.y, -var.half_size.z);
	v[1] = VEC3(var.half_size.x, -var.half_size.y, -var.half_size.z);
	v[2] =  VEC3(-var.half_size.x, var.half_size.y, -var.half_size.z);
	v[3] =  VEC3(var.half_size.x, var.half_size.y, -var.half_size.z);
	v[4] =  VEC3(-var.half_size.x, -var.half_size.y, var.half_size.z);
	v[5] =  VEC3(var.half_size.x, -var.half_size.y, var.half_size.z);
	v[6] = VEC3(-var.half_size.x, var.half_size.y, var.half_size.z);
	v[7] =  VEC3(var.half_size.x, var.half_size.y, var.half_size.z);

	VEC3 vW[8];
	for (int i = 0; i< 8; ++i)
	{	
		// local position 
		TCompTransform l_pos;
		l_pos.setPosition(v[i]);
		
		// local rotation
		TCompTransform l_rot;
		l_rot.setRotation(var.rot);

		// combine both
		CTransform pW1;
		pW1 = l_pos.combineWith((static_cast<CTransform>(l_rot)));

		// entity translation + shape translation (local mode) + entity rotation
		VEC3 direction = comp_transform->getFront() * var.pos.z + comp_transform->getLeft() *  var.pos.x + +comp_transform->getUp() *  var.pos.y;
		TCompTransform parent;
		parent.setPosition(comp_transform->getPosition() + direction);// shape exact position
		parent.setRotation(comp_transform->getRotation());

		// combine the relative collide position with the transform entity
		CTransform pW;
		pW = pW1.combineWith((static_cast<CTransform>(parent)));
		vW[i] = VEC3(pW.getPosition().x, pW.getPosition().y, pW.getPosition().z);
	}


	TCompTransform pminT;
	// combine half size with transform of the physics pos
	// combine the relative collide position with the transform entity
	pminT.setPosition(VEC3(var.pos.x, var.pos.y, var.pos.z) + VEC3(-var.half_size.x, -var.half_size.y, -var.half_size.z));
	pminT.setRotation(var.rot);
	CTransform pminW;
	pminW = pminT.combineWith(*(static_cast<CTransform*>(comp_transform)));

	CTransform pmaxT;
	pmaxT.setPosition(VEC3(var.pos.x, var.pos.y, var.pos.z) + VEC3(var.half_size.x, var.half_size.y, var.half_size.z));
	pmaxT.setRotation(var.rot);
	CTransform pmaxW;
	pmaxW = pmaxT.combineWith(*(static_cast<CTransform*>(comp_transform)));


	// box in the world coord
	pmin = pminW.getPosition();
	pmax = pmaxW.getPosition();

	// To draw
	CTransform pTransformLocal;
	pTransformLocal.setPosition(VEC3(var.pos.x, var.pos.y, var.pos.z));
	pTransformLocal.setRotation(var.rot);
	CTransform pTransformWorld;
	pTransformWorld = pTransformLocal.combineWith(*(static_cast<CTransform*>(comp_transform)));

	// add it to the inputNavmesh
	addInput(pmin, pmax, vW, 8, pTransformWorld, var.half_size);

}

// ---------------------------------------------------
void CNavmeshInput::prepareInput( const TInput& input ) {
  unprepareInput( );

  nverts = 8;
  ntris = 10;

  verts = new float[ nverts * 3 ];
  tris = new int[ ntris * 3 ];

  memset( verts, 0, nverts * 3 * sizeof( float ) );
  memset( tris, 0, ntris * 3 * sizeof( int ) );

  VEC3 v[8] = {
	 input.v[0],
	 input.v[1],
	 input.v[2],
	 input.v[3],
	 input.v[4],
	 input.v[5],
	 input.v[6],
	 input.v[7]
  };

  static const int idxs[ 6 ][ 4 ] = {
      { 4, 6, 7, 5 }
      , { 5, 7, 3, 1 }
      , { 1, 3, 2, 0 }
      , { 0, 2, 6, 4 }
      , { 3, 7, 6, 2 }
      , { 5, 1, 0, 4 }
  };

  for( int i = 0; i<8; ++i ) {
    VEC3 p = v[ i ];
    int idx = i * 3;
    verts[ idx ] = p.x;
    verts[ idx + 1 ] = p.y;
    verts[ idx + 2 ] = p.z;
  }

  int idx = 0;
  for( int i = 0; i<5; ++i ) {
    tris[ idx++ ] = idxs[ i ][ 0 ];
    tris[ idx++ ] = idxs[ i ][ 2 ];
    tris[ idx++ ] = idxs[ i ][ 1 ];

    tris[ idx++ ] = idxs[ i ][ 0 ];
    tris[ idx++ ] = idxs[ i ][ 3 ];
    tris[ idx++ ] = idxs[ i ][ 2 ];
  }

  assert( idx == ntris * 3 );
}

void CNavmeshInput::unprepareInput( ) {
  delete [] verts;
  delete [] tris;
  verts = 0;
  tris = 0;
}

void CNavmeshInput::computeBoundaries( ) {
  aabb_min = VEC3( 0, 0, 0 );
  aabb_max = VEC3( 0, 0, 0 );

  // cube in the world where we operate all the navmesh paths. All the map have to be inside this cube.
  for( auto& i : inputs ) {
    if( i.pmin.x < aabb_min.x )   aabb_min.x = i.pmin.x;
    if( i.pmin.y < aabb_min.y )   aabb_min.y = i.pmin.y;
    if( i.pmin.z < aabb_min.z )   aabb_min.z = i.pmin.z;
    if( i.pmax.x > aabb_max.x )   aabb_max.x = i.pmax.x;
    if( i.pmax.y > aabb_max.y )   aabb_max.y = i.pmax.y;
    if( i.pmax.z > aabb_max.z )   aabb_max.z = i.pmax.z;
  }
}


