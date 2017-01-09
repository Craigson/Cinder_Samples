#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/CameraUi.h"

#include "Spring.hpp"
#include "Particle.hpp"
#include "Tentacle.hpp"
#include "cinder/Rand.h"
#include "cinder/Utilities.h"

using namespace ci;
using namespace ci::app;
using namespace std;


#define NUM_PARTICLES 10


class SimpleTentacleApp : public App {
  public:
	void setup() override;
	void mouseDown( MouseEvent event ) override;
    void mouseUp(MouseEvent event) override;
    void mouseDrag (MouseEvent event) override;
	void update() override;
	void draw() override;
    void saveFrames();
    
    gl::BatchRef mPlane;
    
    CameraPersp mCam;
    CameraUi    mCamUi;
    
    vec2 mousePos;
    
    // container to store the prey items
    std::vector<ParticleRef> mParticles;
    
    TentacleRef     mTentacle;
    
    // create a struct for a light that contains the particle
    
    float rot = 0;
};

void SimpleTentacleApp::setup()
{
    // Set up the camera.
    mCam.lookAt( vec3( 0.0, 15.0f, 35.0f ), vec3( 0.,10.,0. ) );
    mCam.setPerspective( 60.0f, getWindowAspectRatio(), 0.01f, 1000.0f );
    mCamUi = CameraUi( &mCam, getWindow() );
    
    auto lambert = gl::getStockShader(gl::ShaderDef().color());
    mPlane = gl::Batch::create( geom::Plane().size( vec2( 60 ) ).subdivisions( ivec2( 20 ) ), lambert );
    
    for (size_t i = 0; i < NUM_PARTICLES; i++)
    {
        ParticleRef mParticle = Particle::create( vec3(randFloat(-15.f,15.f), randFloat(0.f,50.f), randFloat(-15.f,15.f)) );
        mParticles.push_back(mParticle);
    }
    
    /*
    mBase = Particle::create(vec3(0));
    mHead = Particle::create(vec3(2.,7.,-1.));
    mSpring = Spring::create(mBase, mHead, 5.0);
    mBase->lock();
    */
    
    mTentacle = Tentacle::create(vec3(0., 0., 0.), vec3(0.,50.,0.), 5);
    
}

void SimpleTentacleApp::mouseDown( MouseEvent event )
{
    mousePos = vec2( event.getPos() );
}

void SimpleTentacleApp::mouseUp( MouseEvent event )
{

}

void SimpleTentacleApp::mouseDrag( MouseEvent event )
{

}

void SimpleTentacleApp::update()
{

    // make the tentacle check each object
    for (auto iter = mParticles.begin(); iter != mParticles.end(); ++iter){
        (*iter)->moveRandomly();
        mTentacle->getHead()->attract((*iter)->getLocation());
    }

    mTentacle->update();
    
    cout << getAverageFps() << endl;
    
    rot += 0.0015;

}

void SimpleTentacleApp::draw()
{
    // clear out the window with black
    gl::clear( Color::black());
    
    // Set up the camera.
    gl::ScopedMatrices push;
    gl::setMatrices( mCam );
    
    // Enable depth buffer.
    gl::ScopedDepth depth( true );
    
    gl::enableWireframe();
    
    gl::rotate(angleAxis(rot,vec3(0.,1.,0)));
    
    // draw the plane
    // Draw the grid on the floor.
    {
        ci::gl::ScopedColor color( ci::ColorAf( 1., 1., 1.,0.2 ) );
        mPlane->draw();
    }
    
    // draw each prey object
    for (auto iter = mParticles.begin(); iter != mParticles.end(); ++iter){
        ci::gl::ScopedColor color( ci::Color( 1., 0., 0. ) );
        (*iter)->display();
    }
    
    ci::gl::ScopedColor color( ci::Color( 1., 1., 1. ) );
    mTentacle->display();
    
//    saveFrames();

}

void SimpleTentacleApp::saveFrames(){
    if (getElapsedFrames() < 1800 && getElapsedFrames()%2==0) writeImage( getDocumentsDirectory() / "ITP" / "Residency"/ "Tentacle_Render" / ("000" + toString( getElapsedFrames() ) + ".jpg" ), copyWindowSurface() );
}

CINDER_APP( SimpleTentacleApp, RendererGl(RendererGl::Options().msaa( 4 )), []( App::Settings* settings ) {
    settings->setWindowSize( 960, 720 );
    //    settings->setHighDensityDisplayEnabled();
});

