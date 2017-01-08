#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/CameraUi.h"

#include "Spring.hpp"
#include "Particle.hpp"
#include "Tentacle.hpp"

using namespace ci;
using namespace ci::app;
using namespace std;



class SimpleTentacleApp : public App {
  public:
	void setup() override;
	void mouseDown( MouseEvent event ) override;
    void mouseUp(MouseEvent event) override;
    void mouseDrag (MouseEvent event) override;
	void update() override;
	void draw() override;
    
    gl::BatchRef mPlane;
    
    CameraPersp mCam;
    CameraUi    mCamUi;
    
    vec2 mousePos;
    
//    SpringRef mSpring;
    
    ParticleRef mParticle;
    SpringRef   mSpring;
    
    ParticleRef    mBase, mHead;
    
    TentacleRef     mTentacle;
};

void SimpleTentacleApp::setup()
{
    // Set up the camera.
    mCam.lookAt( vec3( 0.0, 5.0f, 15.0f ), vec3( 0 ) );
    mCam.setPerspective( 60.0f, getWindowAspectRatio(), 0.01f, 1000.0f );
    mCamUi = CameraUi( &mCam, getWindow() );
    
    auto lambert = gl::getStockShader(gl::ShaderDef().color());
    mPlane = gl::Batch::create( geom::Plane().size( vec2( 150 ) ).subdivisions( ivec2( 10 ) ), lambert );
    
    mParticle = Particle::create(vec3(0.,5.,0.));
    mBase = Particle::create(vec3(0));
    mHead = Particle::create(vec3(2.,7.,-1.));
    
    mSpring = Spring::create(mBase, mHead, 5.0);
    
    mBase->lock();
    
    mTentacle = Tentacle::create(vec3(2., 0., 0.), vec3(2.,12.,0.), 10);
    
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
    // Apply a gravity force to the bob
//    vec3 gravity = vec3( 0.0, -2.0, 0.0 );
    
    mParticle->moveRandomly();
    mHead->attract(mParticle->getLocation());
//    mHead->debug();
    mSpring->update();
    
    mTentacle->getHead()->attract(mParticle->getLocation());
    mTentacle->update();


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
    
    // draw the plane
    // Draw the grid on the floor.
    {
        gl::ScopedColor color( Color::gray( 0.7f ) );
        mPlane->draw();
    }
    
    mParticle->display();
    
////    mParticle->drawSphereOfInfluence();
//    mBase->display();
//    mHead->display();
//    mSpring->display();
    
    mTentacle->display();

}

CINDER_APP( SimpleTentacleApp, RendererGl, []( App::Settings* settings ) {
    settings->setWindowSize( 1024, 768 );
    //    settings->setHighDensityDisplayEnabled();
});

