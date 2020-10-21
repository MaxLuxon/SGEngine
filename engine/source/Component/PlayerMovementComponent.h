#ifndef PlayerMovementComponent_h
#define PlayerMovementComponent_h

#include "SGEngine.h"

#define PERSON_FIRST 0
#define PERSON_THIRD 1

class PlayerMovementComponent: public sge::Component{
        
    protected:
        
        sge::UpdateReceiver* _updateReceiver;
        sge::KeyboardReceiver* _keyBoardReceiver;
        sge::TouchReceiver* _touchReceiver;
    
        // How fast does the entity move
        float _velocity;
        
        // Buttons pressed
        bool _forward;
        bool _backward;
        bool _left;
        bool _right;
        bool _up;
        bool _space;
        bool _down;
        bool _run;
        
        sge::Vec3 _moveVec;
    
        sge::Vec3 _moveDirForward;
        sge::Vec3 _moveDirRight;
    
    sge::Camera* _moveCamera;

        // Is it third person or thirst person
        int _mode;
        
        virtual void setTypeName();
    
        sge::PhysicsComponent* _physicsComponent;
    
        
    public:
        
        PlayerMovementComponent();
        ~PlayerMovementComponent();
    
    void setMoveCamera(sge::Camera* camera);
        
        virtual void setEnabled(bool enabled);
        void setLockAngles(bool lock);
        
        void setVelocity(float velocity);
        
        bool isRunning();
        void setMode(int mode);
    
        void setMoveDir(const sge::Vec3& moveDir);
        
        // Registered Functions
        void update(float delta);
        virtual void addedToEntity(sge::Entity* entity);

    
        void didReleaseKey(KeyEvent* event);
        void didPressKey(KeyEvent* event);
        
        void touchBegan(sge::TouchEvent* event);
        void touchMoved(sge::TouchEvent* event);
        void touchEnded(sge::TouchEvent* event);
        
    };
    

#endif