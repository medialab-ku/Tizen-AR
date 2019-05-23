#ifndef __MODEL_H__
#define __MODEL_H__

#include <dali-toolkit/dali-toolkit.h>
#include <string>
#include <vector>
#include "Assets.h"

struct TextureInfo
{
	int index;
	std::string name;
};

/*
 * Container of graphics data
 */
class Model
{
	public:
		Model(Dali::Shader &shader);
		Dali::Shader& GetShader() { return _shader; }
		Dali::Geometry& GetGeometry() { return _geometry; }
		std::vector<TextureInfo> GetTextures() { return _textures; }
		void AddTexture(int index, const std::string name);
		void SetShader(const Dali::Shader &shader) { _shader = shader; }
		void SetGeometry(const Dali::Geometry geometry) { _geometry = geometry; }

	private:
		std::vector<TextureInfo> _textures;
		Dali::Shader _shader;
		Dali::Geometry _geometry;
};

#endif