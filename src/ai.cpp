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

    NormalizedOutput SourceOutput::normalize()
    {
        NormalizedOutput normalizedOutput;

        normalizedOutput.fire = normalizeByInterval(fire, 0, 1);
        normalizedOutput.reload = normalizeByInterval(reload, 0, 1);
        normalizedOutput.shift = normalizeByInterval(shift, 0, 1);
        normalizedOutput.alt = normalizeByInterval(alt, 0, 1);

        normalizedOutput.changeViewAngleX = normalizeByInterval(changeViewAngleX, 0, 1);
        normalizedOutput.changeViewAngleY = normalizeByInterval(changeViewAngleY, 0, 1);

        normalizedOutput.changeSpeedX = normalizeByInterval(changeSpeedX, 0, 1);
        normalizedOutput.changeSpeedY = normalizeByInterval(changeSpeedY, 0, 1);
        normalizedOutput.changeSpeedZ = normalizeByInterval(changeSpeedZ, 0, 1);

        return normalizedOutput;
    }

}