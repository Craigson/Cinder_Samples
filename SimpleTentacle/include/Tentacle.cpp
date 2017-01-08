//
//  Chain.cpp
//  SimpleTentacle
//
//  Created by Craig Pickard on 1/7/17.
//
//

#include "Tentacle.hpp"



Tentacle::Tentacle(){}

Tentacle::~Tentacle(){}

Tentacle::Tentacle(ci::vec3 base, ci::vec3 head, int numSprings){
    
    mGravity = ci::vec3(0.,-0.000005,0.);
    
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
        std::cout << newPos << std::endl;
        ParticleRef next = Particle::create(newPos);
        mParticles.push_back( std::move(next) );
    }
    
    // create the tip
    ParticleRef mTipParticle = Particle::create(head);
    mParticles.push_back(mTipParticle);
    
    // create the springs
    
    for (size_t i = 1; i < mParticles.size(); i ++)
    {
        std::cout << "prev: " << mParticles[i-1]->getLocation() << std::endl;
        std::cout << "curr: " << mParticles[i]->getLocation() << std::endl;
        
        SpringRef spring = Spring::create( (mParticles[i-1] ), (mParticles[i]), springLength);
        mSprings.push_back(spring);
    }
    std::cout << "numSprings: " << mSprings.size() << std::endl;
    std::cout << "numPArticles: " << mParticles.size() << std::endl;
    
    mPerlin.setSeed( clock() );
    
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
        // if ( (*iter)->getLocation().y > 3.) (*iter)->addToAcceleration(mGravity);
    }
}

void Tentacle::display(){
    
    for( auto iter = mParticles.begin(); iter != mParticles.end(); ++iter )
    {
        (*iter)->display();
    }
    
    for (auto iter = mSprings.begin(); iter != mSprings.end(); ++iter){
        (*iter)->display();
    }
}