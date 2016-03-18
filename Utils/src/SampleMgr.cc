/*******************************************************************************
 *
 *  Filename    : SampleMgr.cc
 *  Description : Implementation for sample Manager
 *  Author      : Yi-Mu "Enoch" Chen [ ensc@hep1.phys.ntu.edu.tw ]
 *
*******************************************************************************/
#include "TstarAnalysis/Utils/interface/SampleMgr.hh"
#include "DataFormats/FWLite/interface/Handle.h"
#include "SimDataFormats/GeneratorProducts/interface/LHEEventProduct.h"

#include <boost/property_tree/json_parser.hpp>
#include <iostream>
#include <string>

using namespace std;

//------------------------------------------------------------------------------
//   Constructor and desctructor
//------------------------------------------------------------------------------
SampleMgr::SampleMgr( const std::string& filename, const std::string& label ):
   JsonLoader( filename, label ),
   _event         ( GetStringList("EDM Files" )),
   _name          ( label ),
   _latexname     ( JsonParser().get<string>("Latex Name") ),
   _cross_section ( GetParameter( "Cross Section" )),
   _k_factor      ( GetParameter( "K Factor")),
   _selection_eff ( MakeSelectionEfficiency() )
{
}

SampleMgr::~SampleMgr()
{}

//------------------------------------------------------------------------------
//   Computation results
//------------------------------------------------------------------------------
Parameter SampleMgr::ExpectedYield( const double totalLumi )const
{
   return totalLumi * CrossSection()* SelectionEfficiency() * KFactor();
}

Parameter SampleMgr::GetSampleWeight( const double totalLumi )
{
   return ExpectedYield(totalLumi);
}

Parameter SampleMgr::MakeSelectionEfficiency()
{
   return Parameter( 0,0,0);
}
