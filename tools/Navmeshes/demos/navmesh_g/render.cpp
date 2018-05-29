#include "mcv_platform.h"
#include "windows/app.h"
#include "geometry/angular.h"

#include "resources/resources_manager.h"
#include "render/render.h"
#include "render/render_utils.h"
#include "shader_ctes.h"

CRender Render;

#pragma comment(lib, "d3d11.lib" )
#pragma comment(lib, "d3d9.lib" )
#pragma comment(lib, "dxguid.lib" )

IMGUI_API void        ImGui_ImplDX11_InvalidateDeviceObjects();

void CRender::destroyDevice()
{
	renderUtilsDestroy();
	if (ctx) ctx->ClearState();
	SAFE_RELEASE(depth_stencil_view);
	SAFE_RELEASE(depth_stencil);
	SAFE_RELEASE(render_target_view);
	swap_chain->SetFullscreenState(FALSE, NULL);
	SAFE_RELEASE(swap_chain);
	SAFE_RELEASE(ctx);

// Force removing all ImGui Objects
#ifdef _DEBUG
	ID3D11Debug* debug = nullptr;
	device->QueryInterface(__uuidof(ID3D11Debug), reinterpret_cast<void**>(&debug));
	if (debug)
	debug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
	debug->Release();
	// At this point search for objects with "Refcount: xx" where xx != 0 in the Output
#endif

	SAFE_RELEASE(device);
}

bool CRender::createDevice()
{
	CApp& app = CApp::get();

	HRESULT hr = S_OK;
	RECT rc;
	GetClientRect(app.getHWnd(), &rc);
	UINT width = rc.right - rc.left;
	UINT height = rc.bottom - rc.top;

	//MARC ANGEL: NEED TO BE IN ANOTHER PLACE
	std::ifstream json_file("data/config.json");
	json json_data;
	json_file >> json_data;
	bool FULLSCREEN = json_data.value("FULLSCREEN", false);
	//

	UINT createDeviceFlags = 0;
#ifdef _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_DRIVER_TYPE driverTypes[] =
	{
	  D3D_DRIVER_TYPE_HARDWARE,
	};
	UINT numDriverTypes = ARRAYSIZE(driverTypes);
	
	D3D_FEATURE_LEVEL featureLevels[] =
	{
	  D3D_FEATURE_LEVEL_11_0,
	};
	UINT numFeatureLevels = ARRAYSIZE(featureLevels);
	
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 1;
	sd.BufferDesc.Width = width;
	sd.BufferDesc.Height = height;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = app.getHWnd();
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	if (FULLSCREEN)
		sd.Windowed = FALSE;
	else
		sd.Windowed = TRUE;
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	hr = D3D11CreateDeviceAndSwapChain(NULL,
		D3D_DRIVER_TYPE_HARDWARE, NULL,
		createDeviceFlags,
		featureLevels,
		numFeatureLevels,
		D3D11_SDK_VERSION,
		&sd, &swap_chain, &device, &featureLevel, &ctx);
	if (!SUCCEEDED(hr))
		return false;
	
	// Create a render target view
	ID3D11Texture2D* pBackBuffer = NULL;
	hr = swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
	if (FAILED(hr))
		return false;

	hr = device->CreateRenderTargetView(pBackBuffer, NULL, &render_target_view);
	pBackBuffer->Release();
	if (FAILED(hr))
		return false;

	createDepthStencil("BackbufferDepthStencil", width, height, DXGI_FORMAT_D24_UNORM_S8_UINT,
		&depth_stencil,
		&depth_stencil_view,
		nullptr
	);

	render_width = width;
	render_height = height;

	// Hasta nuevo aviso, pintamos en el render_target_view del backbuffer
	// y el backbuffer que acabamos de crear
	renderToBackBuffer();

	printf("Render.device created\n");

	if (!renderUtilsCreate())
		return false;

	return true;
}

void CRender::renderToBackBuffer()
{
	ctx->OMSetRenderTargets(1, &Render.render_target_view, Render.depth_stencil_view);
	// Setup the viewport
	D3D11_VIEWPORT vp;
	vp.Width = (FLOAT)render_width;
	vp.Height = (FLOAT)render_height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	ctx->RSSetViewports(1, &vp);
}

void CRender::swapChain()
{
	swap_chain->Present(1, 0);
}

// --------------------------------------------
void CRender::drawPrimitive(const CRenderMesh* mesh, MAT44 world, VEC4 color) const
{
	assert(mesh);

	if (mesh->getVertexDecl()->vertex_type == CVertexDeclaration::VTX_TYPE_POS_NORMAL_UV)
	{
		auto tech = Resources.get("debugMesh.tech")->as< CRenderTechnique >();
		tech->activate();
	}
	else if (mesh->getVertexDecl()->vertex_type == CVertexDeclaration::VTX_TYPE_POS_COLOR)
	{
		auto tech = Resources.get("debug.tech")->as< CRenderTechnique >();
		tech->activate();
	}
	activateRSConfig(RSConfig::RSCFG_WIREFRAME);

	cte_object.World = world;
	cte_object.ObjColor = color;
	cte_object.uploadToGPU();
	cte_object.activate();
	mesh->activate();
	mesh->render();

	activateRSConfig(RSConfig::RSCFG_DEFAULT);
}

// --------------------------------------------
void CRender::drawPrimitiveSolid(const CRenderMesh* mesh, MAT44 world, VEC4 color) const {
	assert(mesh);

	if (mesh->getVertexDecl()->vertex_type == CVertexDeclaration::VTX_TYPE_POS_NORMAL_UV) {
		auto tech = Resources.get("debugMesh.tech")->as< CRenderTechnique >();
		tech->activate();
	}
	else if (mesh->getVertexDecl()->vertex_type == CVertexDeclaration::VTX_TYPE_POS_COLOR) {
		auto tech = Resources.get("debug.tech")->as< CRenderTechnique >();
		tech->activate();
	}
	activateRSConfig(RSConfig::RSCFG_CULL_NONE);

	cte_object.World = world;
	cte_object.ObjColor = color;
	cte_object.uploadToGPU();
	cte_object.activate();
	mesh->activate();
	mesh->render();

	activateRSConfig(RSConfig::RSCFG_DEFAULT);
}

void CRender::drawLine(const VEC3& src, const VEC3& dst, VEC4 color) const
{
	auto lineZ = Resources.get("unitLineZ.mesh")->as< CRenderMesh >();
	MAT44 world;
	float yaw, pitch;
	VEC3  delta = dst - src;
	getYawPitchFromVector(delta, &yaw, &pitch);
	MAT44 rot = MAT44::CreateFromYawPitchRoll(yaw, -pitch, 0.f);
	MAT44 scale = MAT44::CreateScale(delta.Length());
	world = rot * scale * MAT44::CreateTranslation(src);
	drawPrimitive(lineZ, world, color);
}

void CRender::drawPoints(const VEC3& src, VEC4 color) const
{
	auto lineZ = Resources.get("unitPoints.mesh")->as< CRenderMesh >();
	MAT44 world;
	
	world = MAT44::CreateTranslation(src);
	drawPrimitive(lineZ, world, color);
}

void CRender::drawSquare(const CTransform& transform,float x,float y, VEC4 color) const
{
	auto square = Resources.get("unitSquare.mesh")->as< CRenderMesh >();

	MAT44 world;
	world = transform.asMatrix();
	// width and height
	VEC3 scaleFactor;
	scaleFactor.x = x; // x
	scaleFactor.y = y; // y

	MAT44 unit_square_to_square = MAT44::CreateScale(scaleFactor) * world;

	drawPrimitive(square, unit_square_to_square, color);
}

void CRender::drawBox(const CTransform& transform, float x, float y, float z, VEC4 color) const
{
	auto box = Resources.get("wired_unit_cube.mesh")->as< CRenderMesh >();

	MAT44 world;
	world = transform.asMatrix();
	// width and height
	VEC3 scaleFactor;
	scaleFactor.x = x; 
	scaleFactor.y = y;
	scaleFactor.z = z; 

	MAT44 unit_square_to_square = MAT44::CreateScale(scaleFactor * 2.f) * world;

	drawPrimitive(box, unit_square_to_square, color);
}

void CRender::drawCircle(VEC3 center, float radius,VEC4 color) const
{
	const CRenderMesh* mesh_circle = Resources.get("circle.mesh")->as<CRenderMesh>();

	CTransform c_tmx;
	c_tmx.setPosition(center);
	c_tmx.setScale(VEC3(radius,0.0f,radius));

	drawPrimitive(mesh_circle, c_tmx.asMatrix(), color);
}

void CRender::drawWiredAABB(const AABB& aabb, MAT44 world, VEC4 color) const {
	// Accede a una mesh que esta centrada en el origen y
	// tiene 0.5 de half size
	auto mesh = Resources.get("wired_unit_cube.mesh")->as<CRenderMesh>();
	MAT44 unit_cube_to_aabb = MAT44::CreateScale(VEC3(aabb.Extents) * 2.f)
		* MAT44::CreateTranslation(aabb.Center)
		* world;
	drawPrimitiveSolid(mesh, unit_cube_to_aabb, color);
}


