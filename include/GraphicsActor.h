#ifndef __GRAPHICSACTOR_H__
#define __GRAPHICSACTOR_H__

#include "FrameActor.h"
#include "Model.h"

/**
 * FrameActor that can draw geometry
 * The geometry info is passed through constructor as a form of Model instance
 */
class GraphicsActor : public FrameActor
{
public:
	GraphicsActor(Dali::Stage &stage, Model &model);

private:
	void _SetTextureSet(TextureInfo tex);
	void _SetRenderer(Dali::Shader &shader, Dali::Geometry &geometry);

private:
	Dali::Shader mShader;
	Dali::Geometry mGeometry;
	Dali::TextureSet mTextureSet;
	Dali::Renderer mRenderer;
};

#endif