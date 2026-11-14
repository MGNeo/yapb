#include <ai.h>
#include <stdio.h>
#include <cstdlib>

namespace ai
{
    SmartFloat::SmartFloat(const float min, const float max)
    {
	if (min > max)
	{
	    fprintf(stderr, "%s, min > max.\n", __func__);
	    abort();
	}
	minSourceValue = min;
	maxSourceValue = max;
	normalizedValue = 0;
    }

    SmartFloat& SmartFloat::operator=(const SmartFloat &smartFloat)
    {
	if (this != &smartFloat)
	{
	    minSourceValue = smartFloat.minSourceValue;
	    maxSourceValue = smartFloat.maxSourceValue;
	    normalizedValue = smartFloat.normalizedValue;
	}
	return *this;
    }

    float SmartFloat::get() const
    {
	return minSourceValue + normalizedValue * (maxSourceValue - minSourceValue);
    }

    void SmartFloat::set(const float sourceValue)
    {
	if (sourceValue > maxSourceValue)
	{
	    fprintf(stderr, "%s, sourceValue > maxSourceValue.\n", __func__);
	    normalizedValue = 1.f;
	    return;
	}

	if (sourceValue < minSourceValue)
	{
	    fprintf(stderr, "%s, sourceValue < minSourceValue.\n", __func__);
	    normalizedValue = 0.f;
	    return;
	}
	
	normalizedValue = (sourceValue - minSourceValue) / (maxSourceValue - minSourceValue);
    }

    Input::Input() :
	time(0, 30 * 60),
	health(0, 100),
	armor(0, 100),
	currentWeapon(0, 32),
	currentMagazine(0, 1),// TODO
	currentTotalAmmo(0, 1),// TODO
	viewAngleX(-89, +89),
	viewAngleY(-180, +180),
	speedX(-250, +250),
	speedY(-200, +200),
	speedZ(-250, +250),
	punchAngleX(-89, +89),
	punchAngleY(-89, +89)
    {
        for (size_t i = 0; i < sizeof(view) / sizeof(view[0]); ++i)
        {
            view[i] = SmartFloat(0, 1);
        }

        for (size_t i = 0; i < sizeof(nearestInvisibleEnemyNoises) / sizeof(nearestInvisibleEnemyNoises[0]); ++i)
        {
            nearestInvisibleEnemyNoises[i].x = SmartFloat(-89, +89);
            nearestInvisibleEnemyNoises[i].y = SmartFloat(-180, +180);
            nearestInvisibleEnemyNoises[i].f = SmartFloat(0, 1);
        }

        for (size_t i = 0; i < sizeof(nearestVisibleEnemies) / sizeof(nearestVisibleEnemies[0]); ++i)
        {
            nearestVisibleEnemies[i].x = SmartFloat(-89, +89);
            nearestVisibleEnemies[i].y = SmartFloat(-180, +180);
            nearestVisibleEnemies[i].f = SmartFloat(0, 1);
        }

        for (size_t i = 0; i < sizeof(nearestFriends) / sizeof(nearestFriends[0]); ++i)
        {
            nearestFriends[i].x = SmartFloat(-89, +89);
            nearestFriends[i].y = SmartFloat(-180, +180);
            nearestFriends[i].f = SmartFloat(0, 1);
        }
    }

    Output::Output() :
	fire(0, 1),
	reload(0, 1),
	shift(0, 1),
	alt(0, 1),
	changeViewAngleX(-89, +89),
	changeViewAngleY(-180, +180),
	changeSpeedX(-250, +250),
	changeSpeedY(-200, +200),
	changeSpeedZ(-250, +250)
    {
    }

    Brain::Brain()
    {
	settings.input_count = 50;
	settings.output_count = 50;
	settings.frame_count = 30;
	settings.scale_stage_count = 3;
	settings.hidden_actor_layer_size = 256;
	settings.hidden_critic_layer_size = 64;

	brain = brain_new(settings);

	if (brain == NULL)
	{
	    fprintf(stderr, "%s, brain == NULL.\n", __func__);
	}
    }

    Brain::~Brain()
    {
	brain_delete(brain);
    }

    Input& Brain::getInput()
    {
	return input;
    }

    void Brain::tick()
    {
	
	brain_tick(brain, 0);
	
    }

    const Output& Brain::getOutput()
    {
	return output;
    }

}