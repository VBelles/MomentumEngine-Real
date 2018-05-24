#include "mcv_platform.h"
#include "navmesh_render.h"
#include "DebugUtils/Include/DetourDebugDraw.h"

#include <stdio.h>
#include <stdarg.h>
#include <assert.h>

void NavMeshDebugDrawDX::depthMask(bool state) {
	/*if( state )
	  Renderer.enableZWrite( );
	else
	  Renderer.disableZWrite( );*/
}

void NavMeshDebugDrawDX::texture(bool state) {
	/*if (state)
	{
		glEnable(GL_TEXTURE_2D);
		g_tex.bind();
	}
	else
	{
		glDisable(GL_TEXTURE_2D);
	}*/
}

void NavMeshDebugDrawDX::begin(duDebugDrawPrimitives prim, float size) {
	primitive = prim;
	primitive_size = size;
	num_colored_vertices = 0;
}

void NavMeshDebugDrawDX::vertex(const float* pos, unsigned int color) {
	assert(num_colored_vertices < MAX_VERTICES - 1);
	colored_vertices[num_colored_vertices] = TVtxPosClr(VEC3(pos[0], pos[1], pos[2]), VEC4(color));
	++num_colored_vertices;
}

void NavMeshDebugDrawDX::vertex(const float x, const float y, const float z, unsigned int color) {
	assert(num_colored_vertices < MAX_VERTICES - 1);
	colored_vertices[num_colored_vertices] = TVtxPosClr(VEC3(x, y, z), VEC4(color));
	++num_colored_vertices;
}

void NavMeshDebugDrawDX::vertex(const float* pos, unsigned int color, const float* uv) {
	vertex(pos, color);
	/*assert( num_textured_colored_vertices < MAX_VERTICES-1 );
	textured_colored_vertices[ num_textured_colored_vertices ].assign( pos[0], pos[1], pos[2], color, uv[0], uv[1] );
	++num_textured_colored_vertices;*/
}

void NavMeshDebugDrawDX::vertex(const float x, const float y, const float z, unsigned int color, const float u, const float v) {
	vertex(x, y, z, color);
	/*assert( num_textured_colored_vertices < MAX_VERTICES - 1 );
	textured_colored_vertices[ num_textured_colored_vertices ].assign( x, y, z, color, u, v );
	++num_textured_colored_vertices;*/
}

void NavMeshDebugDrawDX::end() {
	g_pd3dDevice->SetTexture(0, NULL);
	g_pd3dDevice->SetFVF(D3DFVF_CUSTOMVERTEX);
	g_pd3dDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	g_pd3dDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	g_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	g_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	if (primitive == DU_DRAW_POINTS) {
		g_pd3dDevice->DrawPrimitiveUP(D3DPT_POINTLIST, num_colored_vertices, colored_vertices, sizeof(CUSTOMVERTEX));
	}
	else if (primitive == DU_DRAW_LINES) {
		g_pd3dDevice->DrawPrimitiveUP(D3DPT_LINELIST, num_colored_vertices / 2, colored_vertices, sizeof(CUSTOMVERTEX));
	}
	else if (primitive == DU_DRAW_TRIS) {
		g_pd3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST, num_colored_vertices / 3, colored_vertices, sizeof(CUSTOMVERTEX));
	}
	else if (primitive == DU_DRAW_QUADS) {
		int num_quads = num_colored_vertices / 4;
		CUSTOMVERTEX vs[6];

		for (int i = 0; i < num_quads; ++i) {
			int idx = i * 4;
			CUSTOMVERTEX& v1 = colored_vertices[idx];
			CUSTOMVERTEX& v2 = colored_vertices[idx + 1];
			CUSTOMVERTEX& v3 = colored_vertices[idx + 2];
			CUSTOMVERTEX& v4 = colored_vertices[idx + 3];

			vs[0].assign(v1.x, v1.y, v1.z, v1.color);
			vs[1].assign(v3.x, v3.y, v3.z, v3.color);
			vs[2].assign(v2.x, v2.y, v2.z, v2.color);

			vs[3].assign(v1.x, v1.y, v1.z, v1.color);
			vs[4].assign(v4.x, v4.y, v4.z, v4.color);
			vs[5].assign(v2.x, v2.y, v2.z, v2.color);

			g_pd3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 2, vs, sizeof(CUSTOMVERTEX));
		}
	}
	g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	g_pd3dDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	num_colored_vertices = 0;
	num_textured_colored_vertices = 0;
}
