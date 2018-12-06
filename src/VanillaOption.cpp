#include "VanillaOption.hpp"

VanillaOption::~VanillaOption(){
}

VanillaOption::VanillaOption():Option(){
    strike_ = 0;
}

BasketOption::BasketOption(double T, int nbTimeSteps, int size, double strike):Option(T,nbTimeSteps,size){
    strike_ = strike;
}

VanillaOption::VanillaOption(const VanillaOption &vanillaOption):Option(vanillaOption.T_,vanillaOption.nbTimeSteps_,vanillaOption.size_){
    strike_ = vanillaOption.strike_;
}

double VanillaOption::payoff(const PnlMat *path) {
    PnlVect * vector = pnl_vect_create(size_);
    pnl_mat_get_row(pnlVect, path, nbTimeSteps_);
    double returnedValue = GET(vector,0) - strike_;
    double payOffReturned = ( returnedValue > 0) ? returnedValue : 0;
    pnl_vect_free(&vector);
    return payOffReturned;
}
