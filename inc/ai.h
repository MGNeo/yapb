#ifndef __AI_H__
#define __AI_H__

#include "brain.h"

namespace ai
{
    class SmartFloat
    {
	public:
	SmartFloat(const float min = 0.f, const float max = 1.f);
	
	SmartFloat &operator=(const SmartFloat &smartFloat);
	// Get source value.
	float get() const;

	// Set source valuel.
	void set(const float value);

	private:
	float minSourceValue;
	float maxSourceValue;
	float normalizedValue;
    };

    struct XYF
    {
        SmartFloat x;
        SmartFloat y;
        SmartFloat f;
    };

    class Input
    {
        public:

	Input();

        SmartFloat time;

        SmartFloat health;
        SmartFloat armor;

        SmartFloat currentWeapon;
        SmartFloat currentMagazine;
        SmartFloat currentTotalAmmo;
	SmartFloat viewAngleX;
        SmartFloat viewAngleY;

        SmartFloat speedX;
        SmartFloat speedY;
        SmartFloat speedZ;

        SmartFloat punchAngleX;
        SmartFloat punchAngleY;

        SmartFloat view[16];

        XYF nearestInvisibleEnemyNoises[4];
        XYF nearestVisibleEnemies[4];
        XYF nearestFriends[4];
    };

    class Output
    {
        public:

	Output();

        SmartFloat fire;
        SmartFloat reload;
        SmartFloat shift;
        SmartFloat alt;
        
        // TODO: What about weapon selection?

        SmartFloat changeViewAngleX;
        SmartFloat changeViewAngleY;

        SmartFloat changeSpeedX;
        SmartFloat changeSpeedY;
        SmartFloat changeSpeedZ;

        // ...
    };

    class Brain
    {
        public:

        Brain();
	~Brain();

        Input& getInput();

        void tick() noexcept;

        const Output& getOutput();

        private:

        Input input;
        Output output;

	struct settings settings;
	struct brain* brain;
    };
    
    
}

#endif
