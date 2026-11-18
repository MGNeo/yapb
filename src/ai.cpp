#include <ai.h>
#include <stdio.h>

#include <cstdlib>

namespace ai
{

SmartFloat::SmartFloat(const float min, const float max)
{
    if (min > max)
    {
        fprintf(stderr, "%s, min(%f) > max(%f).\n", __func__, min, max);
        abort();
    }
    minSourceValue = min;
    maxSourceValue = max;
    normalizedValue = 0.0f;
}

SmartFloat &SmartFloat::operator=(const SmartFloat &smart)
{
    if (this != &smart)
    {
        minSourceValue = smart.minSourceValue;
        maxSourceValue = smart.maxSourceValue;
        normalizedValue = smart.normalizedValue;
    }
    return *this;
}

float SmartFloat::getSource() const
{
    return minSourceValue + normalizedValue * (maxSourceValue - minSourceValue);
}

void SmartFloat::setSource(const float source)
{
    if (source >= maxSourceValue)
    {
        normalizedValue = 1.0f;
        return;
    }
    if (source <= minSourceValue)
    {
        normalizedValue = 0.0f;
        return;
    }
    normalizedValue = (source - minSourceValue) / (maxSourceValue - minSourceValue);
}

float SmartFloat::getNormalized() const
{
    return normalizedValue;
}

void SmartFloat::setNormalized(const float normalized)
{
    if (normalized >= 1.0f)
    {
        normalizedValue = 1.0f;
        return;
    }
    if (normalized <= 0.0f)
    {
        normalizedValue = 0.0f;
        return;
    }
    normalizedValue = normalized;
}

Input::Input()
    : time(0, 30 * 60), health(0, 100), armor(0, 100), currentWeapon(0, 32), currentMagazine(0, 1),
      currentTotalAmmo(0, 1), viewAngleX(-89, +89), viewAngleY(-180, +180), speedX(-250, +250), speedY(-250, +250),
      speedZ(-200, +200), punchAngleX(-89, +89), punchAngleY(-89, +89)
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

Output::Output()
    : fire(0, 1), reload(0, 1), shift(0, 1), alt(0, 1),
      changeViewAngleX(-5.0f, +5.0f), // Макс. поворот по вертикали за 1 шаг (плавно, без юлы)
      changeViewAngleY(-12.0f,
                       +12.0f),      // Макс. поворот по горизонтали за 1 шаг (плавно)
      changeSpeedX(-250.0f, +250.0f), // Вперед / Назад (forwardmove)
      changeSpeedY(-250.0f, +250.0f), // Влево / Вправо (sidemove)
      changeSpeedZ(-200.0f, +200.0f)  // Вверх / Вниз (upmove)
{
}

size_t Brain::global_id = 0;

Brain::Brain()
{
    settings.input_count = 65;
    settings.output_count = 9;
    // Fully-connected recurrent network sizes (input + hidden + output).
    // Actor: 65 sensors + 54 hidden + 9 motor = 128 neurons.
    settings.actor_neuron_count = 128;
    // Critic: 65 sensors + 30 hidden + 1 value neuron = 96 neurons.
    settings.critic_neuron_count = 96;
    // Multi-scale activity traces: windows ~1, 2, 4, 8 ticks.
    settings.trace_count = 4;
    settings.max_adaptation_step = 0.002f;

    char filename[128] {0};
    id = global_id;
    sprintf(filename, "bot.%zu.brain", global_id);

    // Try to load brain from file.
    brain = brain_load_from_file(filename);
    if (brain == NULL)
    {
	brain = brain_new(settings);

	if (brain == NULL)
	{
    	    fprintf(stderr, "%s, brain == NULL.\n", __func__);
	    abort();
	}
	brain_fill_weights_by_noise(brain);
	++global_id;
    }
}

Brain::~Brain()
{
    if (brain != NULL)
    {
        brain_delete(brain);
    }
}

Input &Brain::getInput()
{
    return input;
}

void Brain::tick()
{
    if (brain == NULL)
    {
        return;
    }

    size_t i = 0;
    float inputs[1024]{};

    inputs[i++] = input.time.getNormalized();
    inputs[i++] = input.health.getNormalized();
    inputs[i++] = input.armor.getNormalized();
    inputs[i++] = input.currentWeapon.getNormalized();
    inputs[i++] = input.currentMagazine.getNormalized();
    inputs[i++] = input.currentTotalAmmo.getNormalized();
    inputs[i++] = input.viewAngleX.getNormalized();
    inputs[i++] = input.viewAngleY.getNormalized();
    inputs[i++] = input.speedX.getNormalized();
    inputs[i++] = input.speedY.getNormalized();
    inputs[i++] = input.speedZ.getNormalized();
    inputs[i++] = input.punchAngleX.getNormalized();
    inputs[i++] = input.punchAngleY.getNormalized();

    for (size_t v = 0; v < sizeof(input.view) / sizeof(input.view[0]); ++v)
    {
        inputs[i++] = input.view[v].getNormalized();
    }

    for (size_t n = 0; n < sizeof(input.nearestInvisibleEnemyNoises) / sizeof(input.nearestInvisibleEnemyNoises[0]);
         ++n)
    {
        inputs[i++] = input.nearestInvisibleEnemyNoises[n].x.getNormalized();
        inputs[i++] = input.nearestInvisibleEnemyNoises[n].y.getNormalized();
        inputs[i++] = input.nearestInvisibleEnemyNoises[n].f.getNormalized();
    }

    for (size_t n = 0; n < sizeof(input.nearestVisibleEnemies) / sizeof(input.nearestVisibleEnemies[0]); ++n)
    {
        inputs[i++] = input.nearestVisibleEnemies[n].x.getNormalized();
        inputs[i++] = input.nearestVisibleEnemies[n].y.getNormalized();
        inputs[i++] = input.nearestVisibleEnemies[n].f.getNormalized();
    }

    for (size_t n = 0; n < sizeof(input.nearestFriends) / sizeof(input.nearestFriends[0]); ++n)
    {
        inputs[i++] = input.nearestFriends[n].x.getNormalized();
        inputs[i++] = input.nearestFriends[n].y.getNormalized();
        inputs[i++] = input.nearestFriends[n].f.getNormalized();
    }

    brain_set_inputs(brain, inputs);

    // Передаем накопленную награду за шаг и сбрасываем ее
    brain_tick(brain, stepReward);
    stepReward = 0.0f;

    size_t o = 0;
    const float *outputs = brain_get_outputs(brain);

    output.fire.setNormalized(outputs[o++]);
    output.reload.setNormalized(outputs[o++]);
    output.shift.setNormalized(outputs[o++]);
    output.alt.setNormalized(outputs[o++]);

    output.changeViewAngleX.setNormalized(outputs[o++]);
    output.changeViewAngleY.setNormalized(outputs[o++]);
    output.changeSpeedX.setNormalized(outputs[o++]);
    output.changeSpeedY.setNormalized(outputs[o++]);
    output.changeSpeedZ.setNormalized(outputs[o++]);

    // Save brain to file every 1000 ticks.
    if (tick_count++ == 1000)
    {
	tick_count = 0;
	char filename[128]{0};
	sprintf(filename, "bot.%zu.brain", id);
	if (brain_save_to_file(brain, filename) == false)
	{
	    fprintf(stderr, "%s. brain_save_to_file() == false.\n", __func__);
	    abort();
	}
    }
}

const Output &Brain::getOutput()
{
    return output;
}

} // namespace ai
