#include "mcv_platform.h"
#include "navmesh_render.h"

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
    //if (firstRender)
        num_colored_vertices = 0;
}

void NavMeshDebugDrawDX::vertex(const float* pos, unsigned int color) {
    //if (!firstRender) return;
	assert(num_colored_vertices < MAX_VERTICES - 1);
	colored_vertices[num_colored_vertices] = TVtxPosClr(VEC3(pos[0], pos[1], pos[2]), VEC4(color));
	++num_colored_vertices;
}

void NavMeshDebugDrawDX::vertex(const float x, const float y, const float z,
                                unsigned int color) {
    //if (!firstRender) return;
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

void NavMeshDebugDrawDX::vertex(const float x, const float y, const float z,
                                unsigned int color, const float u, const float v) {
	vertex(x, y, z, color);
	/*assert( num_textured_colored_vertices < MAX_VERTICES - 1 );
	textured_colored_vertices[ num_textured_colored_vertices ].assign( x, y, z, color, u, v );
	++num_textured_colored_vertices;*/
}

void NavMeshDebugDrawDX::end() {
	if (primitive == DU_DRAW_POINTS) {
		int numPoints = num_colored_vertices;
		TVtxPosClr vs[1];
		for (int i = 0; i < numPoints; ++i) {
			TVtxPosClr& v1 = colored_vertices[i];
			vs[0] = TVtxPosClr(VEC3(v1.pos.x, v1.pos.y, v1.pos.z), v1.color);

			renderDots(VEC3(vs[0].pos.x, vs[0].pos.y, vs[0].pos.z),
					   VEC4(1, 0, 0, 1));
		}

		//g_pd3dDevice->DrawPrimitiveUP(D3DPT_POINTLIST, num_colored_vertices, colored_vertices, sizeof(TVtxPosClr));
	}
	else if (primitive == DU_DRAW_LINES) {
		int pairsOfVertices = num_colored_vertices / 2;
		TVtxPosClr vs[2];
        for (int i = 0; i < pairsOfVertices; ++i) {
			int idx = i * 2;

			TVtxPosClr v1 = colored_vertices[idx];
			TVtxPosClr v2 = colored_vertices[idx + 1];

			vs[0] = TVtxPosClr(VEC3(v1.pos.x, v1.pos.y, v1.pos.z), v1.color);
			vs[1] = TVtxPosClr(VEC3(v2.pos.x, v2.pos.y, v2.pos.z), v1.color);

			renderLine(VEC3(vs[0].pos.x, vs[0].pos.y, vs[0].pos.z),
					   VEC3(vs[1].pos.x, vs[1].pos.y, vs[1].pos.z),
					   VEC4(1, 0, 0, 1));
        }
		//g_pd3dDevice->DrawPrimitiveUP(D3DPT_LINELIST, num_colored_vertices / 2, colored_vertices, sizeof(TVtxPosClr));
	}
	else if (primitive == DU_DRAW_TRIS) {
		//g_pd3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST, num_colored_vertices / 3, colored_vertices, sizeof(TVtxPosClr));
	}
	else if (primitive == DU_DRAW_QUADS) {
		int num_quads = num_colored_vertices / 4;
        TVtxPosClr vs[6];

		for (int i = 0; i < num_quads; ++i) {
			int idx = i * 4;
			TVtxPosClr& v1 = colored_vertices[idx];
			TVtxPosClr& v2 = colored_vertices[idx + 1];
			TVtxPosClr& v3 = colored_vertices[idx + 2];
			TVtxPosClr& v4 = colored_vertices[idx + 3];

            vs[0] = TVtxPosClr(VEC3(v1.pos.x, v1.pos.y, v1.pos.z), v1.color);
            vs[1] = TVtxPosClr(VEC3(v3.pos.x, v3.pos.y, v3.pos.z), v3.color);
            vs[2] = TVtxPosClr(VEC3(v2.pos.x, v2.pos.y, v2.pos.z), v2.color);

            vs[3] = TVtxPosClr(VEC3(v1.pos.x, v1.pos.y, v1.pos.z), v1.color);
            vs[4] = TVtxPosClr(VEC3(v4.pos.x, v4.pos.y, v4.pos.z), v4.color);
            vs[5] = TVtxPosClr(VEC3(v2.pos.x, v2.pos.y, v2.pos.z), v2.color);

			renderLine(VEC3(vs[0].pos.x, vs[0].pos.y, vs[0].pos.z),
					   VEC3(vs[1].pos.x, vs[1].pos.y, vs[1].pos.z),
					   VEC4(1, 0, 0, 1));
			//g_pd3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 2, vs, sizeof(TVtxPosClr));
		}
	}
	//g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	//g_pd3dDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	num_colored_vertices = 0;
	num_textured_colored_vertices = 0;
    firstRender = false;
}
