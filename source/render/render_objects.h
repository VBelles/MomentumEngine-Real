#pragma once

bool createRenderObjects();
void destroyRenderObjects();

#include "render/cte_buffer.h"
#include "ctes.h"

extern CRenderCte<CCteCamera>  cb_camera;
extern CRenderCte<CCteObject>  cb_object;
extern CRenderCte<CCteLight>   cb_light;
extern CRenderCte<CCteGlobals> cb_globals;
extern CRenderCte<CCteBlur>    cb_blur;
extern CRenderCte<CCteGUI>	   cb_gui;
extern CRenderCte<CCteFog>	   cb_fog;

class CTexture;

struct TVtxPosClr {
	VEC3 pos;
	VEC4 color;
	TVtxPosClr() {}
	TVtxPosClr(VEC3 new_pos, VEC4 new_color) : pos(new_pos), color(new_color) {}
};

void activateCamera(CCamera& cam, int width, int height);
void setWorldTransform(MAT44 new_matrix, VEC4 color = VEC4(1, 1, 1, 1));
void renderMesh(const CRenderMesh* mesh, MAT44 new_matrix, VEC4 color = VEC4(1, 1, 1, 1));
void renderWiredAABB(const AABB& aabb, MAT44 world, VEC4 color);
void renderFullScreenQuad(const std::string& tech_name, const CTexture* texture);
void renderDots(TVtxPosClr vertices[], int numVertices);
void renderTriangles(TVtxPosClr vertices[], int numVertices);
void renderLines(TVtxPosClr vertices[], int numVertices);
void renderLine(VEC3 src, VEC3 dst, VEC4 color);
void renderWiredCube(CTransform* transform, const VEC3 halfExent, VEC4 color);
void renderCircle(CTransform* transform, float radius, VEC4 color);
void renderSphere(CTransform* transform, float radius, VEC4 color);

// Helper to create a depth stencil, returning all the dx objects
bool createDepthStencil(const std::string& aname,
                        int xres, int yres,
                        DXGI_FORMAT format,
                        ID3D11Texture2D** depth_stencil_resource,    // resulting resource
                        ID3D11DepthStencilView** depth_stencil_view, // RSV for this resource
                        CTexture** ztexture
);
