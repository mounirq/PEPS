#include <iostream>
#include "AbstractPricer.hpp"
#include "PnlRandom.hpp"

AbstractPricer::~AbstractPricer() {}

AbstractPricer::AbstractPricer(BlackScholesModel *mod, Option *opt, RandomGenerator *rng, double fdStep, size_t nbSamples, int H) {
    mod_ = mod;
    opt_ = opt;
    rng_ = rng;
    fdStep_ = fdStep;
    nbSamples_ = nbSamples;
    H_ = H;
}

void AbstractPricer::delta(const PnlMat *past, double t, PnlVect *delta, PnlVect *icDelta) {}

void AbstractPricer::price(double &prix, double &ic) {}

void AbstractPricer::price(const PnlMat *past, double t, double &prix, double &ic) {}

AbstractPricer::AbstractPricer() {

}
