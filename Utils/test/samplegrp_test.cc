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

   cout << "Hello world!" << endl;

   SampleGroup  myGroup("group.json", "MyGroup");
   cout << myGroup.LatexName() << endl;
   for( auto& sample : myGroup.SampleList() ){
      cout << sample.Name() << endl;
      cout << sample.ExpectedYield( 12.2 ) << endl;

      int i = 0 ;
      LOOP_EVENT( sample.Event() ){
         ++i;
         if( i > 10 ){ break;}
         cout << sample.Event().getLuminosityBlock().id()  << endl;
      }
   }

   return 0;
}
