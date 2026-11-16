#ifndef __AI_H__
#define __AI_H__

extern "C"
{
#include <brain.h>
}

namespace ai
{
    class SmartFloat
    {
	public:
	SmartFloat(const float min = 0.f, const float max = 1.f);
	
	SmartFloat &operator=(const SmartFloat &smart);
	
	float getSource() const;
	void setSource(const float source);

	float getNormalized() const;
	void setNormalized(const float normalized);

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

// Внутри class Brain в вашем ai.h:
class Brain {
private:
   struct settings settings;
   struct brain *brain = nullptr;

   Input input {};
   Output output {};

   float stepReward = 0.0f; // Накопленная награда за шаг

public:
   Brain ();
   ~Brain ();

   Input &getInput ();
   const Output &getOutput ();

   void addReward (float reward) { stepReward += reward; }
   void tick ();
};
    
}

#endif
