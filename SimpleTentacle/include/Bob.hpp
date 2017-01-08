#pragma once

class Bob {
public:
    Bob();
    Bob( float x, float y );
    
    void update();
    void display();
    void applyForce( ci::vec2 force );
    
    void clicked( ci::vec2 mousePos );
    void drag( ci::vec2 mousePos );
    void stopDragging();
    
    ci::vec2 mLocation;
    ci::vec2 mVelocity;
    
private:
    ci::vec2 mAcceleration;
    float mMass = 24.0;
    
    // Arbitrary damping to simulate friction / drag
    float mDamping = 0.98;
    
    // For mouse interaction
    bool mDragging = false;
    ci::vec2 mDragOffset;
};