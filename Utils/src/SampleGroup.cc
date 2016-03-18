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
SampleGroup::SampleGroup( const string& jsonfile, const string& label ):
   JsonLoader( jsonfile , label ),
   _name( label ),
   _latexname( JsonParser().get<string>("Latex Name"))
{
   const vector<string> sample_config_list = GetStringList("Sample List");
   for( const auto& sample_config : sample_config_list ){
      _samplelist.push_back( SampleMgr(sample_config , "TEST" ) );
   }
}

SampleGroup::~SampleGroup(){}
