/*******************************************************************************
 *
 *  Filename    : MakeRooFit.cc
 *  Description : Making RooFit objects to be passed over to Higgs Combine
 *  Author      : Yi-Mu "Enoch" Chen [ ensc@hep1.phys.ntu.edu.tw ]
 *
*******************************************************************************/
#include "TstarAnalysis/CompareDataMC/interface/SampleRooFitMgr.hh"
#include "TstarAnalysis/CompareDataMC/interface/FileNames.hh"
#include <string>
#include <vector>
#include <iostream>
#include <boost/exception/diagnostic_information.hpp>
#include <boost/program_options.hpp>

using namespace std;
namespace opt = boost::program_options;

//------------------------------------------------------------------------------
//   Static data members
//------------------------------------------------------------------------------
static SampleRooFitMgr* data;
static SampleRooFitMgr* mc;
static vector<SampleRooFitMgr*> signal_list;

//------------------------------------------------------------------------------
//   Helper functions
//------------------------------------------------------------------------------
void InitDataAndSignal();
void InitMC();
extern void MakeSimFit(SampleRooFitMgr*, SampleRooFitMgr*);
extern void MakeSideBand(SampleRooFitMgr*,SampleRooFitMgr*);
extern void MakeTemplate(SampleRooFitMgr*,SampleRooFitMgr*,vector<SampleRooFitMgr*> );
//------------------------------------------------------------------------------
//   Main control flow
//------------------------------------------------------------------------------

int main(int argc, char* argv[]) {

   opt::options_description desc("MakeRooFit options");
   opt::variables_map       vm;

   desc.add_options()
      ("help","produce help message and exit")
      ("channel", opt::value<string>(), "Channel to run" )
      ("method" , opt::value<string>(), "Which fitting method to run" )
   ;

   try{
      opt::store(opt::parse_command_line(argc, argv, desc), vm);
      opt::notify(vm);
   } catch( boost::exception& e ){
      cerr << "Error parsing command!" << endl;
      cerr << boost::diagnostic_information(e);
      return 1;
   }

   if(vm.count("help")) {
      cerr << desc << "\n";
      return 0;
   }

   if( vm.count("channel") ){
      SetChannelType( vm["channel"].as<string>() );
      SampleRooFitMgr::LoadJsonFile( GetJsonFile() );
   } else {
      cerr << "Settings for option [channel] not found!" << endl;
      return 1;
   }

   // InitDataAndSignal();
   if( !vm.count("method") ){
      cerr << "options [method] not found!" << endl;
      return 1;
   }

   if( vm["method"].as<string>() == "SimFit" ){
      cout  << "Running SimFit Method!" << endl;
      InitDataAndSignal();
      for( auto& sample : signal_list ){
         MakeSimFit(data,sample);
      }
   } else if( vm["method"].as<string>() == "Template" ){
      cout << "Running MC template method!" << endl;
      InitDataAndSignal();
      InitMC();
      MakeTemplate(data,mc,signal_list);
   } else if( vm["method"].as<string>() == "SideBand" ){
      cout << "Running side band method!" << endl;
      InitDataAndSignal();
      for( auto& sample : signal_list ){
         MakeSideBand(data,sample);
      }
   } else {
      cerr << "Unrecognized method!" << endl;
      return 1;
   }

   return 0;
}

//------------------------------------------------------------------------------
//   Helper function implementations
//------------------------------------------------------------------------------
void InitDataAndSignal()
{
   data = new SampleRooFitMgr("Data");
   data->MakeReduceDataSet("FitRange",RooFit::CutRange("FitRange"));

   signal_list.push_back( new SampleRooFitMgr("tstar_M700"));
   signal_list.push_back( new SampleRooFitMgr("tstar_M800"));
   signal_list.push_back( new SampleRooFitMgr("tstar_M900"));
   signal_list.push_back( new SampleRooFitMgr("tstar_M1000"));
   signal_list.push_back( new SampleRooFitMgr("tstar_M1100"));
   signal_list.push_back( new SampleRooFitMgr("tstar_M1200"));
   // signal_list.push_back( new SampleRooFitMgr("tstar_M1300"));
   signal_list.push_back( new SampleRooFitMgr("tstar_M1400"));
   signal_list.push_back( new SampleRooFitMgr("tstar_M1500"));
   signal_list.push_back( new SampleRooFitMgr("tstar_M1600"));
}

void InitMC()
{
   mc = new SampleRooFitMgr("Background");
}
