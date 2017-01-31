/*
 TODO:
 - implement base vertical
 - implement maximum angle between segments
 - fix forces
 
 */

#include "Tentacle.hpp"



Tentacle::Tentacle(){}

Tentacle::~Tentacle(){}

Tentacle::Tentacle(ci::vec3 base, ci::vec3 head, int numSprings){
    
    mGravity = ci::vec3(0.,-0.000005,0.);
    
    mNumSegments = numSprings;
    
    Simplex::seed(clock());
    
    ci::vec3 dir = head - base; // direction from base to tip
    dir = normalize(dir); // normalize the direction
    
//    std::cout << "dir: " << dir << std::endl;
    
    float dist = distance(base,head); // compute the distance between tip
    float springLength = dist / numSprings; // divide the length / number of springs to get length
    
//    std::cout << "spring length: " << springLength << std::endl;
    
    ParticleRef mBaseParticle = Particle::create(base); // create the base
    mBaseParticle->lock(); // lock it in place
    mParticles.push_back( std::move(mBaseParticle) );
    
    // create the particles that form the joints of the tentacle
    for (size_t i = 1; i < numSprings; i++){
        
        ci::vec3 step = dir * ( i * springLength );
        ci::vec3 newPos = step + base;
//        std::cout << newPos << std::endl;
        ParticleRef next = Particle::create(newPos);
        mParticles.push_back( std::move(next) );
    }
    
    // create the tip
    ParticleRef mTipParticle = Particle::create(head);
    mParticles.push_back(mTipParticle);
    
    mParticles[1]->lock(); // lock the second particle from the base, so that the base spring is always vertical.
    
    // create the springs by connecting every particle with it's previous neighbour
    for (size_t i = 1; i < mParticles.size(); i ++)
    {
        SpringRef spring = Spring::create( (mParticles[i-1] ), (mParticles[i]), springLength);
        mSprings.push_back(spring);
    }
    
    // circle helper function
    const auto createCircle = []( float radius, int segments ) {
        ci::Shape2d shape;
        shape.moveTo( ci::vec2( 1, 0 ) * radius );
        float inc = ( 2.0f * M_PI ) / (float) segments;
        for( float angle = 0.0f; angle <= M_PI * 2.0f; angle += inc ) {
            shape.lineTo( ci::vec2( cos( angle ), sin( angle ) ) * radius );
        }
        shape.lineTo( ci::vec2( 1, 0 ) * radius );
        return shape;
    };
    
    
    
    mSectionShape = createCircle( 0.75f, 6 );
    
    // finally, create the skin / body
    createBody();
    
    mPerlin.setSeed( clock() );
    
}

// method to create the tentacles body / skin ( called at the end of init)
void Tentacle::createBody(){
    

    // set spline points as initial particle positions
    for (size_t i = 0; i < mParticles.size(); i++){
        mSplinePoints.push_back(mParticles[i]->getLocation());
    }
    
    // create the initial spline
    ci::BSpline3f spline = ci::BSpline3f(mSplinePoints, mSplinePoints.size() - 1, false, true);
    
    // create trimesh by extruding along spline
    ci::TriMesh source = ci::geom::ExtrudeSpline( mSectionShape, // last segment normals / black faces
                                                 spline,
                                                 mNumSegments, 3.f ).thickness( []( float t ) { return ci::smoothstep( 30.0f, 5.f, t ); } );
    
    const auto shader = ci::gl::getStockShader( ci::gl::ShaderDef().lambert().color() );
    mBody = ci::gl::Batch::create( source, shader );
    
}

void Tentacle::reach(ci::vec3 attractor){
    mParticles[mParticles.size()-1]->applyForce(attractor);
}

std::shared_ptr<Particle> Tentacle::getHead(){
    return mParticles[mParticles.size()-1];
};

void Tentacle::update()
{
    float scale = 0.00005f;
    float multi = 0.000001f;
    
    
    for (auto iter = mSprings.begin(); iter != mSprings.end(); ++iter){

        (*iter)->update();
        
    }
    
    for( auto iter = mParticles.begin(); iter != mParticles.end(); ++iter )
    {
        // current ( as if underwater )
        (*iter)->addToAcceleration(mPerlin.dfBm( (*iter)->getLocation() * scale ) * multi);
        
        // gravity
        if ( (*iter)->getLocation().y > 3.) {
            (*iter)->addToAcceleration(mGravity);
        } 
    }
    
    updateBody();
    
}

void Tentacle::updateBody(){
    
    
    // update the splinepoint locations with the current particle locations
    for (size_t i = 0; i < mParticles.size(); i++){
        mSplinePoints[i] = mParticles[i]->getLocation();
    }
    
    // rebuild the spline
    ci::BSpline3f spline = ci::BSpline3f(mSplinePoints, mSplinePoints.size() - 1, false, true);
    
    // rebuild the geometry
    ci::TriMesh source = ci::geom::ExtrudeSpline( mSectionShape, // last segment normals / black faces
                                                 spline,
                                                 30 ).thickness( []( float t ) { return ci::smoothstep( 1.0f, 0.2f, t ); } );
    // create a new vbomesh
    auto newVboMesh = ci::gl::VboMesh::create(source);
    
    // replace the Batch's vbomesh with the newly generated one
    mBody->replaceVboMesh( newVboMesh );
    
}

void Tentacle::display(){
    
//    for( auto iter = mParticles.begin(); iter != mParticles.end(); ++iter )
//    {
//        (*iter)->display();
//    }
//
//    for (auto iter = mSprings.begin(); iter != mSprings.end(); ++iter){
//        (*iter)->display();
//    }
    
    // draw the skin
    mBody->draw();
}