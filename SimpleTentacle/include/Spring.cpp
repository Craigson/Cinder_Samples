#include "Spring.hpp"

using namespace ci;
using namespace ci::app;
using namespace std;

Spring::Spring()
{
}

Spring::Spring(std::shared_ptr<Particle> from,std::shared_ptr<Particle> to, float l)
: mBaseRef(move(from)), mHeadRef(move(to)), mRestLength(l){}

Spring::~Spring(){}

// Calculate spring force
void Spring::update() {
    
    ci::vec3 deltaDir = mBaseRef->getLocation() - mHeadRef->getLocation();
    float length = distance(mBaseRef->getLocation(), mHeadRef->getLocation());
    
    float invMassA = 1.0f / mBaseRef->getMass();
    float invMassB = 1.0f / mHeadRef->getMass();
    float normDist = ( length - mRestLength ) / ( length * ( invMassA + invMassB ) ) * mStrength;
    
    mBaseRef->applyForce( -deltaDir * normDist * invMassA );
    mHeadRef->applyForce( deltaDir * normDist * invMassA );
    
    mBaseRef->update();
    mHeadRef->update();
}

void Spring::display()
{
    gl::color( Color::white() );
    gl::drawLine(mBaseRef->getLocation(), mHeadRef->getLocation());
}
