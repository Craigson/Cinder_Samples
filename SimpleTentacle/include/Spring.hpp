#pragma once
#include "Bob.hpp"
#include "Particle.hpp"

typedef std::shared_ptr<class Spring>		SpringRef;

class Spring {
public:
    
    static SpringRef create(std::shared_ptr<Particle> from,std::shared_ptr<Particle> to, float l)
    {
        return SpringRef(new Spring(from, to, l));
    }
    
    Spring();
    Spring(std::shared_ptr<Particle> from,std::shared_ptr<Particle> to , float l);
    ~Spring();
    
//    void constrainLength(float minlen, float maxlen );
    void update();
    void display();
    
private:
    // Rest length and spring constant
    float mRestLength;
    float k = 0.2;
    float mDamping = 0.99;
    float mStrength = 0.1;
    
    std::shared_ptr<Particle> mBaseRef;
    std::shared_ptr<Particle> mHeadRef;
};