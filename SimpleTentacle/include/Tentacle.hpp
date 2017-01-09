//
//  Chain.hpp
//  SimpleTentacle
//
//  Created by Craig Pickard on 1/7/17.
//
//

#pragma once

#include "Particle.hpp"
#include "Spring.hpp"
#include "cinder/Perlin.h"
#include "Simplex.h"

typedef std::shared_ptr<class Tentacle>		TentacleRef;

class Tentacle {
    
public:
    
    static TentacleRef create(ci::vec3 base, ci::vec3 head, int numSprings)
    {
        return TentacleRef(new Tentacle(base, head, numSprings));
    }
    
    Tentacle();
    Tentacle(ci::vec3 baseLocation, ci::vec3 headLocation, int numSprings);
    ~Tentacle();
    
    void update();
    void display();
    void reach(ci::vec3 attractor);
    void createBody();
    void updateBody();
    
    std::shared_ptr<Particle> getHead();
    
private:
    
    std::vector< std::shared_ptr<Spring> >      mSprings;
    std::vector< std::shared_ptr<Particle> >    mParticles;
    std::vector<ci::vec3>                       mSplinePoints;
    ci::vec3                                    mBase;
    ci::vec3                                    mHead;
    cinder::Perlin                              mPerlin;
    ci::vec3                                    mGravity;
    ci::gl::BatchRef                            mBody;
    ci::Shape2d                                 mSectionShape;
    
    int                                         mNumSegments;
    

    
};
