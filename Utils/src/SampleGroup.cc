/*******************************************************************************
 *
 *  Filename    : SampleGroup.cc
 *  Description : Implementation for sample grouping class
 *  Author      : Yi-Mu "Enoch" Chen [ ensc@hep1.phys.ntu.edu.tw ]
 *
*******************************************************************************/
#include "TstarAnalysis/Utils/interface/SampleGroup.hh"

using namespace std;
//------------------------------------------------------------------------------
//   Constructors
//------------------------------------------------------------------------------
SampleGroup::SampleGroup( const string& name ):
   JsonLoader( name ),
   _latexname( GetString("Latex Name"))
{
   const string sample_json = GetString( "Sample Json" );
   const vector<string> sample_name_list = GetStringList( "Samples");
   for( const auto& sample_name : sample_name_list ){
      _samplelist.push_back( SampleMgr( sample_name ) );
   }
}

SampleGroup::~SampleGroup(){}
