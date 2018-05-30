#pragma once

#include <DebugDraw.h>

#include "render/render_objects.h"

class NavMeshDebugDrawDX : public duDebugDraw {
private:
	duDebugDrawPrimitives   primitive;
	float                   primitive_size;

	static const int        MAX_VERTICES = 200000;
	TVtxPosClr              colored_vertices[MAX_VERTICES];
	int                     num_colored_vertices;
	int                     num_textured_colored_vertices;


public:
    // TODO: Do some calculations only on first render because data won't change.
    bool                    firstRender = true;

	virtual void depthMask(bool state);
	virtual void texture(bool state);
	virtual void begin(duDebugDrawPrimitives prim, float size = 1.0f);
	virtual void vertex(const float* pos, unsigned int color);
	virtual void vertex(const float x, const float y, const float z, unsigned int color);
	virtual void vertex(const float* pos, unsigned int color, const float* uv);
	virtual void vertex(const float x, const float y, const float z,
                        unsigned int color, const float u, const float v);
	virtual void end();
};
