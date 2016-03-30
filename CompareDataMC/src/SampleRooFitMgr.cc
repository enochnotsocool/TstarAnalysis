/*******************************************************************************
 *
 *  Filename    : SampleRooFitMgr.cc
 *  Description : implementation of roofit manager class
 *  Author      : Yi-Mu "Enoch" Chen [ ensc@hep1.phys.ntu.edu.tw ]
 *
*******************************************************************************/
#include "TstarAnalysis/CompareDataMC/interface/SampleRooFitMgr.hh"
#include "TstarAnalysis/Utils/interface/SampleMgr.hh"
#include "TstarAnalysis/TstarMassReco/interface/ChiSquareResult.h"
#include "SimDataFormats/GeneratorProducts/interface/LHEEventProduct.h"
#include "DataFormats/FWLite/interface/Handle.h"

#include <stdio.h>

using namespace std;
//------------------------------------------------------------------------------
//   Static member function delcarations
//------------------------------------------------------------------------------
static const double minmass = 0.;
static const double maxmass = 3000.;
static const double minweight = -10000.;
static const double maxweight = +10000.;
RooRealVar SampleRooFitMgr::_x( "x" , "M_{t+g}"      , minmass     , maxmass , "GeV/c^{2}" );
RooRealVar SampleRooFitMgr::_w( "w" , "event_weight" , minweight , maxweight , "");

void SampleRooFitMgr::LoadJsonFile( const string& filename )
{
   SampleMgr::LoadJsonFile(filename);
}

//------------------------------------------------------------------------------
//   Constructor and desctructor
//------------------------------------------------------------------------------
SampleRooFitMgr::SampleRooFitMgr( const string& name ):
   _name(name)
{
   _dataset = new RooDataSet( name.c_str(), name.c_str(), RooArgSet(_x,_w), RooFit::WeightVar(_w) );
   FillDataSet( name );
}

SampleRooFitMgr::SampleRooFitMgr( const string& name , const vector<string>& sample_list ):
   _name(name)
{
   _dataset = new RooDataSet( name.c_str(), name.c_str(), RooArgSet(_x,_w), RooFit::WeightVar(_w) );
   for( const auto& sample : sample_list ){
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
void SampleRooFitMgr::FillDataSet( const string& name )
{
   SampleMgr sample( name );
   fwlite::Handle<LHEEventProduct>  lheHandle;
   fwlite::Handle<ChiSquareResult>  chiHandle;
   double total_lumi = -1.0;
   double sample_weight =  1. ;

   if( name.find("Data") == string::npos ){
      total_lumi = SampleMgr::TotalLuminosity();
      sample_weight = sample.GetSampleWeight( total_lumi );
   }

   unsigned i = 0 ;
   LOOP_EVENT( sample.Event() ){
      printf( "\rSample [%20s], Event[%6u/%6llu]..." ,
      sample.Name().c_str(),
      ++i ,
      sample.Event().size() );
      fflush(stdout);

      chiHandle.getByLabel( sample.Event() , "tstarMassReco" , "ChiSquareResult" , "TstarMassReco" );
      if( total_lumi > 0 ){
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
   const string new_name = _name + name;
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
      if( dataset->GetName() == _name + name ){
         return dataset;
      }
   }
   return NULL;
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
   return _name + alias + "_pdf";
}
