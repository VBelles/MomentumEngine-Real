#ifndef INC_MODULE_NAVMESH_H_
#define INC_MODULE_NAVMESH_H_

#include "module.h"
#include "navmesh\navmesh.h"
#include "navmesh\navmesh_query.h"
#include "navmesh\navmesh_input.h"

class  NavMesh {
public:
	std::string name;
	std::string tag;

	CNavmesh nav;
	CNavmeshQuery nav_query;

	NavMesh() {}
	// Destructor
	
	std::string getName() { return name; }
	void setName(std::string new_name) { name = new_name; }
	std::string getTag() { return tag; }
	void setTag(std::string new_tag) { tag = new_tag; }

	void renderGeometryNavmesh()
	{
		std::vector<CNavmeshInput::TInput>::iterator iter;

		if (nav.m_input.inputs.empty())
			return;

		auto mesh_box = Resources.get("wired_unit_cube.mesh")->as<CRenderMesh>();
		MAT44 world_mat;

		// all inputs
		for (iter = nav.m_input.inputs.begin() ; iter != nav.m_input.inputs.end() ; ++iter)
		{
			int next = 0;
			for (int i = 0; i < 8; ++i)
			{
				/*
				if (next < 7)next++;
				Render.drawLine((*iter).v[i], (*iter).v[next], VEC4(0, 1, 0, 1));
				*/
				
				world_mat = (*iter).transWorld.asMatrix();
				world_mat = MAT44::CreateScale((*iter).half_size * 2) * world_mat;
				Render.drawPrimitive(mesh_box, world_mat, VEC4(0, 1, 0, 1));
				//dbg("v1: %f %f %f  v2: %f %f %f \n", v[i].x, v[i].y, v[i].z, v2[i].x, v2[i].y, v2[i].z);
			}
		}
	}

	void addGeometryWithTagToBuild() {
		nav.m_input.clearInput();
		nav.m_input.addSceneCollidersWithTag(getTag());
		//nav.m_input.addAllSceneCollidersToInput();
		nav.m_input.computeBoundaries();// create 'box'
		//nav.m_input.aabb_max.y = 0.25f;
	}

	void addAllGeometryToBuild() {
		nav.m_input.clearInput();
		nav.m_input.addAllSceneCollidersToInput();
		nav.m_input.computeBoundaries();// create 'box'
		//nav.m_input.aabb_max.y = 0.25f;
	}

	void buildNavMesh() {
		nav.build();
	}

	std::vector< VEC3 > getPath(VEC3 start, VEC3 end) {
		// set the points and calculate the path
		nav_query.setStartAndEndPosition(start, end);
		// save the path where we want

		std::vector< VEC3 > path_points = nav_query.getPathArray();
	
		return path_points;
	}
	
};

//using VNavMesh = std::vector<NavMesh>;

class NavMeshModule: public IModule {
	
	std::vector<NavMesh*> m_all_navmeshes;
	
	//CNavmesh nav;
	//CNavmeshQuery nav_query;

public:
	bool m_render_navmesh = false;
	bool m_render_geom_navmesh = false;

	const float ROTATION_RECALC = 15.0f;
	const float MOVEMENT_RECALC = 0.5f;

	const char* getName() const override { return "navmesh"; }
	bool start() override;
	void stop() override;
	void render() override;
	void update(float dt) override;
	void startFrame();
	void loadJsonNavMesh();

	NavMesh* getNavmeshByName(std::string name);
	NavMesh* getNavmeshByTag(std::string tag);
	void insertImGuiNavMesh(NavMesh*  a);
	//render
	void renderGeometryNavmesh(std::string name);
	// add
	void addNavMesh(std::string name,std::string tag);
	void addAndBuildNavMesh(std::string name,std::string tag);
	// build
	void buildAllNavMeshes();
	void buildNavMesh(std::string tag_name);
	void buildNavMeshWithString(std::string tag_word);
  
};

#endif