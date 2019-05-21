#include "Background.h"
#include "Shader.h"
#include <algorithm>

using namespace Dali;
using namespace Toolkit;
using namespace std;

Actor mBackgroundActor;
TextureSet mBackgroundTextureSet;

Geometry CreateBackgroundGeometry()
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

    PropertyBuffer vertexBuffer = PropertyBuffer::New( Property::Map()
                                                        .Add( "aPosition", Property::VECTOR2 )
                                                        .Add( "aTexCoord",  Property::VECTOR2 ) );
    vertexBuffer.SetData( backgroundVertices, sizeof(backgroundVertices) / sizeof(Vertex) );

    Geometry mBackgroundGeometry = Geometry::New();
    mBackgroundGeometry.AddVertexBuffer( vertexBuffer );
    mBackgroundGeometry.SetType( Geometry::TRIANGLES );
    return mBackgroundGeometry;
}

void DisplayBackground(PixelData& pixels, Geometry& geo)
{
    Texture texture = Texture::New( TextureType::TEXTURE_2D, pixels.GetPixelFormat(), pixels.GetWidth(), pixels.GetHeight() );
    texture.Upload( pixels, 0, 0, 0, 0, pixels.GetWidth(), pixels.GetHeight() );

    mBackgroundTextureSet = TextureSet::New();
    mBackgroundTextureSet.SetTexture( 0, texture );

    Shader mShaderBackground = LoadShaders("vertexBackground.glsl", "fragmentBackground.glsl");

    Renderer mBackgroundRenderer = Renderer::New(geo, mShaderBackground );
    mBackgroundRenderer.SetTextures( mBackgroundTextureSet );
    mBackgroundRenderer.SetProperty( Renderer::Property::DEPTH_INDEX, 2.0f );

    // Enables the depth test.
    mBackgroundRenderer.SetProperty( Renderer::Property::DEPTH_TEST_MODE, DepthTestMode::ON );

    // The fragment shader will run only is those pixels that have the max depth value.
    mBackgroundRenderer.SetProperty( Renderer::Property::DEPTH_FUNCTION, DepthFunction::LESS_EQUAL );

    Stage stage = Stage::GetCurrent();
    CameraActor Camera = stage.GetRenderTaskList().GetTask(0).GetCameraActor();

    mBackgroundActor = Dali::Actor::New();
    mBackgroundActor.SetName( "Background" );
    mBackgroundActor.SetAnchorPoint( Dali::AnchorPoint::CENTER );
    mBackgroundActor.SetParentOrigin( Dali::ParentOrigin::CENTER );
    mBackgroundActor.AddRenderer( mBackgroundRenderer );
    //Camera.Add( mBackgroundActor );
    //mBackgroundActor.SetInheritPosition(true);
    mBackgroundActor.SetPosition( Dali::Vector3(0, 0, 1) );
    stage.Add(mBackgroundActor);
}

PixelData CVMat2Pixel(cv::Mat& img)
{
    uint32_t size = img.rows * img.cols * 4;
    uint8_t* buf = new uint8_t[size];
    copy(img.datastart, img.dataend, &buf[0]);
    return PixelData::New(buf, size, img.cols, img.rows, Pixel::RGB888, PixelData::DELETE_ARRAY);
}

void CreateBackgroundMat(cv::Mat& img)
{
    Geometry geo = CreateBackgroundGeometry();
    PixelData pixels = CVMat2Pixel(img);
    DisplayBackground(pixels, geo);
}

void CreateBackgroundImg(string path)
{
    Geometry geo = CreateBackgroundGeometry();
    PixelData pixels = SyncImageLoader::Load( path.c_str() );
    DisplayBackground(pixels, geo);
}

void UpdateBackgroundImg(string path)
{
    PixelData pixels = SyncImageLoader::Load( path.c_str() );

    Texture texture = Texture::New( TextureType::TEXTURE_2D, pixels.GetPixelFormat(), pixels.GetWidth(), pixels.GetHeight() );
    texture.Upload( pixels, 0, 0, 0, 0, pixels.GetWidth(), pixels.GetHeight() );

    mBackgroundTextureSet.SetTexture( 0, texture );
    mBackgroundActor.GetRendererAt(0).SetTextures(mBackgroundTextureSet);
}

void UpdateBackgroundMat(cv::Mat img)
{
    PixelData pixels = CVMat2Pixel(img);

    Texture texture = Texture::New( TextureType::TEXTURE_2D, pixels.GetPixelFormat(), pixels.GetWidth(), pixels.GetHeight() );
    texture.Upload( pixels, 0, 0, 0, 0, pixels.GetWidth(), pixels.GetHeight() );

    mBackgroundTextureSet.SetTexture( 0, texture );
    mBackgroundActor.GetRendererAt(0).SetTextures(mBackgroundTextureSet);
}