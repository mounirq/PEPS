#include <iostream>
#include <string>
#include <pnl/pnl_vector.h>
#include <pnl/pnl_random.h>
#include "BlackScholesModel.hpp"
#include "MonteCarlo.hpp"
#include "PricerMC.hpp"
#include "PricerBS.hpp"
#include "ActicciaProduct.hpp"
#include "Couverture.hpp"
#include <ctime>

using namespace std;

int main(int argc, char **argv) {
	/* Test pricing of a vanillaOption using MonteCarlo and compare it to the BS formula */

	/*VanillaOption *vanillaOption = new VanillaOption(3, 26, 1, 107);
	PnlVect *sigma = pnl_vect_create_from_double(1, 0.2);
	PnlVect *spot = pnl_vect_create_from_double(1, 100);
	BlackScholesModel* model = new BlackScholesModel(1, 0.2, 0, sigma, spot);

	PnlRandom* rng = new PnlRandom();
	PricerMC *pricerMC = new PricerMC(model, vanillaOption, rng, 0.1, 5000, 1);

	double prix, ic;
	pricerMC->price(prix, ic);
	PnlVect *delta = pnl_vect_create_from_double(1, 0);
	PnlVect *icDelta = pnl_vect_create_from_double(1, 0);
	PnlMat *past = pnl_mat_create(1, 1);
	pricerMC->delta(past, 0, delta, icDelta);
	cout << "Voici le prix MC : " << prix << "et l'IC " << ic << " et Delta : ";
	pnl_vect_print(delta);

	PricerBS *pricerBS = new PricerBS(model, vanillaOption, rng, 0.1, 1000, 1);
	double prixBS, ic2 = 1;
	PnlVect *deltaBS = pnl_vect_create_from_double(1, 0);
	PnlVect *icDeltaBS = pnl_vect_create_from_double(1, 0);
	PnlMat *pastBS = pnl_mat_create(1, 20);
	pnl_mat_set(pastBS, 0, 0, 100);
	pricerBS->price(prixBS, ic2);
	pricerBS->delta(pastBS, 0, deltaBS, icDeltaBS);
	cout << "Voici le prix BS : " << prixBS << "et l'IC " << ic2 << " et Delta : ";
	pnl_vect_print(deltaBS);*/


	/* Compute Price and Deltas of Acticca*/
	/*ActicciaProduct* acticciaProduct = new ActicciaProduct(2, 26, 20);
	PnlVect *sigma = pnl_vect_create_from_double(20, 0.2);
	PnlVect *spot = pnl_vect_create_from_double(20, 100);
	BlackScholesModel* model = new BlackScholesModel(20, 0.2, 0, sigma, spot);

	PnlRandom* rng = new PnlRandom();
	PricerMC *pricerMC = new PricerMC(model, acticciaProduct, rng, 0.1, 50000, 1);

	double prix = 0 , ic = 0;
	pricerMC->price(prix, ic);
	cout << "Voici le prix MC d'Acticcia : " << prix << "et l'IC " << ic << "\n";

	double t = 1.0 / 27.0;
	PnlVect *delta = pnl_vect_create_from_double(20, 0);
	PnlVect *icDelta = pnl_vect_create_from_double(20, 0);
	PnlMat *past = pnl_mat_create(1, 20);
	pnl_mat_set_row(past, spot, 0);
	cout << "this is the past matrix : \n";
	pnl_mat_print(past);
	pricerMC->delta(past, t, delta, icDelta);
	cout << " \nVecteur des Deltas : " << "\n";
	pnl_vect_print(delta);*/

	int H = 12;
	int N = 4;
	double T = 0.5;
	ActicciaProduct* acticciaProduct = new ActicciaProduct(T, N, 20);
	PnlVect *sigma = pnl_vect_create_from_double(20, 0.2);
	PnlVect *spot = pnl_vect_create_from_double(20, 100);
	BlackScholesModel* model = new BlackScholesModel(20, 0.2, 0, sigma, spot);
	PnlRandom* rng = new PnlRandom();
	PricerMC *pricerMC = new PricerMC(model, acticciaProduct, rng, 0.1, 10000, H);

	PnlMat* market_trajectory = pnl_mat_create(13, 20);
	model->simul_market(market_trajectory, T, H, rng);

	Couverture* couverture = new Couverture(H, pricerMC);
	double P_and_L = 0;
	couverture->profits_and_losses2(market_trajectory, P_and_L);
	cout << "Acticcia prices : \n";
	pnl_vect_print(couverture->accticia_prices_);
	cout << "Portfolio values : \n";
	pnl_vect_print(couverture->portfolio_values_);
	/*FILE *file1 = fopen("C:/Users/Mounir/Desktop/3A/PEPS/acticcia_prices.txt", "w");
	if (file1 == NULL) {
		perror("Error opening file");
	}
	else {
		pnl_vect_fprint(file1, couverture->accticia_prices_);
	}
	
	FILE *file2 = fopen("C:/Users/Mounir/Desktop/3A/PEPS/portfolio_values.txt", "w");
	if (file2 == NULL) {
		perror("Error opening file");
	}
	else {
		pnl_vect_fprint(file2, couverture->portfolio_values_);
	}*/

	return 0;
}
