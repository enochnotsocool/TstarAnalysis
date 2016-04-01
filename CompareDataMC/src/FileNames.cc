/*******************************************************************************
 *
 *  Filename    : FileNames.cc
 *  Description : Implementations for getting the files save locations
 *  Author      : Yi-Mu "Enoch" Chen [ ensc@hep1.phys.ntu.edu.tw ]
 *
*******************************************************************************/
#include <string>
#include <cstdlib>

using namespace std;
//------------------------------------------------------------------------------
//   Static constants
//------------------------------------------------------------------------------
static const string base_dir = getenv( "CMSSW_BASE");
static const string work_dir = base_dir + "/src/TstarAnalysis/CompareDataMC";
static string channel_name;

void SetChannelType( const string& x ) { channel_name = x; }
string GetChannel() { return channel_name; }
string  GetJsonFile() { return work_dir + "/data/" + channel_name + ".json"; }

string ResultsDir() { return work_dir + "/results/" + channel_name + "/" ; }
string GetTexSummaryFile() { return ResultsDir() + channel_name + "_summary.tex" ; }
string GetKinematicPlotFile( const string& plot_name ) { return ResultsDir() + plot_name + ".png"; }

string GetRooFitPlot_Template_File(){ return ResultsDir() + "RooFit_MCTemplate" + channel_name + ".png"; }
string GetRooFitObj_Template_File(){ return ResultsDir() + "RooFit_MCTemplate" + channel_name + ".root"; }
string GetTemplate_CardFile( const string& x ){ return ResultsDir() + "card_" + channel_name +"_"+  x + ".txt" ; }
string GetTemplate_HiggCombineStoreFile( const string& method, int mass ){
   char buffer[2048];
   sprintf( buffer , "%s/higgscombine_template_%s_%d.root" ,
      ResultsDir().c_str(),
      method.c_str(),
      mass  );
   return buffer;
}

string GetTemplate_PlotFile(){
   return ResultsDir() + "Confidnece_Level_Template.png" ;
}

string GetHiggCombineOutputFile( const string& method , int mass ){
   char buffer[2048];
   sprintf( buffer , "%s/higgsCombineTest.%s.mH%d.root",
      getenv("PWD"),
      method.c_str(),
      mass
   );
   return buffer;
}
