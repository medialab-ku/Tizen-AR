#include "Model.h"

Model::Model(Dali::Shader &shader)
    : _shader(shader),
      _textures()
{

}

void
Model::AddTexture(int index, const std::string name)
{
    TextureInfo info;
    info.index = index;
    info.name = name;
    _textures.push_back(info);
}