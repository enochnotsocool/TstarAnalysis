/*******************************************************************************
 *
 *  Filename    : SampleMgr.cc
 *  Description : Implementation for sample Manager
 *  Author      : Yi-Mu "Enoch" Chen [ ensc@hep1.phys.ntu.edu.tw ]
 *
*******************************************************************************/
#include "TstarAnalysis/Utils/interface/SampleMgr.hh"

#include <boost/property_tree/json_parser.hpp>
#include <iostream>
#include <string>

using namespace std;

//------------------------------------------------------------------------------
//   Constructor and desctructor
//------------------------------------------------------------------------------
SampleMgr::SampleMgr( const std::string& filename ):
   JsonLoader( filename ),
   _name      ( JsonParser().get<string>("Name") ),
   _latexname ( JsonParser().get<string>("Latex Name") ),
   _crossSection( GetParameter ( "Cross Section" )),
   _selectionEff( GetParameter ( "Selection Efficiency" )),
   _weight      ( GetParameter ( "Sample Weight")),
   _event       ( GetStringList("EDM Files" ))
{
}

SampleMgr::~SampleMgr()
{}

//------------------------------------------------------------------------------
//   Computation results
//------------------------------------------------------------------------------
Parameter SampleMgr::ExpectedYield( const double totalLumi )const
{
   return totalLumi * CrossSection() * SelectionEfficiency() * Weight();
}
