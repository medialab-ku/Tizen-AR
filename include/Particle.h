#ifndef __PARTICLE_H__
#define __PARTICLE_H__

#include <dali-toolkit/dali-toolkit.h>
#include <dali/devel-api/common/stage-devel.h>
#include <dali/dali.h>
#include "FrameActor.h"
#include <vector>
#include <cmath>

struct Particle
{
    Particle() 
        : m_Position(Dali::Vector3::ZERO)
        , m_Velocity(Dali::Vector3::ZERO)
        , m_Color(Dali::Vector4(1, 1, 1, 1))
        , m_Alpha(0.0f)
        , m_fRotate(0)
        , m_fSize(0.5f)
        , m_fSizeMultiplier(1.0f)
        , m_fAge(0)
        , m_fLifeTime(0)
        , m_bIsActive(false)
    {}

    Dali::Vector3   m_Position; // Center point of particle
    Dali::Vector3   m_Velocity; // Current particle velocity
    Dali::Vector4   m_Color;    // Particle color
    float           m_Alpha;
    float           m_fRotate;  // Rotate the particle the center
    float           m_fSize;    // Size of the particle
    float           m_fSizeMultiplier;
    float           m_fAge;
    float           m_fLifeTime;
    bool            m_bIsActive;
};


class ParticleEmitter
{
public:
    ParticleEmitter() {}
    virtual ~ParticleEmitter() {}
    virtual void EmitParticle( Particle& particle ) = 0;
    virtual void UpdateParticle( Particle& particle ) = 0;
};

class FireEmitter : public ParticleEmitter
{
public:
    FireEmitter();
    ~FireEmitter() override {}
    void EmitParticle( Particle& particle ) override;
    void UpdateParticle( Particle& particle ) override;
};

class ParticleEffect : public FrameActor
{
public:
    // A vertex for the particle
    struct Vertex
    {
        Vertex()
            : m_Pos(Dali::Vector3::ZERO)
            , m_Tex0(Dali::Vector2::ZERO)
            , m_Color(Dali::Vector4(1, 1, 1, 1))
        {}

        Dali::Vector3   m_Pos;      // Vertex position
        Dali::Vector2   m_Tex0;     // Texture coordinate
        Dali::Vector4   m_Color;
    };

    ParticleEffect(Dali::Stage &stage, Dali::Shader &shader, unsigned int numParticles = 0 );
    virtual ~ParticleEffect();

    void OnStart() override;
    void OnUpdate(double deltaTime) override;

    void SetParticleEmitter( ParticleEmitter* pEmitter );
    void SetShader(Dali::Shader &shader) {m_Shader = shader;}
    Dali::Shader& GetShader() {return m_Shader;}

    // Test method to randomize the particles in an interesting way
    void RandomizeParticles();
    void EmitParticles();

    bool LoadTexture( unsigned int index, const std::string& fileName );
    
    // Resize the particle buffer with numParticles
    void Resize( unsigned int numParticles );

    // Temporal code for tile texture
    void SetTile(int width, int height) 
    { 
        texW = width;
        texH = height;
    }

protected:
    void RandomizeParticle( Particle& particle );
    void EmitParticle( Particle& particle );
    void SetGeometry();
    Particle* GetFreeParticle();
public:
    // Build the vertex buffer from the particle buffer
    void BuildVertexBuffer();
private:
    Dali::CameraActor               m_pCamera;
    ParticleEmitter*                m_pParticleEmitter;
    float                           particleTimer;
    int                             particlePerSec;

    std::vector<Particle>           m_Particles;
    std::vector<Particle*>          m_ActiveParticles;
    std::vector<Vertex>             m_VertexBuffer;
    std::vector<unsigned short>     m_IndexBuffer; 

    Dali::TextureSet                m_TextureSet;
    Dali::Shader                    m_Shader;
    Dali::Geometry                  m_Geometry;
    Dali::Renderer                  m_Renderer;

    // Temporal code for tile texture
    int texW = 1;
    int texH = 1;

    // Apply this force to every particle in the effect
    Dali::Vector3       m_Force;
};


#endif