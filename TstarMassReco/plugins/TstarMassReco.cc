/*******************************************************************************
 *
 *  Filename    : TstarMassReco.cc
 *  Description : Tstar Mass reconstruction algorithms
 *  Author      : Yi-Mu "Enoch" Chen [ ensc@hep1.phys.ntu.edu.tw ]
 *
*******************************************************************************/
#include <memory>
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/stream/EDFilter.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "DataFormats/PatCandidates/interface/MET.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include <vector>
#include <iostream>

#include "TstarAnalysis/TstarMassReco/interface/ChiSquareResult.h"
#include "TstarAnalysis/TstarMassReco/interface/ChiSquareSolver.h"
#include "TstarAnalysis/TstarMassReco/interface/HitFitResult.h"
#include "TstarAnalysis/TstarMassReco/interface/HitFitter.h"

typedef std::vector<pat::MET> METList;
typedef std::vector<pat::Muon>  MuonList;
typedef std::vector<pat::Electron> ElectronList ;
typedef std::vector<pat::Jet>   JetList;

//------------------------------------------------------------------------------
//   Class Definition
//------------------------------------------------------------------------------
class TstarMassReco : public edm::EDProducer {
public:
   TstarMassReco(const edm::ParameterSet& );
   virtual ~TstarMassReco();

private:
   virtual void produce( edm::Event&, const edm::EventSetup& );

   const edm::EDGetToken  _metsrc;
   const edm::EDGetToken  _muonsrc;
   const edm::EDGetToken  _elecsrc;
   const edm::EDGetToken  _jetsrc;
   edm::Handle<METList>      _metHandle;
   edm::Handle<MuonList>     _muonHandle;
   edm::Handle<ElectronList> _elecHandle;
   edm::Handle<JetList>      _jetHandle;

   ChiSquareSolver _solver;
   HitFitter       _hitfitter;
};

//------------------------------------------------------------------------------
//   Constructor and Desctructor
//------------------------------------------------------------------------------
TstarMassReco::TstarMassReco( const edm::ParameterSet& iConfig ):
   _metsrc(  consumes<METList>     (iConfig.getParameter<edm::InputTag>("metsrc"))      ),
   _muonsrc( consumes<MuonList>    (iConfig.getParameter<edm::InputTag>("muonsrc"))     ),
   _elecsrc( consumes<ElectronList>(iConfig.getParameter<edm::InputTag>("electronsrc")) ),
   _jetsrc(  consumes<JetList>     (iConfig.getParameter<edm::InputTag>("jetsrc"))      ),
   _solver   (iConfig),
   _hitfitter(iConfig)
{
   produces<ChiSquareResult>("ChiSquareResult").setBranchAlias("ChiSquareResult");
   produces<HitFitResult>   ("HitFitResult")   .setBranchAlias("HitFitResult");
}

TstarMassReco::~TstarMassReco()
{}


//------------------------------------------------------------------------------
//   Main control flow
//------------------------------------------------------------------------------
void TstarMassReco::produce( edm::Event& iEvent, const edm::EventSetup& )
{
   iEvent.getByToken( _metsrc  , _metHandle  );
   iEvent.getByToken( _muonsrc , _muonHandle );
   iEvent.getByToken( _elecsrc , _elecHandle );
   iEvent.getByToken( _jetsrc  , _jetHandle  );
   std::auto_ptr<ChiSquareResult>  _chisq( new ChiSquareResult );
   std::auto_ptr<HitFitResult>     _hitfit( new HitFitResult );

   const METList&  metList   = *(_metHandle.product() );
   const MuonList& muList    = *(_muonHandle.product());
   const ElectronList& elecList = *(_elecHandle.product());
   const JetList&  jetList   = *(_jetHandle);

   //----- ChiSquare solver -----
   _solver.ClearAll();
   _solver.SetMET( metList.front().pt() , metList.front().phi() );
   for( const auto& mu : muList ){
      _solver.SetLepton( mu.pt(), mu.eta(), mu.phi(), mu.energy() );
   }
   for( const auto& el : elecList ){
      _solver.SetLepton( el.pt(), el.eta(), el.phi(), el.energy() );
   }
   for( const auto& jet : jetList ){
      if( jet.bDiscriminator("pfCombinedInclusiveSecondaryVertexV2BJetTags") > 0.86 ){
         _solver.AddBTagJet( jet.pt(), jet.eta(), jet.phi(), jet.energy() );
      }else{
         _solver.AddLightJet( jet.pt(), jet.eta(), jet.phi(), jet.energy() );
      }
   }
   _solver.RunPermutations();
   *_chisq = _solver.BestResult();

   //----- HitFitter -----
   _hitfitter.ClearAll();
   _hitfitter.SetMET( metList.front().pt() , metList.front().phi() );
   for( const auto& mu : muList ){
    _hitfitter.SetLepton( mu.pt(), mu.eta(), mu.phi(), mu.energy() , ISMUON );
   }
   for( const auto& el : elecList ){
    _hitfitter.SetLepton( el.pt(), el.eta(), el.phi(), el.energy() , ISELECTRON );
   }
   for( const auto& jet : jetList ){
    if( jet.eta() > 2.4 ){
       std::cout << "Weird Jet Found!" << std::endl;
    }
    if( jet.bDiscriminator("pfCombinedInclusiveSecondaryVertexV2BJetTags") > 0.86 ){
       _hitfitter.AddBTagJet( jet.pt(), jet.eta(), jet.phi(), jet.energy() );
    }else{
       _hitfitter.AddLightJet( jet.pt(), jet.eta(), jet.phi(), jet.energy() );
    }
   }
   _hitfitter.RunPermutations();
   _hitfit->MakeResult( *(_hitfitter.GetBest()) );

   iEvent.put( _chisq , "ChiSquareResult" );
   iEvent.put( _hitfit, "HitFitResult");
}

DEFINE_FWK_MODULE(TstarMassReco);
