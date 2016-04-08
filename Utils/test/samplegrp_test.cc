/*******************************************************************************
 *
 *  Filename    : samplegrp_test.cc
 *  Description : Unit testing for Sample group class
 *  Author      : Yi-Mu "Enoch" Chen [ ensc@hep1.phys.ntu.edu.tw ]
 *
*******************************************************************************/
#include "TstarAnalysis/Utils/interface/SampleGroup.hh"
#include <iostream>

using namespace std;

int main(int argc, char* argv[]) {

   SampleGroup::LoadJsonFile( "sample.json" );
   SampleGroup all("All");
   SampleGroup bg("Background");
   SampleGroup ttjets("TTJets");

   cout << ttjets.ExpectedYield() << endl;
   cout << bg.ExpectedYield() << endl;
   for( const auto& sample : bg.SampleList() ){
      cout << sample.ExpectedYield() << endl;
   }
   return 0;
}
