/*******************************************************************************
 *
 *  Filename    : MakeRooFitObj.cc
 *  Description : Makes RooFit objects to pass to HiggsCombine package
 *  Author      : Yi-Mu "Enoch" Chen [ ensc@hep1.phys.ntu.edu.tw ]
 *
*******************************************************************************/
#include "TstarAnalysis/CompareDataMC/interface/SampleRooFitMgr.hh"
#include "TstarAnalysis/CompareDataMC/interface/FileNames.hh"
#include "TstarAnalysis/CompareDataMC/interface/PlotConfig.hh"

#include "RooFit.h"
#include "RooRealVar.h"
#include "RooDataSet.h"
#include "RooWorkspace.h"


#include <iostream>
#include <vector>

using namespace std;

//------------------------------------------------------------------------------
//   External functions see src/RooFit_MCTemplate.cc
//------------------------------------------------------------------------------
extern void MakeBGFromMC( SampleRooFitMgr& );
extern void MakeSignalPdf( SampleRooFitMgr& );
extern void MakeTemplatePlots( SampleRooFitMgr& , SampleRooFitMgr&, SampleRooFitMgr& );

//------------------------------------------------------------------------------
//   Begin control flow
//------------------------------------------------------------------------------
int main(int argc, char* argv[])
{
   if( argc != 2 ){
      cerr << "Error! Expected exactly one arguement!" << endl;
      return 1;
   }
   const string channel = argv[1];
   SetChannelType( argv[1] );
   SampleRooFitMgr::LoadJsonFile( GetJsonFile() );
   SampleRooFitMgr::x().setRange("FitRange" , 300 , 3000 );

   //------------------------------------------------------------------------------
   //   Delcaring sample
   //------------------------------------------------------------------------------
   SampleRooFitMgr data( "data" , { "Data_1", "Data_2" } );

   SampleRooFitMgr bg( "bg" , {
      "TTJets",
      "SingleT_S", "SingleT_T", "SingleT_TW", "SingleTbar_TW",
      "WJets_100_200", "WJets_200_400" ,"WJets_400_600" ,"WJets_600_800" ,"WJets_800_1200", "WJets_1200_2500", "WJets_2500_Inf",
      "ZJets_100_200", "ZJets_200_400", "ZJets_400_600", "ZJets_600_Inf",
      "WW" ,"WZ" , "ZZ",
      "TTW_Lepton",  "TTW_Quark" ,   "TTZ_Lepton",   "TTZ_Quark" } );

   vector<SampleRooFitMgr*>  signal_list;
   signal_list.push_back( new SampleRooFitMgr( "tstar_M700" ));
   signal_list.push_back( new SampleRooFitMgr( "tstar_M800" ));
   signal_list.push_back( new SampleRooFitMgr( "tstar_M900" ));
   signal_list.push_back( new SampleRooFitMgr( "tstar_M1000" ));
   signal_list.push_back( new SampleRooFitMgr( "tstar_M1100" ));
   signal_list.push_back( new SampleRooFitMgr( "tstar_M1200" ));
   // signal_list.push_back( new SampleRooFitMgr( "tstar_M1300" ));
   signal_list.push_back( new SampleRooFitMgr( "tstar_M1400" ));
   signal_list.push_back( new SampleRooFitMgr( "tstar_M1500" ));
   signal_list.push_back( new SampleRooFitMgr( "tstar_M1600" ));


   //------------------------------------------------------------------------------
   //   Fitting background PDF
   //------------------------------------------------------------------------------
   MakeBGFromMC( bg );

   //------------------------------------------------------------------------------
   //   Fitting signal to PDF
   //------------------------------------------------------------------------------
   for( auto& signal : signal_list ){
      MakeSignalPdf( *signal );
   }

   //------------------------------------------------------------------------------
   //   Making confirmation plots
   //------------------------------------------------------------------------------
   MakeTemplatePlots( data, bg, *(signal_list.front()) );

   //------------------------------------------------------------------------------
   //   Saving all relavent obejcts by RooWorkSpace
   //------------------------------------------------------------------------------
   const string roofit_file = GetRooFitObj_Template_File();
   const string ws_name = "wspace";
   RooWorkspace ws( ws_name.c_str() , ws_name.c_str() );
   RooDataSet* data_selc = data.MakeReduceDataSet( "selc" , RooFit::CutRange("FitRange") );
   ws.import( *data_selc );
   ws.import( *(bg.GetPdfFromAlias("fit")) );
   for( auto& signal : signal_list ){
      ws.import( *(signal->GetPdfFromAlias("fit")) );
   }
   ws.writeToFile( roofit_file.c_str() );

   //------------------------------------------------------------------------------
   //   Making Higgs Combine datacards
   //------------------------------------------------------------------------------
   for( auto& signal: signal_list ){
      const string cardfile_name = GetTemplate_CardFile( signal->Name() );
      RooDataSet* data_obs       = data.GetReduceDataSet("selc");
      RooDataSet* signal_dataset = signal->GetReduceDataSet("selc");
      RooAbsPdf*  signal_pdf     = signal->GetPdfFromAlias("fit");
      RooAbsPdf*  bg_pdf         = bg.GetPdfFromAlias("fit");

      FILE* cardfile = fopen( cardfile_name.c_str() , "w" );

      // Priting header
      fprintf( cardfile , "imax 1\n" );
      fprintf( cardfile , "jmax *\n" );
      fprintf( cardfile , "kmax *\n" );
      fprintf( cardfile , "----------------------------------------\n" );

      // Printing objects
      fprintf( cardfile , "shapes %10s %15s %30s %s:%s\n" ,
            "bg",
            GetChannel().c_str() ,
            roofit_file.c_str() ,
            ws_name.c_str(),
            bg_pdf->GetName() );
      fprintf( cardfile , "shapes %10s %15s %30s %s:%s\n" ,
            "sig",
            GetChannel().c_str() ,
            roofit_file.c_str() ,
            ws_name.c_str(),
            signal_pdf->GetName() );
      fprintf( cardfile , "shapes %10s %15s %30s %s:%s\n" ,
            "data_obs",
            GetChannel().c_str() ,
            roofit_file.c_str() ,
            ws_name.c_str(),
            data_obs->GetName() );
      fprintf( cardfile , "----------------------------------------\n" );

      // Printing data correspondence
      fprintf( cardfile , "%12s %s\n" , "bin" , GetChannel().c_str() );
      fprintf( cardfile , "%12s %lg\n" , "observation" , data_obs->sumEntries() );
      fprintf( cardfile , "----------------------------------------\n" );

      // Printing expected
      fprintf( cardfile , "%12s %15s %15s\n" , "bin"     , GetChannel().c_str(), GetChannel().c_str() );
      fprintf( cardfile , "%12s %15s %15s\n" , "process" , "sig", "bg" );
      fprintf( cardfile , "%12s %15s %15s\n" , "process" , "-1" , "1" );
      fprintf( cardfile , "%12s %15lf %15lf\n" , "rate",
            signal_dataset->sumEntries() ,
            data_obs->sumEntries()
         );

      // Listing Nuisance parameters
      fprintf( cardfile , "----------------------------------------\n" );
      const string sig_unc = "1.05";
      fprintf( cardfile, "%8s lnN %15s %15s\n" , "Lumi"   , "1.05"          , "1.05" );
      fprintf( cardfile, "%8s lnN %15s %15s\n" , "sig_unc", sig_unc.c_str() , "--"   );
      fprintf( cardfile, "%8s lnN %15s %15s\n" , "bg_unc" , "--"            , "1.05" );
      fclose( cardfile );
   }

   return 0;
}
