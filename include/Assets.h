#ifndef __ASSETS_H__
#define __ASSETS_H__

#include <map>
#include <string>
#include <dali-toolkit/dali-toolkit.h>
#include "obj-loader.h"

class Assets
{
    public:
        static void Init();
        static void LoadAllTextures();
        static void LoadAllObjs();
        static void LoadAllShaders();
        static bool GetObj(const std::string name, ObjLoader &buff);
        static bool GetTexture(const std::string name, Dali::Texture &buff);
        static bool GetShader(const std::string vertName, const std::string fragName, Dali::Shader &buff);

    private:
        static std::map<std::string, ObjLoader> mObjs;
        static std::map<std::string, Dali::PixelData> mImgs;
        static std::map<std::string, std::stringstream> mShaderCodes;
};

#endif