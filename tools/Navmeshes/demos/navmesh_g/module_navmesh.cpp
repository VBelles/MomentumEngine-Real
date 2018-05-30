#include "mcv_platform.h"
#include "module_navmesh.h"
#include "module_imgui.h"
#include "windows/app.h"

bool NavMeshModule::start()
{
	dbg("NAVEGATION MESH\n");
	return true;
}

void NavMeshModule::stop()
{
	
}

void NavMeshModule::startFrame()
{


}

void NavMeshModule::loadJsonNavMesh()
{

	// remove all old navmeshes
	for (auto& navmesh : m_all_navmeshes)
	{
		delete(navmesh);
	}

	m_all_navmeshes.clear();

	std::ifstream json_file("data/navmesh.json");
	assert(json_file);
	json json_data;
	json_file >> json_data;

	for (int i = 0; i < json_data.size(); ++i)
	{
		auto& j = json_data.at(i);
		
		if (j.count("navmesh"))
		{
			auto& navmesh_json = j["navmesh"];
			addNavMesh(navmesh_json.value("name", "no name"), navmesh_json.value("tag", ""));

			NavMesh* nv = m_all_navmeshes.at(m_all_navmeshes.size() - 1);
			//params
			auto& params_json = navmesh_json["params"];

			nv->nav.m_rcConfigGUI.tileSize = params_json.value("tileSize", 32);
			nv->nav.m_rcConfigGUI.borderSize = params_json.value("borderSize", 1);
			nv->nav.m_rcConfigGUI.cs = params_json.value("cs", 0.55f);
			nv->nav.m_rcConfigGUI.ch = params_json.value("ch", 0.1f);
			nv->nav.m_rcConfigGUI.walkableSlopeAngle = params_json.value("walkableSlopeAngle", 45.f);
			nv->nav.m_rcConfigGUI.walkableHeight = params_json.value("walkableHeight", 2.f);
			nv->nav.m_rcConfigGUI.walkableClimb = params_json.value("walkableClimb", 0.1f);
			nv->nav.m_rcConfigGUI.walkableRadius = params_json.value("walkableRadius", 0.5f);
			nv->nav.m_rcConfigGUI.maxEdgeLen = params_json.value("maxEdgeLen", 10.f);
			nv->nav.m_rcConfigGUI.maxSimplificationError = params_json.value("maxSimplificationError", 1.f);
			nv->nav.m_rcConfigGUI.minRegionArea = params_json.value("minRegionArea", 1.f);
			nv->nav.m_rcConfigGUI.mergeRegionArea = params_json.value("mergeRegionArea", 1.f);
			nv->nav.m_rcConfigGUI.maxVertsPerPoly = params_json.value("maxVertsPerPoly", 6);
			nv->nav.m_rcConfigGUI.detailSampleDist = params_json.value("detailSampleDist", 1.f);
			nv->nav.m_rcConfigGUI.detailSampleMaxError = params_json.value("detailSampleMaxError", 0.1f);
		}
	}
}

void NavMeshModule::update(float dt)
{	

	POINT p;
	GetCursorPos(&p);
	VEC3 mpos((float)p.x,(float)p.y,(float)0.f);

	std::vector<NavMesh*>::iterator iter;

	for (iter = m_all_navmeshes.begin(); iter != m_all_navmeshes.end(); ++iter)
	{
		// update path 
		(*iter)->nav_query.update(mpos);
	}

	//nav_query.update(mpos);

}

void NavMeshModule::render()
{
	#if CODE_DEBUG

 	 auto imgui_module =(ImGUIModule*) CModuleManager::get().getModule("imgui");
	
	 
	 if (imgui_module->show_navmesh_panel)
	 {
		 ImGuiWindowFlags window_flags = 0;
		 if (false)  window_flags |= ImGuiWindowFlags_NoTitleBar;
		 if (true)   window_flags |= ImGuiWindowFlags_ShowBorders;
		 if (false)    window_flags |= ImGuiWindowFlags_NoResize;
		 if (false)      window_flags |= ImGuiWindowFlags_NoMove;
		 if (false) window_flags |= ImGuiWindowFlags_NoScrollbar;
		 if (false)  window_flags |= ImGuiWindowFlags_NoCollapse;
		 if (false)     window_flags |= ImGuiWindowFlags_MenuBar;

		 ImGui::SetNextWindowSize(ImVec2(200, 200), ImGuiSetCond_FirstUseEver);
		 ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiSetCond_FirstUseEver);
		 ImGui::Begin("Navegation meshes", &imgui_module->show_navmesh_panel, window_flags);

		
		 ImGui::Checkbox("Render navmeshes/paths", &m_render_navmesh);
		 ImGui::Checkbox("Render geometry navmeshes", &m_render_geom_navmesh);

		 if (ImGui::TreeNode("Creation:"))
		 {
			 static char name_nav[128] = "name the navmesh";
			 static char tag_nav[128] = "tag geometry";

			 ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "CREATION:");
			 ImGui::InputText("name", name_nav, sizeof(name_nav));
			 ImGui::InputText("tag", tag_nav, sizeof(tag_nav));

			 if (ImGui::Button("Create NavMesh")) {
				 addNavMesh(name_nav, tag_nav);
			 }; 
			 ImGui::TreePop();

		 }
		
		if (ImGui::TreeNode("All navegation meshes:"))
		{
			std::vector<NavMesh*>::iterator iter;
			// show all list
			//for (iter = m_all_navmeshes.begin(); iter != m_all_navmeshes.end(); ++iter)
			for (auto nv : m_all_navmeshes)
			{
				ImGui::PushID(nv->getName().c_str());
				if (ImGui::TreeNode("%s", nv->getName().c_str()))
				{
					insertImGuiNavMesh(nv);
					ImGui::TreePop();
				}
				ImGui::PopID();
			}
			ImGui::TreePop();
		}
		ImGui::End();
	 }
	 
	 if (m_render_navmesh) {
		std::vector<NavMesh*>::iterator iter2;
		// show all list
		 for (iter2 = m_all_navmeshes.begin(); iter2 != m_all_navmeshes.end(); ++iter2)
		{
			 //render navmesh
			(*iter2)->nav.render(false);
			// render path
			(*iter2)->nav_query.render();
		}
	 }

	 if (m_render_geom_navmesh)
	 {
		 std::vector<NavMesh*>::iterator iter2;
		 // show all list
		 for (iter2 = m_all_navmeshes.begin(); iter2 != m_all_navmeshes.end(); ++iter2)
		 {
			renderGeometryNavmesh((*iter2)->getName());
		 }
	 }

#endif
}


NavMesh* NavMeshModule::getNavmeshByName(std::string name)
{
	for each (NavMesh* nav in m_all_navmeshes)
	{
		if (nav->getName() == name)
			return nav;
	}
	return NULL;
}

NavMesh* NavMeshModule::getNavmeshByTag(std::string tag)
{
	for each (NavMesh* nav in m_all_navmeshes)
	{
		if (nav->getTag() == tag)
			return nav;
	}
	return NULL;

}

void NavMeshModule::addAndBuildNavMesh(std::string name,std::string tag)
{
	addNavMesh(name, tag);

	for each (NavMesh* nm in m_all_navmeshes)
	{
		if (nm->getName() == name) {
			nm->addGeometryWithTagToBuild();
			nm->buildNavMesh();
		}
	}
}

void NavMeshModule::buildAllNavMeshes()
{
	for each (NavMesh* nm in m_all_navmeshes)
	{
		nm->addGeometryWithTagToBuild();
		nm->buildNavMesh();	
	}
}


void NavMeshModule::buildNavMeshWithString(std::string tag_word)
{
	for (NavMesh* nm : m_all_navmeshes)
	{
		if (nm->getTag().find(tag_word) != std::string::npos)
		{
			nm->addGeometryWithTagToBuild();
			nm->buildNavMesh();
		}
	}

}

void NavMeshModule::buildNavMesh(std::string tag_name)
{
	for(NavMesh* nm : m_all_navmeshes)
	{
		if (nm->getTag() == tag_name)
		{
			nm->addGeometryWithTagToBuild();
			nm->buildNavMesh();
		}
	}
}

void NavMeshModule::insertImGuiNavMesh(NavMesh* nv)
{
	
	CNavmesh& nav = nv->nav;
	CNavmeshQuery &nav_query = nv->nav_query;

	// add geometry to build
	if (ImGui::Button("Add Geometry")) {
		dbg("---------------------Add geometry to build\n");
		nv->addGeometryWithTagToBuild();
	};

	// Build Nav Mesh
	if (ImGui::Button("Build NavMesh")) {
		nv->buildNavMesh();
	};

	
	// Choose Draw mode
	const char* items[] = { "NAVMESH_DRAW_NONE", "NAVMESH_DRAW_TRANS","NAVMESH_DRAW_BVTREE"
		, "NAVMESH_DRAW_NODES"
		, "NAVMESH_DRAW_INVIS"
		, "NAVMESH_DRAW_MESH"
		, "NAVMESH_DRAW_COUNTOURS"
		, "NAVMESH_DRAW_POLYMESH"
		, "NAVMESH_DRAW_POLYMESH_DETAILS"
		, "NAVMESH_DRAW_TYPE_COUNT" };
	int item2 = (int)nav.m_draw_mode;
	ImGui::Combo("Draw Mode", &item2, items, (int)(sizeof(items) / sizeof(*items)));
	nav.m_draw_mode = static_cast<CNavmesh::EDrawMode>(item2);

	if (ImGui::Button("Print NavMesh")) {
		dbg("--------------------- Change drawing mode\n");
		nav.m_draw_mode = (CNavmesh::EDrawMode) ((nav.m_draw_mode + 1) % CNavmesh::NAVMESH_DRAW_TYPE_COUNT);
	};

	if (ImGui::Button("Tool")) {
		dbg("--------------------- Change request tool\n");
		nav_query.tool = (CNavmeshQuery::ETool) ((nav_query.tool + 1) % CNavmeshQuery::NUM_TOOLS);
	};

	ImGui::Separator();

	if (ImGui::DragFloat3("Start position", &nav_query.p1.p.x, 0.01f, -100.f, 100.f))
	{
		nav_query.setStartPosition(nav_query.p1.p);
	}
	
	if (ImGui::DragFloat3("End position", &nav_query.p2.p.x, 0.01f, -100.f, 100.f))
	{
		nav_query.setEndPosition(nav_query.p2.p);
	}
	
	ImGui::Separator();
	
	ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "PARAMS:");
	ImGui::Separator();
	ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Rasterization:");
	
	ImGui::SliderFloat("Cell Size", &nav.m_rcConfigGUI.cs, 0.1f, 1.0f, "cs = %.3f");
	ImGui::SliderFloat("Cell Height", &nav.m_rcConfigGUI.ch, 0.1f, 1.0f, "ch = %.3f");
	ImGui::Separator();
	ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Agent:");
	
	ImGui::SliderFloat("Height", &nav.m_rcConfigGUI.walkableHeight, 0.1f, 5.0f, "walkableHeight = %.3f");
	ImGui::SliderFloat("Radius", &nav.m_rcConfigGUI.walkableRadius, 0.1f, 5.0f, "walkableRadius = %.3f");
	ImGui::SliderFloat("Max Climb", &nav.m_rcConfigGUI.walkableClimb, 0.0f, 5.0f, "walkableClimb = %.3f");
	ImGui::SliderFloat("Max Slope", &nav.m_rcConfigGUI.walkableSlopeAngle, 0.0f, 90.0f, "walkableSlopeAngle = %.3f");
	ImGui::Separator();
	ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Region:");
	ImGui::SliderFloat("Min Region Size", &nav.m_rcConfigGUI.minRegionArea, 0.0f, 150.0f, "minRegionArea = %.3f");
	ImGui::SliderFloat("Merged Region Size", &nav.m_rcConfigGUI.mergeRegionArea, 0.0f, 150.0f, "mergeRegionArea = %.3f");
	//ImGui::Separator();
	// ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Partitioning:");

	ImGui::Separator();
	ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Polygonization:");
	ImGui::SliderFloat("Max Edge Length", &nav.m_rcConfigGUI.maxEdgeLen, 0.0f, 50.0f, "maxEdgeLen = %.3f");
	ImGui::SliderFloat("Max Edge Error", &nav.m_rcConfigGUI.maxSimplificationError, 0.1f, 3.0f, "maxSimplificationError = %.3f");
	ImGui::SliderInt("Verts Per Poly", &nav.m_rcConfigGUI.maxVertsPerPoly, 3, 12, "maxVertsPerPoly = %.3f");
	ImGui::Separator();
	ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Detail Mesh:");
	ImGui::SliderFloat("Sample Dist", &nav.m_rcConfigGUI.detailSampleDist, 0.0f, 16.0f, "detailSampleDist = %.3f");
	ImGui::SliderFloat("Max Sample Error", &nav.m_rcConfigGUI.detailSampleMaxError, 0.0f, 16.0f, "detailSampleMaxError = %.3f");
}


void NavMeshModule::addNavMesh(std::string name, std::string tag)
{
	NavMesh* new_navMesh = new NavMesh;
	new_navMesh->nav_query.data = &new_navMesh->nav;
	new_navMesh->nav_query.resetTools();
	new_navMesh->setName(name);
	new_navMesh->setTag(tag);
	new_navMesh->nav.resetRecastConfigGUIValues();
	m_all_navmeshes.push_back(new_navMesh);
}

void NavMeshModule::renderGeometryNavmesh(std::string name)
{
	NavMesh* navMesh = getNavmeshByName(name);
	if(navMesh)
		navMesh->renderGeometryNavmesh();
}