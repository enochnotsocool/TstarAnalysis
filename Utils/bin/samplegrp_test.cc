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

   SampleGroup  myGroup("group.json");
   cout << myGroup.Name() << endl;
   for( auto& sample : myGroup.SampleList() ){
      cout << sample.Name() << endl;
      for( sample.Event().toBegin() ; !sample.Event().atEnd(); ++sample.Event() ){
         
      }
   }

   return 0;
}
