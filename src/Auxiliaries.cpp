#include "Auxiliaries.h"

#include <random>

int getSign(float num)
{
	if (num == 0) return 0;
	else return (signbit(num) ? -1 : 1);
}

int GetRandomNumber(pcg32& rng, int min, int max, bool normal) {
    if (max == min) return min;

    if (!normal)
        return min + rng(max - min + 1);

    int mean = (max + min) / 2;
    std::normal_distribution<double> dist(mean, sqrt(max - mean));
    int rand = static_cast<int>(dist(rng));

    if (rand >= max) rand = max;
    else if (rand <= min) rand = min;
    return rand;
}

int GetRandomOddNumber(pcg32& rng, int min, int max, bool normal) {
    return GetRandomNumber(rng, min / 2, max / 2, normal) * 2 + 1;
}

int GetRandomEvenNumber(pcg32& rng, int min, int max, bool normal) {
    return GetRandomNumber(rng, min / 2, max / 2, normal) * 2;
}

const int GetRandomSeed() {
    std::random_device rd;
    return rd();
}
