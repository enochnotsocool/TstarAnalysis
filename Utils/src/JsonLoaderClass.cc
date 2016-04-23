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
ptree JsonLoader::_json_parser ; // Initializing static member

JsonLoader::JsonLoader( const string& name ):
   _name( name )
{
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
//   Property Tree Access functions
//------------------------------------------------------------------------------
void JsonLoader::LoadJsonFile( const std::string& filename )
{
   read_json( filename , _json_parser );
}

boost::property_tree::ptree&  JsonLoader::ClassInstance()
{
   return _json_parser.get_child( _name );
}
const boost::property_tree::ptree& JsonLoader::ClassInstance() const
{
   return _json_parser.get_child( _name );
}
boost::property_tree::ptree& JsonLoader::JsonParser()
{
   return _json_parser;
}

//------------------------------------------------------------------------------
//   Static member settings helper functions
//------------------------------------------------------------------------------
string JsonLoader::GetStaticString( const string& label )
{
   return JsonParser().get<string>( label );
}
double JsonLoader::GetStaticDouble( const string& label )
{
   return JsonParser().get<double>( label );
}

vector<string> JsonLoader::GetStaticStringList( const string& label )
{
   vector<string> ans;
   BOOST_FOREACH( const ptree::value_type &v , JsonParser().get_child(label) ){
      if( !v.first.empty() ){
         cerr << "Warning! Skipping over illegal format at branch: (" << label
         << ")  with index value: (" << v.first.data() << ")" << endl;
         continue;
      }
      // cout << v.second.data() << endl;
      ans.push_back( v.second.data() );
   }
   return ans;
}

//------------------------------------------------------------------------------
//   Class Instance member helper functions
//------------------------------------------------------------------------------
string JsonLoader::GetString( const string& label ) const
{
   return ClassInstance().get<string>( label );
}

double JsonLoader::GetDouble( const string& label ) const
{
   return ClassInstance().get<double>( label );
}

vector<string> JsonLoader::GetStringList( const string& label ) const
{
   vector<string> ans;
   BOOST_FOREACH( const ptree::value_type &v , ClassInstance().get_child(label) ){
      if( !v.first.empty() ){
         cerr << "Warning! Skipping over illegal format at branch: (" << label
         << ")  with index value: (" << v.first.data() << ")" << endl;
         continue;
      }
      // cout << v.second.data() << endl;
      ans.push_back( v.second.data() );
   }
   return ans;
}

vector<double> JsonLoader::GetDoubleList( const string& label ) const
{
   vector<double> ans;
   BOOST_FOREACH( const ptree::value_type &v , ClassInstance().get_child(label) ){
      if( !v.first.empty() ){
         cerr << "Warning! Skipping over illegal format at branch: (" << label
         << ")  with index value: (" << v.first.data() << ")" << endl;
         continue;
      }
      // cout << v.second.data() << endl;
      ans.push_back( stod(v.second.data()) );
   }
   return ans;
}

vector<uint64_t> JsonLoader::GetUIntList( const string& label ) const
{
   vector<uint64_t> ans;
   BOOST_FOREACH( const ptree::value_type& v , ClassInstance().get_child(label) ){
      if( !v.first.empty() ){
         cerr << "Warning! Skipping over illegal format at branch: (" << label
         << ")  with index value: (" << v.first.data() << ")" << endl;
         continue;
      }
      ans.push_back( stoul(v.second.data()) );
   }
   return ans;
}


Parameter JsonLoader::GetParameter( const string& label ) const
{
   vector<double> input = GetDoubleList( label );
   input.resize( 3 , 0 );
   if( input[0] == 0 ){
      input[0] = 1;
   }
   if( input[1] != 0 && input[2] == 0 ){
      input[2] = input[1];
   }
   return Parameter( input[0], input[1], input[2] );
}
