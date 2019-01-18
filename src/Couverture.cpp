#include "Couverture.hpp"

using namespace std;

Couverture::Couverture(int H, AbstractPricer *pricer)
{
	H_ = H;
	accticia_prices_ = pnl_vect_create(H_ + 1);
	portfolio_values_ = pnl_vect_create(H_ + 1);
	pricer_ = pricer;
}

Couverture::~Couverture() {
	delete pricer_;
}

void Couverture::profits_and_losses2(const PnlMat *market_trajectory, double &p_and_l)
{
	// Create variables needed
	int nbAssets = pricer_->opt_->size_;
	PnlVect *current_spot = pnl_vect_create(nbAssets);
	PnlVect *previous_delta = pnl_vect_create(nbAssets);
	PnlVect *current_delta = pnl_vect_create(nbAssets);
	PnlVect *icDelta = pnl_vect_create(nbAssets);
	int N = pricer_->opt_->nbTimeSteps_;
	PnlMat *past = pnl_mat_create(N + 1, nbAssets);
	PnlVect *vector_tmp = pnl_vect_create(nbAssets);
	PnlVect *delta_diff = pnl_vect_create(nbAssets);
	double accticia_current_price = 0;
	double portfolio_current_value = 0;
	double ic = 0;
	double portfolio_current_risk_value = 0;
	double portfolio_current_risk_free_value = 0;
	int past_matrix_size = 2;
	double time = 0;
	double actualization_factor = exp(pricer_->mod_->r_*pricer_->opt_->T_ / H_);

	if (H_ % N != 0) {
		cerr << "H is not a multiple of N";
		throw;
	}

	int numberDatesBetweenRebalancing = H_ / N;

	// In t = 0 :
	PnlMat *sub_past = pnl_mat_create(1, 20);
	pnl_vect_clone(current_spot, pricer_->mod_->spot_);
	pnl_mat_set_row(past, current_spot, 0);
	pnl_mat_set_row(sub_past, current_spot, 0);

	pricer_->price(accticia_current_price, ic);
	pnl_vect_set(accticia_prices_, 0, accticia_current_price);

	pricer_->delta(sub_past, time, current_delta, icDelta);
	portfolio_current_risk_value = pnl_vect_scalar_prod(current_delta, current_spot);
	portfolio_current_risk_free_value = accticia_current_price - portfolio_current_risk_value;
	portfolio_current_value = portfolio_current_risk_value + portfolio_current_risk_free_value;
	pnl_vect_clone(previous_delta, current_delta);

	pnl_vect_set(portfolio_values_, 0, portfolio_current_value);

	for (int i = 1; i <= H_; i++) {
		time = double(i) * pricer_->opt_->T_ / H_;

		pnl_mat_extract_subblock(sub_past, past, 0, past_matrix_size - 1, 0, nbAssets);

		pnl_mat_get_row(vector_tmp, market_trajectory, i);
		pnl_mat_add_row(sub_past, past_matrix_size - 1, vector_tmp);


		// Test if we are in an observation date
		if (i % numberDatesBetweenRebalancing == 0) {
			pnl_mat_set_row(past, vector_tmp, past_matrix_size - 1);
			past_matrix_size++;
		}

		// Price accticia and set acticcia_prices array
		pricer_->price(sub_past, time, accticia_current_price, ic);
		pnl_vect_set(accticia_prices_, i, accticia_current_price);

		// Compute delta and set portfolio_values array
		pricer_->delta(sub_past, time, current_delta, icDelta);
		// Delta_diff = delta(i) - delta(i-1)
		pnl_vect_clone(delta_diff, current_delta);
		pnl_vect_minus_vect(delta_diff, previous_delta);

		portfolio_current_risk_value = pnl_vect_scalar_prod(current_delta, vector_tmp);
		portfolio_current_risk_free_value = portfolio_current_risk_free_value * actualization_factor - pnl_vect_scalar_prod(delta_diff, vector_tmp);
		portfolio_current_value = portfolio_current_risk_value + portfolio_current_risk_free_value;

		pnl_vect_set(portfolio_values_, i, portfolio_current_value);
		pnl_vect_clone(previous_delta, current_delta);
	}

	p_and_l = portfolio_current_value + pnl_vect_scalar_prod(current_delta, vector_tmp) - pricer_->opt_->payoff(market_trajectory);

	pnl_vect_free(&current_spot);
	pnl_vect_free(&previous_delta);
	pnl_vect_free(&current_delta);
	pnl_vect_free(&icDelta);
	pnl_mat_free(&past);
	pnl_vect_free(&vector_tmp);
	pnl_vect_free(&delta_diff);
	pnl_mat_free(&sub_past);
}

void Couverture::profits_and_losses(const PnlMat *market_trajectory, double &p_and_l, double &pl_sur_P0)
{
	int H = market_trajectory->m - 1;
	int nbAssets = market_trajectory->n;

	PnlVect *vect_V = pnl_vect_create_from_scalar(H + 1, 0);

	PnlVect *spots = pnl_vect_create_from_scalar(nbAssets, 0);
	pnl_vect_clone(spots, pricer_->mod_->spot_);

	PnlMat *past = pnl_mat_create_from_scalar(pricer_->opt_->nbTimeSteps_ + 1, nbAssets, 0);
	PnlMat *sub_past = pnl_mat_new();

	PnlVect *ics = pnl_vect_create_from_scalar(pricer_->opt_->size_, 0); //pour les deltas

	PnlVect *diff_delta = pnl_vect_create_from_scalar(nbAssets, 0);
	PnlVect *prev_delta = pnl_vect_create_from_scalar(nbAssets, 0);
	PnlVect *deltas = pnl_vect_create_from_scalar(nbAssets, 0);

	PnlVect *tmp_row = pnl_vect_create_from_scalar(nbAssets, 0);

	pnl_mat_get_row(tmp_row, market_trajectory, 0);
	pnl_mat_set_row(past, tmp_row, 0);
	pnl_mat_extract_subblock(sub_past, past, 0, 1, 0, nbAssets);

	double prix = 0;
	double ic = 0;
	double v = 0;
	pricer_->price(prix, ic);

	double prix0 = prix;
	pricer_->delta(sub_past, 0, deltas, ics);
	//calcul de V(0)
	v = prix - pnl_vect_scalar_prod(deltas, spots);
	pnl_vect_set(vect_V, 0, v);

	double step_for_delta = pricer_->opt_->T_ / H;  // T/H
	double actualisationFactor = exp(pricer_->mod_->r_ * step_for_delta);

	int step_for_payoff = H / pricer_->opt_->nbTimeSteps_; // H/N

	double nbOfSpotsNeeded = 0;

	double index_row_to_fill = 1;
	for (int i = 1; i <= H; i++) {

		//Mise à jour de la matrice past
		if (i%step_for_payoff == 0) {
			pnl_mat_get_row(tmp_row, market_trajectory, i);
			pnl_mat_set_row(past, tmp_row, index_row_to_fill);
			index_row_to_fill++;
		}
		else {
			pnl_mat_get_row(tmp_row, market_trajectory, i);
			pnl_mat_set_row(past, tmp_row, index_row_to_fill);
		}

		nbOfSpotsNeeded = ceil(((double)i) / (double)step_for_payoff) + 1;
		pnl_mat_extract_subblock(sub_past, past, 0, nbOfSpotsNeeded, 0, nbAssets);

		// diff_delta = delta(i) - delta(i-1)
		pnl_vect_clone(prev_delta, deltas);
		pricer_->delta(sub_past, i*step_for_delta, deltas, ics);
		pnl_vect_clone(diff_delta, deltas);
		pnl_vect_minus_vect(diff_delta, prev_delta);

		//spots = S(tho_i)
		pnl_mat_get_row(spots, market_trajectory, i);

		// v = V(i-1) * exp(rT/H) - (delta(i) - delta(i-1)) * S(tho_i)
		v = pnl_vect_get(vect_V, i - 1) * actualisationFactor - pnl_vect_scalar_prod(diff_delta, spots);
		pnl_vect_set(vect_V, i, v);

	}

	// A la sortie de la boucle : deltas = delta(H) et spots = S(tho_H)
	p_and_l = pnl_vect_get(vect_V, H) + pnl_vect_scalar_prod(deltas, spots) - pricer_->opt_->payoff(past);

	pl_sur_P0 = p_and_l / prix0;
	//Free
	pnl_vect_free(&vect_V);
	pnl_vect_free(&spots);
	pnl_vect_free(&ics);
	pnl_vect_free(&diff_delta);
	pnl_vect_free(&prev_delta);
	pnl_vect_free(&deltas);
	pnl_vect_free(&tmp_row);
	pnl_mat_free(&sub_past);
	pnl_mat_free(&past);
}
