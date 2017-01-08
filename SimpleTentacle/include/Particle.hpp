//
//  Particle.hpp
//  SimpleTentacle
//
//  Created by Craig Pickard on 1/7/17.
//
//

#pragma once

//create the object using a shared pointer for automatic memory management
typedef std::shared_ptr<class Particle>		ParticleRef;

class Particle {
public:
    
    static ParticleRef create(ci::vec3 location)
    {
        return ParticleRef(new Particle(location));
    }
    
    Particle();
    Particle(ci::vec3 location);
    ~Particle();
    
    void applyForce( ci::vec3 force );
    void update();
    void display();
    void moveRandomly();
    void attract(ci::vec3 loc);
    void lock();
    void debug();
    void drawSphereOfInfluence();
    
    ci::vec3 mLocation;
    
    ci::vec3 getLocation() const { return mLocation; };
    float getMass() const { return mMass; };
    
private:
    
    ci::vec3 mAcceleration;
    ci::vec3 mVelocity;
    float mMass;
    float mDamp;
    bool mLocked;
    float mRadiusOfInfluence;
};