#include "ActicciaProduct.hpp"

ActicciaProduct::~ActicciaProduct(){
}

ActicciaProduct::ActicciaProduct():Option(){
    vlr_ = 0;
}

ActicciaProduct::ActicciaProduct(double T, int nbTimeSteps, int size, double vlr):Option(T,nbTimeSteps,size){
    vlr_ = vlr;
}

ActicciaProduct::ActicciaProduct(const ActicciaProduct &acticciaProduct):Option(acticciaProduct.T_,acticciaProduct.nbTimeSteps_,acticciaProduct.size_){
    vlr_ = acticciaProduct.vlr_;
}

double ActicciaProduct::payoff(const PnlMat *path) {
    PnlVect * vector = pnl_vect_create(size_);
	double intialValue = 0.;
	double finalValue = 0.;
	double perf = 0.0;
	double tmp = 0;
	for (int i = 0; i< size_; i++){
		pnl_mat_get_col(vector, path,i);
		initalValue = GET(vector, 0);
		finalValue = GET(vector, nbTimeSteps_);
		tmp = (finalValue - intialValue)/initialValue;
		tmp = (tmp < 0.85) ? tmp : 0.85;
		perf += tmp;
	}
	perf = perf/size_;
	double perfFinal = 0;
	perfFinal = (perf > -0.1) ? perf : -0.1;
    pnl_vect_free(&vector);
    return vlr_*(1+perfFinal);
}
