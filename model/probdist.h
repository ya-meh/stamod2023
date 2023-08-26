#ifndef STATMOD_PROBDIST_H
#define STATMOD_PROBDIST_H


#include <cstdio>


void NORMAL(int type, double &x, double &p);

double pNormal(double x);

double xNormal(double prob);

void CHI(int type, double n, double &x, double &p);

double pChi(double x, size_t n);

double xChi(double prob, int n);


#endif //STATMOD_PROBDIST_H
