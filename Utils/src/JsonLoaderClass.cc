/*******************************************************************************
 *
 *  Filename    : JsonLoaderClass.cc
 *  Description : Implementation of base json loader class
 *  Author      : Yi-Mu "Enoch" Chen [ ensc@hep1.phys.ntu.edu.tw ]
 *
*******************************************************************************/
#include "TstarAnalysis/Utils/interface/JsonLoaderClass.hh"
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>
#include <iostream>
#include <string>

using namespace std;
using boost::property_tree::ptree;

//------------------------------------------------------------------------------
//   Constructor and desctructor
//------------------------------------------------------------------------------
JsonLoader::JsonLoader( const string& jsonfile )
{
   boost::property_tree::read_json( jsonfile , _json_parser );
}

JsonLoader::~JsonLoader(){}

//------------------------------------------------------------------------------
//   Public member fucntions
//------------------------------------------------------------------------------
void JsonLoader::Print_Json( const ptree& pt, unsigned level ) const
{
    ptree::const_iterator end = pt.end();
    for(ptree::const_iterator it = pt.begin(); it != end; ++it) {
      for( unsigned i = 0 ; i < level ; ++ i) {
         cout << "\t" << flush ;
      }
      std::cout << it->first << ": " << it->second.get_value<std::string>() << std::endl;
      Print_Json(it->second,level + 1 );
    }
}

//------------------------------------------------------------------------------
//   Helper protected functions
//------------------------------------------------------------------------------
vector<string> JsonLoader::GetStringList( const string& name ) const
{
   vector<string> ans;
   BOOST_FOREACH( const ptree::value_type &v , _json_parser.get_child(name) ){
      if( !v.first.empty() ){
         cerr << "Warning! Skipping over illegal format at branch: (" << name
              << ")  with index value: (" << v.first.data() << ")" << endl;
         continue;
      }
      // cout << v.second.data() << endl;
      ans.push_back( v.second.data() );
   }
   return ans;
}

vector<double> JsonLoader::GetDoubleList( const string& name ) const
{
   vector<double> ans;
   BOOST_FOREACH( const ptree::value_type &v , _json_parser.get_child(name) ){
      if( !v.first.empty() ){
         cerr << "Warning! Skipping over illegal format at branch: (" << name
              << ")  with index value: (" << v.first.data() << ")" << endl;
         continue;
      }
      // cout << v.second.data() << endl;
      ans.push_back( stod(v.second.data()) );
   }
   return ans;
}

Parameter JsonLoader::GetParameter( const string& name ) const
{
   vector<double> input = GetDoubleList( name );
   input.resize( 5 , 0 );
   if( input[0] == 0 ){
      input[0] = 1;
   }
   if( input[1] != 0 && input[2] == 0 ){
      input[2] = input[1];
   }
   return Parameter( input[0], input[1], input[2], input[3], input[4] );
}
