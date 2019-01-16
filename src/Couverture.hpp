#ifndef PROJETPRICERGIT_COUVERTURE_HPP
#define PROJETPRICERGIT_COUVERTURE_HPP

#include <iostream>
#include "AbstractPricer.hpp"


class Couverture {
public:

    AbstractPricer *pricer_;
    int H_;
	PnlVect *accticia_prices_;
	PnlVect *portfolio_values_;

    Couverture(int H, AbstractPricer *pricer);

    ~Couverture();

    /**
     * Calcule le P&L a partir d'une trajectoire de marché donnée
     *
     * @param[in] market_trajectory contient la trajectoire du sous-jacent
     * sur une grille de temps 0 à T
     * @param[out] p_and_l  contient l'erreur de couverture
     */
    void profits_and_losses(const PnlMat *market_trajectory, double &p_and_l, double &pl_sur_P0);
    void profits_and_losses2(const PnlMat *market_trajectory, double &p_and_l);

};


#endif //PROJETPRICERGIT_COUVERTURE_HPP
