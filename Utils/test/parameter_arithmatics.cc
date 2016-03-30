/*******************************************************************************
 *
 *  Filename    : parameter_arithmatics.cc
 *  Description : Testing the arithmatics of paramters
 *  Author      : Yi-Mu "Enoch" Chen [ ensc@hep1.phys.ntu.edu.tw ]
 *
*******************************************************************************/
#include "TstarAnalysis/Utils/interface/Parameter.hh"
#include <iostream>

using namespace std;

int main(int argc, char const *argv[]) {
   cout << "parameter_arithmatics.cc" << endl;

   Parameter a(10,1,1);
   Parameter b(20,1,1);
   Parameter c = a * b ;
   
   return 0;
}
