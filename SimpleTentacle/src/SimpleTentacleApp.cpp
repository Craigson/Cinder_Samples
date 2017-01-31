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


#define NUM_PARTICLES 12
#define NUM_TENTACLES 3


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
    
    std::vector<TentacleRef>    mTentacles;
    
    // create a struct for a light that contains the particle
    
    float rot = 0;
};

void SimpleTentacleApp::setup()
{
    // clear out the window with black
    gl::clear( Color::black());
    
    randSeed(clock());
    
    // Set up the camera.
    mCam.lookAt( vec3( 0.0, 0.0f, 75.0f ), vec3( 0.,0.,0. ) );
    mCam.setPerspective( 60.0f, getWindowAspectRatio(), 0.01f, 1000.0f );
    mCamUi = CameraUi( &mCam, getWindow() );
    
    auto lambert = gl::getStockShader(gl::ShaderDef().color());
    mPlane = gl::Batch::create( geom::Plane().size( vec2( 100 ) ).subdivisions( ivec2( 20 ) ), lambert );
    
    for (size_t i = 0; i < NUM_PARTICLES; i++)
    {
        ParticleRef mParticle = Particle::create( vec3(randFloat(-50.f,50.f), randFloat(-50.f,50.f), randFloat(-50.f,50.f)) );
        mParticles.push_back(mParticle);
    }
    
    /*
    mBase = Particle::create(vec3(0));
    mHead = Particle::create(vec3(2.,7.,-1.));
    mSpring = Spring::create(mBase, mHead, 5.0);
    mBase->lock();
    */
    
    mTentacle = Tentacle::create(vec3(0., 0., 0.), vec3(0.,30.,0.), 60);
    
    for (size_t i = 0; i < NUM_TENTACLES; i++){
        
        vec3 randomVec = vec3(randFloat(-30., 30), randFloat(-30., 30),randFloat(-30., 30));
        TentacleRef mTempTentacle = Tentacle::create(vec3(0.), randomVec, 6);
        mTentacles.push_back(mTempTentacle);
    }
    
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
        
        for (size_t i = 0; i < NUM_TENTACLES; i++) mTentacles[i]->getHead()->attract((*iter)->getLocation());
    }

    mTentacle->update();
    
    for (size_t i = 0; i < NUM_TENTACLES; i++) mTentacles[i]->update();
    
//    cout << getAverageFps() << endl;
    
    rot += 0.0015;

}

void SimpleTentacleApp::draw()
{
//    // clear out the window with black
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
//        mPlane->draw();
    }
    
    // draw each prey object
    for (auto iter = mParticles.begin(); iter != mParticles.end(); ++iter){
        ci::gl::ScopedColor color( ci::ColorAf( 1., 0., 0., 0.5 ) );
        (*iter)->display();
    }

//    gl::ScopedBlend(GL_MULTIPLY_NV);
    gl::enableAlphaBlending();
    ci::gl::ScopedColor color( ci::ColorAf( 1., 1., 1., 0.5 ) );
    mTentacle->display();
    
    for (size_t i = 0; i < NUM_TENTACLES; i++) mTentacles[i]->display();
    
//    gl::drawCube(vec3(0.), vec3(100.));
    saveFrames();

}

void SimpleTentacleApp::saveFrames(){
    if (getElapsedFrames() < 1500 && getElapsedFrames()%2==0) writeImage( getDocumentsDirectory() / "ITP" / "Residency"/ "Tentacle_Multi_Render" / ("000" + toString( getElapsedFrames() ) + ".jpg" ), copyWindowSurface() ); cout << getElapsedFrames() << endl;
}

CINDER_APP( SimpleTentacleApp, RendererGl(RendererGl::Options().msaa( 16 )), []( App::Settings* settings ) {
    settings->setWindowSize( 640, 640 );
    //    settings->setHighDensityDisplayEnabled();
});

