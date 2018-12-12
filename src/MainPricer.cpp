#include <iostream>
#include <string>
#include <pnl/pnl_vector.h>
#include <pnl/pnl_random.h>
#include "BlackScholesModel.hpp"
#include "MonteCarlo.hpp"
#include "PricerMC.hpp"
#include "PricerBS.hpp"
#include "ActicciaProduct.hpp"
#include <ctime>

using namespace std;

int main(int argc, char **argv) {
	/*VanillaOption *vanillaOption = new VanillaOption(3, 26, 1, 107);
	PnlVect *sigma = pnl_vect_create_from_double(1, 0.2);
	PnlVect *spot = pnl_vect_create_from_double(1, 100);
	BlackScholesModel* model = new BlackScholesModel(1, 0.2, 0, sigma, spot);

	PnlRandom* rng = new PnlRandom();
	PricerMC *pricerMC = new PricerMC(model, vanillaOption, rng, 0.1, 500000, 1);

	double prix, ic;
	pricerMC->price(prix, ic);

	PricerBS *pricerBS = new PricerBS(model, vanillaOption, rng, 0.1, 1000, 1);
	double prixBS, ic2 = 1;
	pricerBS->price(prixBS, ic2);

	cout << "Voici le prix MC : " << prix << "et l'IC " << ic << "\n";

	cout << "Voici le prix BS : " << prixBS << "et l'IC " << ic2 << "\n";*/

	ActicciaProduct* acticciaProduct = new ActicciaProduct(2, 26, 20);
	PnlVect *sigma = pnl_vect_create_from_double(20, 0.2);
	PnlVect *spot = pnl_vect_create_from_double(20, 100);
	BlackScholesModel* model = new BlackScholesModel(20, 0.2, 0, sigma, spot);

	PnlRandom* rng = new PnlRandom();
	PricerMC *pricerMC = new PricerMC(model, acticciaProduct, rng, 0.1, 500000, 1);

	double prix, ic;
	pricerMC->price(prix, ic);
	cout << "Voici le prix MC d'Acticcia : " << prix << "et l'IC " << ic << "\n";
	return 0;
	/*

    PricerMC *pricerMC = new PricerMC(argv[1]);
    PnlMat *market_trajectory;
    if(argc == 3){
        market_trajectory = pnl_mat_create_from_file(argv[2]);
    }
    else if (argc == 2){
        market_trajectory = pnl_mat_create(pricerMC->H_ + 1, pricerMC->mod_->size_);
        pricerMC->mod_->simul_market(market_trajectory, pricerMC->opt_->T_, pricerMC->H_,pricerMC->rng_);
    }
    else{
        cerr<<"\nERROR: Number of argument incorrect ";
        throw;
    }

    Couverture *couvertureMC = new Couverture(pricerMC);

    double PL_MC = 0, pl_sur_P0_MC=0;



    clock_t  start;
    start = clock();
    couvertureMC->profits_and_losses(market_trajectory, PL_MC, pl_sur_P0_MC);
    clock_t end = clock();

    cout << "Le P&L par MC est : " << PL_MC << endl;
    cout << "Le P&L/P0 par MC est : " << pl_sur_P0_MC << endl;

    cout<< "The profits and losses took : " << (end - start)/ (double)(CLOCKS_PER_SEC / 1000) << " ms \n";

    cout << "############" << endl;


    pnl_mat_free(&market_trajectory);
    delete couvertureMC;

    exit(0);*/
}