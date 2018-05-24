#pragma once

#include "DebugUtils/Include/DebugDraw.h"
#include "DebugUtils/Include/RecastDebugDraw.h"

struct TVtxPosClr {
	VEC3 pos;
	VEC4 color;
	TVtxPosClr() {}
	TVtxPosClr(VEC3 new_pos, VEC4 new_color) : pos(new_pos), color(new_color) {}
};

class NavMeshDebugDrawDX : public duDebugDraw {
private:
	duDebugDrawPrimitives   primitive;
	float                   primitive_size;

	static const int        MAX_VERTICES = 200000;
	TVtxPosClr              colored_vertices[MAX_VERTICES];
	int                     num_colored_vertices;
	int                     num_textured_colored_vertices;

public:
	// No sería necesario actualizar todo esto cada frame, no cambiará.

	virtual void depthMask(bool state);
	virtual void texture(bool state);
	virtual void begin(duDebugDrawPrimitives prim, float size = 1.0f);
	virtual void vertex(const float* pos, unsigned int color);
	virtual void vertex(const float x, const float y, const float z, unsigned int color);
	virtual void vertex(const float* pos, unsigned int color, const float* uv);
	virtual void vertex(const float x, const float y, const float z, unsigned int color, const float u, const float v);
	virtual void end();
};
