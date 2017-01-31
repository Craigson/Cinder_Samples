//
//  Particle.cpp
//  SimpleTentacle
//
//  Created by Craig Pickard on 1/7/17.
//
//

#include "Particle.hpp"
#include "Simplex.h"

Particle::Particle(){};

Particle::~Particle(){};

Particle::Particle( ci::vec3 location )
{
    mLocation = location;
    mAcceleration = ci::vec3(0.);
    mVelocity = ci::vec3(0.0);
    mMass = 500.0;
    mLocked = false;
    mRadiusOfInfluence = 50.f;
}

void Particle::debug(){
    std::cout << "acc: " << mAcceleration << std::endl;
    std::cout << "vel: " << mVelocity << std::endl;
}

void Particle::applyForce( ci::vec3 force )
{
    force /= mMass;
    mAcceleration += force;
    
}

// Method to update location
void Particle::update()
{
//    std::cout << "particle acc(before): " << mAcceleration << std::endl;
    
    if (!mLocked){
        mVelocity += mAcceleration;
        mLocation += mVelocity;
        mAcceleration = ci::vec3(0.);
//        mVelocity *= (1.0 - mDamp);
    }
    
    // keep particles within bounding box
    if (mLocation.x > 50 || mLocation.x < - 50) mVelocity.x *= -1;
    if (mLocation.y > 50 || mLocation.y < - 50) mVelocity.y *= -1;
    if (mLocation.z > 50 || mLocation.z < - 50) mVelocity.z *= -1;
    

    
//    std::cout << "particle acc(after): " << mAcceleration << std::endl;
}

void Particle::addToAcceleration(ci::vec3 acc){
    mAcceleration += acc;
}

void Particle::attract(ci::vec3 loc){
    
    float dist = distance(mLocation,loc);
    float distForce = dist * 0.01;
    
    ci::vec3 dir = mLocation - loc;
    dir = normalize(dir);
    ci::vec3 force = distForce * dir;
    
//    std::cout << "dist: " << dist << std::endl;
    
    if (dist < mRadiusOfInfluence) {
        applyForce(-force);
//        std::cout << "reaching! " << std::endl;
    }
    
}

void Particle::moveRandomly()
{
    mAcceleration = Simplex::curlNoise(mLocation);
    mAcceleration /= 15000.;
    update();
}

void Particle::lock()
{
    mLocked = true;
}

// Method to display
void Particle::display()
{
    ci::gl::drawSphere(mLocation, .2);
}

void Particle::drawSphereOfInfluence(){
    
    ci::gl::ScopedColor color( ci::Color( 1., 0., 0. ) );
    ci::gl::drawSphere(mLocation, mRadiusOfInfluence);
}

