#include "GraphicsActor.h"

GraphicsActor::GraphicsActor(Dali::Stage &stage, Model &model)
    : FrameActor(stage)
{
	auto textures = model.GetTextures();
	for (auto tex : textures)
		_SetTextureSet(tex);
    _SetRenderer(model.GetShader(), model.GetGeometry());
    _actor.AddRenderer(_renderer);
}

void
GraphicsActor::_SetTextureSet(TextureInfo tex)
{
	_textureSet = Dali::TextureSet::New();

	// Dali::PixelData pixels = Dali::Toolkit::SyncImageLoader::Load( "../res/images/" + textureName );
	// Dali::Texture texture = Dali::Texture::New( Dali::TextureType::TEXTURE_2D, pixels.GetPixelFormat(), pixels.GetWidth(), pixels.GetHeight() );
	// texture.Upload( pixels, 0, 0, 0, 0, pixels.GetWidth(), pixels.GetHeight() );
	// _textureSet.SetTexture(0, texture);

	Dali::Texture texture;
	if (Assets::GetTexture(tex.name, texture))
	{
		// It seems that we can bind multiple textures
		_textureSet.SetTexture( tex.index, texture );
	}
	else
	{
		std::cout << "GraphicsActor : Failed to set texture." << std::endl;
	}
}

void GraphicsActor::_SetRenderer(Dali::Shader &shader, Dali::Geometry &geometry)
{
    _renderer = Dali::Renderer::New( geometry, shader );
	_renderer.SetTextures( _textureSet );
	_renderer.SetProperty( Dali::Renderer::Property::DEPTH_INDEX, 0.0f );
	_renderer.SetProperty( Dali::Renderer::Property::FACE_CULLING_MODE, Dali::FaceCullingMode::BACK );
	_renderer.SetProperty( Dali::Renderer::Property::DEPTH_WRITE_MODE, Dali::DepthWriteMode::ON );
    _renderer.SetProperty( Dali::Renderer::Property::BLEND_MODE, Dali::BlendMode::ON );
}