/*******************************************************************************
 *
 *  Filename    : samplemgr_test.cc
 *  Description : Unit testing range for SampleMgr classes
 *  Author      : Yi-Mu "Enoch" Chen [ ensc@hep1.phys.ntu.edu.tw ]
 *
*******************************************************************************/
#include "TstarAnalysis/Utils/interface/SampleMgr.hh"
#include <iostream>

#include "DataFormats/FWLite/interface/Handle.h"
#include "DataFormats/PatCandidates/interface/Jet.h"

using namespace std;

int main(int argc, char* argv[]) {
   cout << "Hello world!" << endl;

   cout << "Reading Json file" << endl;
   SampleMgr mysample("sample.json", "Single_T" );

   fwlite::Handle<std::vector<pat::Jet>> Jets;

   cout << mysample.KFactor().CentralValue() << endl;
   cout << mysample.EventsInFile() << endl;
   cout << mysample.ExpectedYield( 2.42 ) << endl;
   int i = 0;
   cout << "entering main loop" << endl;
   for( mysample.Event().toBegin() ; !mysample.Event().atEnd() ; ++mysample.Event() , ++i ){
      if( i > 10 ){break;}
      cout << "Event" << i << endl;
      Jets.getByLabel( mysample.Event() , "skimmedPatJets" );
      for( const auto& jet : *Jets.product() ){
         cout << jet.pt() << endl;
      }
   }


   return 0;
}
