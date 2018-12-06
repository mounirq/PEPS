#ifndef VANILLA_OPTION_H
#define VANILLA_OPTION_H
#pragma once

#include "Option.hpp"

/// VanillaOption implement Option

class VanillaOption : public Option
{
public:
    double strike_;

    ~VanillaOption();

    VanillaOption();
    VanillaOption(double T_, int nbTimeSteps_, int size_, double strike);
    VanillaOption(const VanillaOption &vanillaOption);
    double payoff(const PnlMat *path);
};
#endif