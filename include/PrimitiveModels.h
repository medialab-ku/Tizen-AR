#ifndef __PRIMITIVE_MODEL_H__
#define __PRIMITIVE_MODEL_H__

#include "Model.h"
#include "obj-loader.h"
using namespace Dali;

namespace {

const char *VERTEX_SHADER = DALI_COMPOSE_SHADER(
    attribute mediump vec3 aPosition;\n        // DALi shader builtin
        attribute mediump vec3 aColor;\n       // DALi shader builtin
            uniform mediump mat4 uMvpMatrix;\n // DALi shader builtin
                uniform mediump vec3 uSize;\n // DALi shader builtin
\n
                    varying mediump vec4 vColor;\n
\n void main()\n {
                      \n
                          mediump vec4 vertexPosition = vec4(aPosition, 1.0);
                      \n
                          vertexPosition.xyz *= uSize;
                      \n
                          vColor = vec4(aColor, 1.0);
                      \n
                          gl_Position = uMvpMatrix * vertexPosition;
                      \n
                    }\n);

/*
 * Fragment shader
 */
const char *FRAGMENT_SHADER = DALI_COMPOSE_SHADER(
    varying mediump vec4 vColor;\n
\n void main()\n {
      \n
          gl_FragColor = vColor;
      \n
    }\n);

struct Vertex_Textured
{
	Vector3 aPosition;
	Vector2 aTexCoord;
	Vector3 aNormal;
};

static Vertex_Textured texCubeVertices[] = {
	{ Vector3(  1.0f,-1.0f,-1.0f ), Vector2( 1.0, 1.0 ), Vector3(0.0, 0.0, -1.0) },
	{ Vector3( -1.0f, 1.0f,-1.0f ), Vector2( 0.0, 0.0 ), Vector3(0.0, 0.0, -1.0) },
	{ Vector3(  1.0f, 1.0f,-1.0f ), Vector2( 0.0, 1.0 ), Vector3(0.0, 0.0, -1.0) },
	{ Vector3( -1.0f, 1.0f, 1.0f ), Vector2( 1.0, 1.0 ), Vector3(0.0, 0.0, 1.0)  },
	{ Vector3(  1.0f,-1.0f, 1.0f ), Vector2( 0.0, 0.0 ), Vector3(0.0, 0.0, 1.0)  },
	{ Vector3(  1.0f, 1.0f, 1.0f ), Vector2( 0.0, 1.0 ), Vector3(0.0, 0.0, 1.0)  },
	{ Vector3(  1.0f, 1.0f, 1.0f ), Vector2( 1.0, 1.0 ), Vector3(1.0, 0.0, 0.0)  },
	{ Vector3(  1.0f,-1.0f,-1.0f ), Vector2( 0.0, 0.0 ), Vector3(1.0, 0.0, 0.0)  },
	{ Vector3(  1.0f, 1.0f,-1.0f ), Vector2( 0.0, 1.0 ), Vector3(1.0, 0.0, 0.0)  },
	{ Vector3(  1.0f,-1.0f, 1.0f ), Vector2( 1.0, 1.0 ), Vector3(0.0, -1.0, 0.0) },
	{ Vector3( -1.0f,-1.0f,-1.0f ), Vector2( 0.0, 0.0 ), Vector3(0.0, -1.0, 0.0) },
	{ Vector3(  1.0f,-1.0f,-1.0f ), Vector2( 0.0, 1.0 ), Vector3(0.0, -1.0, 0.0) },
	{ Vector3( -1.0f,-1.0f,-1.0f ), Vector2( 1.0, 1.0 ), Vector3(-1.0, 0.0, 0.0) },
	{ Vector3( -1.0f, 1.0f, 1.0f ), Vector2( 0.0, 0.0 ), Vector3(-1.0, 0.0, 0.0) },
	{ Vector3( -1.0f, 1.0f,-1.0f ), Vector2( 0.0, 1.0 ), Vector3(-1.0, 0.0, 0.0) },
	{ Vector3(  1.0f, 1.0f,-1.0f ), Vector2( 1.0, 1.0 ), Vector3(0.0, 1.0, 0.0)  },
	{ Vector3( -1.0f, 1.0f, 1.0f ), Vector2( 0.0, 0.0 ), Vector3(0.0, 1.0, 0.0)  },
	{ Vector3(  1.0f, 1.0f, 1.0f ), Vector2( 0.0, 1.0 ), Vector3(0.0, 1.0, 0.0)  },
	{ Vector3(  1.0f,-1.0f,-1.0f ), Vector2( 1.0, 1.0 ), Vector3(0.0, 0.0, -1.0) },
	{ Vector3( -1.0f,-1.0f,-1.0f ), Vector2( 1.0, 0.0 ), Vector3(0.0, 0.0, -1.0) },
	{ Vector3( -1.0f, 1.0f,-1.0f ), Vector2( 0.0, 0.0 ), Vector3(0.0, 0.0, -1.0) },
	{ Vector3( -1.0f, 1.0f, 1.0f ), Vector2( 1.0, 1.0 ), Vector3(0.0, 0.0, 1.0)  },
	{ Vector3( -1.0f,-1.0f, 1.0f ), Vector2( 1.0, 0.0 ), Vector3(0.0, 0.0, 1.0)  },
	{ Vector3(  1.0f,-1.0f, 1.0f ), Vector2( 0.0, 0.0 ), Vector3(0.0, 0.0, 1.0)  },
	{ Vector3(  1.0f, 1.0f, 1.0f ), Vector2( 1.0, 1.0 ), Vector3(1.0, 0.0, 0.0)  },
	{ Vector3(  1.0f,-1.0f, 1.0f ), Vector2( 1.0, 0.0 ), Vector3(1.0, 0.0, 0.0)  },
	{ Vector3(  1.0f,-1.0f,-1.0f ), Vector2( 0.0, 0.0 ), Vector3(1.0, 0.0, 0.0)  },
	{ Vector3(  1.0f,-1.0f, 1.0f ), Vector2( 1.0, 1.0 ), Vector3(0.0, -1.0, 0.0) },
	{ Vector3( -1.0f,-1.0f, 1.0f ), Vector2( 1.0, 0.0 ), Vector3(0.0, -1.0, 0.0) },
	{ Vector3( -1.0f,-1.0f,-1.0f ), Vector2( 0.0, 0.0 ), Vector3(0.0, -1.0, 0.0) },
	{ Vector3( -1.0f,-1.0f,-1.0f ), Vector2( 1.0, 1.0 ), Vector3(-1.0, 0.0, 0.0) },
	{ Vector3( -1.0f,-1.0f, 1.0f ), Vector2( 1.0, 0.0 ), Vector3(-1.0, 0.0, 0.0) },
	{ Vector3( -1.0f, 1.0f, 1.0f ), Vector2( 0.0, 0.0 ), Vector3(-1.0, 0.0, 0.0) },
	{ Vector3(  1.0f, 1.0f,-1.0f ), Vector2( 1.0, 1.0 ), Vector3(0.0, 1.0, 0.0)  },
	{ Vector3( -1.0f, 1.0f,-1.0f ), Vector2( 1.0, 0.0 ), Vector3(0.0, 1.0, 0.0)  },
	{ Vector3( -1.0f, 1.0f, 1.0f ), Vector2( 0.0, 0.0 ), Vector3(0.0, 1.0, 0.0)  },
};


static const unsigned short texCubeIndex[] = {
	// 2, 1, 0,
	// 5, 4, 3,
	// 8, 7, 6,
	// 11, 10, 9,
	// 14, 13, 12,
	// 17, 16, 15,
	// 20, 19, 18,
	// 23, 22, 21,
	// 26, 25, 24,
	// 29, 28, 27,
	// 32, 31, 30,
	// 35, 34, 33
    2, 0, 1, 
    5, 3, 4, 
    8, 6, 7, 
    11, 9, 10, 
    14, 12, 13, 
    17, 15, 16, 
    20, 18, 19, 
    23, 21, 22, 
    26, 24, 25, 
    29, 27, 28, 
    32, 30, 31, 
    35, 33, 34 
};

static Vertex_Textured planeVertices[] = {
	{Vector3(1.0f, 0.0f, 1.0f),   Vector2(1.0, 1.0), Vector3(0.0, -1.0, 0.0)},
	{Vector3(-1.0f, 0.0f, -1.0f), Vector2(0.0, 0.0), Vector3(0.0, -1.0, 0.0)},
	{Vector3(1.0f, 0.0f, -1.0f),  Vector2(1.0, 0.0), Vector3(0.0, -1.0, 0.0)},
	{Vector3(-1.0f, 0.0f, 1.0f),  Vector2(0.0, 1.0), Vector3(0.0, -1.0, 0.0)},
};

static const unsigned short planeIndex[] = {
	2, 1, 0,
	0, 1, 3,
	3, 1, 0,
	0, 1, 2
};

struct Vertex
    {
      Vector3 aPosition;
      Vector3 aColor;
    };

    const Vector3 COLOR0( 1.0f, 1.0f, 0.0f );
    const Vector3 COLOR1( 0.0f, 1.0f, 1.0f );
    const Vector3 COLOR2( 1.0f, 0.0f, 1.0f );
    const Vector3 COLOR3( 0.0f, 1.0f, 0.0f );
    const Vector3 COLOR4( 0.0f, 0.0f, 1.0f );
    const Vector3 COLOR5( 1.0f, 0.0f, 0.0f );

    Vertex vertices[] = {
      { Vector3(  1.0f,-1.0f,-1.0f ), COLOR5 },
      { Vector3( -1.0f, 1.0f,-1.0f ), COLOR5 },
      { Vector3(  1.0f, 1.0f,-1.0f ), COLOR5 },
      { Vector3( -1.0f, 1.0f, 1.0f ), COLOR3 },
      { Vector3(  1.0f,-1.0f, 1.0f ), COLOR3 },
      { Vector3(  1.0f, 1.0f, 1.0f ), COLOR3 },
      { Vector3(  1.0f, 1.0f, 1.0f ), COLOR4 },
      { Vector3(  1.0f,-1.0f,-1.0f ), COLOR4 },
      { Vector3(  1.0f, 1.0f,-1.0f ), COLOR4 },
      { Vector3(  1.0f,-1.0f, 1.0f ), COLOR1 },
      { Vector3( -1.0f,-1.0f,-1.0f ), COLOR1 },
      { Vector3(  1.0f,-1.0f,-1.0f ), COLOR1 },
      { Vector3( -1.0f,-1.0f,-1.0f ), COLOR0 },
      { Vector3( -1.0f, 1.0f, 1.0f ), COLOR0 },
      { Vector3( -1.0f, 1.0f,-1.0f ), COLOR0 },
      { Vector3(  1.0f, 1.0f,-1.0f ), COLOR2 },
      { Vector3( -1.0f, 1.0f, 1.0f ), COLOR2 },
      { Vector3(  1.0f, 1.0f, 1.0f ), COLOR2 },
      { Vector3(  1.0f,-1.0f,-1.0f ), COLOR5 },
      { Vector3( -1.0f,-1.0f,-1.0f ), COLOR5 },
      { Vector3( -1.0f, 1.0f,-1.0f ), COLOR5 },
      { Vector3( -1.0f, 1.0f, 1.0f ), COLOR3 },
      { Vector3( -1.0f,-1.0f, 1.0f ), COLOR3 },
      { Vector3(  1.0f,-1.0f, 1.0f ), COLOR3 },
      { Vector3(  1.0f, 1.0f, 1.0f ), COLOR4 },
      { Vector3(  1.0f,-1.0f, 1.0f ), COLOR4 },
      { Vector3(  1.0f,-1.0f,-1.0f ), COLOR4 },
      { Vector3(  1.0f,-1.0f, 1.0f ), COLOR1 },
      { Vector3( -1.0f,-1.0f, 1.0f ), COLOR1 },
      { Vector3( -1.0f,-1.0f,-1.0f ), COLOR1 },
      { Vector3( -1.0f,-1.0f,-1.0f ), COLOR0 },
      { Vector3( -1.0f,-1.0f, 1.0f ), COLOR0 },
      { Vector3( -1.0f, 1.0f, 1.0f ), COLOR0 },
      { Vector3(  1.0f, 1.0f,-1.0f ), COLOR2 },
      { Vector3( -1.0f, 1.0f,-1.0f ), COLOR2 },
      { Vector3( -1.0f, 1.0f, 1.0f ), COLOR2 },
    };

    // create indices
    // const unsigned short INDEX_CUBE[] = {
    //   2, 1, 0,
    //   5, 4, 3,
    //   8, 7, 6,
    //   11, 10, 9,
    //   14, 13, 12,
    //   17, 16, 15,
    //   20, 19, 18,
    //   23, 22, 21,
    //   26, 25, 24,
    //   29, 28, 27,
    //   32, 31, 30,
    //   35, 34, 33
    // };

    const unsigned short INDEX_CUBE[] = {
      2, 0, 1, 
      5, 3, 4, 
      8, 6, 7, 
      11, 9, 10, 
      14, 12, 13, 
      17, 15, 16, 
      20, 18, 19, 
      23, 21, 22, 
      26, 24, 25, 
      29, 27, 28, 
      32, 30, 31, 
      35, 33, 34 
    };

class PrimitiveCube : public Model
{
public:
	PrimitiveCube(std::string textureName, Dali::Shader &shader)
		: Model(shader)
	{
    AddTexture(0, textureName);
		PropertyBuffer vertexBuffer = PropertyBuffer::New( Property::Map()
                                                       .Add( "aPosition", Property::VECTOR3 )
                                                       .Add( "aColor", Property::VECTOR3 ) );
    	vertexBuffer.SetData( vertices, sizeof(vertices) / sizeof(Vertex) );
		Geometry geometry = Geometry::New();
    	geometry.AddVertexBuffer( vertexBuffer );
    	geometry.SetIndexBuffer( INDEX_CUBE,
                              	sizeof(INDEX_CUBE)/sizeof(INDEX_CUBE[0])
        	);
    	geometry.SetType( Geometry::TRIANGLES );
		SetGeometry(geometry);
	}
};

class PrimitiveTexturedCube : public Model
{
public:
    PrimitiveTexturedCube(std::string textureName, Dali::Shader &shader)
		: Model(shader)
    {
        AddTexture(0, textureName);
        PropertyBuffer vertexBuffer = PropertyBuffer::New( Property::Map()
															   .Add( "aPosition", Property::VECTOR3 )
															   .Add( "aTexCoord", Property::VECTOR2 )
															   .Add( "aNormal", Property::VECTOR3 )  );
	    vertexBuffer.SetData( texCubeVertices, sizeof(texCubeVertices) / sizeof(Vertex_Textured) );
	    Geometry geometry = Geometry::New();
	    geometry.AddVertexBuffer( vertexBuffer );
	    geometry.SetIndexBuffer( texCubeIndex,
									  sizeof(texCubeIndex)/sizeof(texCubeIndex[0]) );
	    geometry.SetType( Geometry::TRIANGLES );

		SetGeometry(geometry);
    }
};

class PrimitivePlane : public Model
{
public:
    PrimitivePlane(std::string textureName, Dali::Shader &shader)
		: Model(shader)
    {
        AddTexture(0, textureName);
        PropertyBuffer vertexBuffer = PropertyBuffer::New( Property::Map()
															   .Add( "aPosition", Property::VECTOR3 )
															   .Add( "aTexCoord", Property::VECTOR2 )
															   .Add( "aNormal", Property::VECTOR3 )  );
	    vertexBuffer.SetData( planeVertices, sizeof(planeVertices) / sizeof(Vertex_Textured) );
	    Geometry geometry = Geometry::New();
	    geometry.AddVertexBuffer( vertexBuffer );
	    geometry.SetIndexBuffer( planeIndex, sizeof(planeIndex)/sizeof(planeIndex[0]) );
	    geometry.SetType( Geometry::TRIANGLES );

		SetGeometry(geometry);
    }
};

class PrimitiveSphere : public Model
{
public:
    PrimitiveSphere(std::string textureName, Dali::Shader &shader)
		: Model(shader)
    {
        AddTexture(0, textureName);
        ObjLoader obj = ObjLoader();
        std::string filedir = "../res/models/biliard_ball.obj";
        std::ifstream file(filedir, std::ios::binary | std::ios::ate);
        std::vector<char> data;
        size_t size = file.tellg();
        data.resize(size);
        file.seekg(0, std::ios::beg);
        file.read(&data[0], size);
        obj.LoadObject(&data[0], size);
        Geometry geometry = obj.CreateGeometry(7, true);

		SetGeometry(geometry);
    }
};

class PrimitiveObj : public Model
{
public:
    PrimitiveObj(std::string textureName, Dali::Shader &shader, ObjLoader& obj)
		: Model(shader)
    {
        AddTexture(0, textureName);
        Geometry geometry = obj.CreateGeometry(7, true);

		SetGeometry(geometry);
    }
};

}
#endif