/*******************************************************************************
 *
 *  Filename    : WeightCounter.cc
 *  Description : This stores information per event
 *  Author      : Yi-Mu "Enoch" Chen [ ensc@hep1.phys.ntu.edu.tw ]
 *
 *  Reference:
 *    CMSSW ExternalLHEProducer class
 *   https://cmssdt.cern.ch/SDT/doxygen/CMSSW_7_6_4/doc/html/d1/d24/ExternalLHEProducer_8cc_source.html
 *
*******************************************************************************/
// system include files
#include <memory>
#include <vector>
#include <algorithm>
#include <iostream>
#include <string>
// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/one/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/LuminosityBlock.h"
#include "FWCore/Framework/interface/Run.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "SimDataFormats/GeneratorProducts/interface/LHEEventProduct.h"

//------------------------------------------------------------------------------
//   Class Definition
//------------------------------------------------------------------------------
class WeightCounter : public edm::one::EDProducer<edm::BeginRunProducer,edm::EndRunProducer>
{
public:
   explicit WeightCounter(const edm::ParameterSet&);
   ~WeightCounter();

private:
   virtual void produce(edm::Event &, const edm::EventSetup&) override;
   virtual void beginRunProduce(edm::Run&,const edm::EventSetup&) override;
   virtual void endRunProduce(edm::Run&,const edm::EventSetup&)override;

   const std::string      _label;
   const edm::EDGetToken  _lhesrc;
   edm::Handle<LHEEventProduct> _lheHandle;
   unsigned _totalEventCount;
   unsigned _positiveEventCount;
   unsigned _negativeEventCount;
};
using namespace edm;
using namespace std;

//------------------------------------------------------------------------------
//   Constructor and destructor
//------------------------------------------------------------------------------
WeightCounter::WeightCounter(const edm::ParameterSet& iConfig):
   _label( iConfig.getParameter<std::string>("label")),
   _lhesrc( consumes<LHEEventProduct>(iConfig.getParameter<edm::InputTag>("lhesrc")) )
{
   produces<unsigned, edm::InRun>("totalEvents")   .setBranchAlias("totalEvents");
   produces<unsigned, edm::InRun>("positiveEvents").setBranchAlias("positiveEvents");
   produces<unsigned, edm::InRun>("negativeEvents").setBranchAlias("negativeEvents");
}

WeightCounter::~WeightCounter(){}

//------------------------------------------------------------------------------
//   Main Control flow
//------------------------------------------------------------------------------
void WeightCounter::beginRunProduce( edm::Run& , const EventSetup& )
{
   _totalEventCount = 0;
   _positiveEventCount = 0;
   _negativeEventCount = 0;
   return;
}

void WeightCounter::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   _totalEventCount++;
   iEvent.getByToken( _lhesrc , _lheHandle );
   if( _lheHandle.isValid() ){
      if( _lheHandle->hepeup().XWGTUP > 0 ){
         _positiveEventCount++;
      } else{
         _negativeEventCount++; }
   } else {
      _positiveEventCount++; }
}


void WeightCounter::endRunProduce( edm::Run& iRun, const EventSetup& iSetup)
{
   cout << "[" << _label << "] " << endl
      << "\tTotal:    " << _totalEventCount << " events" << endl
      << "\tPositive: " << _positiveEventCount << " events" << endl
      << "\tNegative: " << _negativeEventCount << " events" << endl;
   auto_ptr<unsigned> totalEvents   ( new unsigned);
   auto_ptr<unsigned> positiveEvents( new unsigned);
   auto_ptr<unsigned> negativeEvents( new unsigned);

   *totalEvents= _totalEventCount;
   *positiveEvents = _positiveEventCount;
   *negativeEvents = _negativeEventCount;

   iRun.put(totalEvents, "totalEvents" );
   iRun.put(positiveEvents, "positiveEvents");
   iRun.put(negativeEvents, "negativeEvents");
}



DEFINE_FWK_MODULE(WeightCounter);
