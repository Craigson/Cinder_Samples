#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/CameraUi.h"

#include "Simplex.h"

#define TEX_SIZE 64

using namespace ci;
using namespace ci::app;
using namespace std;

class DisplacementMapApp : public App {
  public:
	void setup() override;
	void update() override;
	void draw() override;
    
    CameraPersp         mCam;
    CameraUi            mCamUi;
    
    gl::GlslProgRef     mShader;
    gl::TextureRef      mTex;
    gl::BatchRef        mPlane;
    
};

void DisplacementMapApp::setup()
{
    // load the shaders
    mShader = gl::GlslProg::create(loadAsset("shader.vert"), loadAsset("shader.frag"));
    
    mCam.lookAt( vec3( 3, 2, 4 ), vec3( 0 ) );
    mCamUi = CameraUi(&mCam, getWindow());
    
    // create the texture that will store the values created by noise ( we only need one channel to
    // to store the data, so we set the internal format to GL_RED )
    mTex = gl::Texture2d::create(TEX_SIZE,TEX_SIZE,gl::Texture2d::Format().internalFormat( GL_RED ) );
    
    // create the plane whose vertices we'll be manipulating in the shader
    mPlane = gl::Batch::create( geom::Plane().subdivisions(ivec2(128,128)) >> geom::Scale( 3.f ), mShader);
    
    gl::enableDepthWrite();
    gl::enableDepthRead();
}

void DisplacementMapApp::update()
{
    // use the global time to animate the noise
    const float time = getElapsedSeconds() / 10.0f;
    
    // create an array to store the new texture data
    static GLubyte data[TEX_SIZE * TEX_SIZE];
    
    // update the texture
    for( size_t x = 0; x < TEX_SIZE; ++x ) {
        for( size_t y = 0; y < TEX_SIZE; ++y ) {
            
            // use a float to change the scale of the noise
            float noiseScale = 0.01f;
            
            //
            vec2 position = vec2( x, y ) * noiseScale;
            
            // use simplex noise to get a value for n ( this will be stored in the texture and passed to each vertex)
            float n = Simplex::noise( position + vec2( Simplex::curlNoise( position, time ).x ) ) * 0.5f + 0.5f;
            
            // clamp the value so that it is within the range of 0 - 255 (for an 8bit texture)
            data[x+y*TEX_SIZE] = glm::clamp( n, 0.0f, 1.f ) * 255;
        }
    }
    
    // update the texture with the new values
    mTex->update( data, GL_RED, GL_UNSIGNED_BYTE, 0, TEX_SIZE, TEX_SIZE, ivec2(0,0) );
    
}

void DisplacementMapApp::draw()
{
    gl::clear( Color( 0.2f, 0.2f, 0.2f ) );
    
    // bind the texture to allow for updating in the shader
    gl::ScopedTextureBind texBind( mTex );
    
    gl::setMatrices( mCam );
    
    // enable wireframe view
//    gl::setWireframeEnabled();
    
    // draw the plane
    gl::pushModelMatrix();
    gl::color( Color::white() );
    mPlane->draw();
    gl::popModelMatrix();
    
    
    // reset the matrices window to draw the texture to the screen
    gl::setMatricesWindow( getWindowSize() );
    
    // disable wireframe view
    gl::setWireframeEnabled(false);

    // draw the noise texture to the screen
    Rectf drawRect( 0, 0, mTex->getWidth(),mTex->getHeight());
    gl::draw( mTex, drawRect );
    

}

CINDER_APP( DisplacementMapApp, RendererGl, []( App::Settings* settings ) {
    settings->setWindowSize( 1024,768 );
    settings->setHighDensityDisplayEnabled();
});
