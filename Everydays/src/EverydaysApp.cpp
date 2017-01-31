#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/CameraUi.h"
#include "cinder/Utilities.h"
#include "cinder/qtime/AvfWriter.h"

#define DAY 16
#define BIT_RATE 5000000
#define NUM_FRAMES 60

using namespace ci;
using namespace ci::app;
using namespace std;

class EverydaysApp : public App {
  public:
	void setup() override;
	void mouseDown( MouseEvent event ) override;
    void keyDown ( KeyEvent event) override;
	void update() override;
	void draw() override;
    void cleanup() override { mMovieExporter.reset(); }
    
    qtime::MovieWriterRef mMovieExporter;
    
    void setupMovie();
    void recordMovie();
    void recordGif();
    
    int frameCount;
    float timeCount;
    
    bool reverse;
    
    gl::GlslProgRef     mGlsl;

};

void EverydaysApp::setup()
{
    frameCount = 0;
    timeCount = 0;
    
    setFrameRate(30.f);
    
    // load the shaders
    mGlsl = gl::GlslProg::create(loadAsset("vertex.vert"), loadAsset("16_pattern.frag"));
    
    reverse = false;
//    setupMovie();
}

void EverydaysApp::keyDown( KeyEvent event){
    if( event.getChar() == 'r' ) reverse = !reverse;
}

void EverydaysApp::mouseDown( MouseEvent event )
{
}

void EverydaysApp::update()
{
//     recordMovie();
    
    timeCount += 0.0523;
}

void EverydaysApp::draw()
{
    gl::ScopedGlslProg glslScp( mGlsl );
    mGlsl->uniform("uResolution", vec2(getWindowWidth(), getWindowHeight()));
    mGlsl->uniform("uTime", timeCount);
    mGlsl->uniform("uReverse", reverse);
    gl::drawSolidRect( getWindowBounds() );
 
//    recordGif();
}

void EverydaysApp::setupMovie()
{
    // setup the quicktime writer
    fs::path path = getSaveFilePath();
    if( ! path.empty() ) {
        auto format = qtime::MovieWriter::Format().codec( qtime::MovieWriter::H264 ).fileType( qtime::MovieWriter::QUICK_TIME_MOVIE )
        .jpegQuality( 1.0f ).averageBitsPerSecond( BIT_RATE );
        mMovieExporter = qtime::MovieWriter::create( path, getWindowWidth(), getWindowHeight(), format );
    }
    
}
void EverydaysApp::recordMovie()
{
    const int maxFrames = 450;
    if( mMovieExporter && getElapsedFrames() > 1 && getElapsedFrames() < maxFrames )
        mMovieExporter->addFrame( copyWindowSurface() );
    else if( mMovieExporter && getElapsedFrames() >= maxFrames ) {
        mMovieExporter->finish();
        mMovieExporter.reset();
    }
    
}

void EverydaysApp::recordGif()
{
    if (getElapsedFrames() % 4 == 0 && frameCount < 35 ) {
        writeImage( getDocumentsDirectory() / "ITP" / "Residency"/ "100days_2017" / toString(DAY) / ((toString(DAY)) + ("_0") + toString( frameCount ) + ".jpg" ), copyWindowSurface() );
        frameCount++;
    }

}


CINDER_APP( EverydaysApp,  RendererGl, []( App::Settings* settings ) {
    settings->setWindowSize( 500,500 );
//    settings->setHighDensityDisplayEnabled();
});
