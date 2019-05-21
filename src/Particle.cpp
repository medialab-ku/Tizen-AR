#include "Particle.h"
#include "Assets.h"
#include <iostream>
#include <cassert>
#include <cstdlib>

using namespace std;

ParticleEffect::ParticleEffect(Dali::Stage &stage, Dali::Shader &shader, unsigned int numParticles /* = 0 */ )
: FrameActor(stage)
{
    m_pParticleEmitter = NULL;
    m_Force = Dali::Vector3(0, 0, 0);
    SetShader(shader);
    particleTimer = 0;
    particlePerSec = 5;
    
    // Set number of particles
    Resize(numParticles);

    // create Dali renderer components
    m_TextureSet = Dali::TextureSet::New();
    m_Geometry = Dali::Geometry::New();
    m_Renderer = Dali::Renderer::New(m_Geometry, m_Shader);

    m_Renderer.SetProperty( Dali::Renderer::Property::DEPTH_INDEX, 1.0f);
    m_Renderer.SetProperty( Dali::Renderer::Property::BLEND_MODE, Dali::BlendMode::ON );
    m_Renderer.SetProperty( Dali::Renderer::Property::BLEND_FACTOR_SRC_RGB, Dali::BlendFactor::ONE );
    m_Renderer.SetProperty( Dali::Renderer::Property::BLEND_FACTOR_DEST_RGB, Dali::BlendFactor::ONE_MINUS_SRC_COLOR );

    LoadTexture(0, "default.jpg");
    m_Renderer.SetTextures(m_TextureSet);
    mActor.AddRenderer(m_Renderer);
}

ParticleEffect::~ParticleEffect()
{
    m_TextureSet.Reset();
}

void ParticleEffect::OnStart()
{
    return;
}

void ParticleEffect::OnUpdate(double deltaTime)
{
    particleTimer += deltaTime;
    if(particleTimer >= 1.0f / particlePerSec) // emit particle
    {
        particleTimer = 0;

        Particle* p = GetFreeParticle();
        if(p != NULL) 
        {
            p->m_bIsActive = true;
            if(m_pParticleEmitter != NULL) m_pParticleEmitter->EmitParticle(*p);
            else RandomizeParticle(*p);
            m_ActiveParticles.push_back(p);
        }   
    }

    for ( unsigned int i = 0; i < m_ActiveParticles.size(); i++ )
    {
        Particle* particle = m_ActiveParticles[i];
        particle->m_fAge += deltaTime;
        if( particle->m_fAge > particle->m_fLifeTime )
        {
            particle->m_bIsActive = false;
            m_ActiveParticles.erase(m_ActiveParticles.begin() + i);
        }
        if(m_pParticleEmitter != NULL) m_pParticleEmitter->UpdateParticle(*particle);
        particle->m_Velocity += ( m_Force * deltaTime );
        particle->m_Position += ( particle->m_Velocity * deltaTime );
    }

    SetGeometry();
}

void ParticleEffect::SetParticleEmitter( ParticleEmitter* pEmitter )
{
    m_pParticleEmitter = pEmitter;
}

bool ParticleEffect::LoadTexture( unsigned int index, const std::string& fileName )
{
    Dali::Texture texture;
    bool result = Assets::GetTexture(fileName, texture);
    if (result) m_TextureSet.SetTexture(index, texture);
    return result;
}

void ParticleEffect::RandomizeParticle( Particle& particle )
{
    particle.m_fAge = (rand() % 10) / 20.0f;
    particle.m_fLifeTime = 1.0f; // (rand() % 3) + 3;

    float a = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
    float b = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
    float c = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
    Dali::Vector3 unitVec = Dali::Vector3(a, b, c);

    particle.m_Position = Dali::Vector3(0, -0.3f, 0);
    //particle->m_Position = unitVec * 1.0f;
    //particle->m_Velocity = unitVec * ((rand() % 11) + 10);
}

void ParticleEffect::RandomizeParticles()
{
    for ( unsigned int i = 0; i < m_Particles.size(); ++i )
    {
        RandomizeParticle(m_Particles[i]);
    }
}

void ParticleEffect::EmitParticle( Particle& particle )
{
    assert( m_pParticleEmitter != NULL );
    m_pParticleEmitter->EmitParticle( particle );
}

void ParticleEffect::EmitParticles()
{
    if ( m_pParticleEmitter == NULL )
    {
        RandomizeParticles();
    }
    else 
    {
        for ( unsigned int i = 0; i < m_Particles.size(); ++i )
        {
            EmitParticle(m_Particles[i]);
        }
    }
}

Particle* ParticleEffect::GetFreeParticle()
{
    static unsigned int lastFound = 0;

    int maxParticles = m_Particles.size();
    for(int i = 0; i < maxParticles; i++)
    {
        int index = lastFound + i;
        if(index >= maxParticles) index -= maxParticles; 

        if(!m_Particles[index].m_bIsActive) 
        {
            cout << "returning " << index+1 << "'th particle" << endl;
            return &m_Particles[index];
        }
    }
    cout << "can't allocate more particle!" << endl;
    return NULL;
}

void ParticleEffect::BuildVertexBuffer()
{
    const Dali::Vector3 X( -0.5, 0, 0 );
    const Dali::Vector3 Y( 0, -0.5, 0 );
    const Dali::Vector3 Z( 0, 0 , -1.0 );

    Dali::Stage mStage = Dali::Stage::GetCurrent();
    Dali::CameraActor mCamera = mStage.GetRenderTaskList().GetTask(0).GetCameraActor();
    Dali::Quaternion cameraRotation = mCamera.GetCurrentOrientation();

    // Make sure the vertex buffer has enough vertices to render the effect
    // If the vertex buffer is already the correct size, no change is made.
    m_VertexBuffer.resize(m_Particles.size() * 4, Vertex() );
    m_IndexBuffer.clear();

    // Temporal code for tile texture
    float tileW = 1.0f / texW;
    float tileH = 1.0f / texH;

    for ( unsigned int i = 0; i < m_ActiveParticles.size(); i++)
    {
        Particle* particle = m_ActiveParticles[i];
        Dali::Quaternion rotation = Dali::Quaternion( Dali::Radian(particle->m_fRotate), Z );
       
        unsigned int vertexIndex = i * 4;
        Vertex& v0 = m_VertexBuffer[vertexIndex + 0];   // Bottom-left
        Vertex& v1 = m_VertexBuffer[vertexIndex + 1];   // Bottom-right
        Vertex& v2 = m_VertexBuffer[vertexIndex + 2];   // Top-right
        Vertex& v3 = m_VertexBuffer[vertexIndex + 3];   // Top-left

        float lifeRatio = particle->m_fAge / particle->m_fLifeTime;
        int texIdx = floor(lifeRatio * texW * texH);
        if(texIdx == texW*texH) texIdx = texW*texH - 1;

        int currTileX = texIdx % texW;
        int currTileY = texIdx / texW;
        float x = currTileX * tileW;
        float y = currTileY * tileH;

        float size = particle->m_fSize * particle->m_fSizeMultiplier;
        // Bottom-left
        v0.m_Pos = particle->m_Position + cameraRotation * ( rotation * ( -X - Y ) * size );
        // v0.m_Tex0 = Dali::Vector2( 0, 1 );
        v0.m_Tex0 = Dali::Vector2( x, y + tileH );
        v0.m_Color = particle->m_Color;

        // Bottom-right
        v1.m_Pos = particle->m_Position + cameraRotation * ( rotation * ( X - Y ) * size );
        // v1.m_Tex0 = Dali::Vector2( 1, 1 );
        v1.m_Tex0 = Dali::Vector2( x + tileW, y + tileH );
        v1.m_Color = particle->m_Color;

        // Top-right
        v2.m_Pos = particle->m_Position + cameraRotation * ( rotation * ( X + Y ) * size );
        // v2.m_Tex0 = Dali::Vector2( 1, 0 );
        v2.m_Tex0 = Dali::Vector2( x + tileW, y );
        v2.m_Color = particle->m_Color;

        // Top-left
        v3.m_Pos = particle->m_Position + cameraRotation * ( rotation * ( -X + Y ) * size );
        // v3.m_Tex0 = Dali::Vector2( 0, 0 );
        v3.m_Tex0 = Dali::Vector2( x, y );
        v3.m_Color = particle->m_Color;

        m_IndexBuffer.push_back(vertexIndex);
        m_IndexBuffer.push_back(vertexIndex+1);
        m_IndexBuffer.push_back(vertexIndex+3);
        m_IndexBuffer.push_back(vertexIndex+1);
        m_IndexBuffer.push_back(vertexIndex+2);
        m_IndexBuffer.push_back(vertexIndex+3);
    }
}

void CleanGeometry(Dali::Geometry &geometry)
{
    int vbCount = geometry.GetNumberOfVertexBuffers();
    for(int i = 0; i < vbCount; i++)
    {
        geometry.RemoveVertexBuffer(i);
    }
}

void ParticleEffect::SetGeometry()
{
    BuildVertexBuffer();

    Dali::PropertyBuffer VBO = Dali::PropertyBuffer::New( Dali::Property::Map()
                                                    .Add( "aPosition", Dali::Property::VECTOR3 )
                                                    .Add( "aTexCoord", Dali::Property::VECTOR2 )
                                                    .Add( "aColor",    Dali::Property::VECTOR4 ));

    VBO.SetData( &m_VertexBuffer[0], m_VertexBuffer.size() );
    CleanGeometry(m_Geometry);
    m_Geometry.AddVertexBuffer( VBO );
    m_Geometry.SetIndexBuffer( &m_IndexBuffer[0], m_IndexBuffer.size() );
    m_Geometry.SetType( Dali::Geometry::TRIANGLES );
}

void ParticleEffect::Resize( unsigned int numParticles )
{
    m_Particles.resize( numParticles,  Particle() );
    m_VertexBuffer.resize( numParticles * 4, Vertex() );
}

float randomF()
{
    // 0.0 ~ 1.0
    return static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
} 

FireEmitter::FireEmitter() : ParticleEmitter()
{

}

void FireEmitter::EmitParticle( Particle& particle )
{
    particle.m_fAge = 0;
    particle.m_fLifeTime = 2.0f;
    particle.m_fSize = (randomF() + 1) / 2.0f; // 0.5 ~ 1 

    float a = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
    float b = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
    float c = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
    Dali::Vector3 unitVec = Dali::Vector3(a, b, c);

    particle.m_Position = Dali::Vector3(0, -0.3f, 0);
    //particle->m_Position = unitVec * 1.0f;
    //particle->m_Velocity = unitVec * ((rand() % 11) + 10);
}

void FireEmitter::UpdateParticle( Particle& particle )
{
    float lifeRatio = particle.m_fAge / particle.m_fLifeTime;

    // velocity over time
    float x, y, z;
    if(lifeRatio < 0.3f)
    {
        float range = 0.5f * lifeRatio / 0.3f;
        x = (randomF() * 2.0f - 1.0f) * range;
        z = (randomF() * 2.0f - 1.0f) * range;
    }
    else if(lifeRatio < 0.7f)
    {
        float range = 0.5f - 0.5f * (lifeRatio - 0.3f) / 0.4f;
        x = (randomF() * 2.0f - 1.0f) * range;
        z = (randomF() * 2.0f - 1.0f) * range;
    }
    else
    {
        x = 0.0f;
        z = 0.0f;
    }
    
    if(lifeRatio < 0.44f)
    {
        float rangeUp = 0.2f * lifeRatio / 0.44f;
        float rangeDn = 0.05f * lifeRatio / 0.44f;
        y = randomF() * (rangeUp - rangeDn) + rangeUp;
    }
    else 
    {
        y = randomF() * (0.2f - 0.05f) + 0.1f;
    }
    particle.m_Velocity = wVector3(x, y, z).ToDali();

    // color over time
    float r, g, b, a;
    Dali::Vector3 color = Dali::Vector3(201, 148, 81) * (1.0 - lifeRatio) + Dali::Vector3(255, 69, 0) * lifeRatio;
    r = color.x / 255.0f;
    g = color.y / 255.0f;
    b = color.z / 255.0f;
    a = 255.0f;
    if(lifeRatio > 0.86f)
    {
        a = 255.0f * (1 - lifeRatio) / 0.14f; 
    }
    particle.m_Color = Dali::Vector4(r, g, b, a);

    // size over time
    float s = 1.0f;
    if(lifeRatio < 0.45f)
    {
        s = lifeRatio / 0.45f;
    }
    particle.m_fSizeMultiplier = s;
}