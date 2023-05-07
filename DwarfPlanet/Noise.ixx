export module Noise;

#include <FastNoise/FastNoiseLite.h>

export class Noise
{
	public:
		Noise(int seed = 0, float frequency = 0.25f)
		{
			this->seed = seed;
			this->frequency = frequency;
			noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
			noise.SetFrequency(frequency);
			noise.SetSeed(seed);
		}

		float getNoise2D(float x, float y)
		{
			return noise.GetNoise(x, y);
		}

		float remap(float input_start, float input_end, float output_start, float output_end, float input_value)
		{
			float r = input_end - input_start;
			float R = output_end - output_start;

			return output_start + (R / r) * (input_value - input_start);
		}

	private:
		int seed;
		float frequency;
		FastNoiseLite noise;

};