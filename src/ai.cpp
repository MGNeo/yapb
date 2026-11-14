#include <ai.h>
#include <stdio.h>

namespace ai
{
    // [-inf; +int] -> [0; 1]
    float normalizeByInterval(const float value, const float min, const float max)
    {
        if (max < min)
        {
            fprintf(stderr, "%s, max < min.\n", __func__);
            return 0.f;
        }

        if (value < min)
        {
            fprintf(stderr, "%s, value < min.\n", __func__);
            return 0.f;
        }

        if (value > max)
        {
            fprintf(stderr, "%s, value > max.\n", __func__);
            return 1.f;
        }
        
        return (value - min) / (max - min);
    }

    // [0; 1] -> [-inf; +inf]
    float sourceByInterval(const float value, const float min, const float max)
    {
	if (max < min)
	{
	    fprintf(stderr, "%s, min > max.\n", __func__);
	    return 0.f;
	}

	if (value < 0.f)
	{
	    fprintf(stderr, "%s, value < 0.f.\n", __func__);
	    return 0.f;
	}

	if (value > 1.f)
	{
	    fprintf(stderr, "%s, value > 1.f.\n", __func__);
	    return 0.f;
	}

	return  value * (max - min) + min;
    }

    NormalizedInput SourceInput::normalize()
    {
        NormalizedInput normalizedInput;

        // 30 minutes is max.
        normalizedInput.time = normalizeByInterval(time, 0, 30 * 60);

        normalizedInput.health = normalizeByInterval(health, 0, 100);
        normalizedInput.armor = normalizeByInterval(health, 0, 100);

        normalizedInput.viewAngleX = normalizeByInterval(viewAngleX, -89, +89);
        normalizedInput.viewAngleY = normalizeByInterval(viewAngleY, -180, +180);

        normalizedInput.speedX = normalizeByInterval(speedX, -250, +250);
        normalizedInput.speedY = normalizeByInterval(speedY, -250, +250);
        normalizedInput.speedZ = normalizeByInterval(speedZ, -200, +200);

        normalizedInput.punchAngleX = normalizeByInterval(punchAngleX, -89, +89);
        normalizedInput.punchAngleY = normalizeByInterval(punchAngleY, -89, +89);

        for (size_t i = 0; i < sizeof(view) / sizeof(view[0]); ++i)
        {
            normalizedInput.view[i] = normalizeByInterval(view[i], 0, 1);
        }

        for (size_t i = 0; i < sizeof(nearestInvisibleEnemyNoises) / sizeof(nearestInvisibleEnemyNoises[0]); ++i)
        {
            normalizedInput.nearestInvisibleEnemyNoises[i].x = normalizeByInterval(nearestInvisibleEnemyNoises[i].x, 0, 1);
            normalizedInput.nearestInvisibleEnemyNoises[i].y = normalizeByInterval(nearestInvisibleEnemyNoises[i].y, 0, 1);
            normalizedInput.nearestInvisibleEnemyNoises[i].z = normalizeByInterval(nearestInvisibleEnemyNoises[i].z, 0, 1);
            normalizedInput.nearestInvisibleEnemyNoises[i].t = normalizeByInterval(nearestInvisibleEnemyNoises[i].t, 0, 1);
        }

        for (size_t i = 0; i < sizeof(nearestVisibleEnemies) / sizeof(nearestVisibleEnemies[0]); ++i)
        {
            normalizedInput.nearestVisibleEnemies[i].x = normalizeByInterval(nearestVisibleEnemies[i].x, 0, 1);
            normalizedInput.nearestVisibleEnemies[i].y = normalizeByInterval(nearestVisibleEnemies[i].y, 0, 1);
            normalizedInput.nearestVisibleEnemies[i].z = normalizeByInterval(nearestVisibleEnemies[i].z, 0, 1);
        }

        for (size_t i = 0; i < sizeof(nearestFriends) / sizeof(nearestFriends[0]); ++i)
        {
            normalizedInput.nearestFriends[i].x = normalizeByInterval(nearestFriends[i].x, 0, 1);
            normalizedInput.nearestFriends[i].y = normalizeByInterval(nearestFriends[i].y, 0, 1);
            normalizedInput.nearestFriends[i].z = normalizeByInterval(nearestFriends[i].z, 0, 1);
        }

        return normalizedInput;
    }

    SourceOutput NormalizedOutput::source()
    {
        SourceOutput sourceOutput;

        sourceOutput.fire = sourceByInterval(fire, 0, 1);
        sourceOutput.reload = sourceByInterval(reload, 0, 1);
        sourceOutput.shift = sourceByInterval(shift, 0, 1);
        sourceOutput.alt = sourceByInterval(alt, 0, 1);

        sourceOutput.changeViewAngleX = sourceByInterval(changeViewAngleX, 0, 1);
        sourceOutput.changeViewAngleY = sourceByInterval(changeViewAngleY, 0, 1);

        sourceOutput.changeSpeedX = sourceByInterval(changeSpeedX, 0, 1);
        sourceOutput.changeSpeedY = sourceByInterval(changeSpeedY, 0, 1);
        sourceOutput.changeSpeedZ = sourceByInterval(changeSpeedZ, 0, 1);

        return sourceOutput;
    }

    Brain::Brain()
    {
	struct settings settings;
	
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

    SourceInput& Brain::getSourceInput()
    {
	return sourceInput;
    }

    void Brain::tick()
    {
	
	brain_tick(brain, 0);
    }

    const SourceOutput& Brain::getSourceOutput()
    {
	
	
	return sourceOutput;
    }

}