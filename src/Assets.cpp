#include "Assets.h"
#include <dirent.h>
#include <sys/types.h>
#include <iostream>
#include <fstream>
#include <sstream>

std::map<std::string, Dali::PixelData> Assets::mImgs;
std::map<std::string, ObjLoader> Assets::mObjs;

void
Assets::Init()
{
    LoadAllTextures();
    LoadAllObjs();
}

void
Assets::LoadAllTextures()
{
    std::string tex_path = "../res/images/";
    struct dirent *entry;

    std::cout << "Loading textures..." << std::endl;
    DIR *dir = opendir(tex_path.c_str());
    if (dir == NULL) 
    {
        return;
    }
    while ((entry = readdir(dir)) != NULL) 
    {
        try
        {
            if (entry->d_name[0] == '.') continue;
            std::cout << entry->d_name << std::endl;
            std::string full_path = tex_path + std::string(entry->d_name);

            Dali::PixelData pixels = Dali::Toolkit::SyncImageLoader::Load( full_path );
            mImgs.insert(std::make_pair(std::string(entry->d_name), pixels));
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
        }
    }

    closedir(dir);
}

void
Assets::LoadAllObjs()
{
    std::string obj_path = "../res/models/";
    struct dirent *entry;

    std::cout << "Loading models..." << std::endl;
    DIR *dir = opendir(obj_path.c_str());
    if (dir == NULL) 
    {
        return;
    }
    while ((entry = readdir(dir)) != NULL) 
    {
        try
        {
            if (entry->d_name[0] == '.') continue;
            std::cout << entry->d_name << std::endl;
            std::string full_path = obj_path + std::string(entry->d_name);

            ObjLoader obj = ObjLoader();
            std::ifstream file(full_path, std::ios::binary | std::ios::ate);
            std::vector<char> data;
            size_t size = file.tellg();
            data.resize(size);
            file.seekg(0, std::ios::beg);
            file.read(&data[0], size);
            obj.LoadObject(&data[0], size);

            mObjs.insert(std::make_pair(std::string(entry->d_name), obj));
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
        }
    }

    closedir(dir);
}

void
Assets::LoadAllShaders()
{
    std::string shader_path = "../res/shaders/";
    struct dirent *entry;

    std::cout << "Loading shaders..." << std::endl;
    DIR *dir = opendir(shader_path.c_str());
    if (dir == NULL) 
    {
        return;
    }
    while ((entry = readdir(dir)) != NULL) 
    {
        try
        {
            if (entry->d_name[0] == '.') continue;
            std::cout << entry->d_name << std::endl;
            std::string full_path = shader_path + std::string(entry->d_name);

            std::ifstream file;
            std::stringstream stream;
            file.exceptions (std::ifstream::failbit | std::ifstream::badbit);
            file.open(full_path.c_str());
		    stream << file.rdbuf();
		    file.close();
            
            mShaderCodes.insert(std::make_pair(std::string(entry->d_name), stream));
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
        }
    }

    closedir(dir);
}

bool
Assets::GetTexture(const std::string name, Dali::Texture &buff)
{
    if (mImgs.count(name) == 0)
    {
        return false;
    } 
    else
    {
        auto img = mImgs.find(name)->second;
        buff = Texture::New( TextureType::TEXTURE_2D, img.GetPixelFormat(), img.GetWidth(), img.GetHeight() );
        buff.Upload( img, 0, 0, 0, 0, img.GetWidth(), img.GetHeight() );
        return true;
    }
}

bool
Assets::GetObj(const std::string name, ObjLoader &buff)
{
    if (mObjs.count(name) == 0)
    {
        return false;
    }
    else
    {
        buff = mObjs.find(name)->second;
        return true;
    }
}

bool 
Assets::GetShader(const std::string vertName, const std::string fragName, Dali::Shader &buff)
{
    if (mShaderCodes.count(vertName) == 0
        || mShaderCodes.count(fragName) == 0)
    {
        return false;
    }
    else
    {
        auto vertCode = mShaderCodes.find(vertName)->second;
        auto fragCode = mShaderCodes.find(fragName)->second;
        buff = Dali::Shader::New(vertCode.str().c_str(), fragCode.str().c_str());
        return true;
    }
    
}