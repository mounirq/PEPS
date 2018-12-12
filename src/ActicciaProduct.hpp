#ifndef ACTICCIA_PRODUCT_H
#define ACTICCIA_PRODUCT_H
#pragma once

#include "Option.hpp"

/// ActicciaProduct implement Option

class ActicciaProduct : public Option
{
public:
	
	double vlr_;

    ~ActicciaProduct();

    ActicciaProduct();
    ActicciaProduct(double T_, int nbTimeSteps_, int size_);
    ActicciaProduct(const ActicciaProduct &acticciaProduct);
    double payoff(const PnlMat *path);
};
#endif