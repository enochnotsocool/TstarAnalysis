/*******************************************************************************
 *
 *  Filename    : SampleRooFitMgr.cc
 *  Description : implementation of roofit manager class
 *  Author      : Yi-Mu "Enoch" Chen [ ensc@hep1.phys.ntu.edu.tw ]
 *
*******************************************************************************/
#include "TstarAnalysis/CompareDataMC/interface/SampleRooFitMgr.hh"
#include "TstarAnalysis/Utils/interface/SampleMgr.hh"
#include "TstarAnalysis/TstarMassReco/interface/RecoResult.hh"
#include "SimDataFormats/GeneratorProducts/interface/LHEEventProduct.h"
#include "DataFormats/FWLite/interface/Handle.h"

#include <stdio.h>

using namespace std;
//------------------------------------------------------------------------------
//   Static member function delcarations
//------------------------------------------------------------------------------
static const double minmass    = 350.;
static const double minfitmass = 350.;
static const double maxmass    = 3000.;
static const double minweight  = -10000.;
static const double maxweight  =  10000.;
RooRealVar SampleRooFitMgr::_x( "x" , "M_{t+g}"      , minmass   , maxmass , "GeV/c^{2}" );
RooRealVar SampleRooFitMgr::_w( "w" , "event_weight" , minweight , maxweight , "");
double SampleRooFitMgr::MinMass()    { return minmass; }
double SampleRooFitMgr::MinFitMass() { return minfitmass; }
double SampleRooFitMgr::MaxMass()    { return maxmass; }

//------------------------------------------------------------------------------
//   Constructor and desctructor
//------------------------------------------------------------------------------
SampleRooFitMgr::SampleRooFitMgr( const string& name ):
   SampleGroup(name)
{
   _dataset = new RooDataSet( Name().c_str(), Name().c_str(), RooArgSet(_x,_w), RooFit::WeightVar(_w) );
   for( auto& sample : SampleList() ){
      FillDataSet( sample );
   }
}

SampleRooFitMgr::~SampleRooFitMgr()
{
   delete _dataset;
   for( auto& dataset : _ext_dataset ){
      delete dataset;
   }
   for( auto& pdf : _pdf_list){
      delete pdf;
   }
}

//------------------------------------------------------------------------------
//   Filling original dataset
//------------------------------------------------------------------------------
void SampleRooFitMgr::FillDataSet( SampleMgr& sample )
{
   fwlite::Handle<LHEEventProduct>  lheHandle;
   fwlite::Handle<RecoResult>  chiHandle;
   double sample_weight =  1. ;

   if( !sample.IsRealData() ){
      sample_weight = sample.GetSampleWeight();
   }

   unsigned i = 0 ;
   LOOP_EVENT( sample.Event() ){
      printf( "\rSample [%s|%s], Event[%6u/%6llu]..." ,
         Name().c_str(),
         sample.Name().c_str(),
         ++i ,
         sample.Event().size() );
      fflush(stdout);

      chiHandle.getByLabel( sample.Event() , "tstarMassReco" , "ChiSquareResult" , "TstarMassReco" );
      if( !sample.IsRealData() ){
         lheHandle.getByLabel( sample.Event() , "externalLHEProducer" );
      }

      double tstarMass = chiHandle->TstarMass() ;
      double event_weight = 1.0 ;
      if( lheHandle.isValid() && lheHandle->hepeup().XWGTUP <= 0 ) { event_weight = -1.; }
      double weight = event_weight * sample_weight ;
      if( minmass <=tstarMass && tstarMass <= maxmass  &&
          minweight <= weight && weight <= maxweight ){
         _x = tstarMass;
         _dataset->add( RooArgSet(_x) , weight );
      }
   }
   cout << "Done!" << endl;
}

//------------------------------------------------------------------------------
//   Dataset augmentation
//------------------------------------------------------------------------------
RooDataSet* SampleRooFitMgr::MakeReduceDataSet(
   const string& name ,
   const RooCmdArg& arg1,
   const RooCmdArg& arg2 )
{
   const string new_name = Name() + name;
   _ext_dataset.push_back( (RooDataSet*) _dataset->reduce(
      RooFit::Name(new_name.c_str()),
      RooFit::Title(new_name.c_str()),
      arg1,
      arg2 ) );
   return _ext_dataset.back();
}

RooDataSet* SampleRooFitMgr::GetReduceDataSet( const string& name )
{
   for( auto dataset : _ext_dataset ){
      if( dataset->GetName() == MakeDataAlias(name) ){
         return dataset;
      }
   }
   return _dataset;
}

void SampleRooFitMgr::AddDataSet( RooDataSet* x )
{
   _ext_dataset.push_back( x );
}

RooDataSet* SampleRooFitMgr::GetDataFromAlias( const string& name )
{
   return GetReduceDataSet(name);
}

std::string SampleRooFitMgr::MakeDataAlias( const string& name ) const
{
   return Name() + name;
}



//------------------------------------------------------------------------------
//   PDF Related functions
//------------------------------------------------------------------------------
void SampleRooFitMgr::AddPdf( RooAbsPdf* x ){
   _pdf_list.push_back(x);
}

RooAbsPdf* SampleRooFitMgr::GetPdfFromName( const string& name )
{
   for( auto pdf : _pdf_list ){
      if( pdf->GetName() == name ){
         return pdf;
      }
   }
   return NULL;
}

RooAbsPdf* SampleRooFitMgr::GetPdfFromAlias( const string& alias )
{
   for( auto pdf : _pdf_list ){
      if( pdf->GetName() == MakePdfAlias(alias) ){
         return pdf;
      }
   }
   return NULL;
}

string SampleRooFitMgr::MakePdfAlias( const string& alias ) const
{
   return Name() + alias + "_pdf";
}
