#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/CameraUi.h"

#include "Simplex.h"

#define TEX_SIZE 256

using namespace ci;
using namespace ci::app;
using namespace std;

class DisplacementIcosApp : public App {
  public:
	void setup() override;
	void mouseDown( MouseEvent event ) override;
    void	keyDown( KeyEvent event ) override;
	void update() override;
	void draw() override;
    
    void updateTexture();
    void drawTexture();
    
    CameraPersp         mCam;
    CameraUi            mCamUi;
    
    float angle;
    bool showTexture;
    
    gl::Texture2dRef    mTex;
    gl::GlslProgRef     mGlsl;
    gl::BatchRef        mIcos;
    
    
    
};

void DisplacementIcosApp::keyDown( KeyEvent event )
{
    if( event.getChar() == 'w' )
        gl::setWireframeEnabled( ! gl::isWireframeEnabled() );
    if( event.getChar() == 't' ) showTexture = !showTexture;
//    if (event.getChar() == 's') record = !record;
}


void DisplacementIcosApp::setup()
{
    


    
    // load the shaders
    mGlsl = gl::GlslProg::create(loadAsset("shader.vert"), loadAsset("shader.frag"));
//    auto lambert = gl::ShaderDef().lambert().color();
//    gl::GlslProgRef shader = gl::getStockShader( lambert );
    
    mCam.lookAt( vec3( 0, 0, 4 ), vec3( 0 ) );
    mCamUi = CameraUi(&mCam, getWindow());
    
    // create the texture that will store the values created by noise ( we only need one channel to
    // to store the data, so we set the internal format to GL_RED )
    mTex = gl::Texture2d::create(TEX_SIZE,TEX_SIZE,gl::Texture2d::Format().internalFormat( GL_RED ).wrap(GL_REPEAT) );
    
    // create the plane whose vertices we'll be manipulating in the shader
//    mIcos = gl::Batch::create( geom::Icosphere() >> geom::Scale( 3.f ), shader);
        mIcos = gl::Batch::create( geom::Icosphere().subdivisions(4), mGlsl);
    
    gl::enableDepthWrite();
    gl::enableDepthRead();
    
    angle = 0.01;
}

void DisplacementIcosApp::mouseDown( MouseEvent event )
{
}

void DisplacementIcosApp::update()
{
    updateTexture();
    float xPos = getMousePos().x;
    cout << xPos << endl;
    mGlsl->uniform("uScale", xPos/getWindowWidth());
    angle += 0.01;
    

}



void DisplacementIcosApp::updateTexture()
{
    // use the global time to animate the noise
    const float time = getElapsedSeconds() / 2.0f;
    
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

void DisplacementIcosApp::drawTexture()
{
    // reset the matrices window to draw the texture to the screen
    gl::setMatricesWindow( getWindowSize() );
    
    // disable wireframe view
    gl::setWireframeEnabled(false);
    
    // draw the noise texture to the screen
    gl::color(Color(1.,1.,1.));
    Rectf drawRect( 0, 0, mTex->getWidth(),mTex->getHeight());
    gl::draw(mTex, drawRect );
    
}

void DisplacementIcosApp::draw()
{
    gl::clear( Color( 0.1f, 0.1f, 0.1f ) );
    
    // bind the texture to allow for updating in the shader
    gl::ScopedTextureBind texBind( mTex );
    
    gl::setMatrices( mCam );
    
    // enable wireframe view
    //    gl::setWireframeEnabled();
    
    // draw the plane
    gl::pushModelMatrix();
    gl::color( Color::white() );
    gl::rotate(angle, vec3(0,1,0));
    mIcos->draw();
    gl::popModelMatrix();
    
    if(showTexture) drawTexture();
    
}

CINDER_APP( DisplacementIcosApp, RendererGl, []( App::Settings* settings ) {
    settings->setWindowSize( 900,900 );
    settings->setHighDensityDisplayEnabled();
});
