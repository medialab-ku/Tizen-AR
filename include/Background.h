#include <dali/dali.h>
#include <dali-toolkit/dali-toolkit.h>
#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <opencv2/opencv.hpp>
#include <algorithm>
#include "Assets.h"

class Background
{
    private:
        static Dali::Actor _bgActor;
        static Dali::TextureSet _bgTextureSet;

    private:
        static Dali::Geometry CreateBackgroundGeometry()
        {
            struct Vertex 
            {
                Dali::Vector2 vert;
                Dali::Vector2 texc;
            };

            Vertex backgroundVertices[] = {
                { Dali::Vector2(-1.0f, -1.0f), Dali::Vector2(0.0f, 1.0f) },
                { Dali::Vector2(-1.0f, 1.0f) , Dali::Vector2(0.0f, 0.0f) },
                { Dali::Vector2(1.0f, 1.0f)  , Dali::Vector2(1.0f, 0.0f) },
                { Dali::Vector2(1.0f, -1.0f) , Dali::Vector2(1.0f, 1.0f) },
                { Dali::Vector2(-1.0f, -1.0f) , Dali::Vector2(0.0f, 1.0f) },
                { Dali::Vector2(1.0f, 1.0f)  , Dali::Vector2(1.0f, 0.0f) },
            };

            Dali::PropertyBuffer vertexBuffer = Dali::PropertyBuffer::New( Dali::Property::Map()
                                                                .Add( "aPosition", Dali::Property::VECTOR2 )
                                                                .Add( "aTexCoord",  Dali::Property::VECTOR2 ) );
            vertexBuffer.SetData( backgroundVertices, sizeof(backgroundVertices) / sizeof(Vertex) );

            Dali::Geometry geo = Dali::Geometry::New();
            geo.AddVertexBuffer( vertexBuffer );
            geo.SetType( Dali::Geometry::TRIANGLES );
            return geo;
        }

        static Dali::PixelData CVMat2Pixel(cv::Mat& img)
        {
            uint32_t size = img.rows * img.cols * 4;
            uint8_t* buf = new uint8_t[size];
            std::copy(img.datastart, img.dataend, &buf[0]);
            return Dali::PixelData::New(buf, size, img.cols, img.rows, Dali::Pixel::RGB888, Dali::PixelData::DELETE_ARRAY);
        }

    public:
        static void Init()
        {
            Dali::Shader shader;
            if (not Assets::GetShader("vertexBackground.glsl", "fragmentBackground.glsl", shader))
            {
                std::cout << "Failed to load shader" << std::endl;
                return;
            }
            //auto shader = LoadShaders("vertexBackground.glsl", "fragmentBackground.glsl");

            auto geo = CreateBackgroundGeometry();
            auto renderer = Dali::Renderer::New(geo, shader);
            renderer.SetProperty( Dali::Renderer::Property::DEPTH_INDEX, 2.0f );
            // Enables the depth test.
            renderer.SetProperty( Dali::Renderer::Property::DEPTH_TEST_MODE, Dali::DepthTestMode::ON );
            // The fragment shader will run only those pixels that have the max depth value.
            renderer.SetProperty( Dali::Renderer::Property::DEPTH_FUNCTION, Dali::DepthFunction::LESS_EQUAL );

            auto stage = Dali::Stage::GetCurrent();

            _bgActor = Dali::Actor::New();
            _bgActor.SetName( "Background" );
            _bgActor.SetAnchorPoint( Dali::AnchorPoint::CENTER );
            _bgActor.SetParentOrigin( Dali::ParentOrigin::CENTER );
            _bgActor.AddRenderer( renderer );
            _bgActor.SetPosition( Dali::Vector3(0, 0, 1) );
            stage.Add(_bgActor);
        }



        static void UpdateBackgroundImg(std::string path)
        {
            Dali::PixelData pixels = Dali::Toolkit::SyncImageLoader::Load( path.c_str() );

            Dali::Texture texture = Dali::Texture::New( Dali::TextureType::TEXTURE_2D, pixels.GetPixelFormat(), pixels.GetWidth(), pixels.GetHeight() );
            texture.Upload( pixels, 0, 0, 0, 0, pixels.GetWidth(), pixels.GetHeight() );

            _bgTextureSet.SetTexture( 0, texture );
            _bgActor.GetRendererAt(0).SetTextures(_bgTextureSet);
        }

        static void UpdateBackgroundMat(cv::Mat img)
        {
            Dali::PixelData pixels = CVMat2Pixel(img);

            Dali::Texture texture = Dali::Texture::New( Dali::TextureType::TEXTURE_2D, pixels.GetPixelFormat(), pixels.GetWidth(), pixels.GetHeight() );
            texture.Upload( pixels, 0, 0, 0, 0, pixels.GetWidth(), pixels.GetHeight() );

            _bgTextureSet.SetTexture( 0, texture );
            _bgActor.GetRendererAt(0).SetTextures(_bgTextureSet);
        }
};