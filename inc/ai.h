#ifndef __AI_H__
#define __AI_H__

namespace ai
{
    class XYZT
    {
        public:
        
        float x;
        float y;
        float z;
        float t;
    };

    class XYZ
    {
        public:

        float x;
        float y;
        float z;
    };

    class Input
    {
        public:
        
        float time;

        float health;
        float armor;

        float currentWeapon;
        float currentMagazine;
        float currentTotalAmmo;

        float viewAngleX;
        float viewAngleY;

        float speedX;
        float speedY;
        float speedZ;

        float punchAngleX;
        float punchAngleY;

        float view[16];
        XYZT nearestInvisibleEnemyNoises[4];
        XYZ nearestVisibleEnemies[4];
        XYZ nearestFriends[4];
    };

    class NormalizedInput : public Input
    {
    };

    class SourceInput : public Input
    {
        public:
        // Normalize all values to [0; +1].
        NormalizedInput normalize();
    };

    class Output
    {
        public:

        float fire;
        float reload;
        float shift;
        float alt;
        
        // TODO: What about weapon selection?

        float changeViewAngleX;
        float changeViewAngleY;

        float changeSpeedX;
        float changeSpeedY;
        float changeSpeedZ;

        // ...
    };
    
    class SourceOutput : public Output
    {
        public:
    };
    
    class NormalizedOutput : public Output
    {
	public:
	SourceOutput source();
    };

    class Brain
    {
        public:

        Brain();

        SourceInput& getSourceInput();

        void tick() noexcept;

        const SourceOutput& getSourceOutput();

        private:

        SourceInput sourceInput;
        SourceOutput sourceOutput;
    };
    
    
}

#endif
