/*******************************************************************************
 *
 *  Filename    : KinematicCompare.cc
 *  Description : Simple comparison of kinematic distribution of various
 *  Author      : Yi-Mu "Enoch" Chen [ ensc@hep1.phys.ntu.edu.tw ]
 *
*******************************************************************************/
#include "TstarAnalysis/CompareDataMC/interface/SampleHistMgr.hh"
#include "TstarAnalysis/CompareDataMC/interface/FileNames.hh"
#include "TCanvas.h"
#include "TLegend.h"
#include "THStack.h"
#include "TColor.h"
#include "TLatex.h"
#include "TLine.h"

#include <boost/range/adaptor/reversed.hpp>
#include <iostream>
using namespace std;

//------------------------------------------------------------------------------
//   Main control flow
//------------------------------------------------------------------------------
int main(int argc, char* argv[])
{
   if( argc != 2 ){
      cerr << "Error! Expected exactly one arguement!" << endl;
      return 1;
   }
   SetChannelType( argv[1] );
   SampleMgr::LoadJsonFile( GetJsonFile() );
   const double total_lumi = SampleMgr::TotalLuminosity();

   // Defining data settings
   vector<SampleHistMgr*> all_data;
   all_data.push_back( new SampleHistMgr( "Data_1" , -1 ) ) ;
   all_data.push_back( new SampleHistMgr( "Data_2" , -1 ) ) ;

   // Defining out channels see data/MuonSignal.json for sample settings
   vector<SampleHistMgr*>  background;

   const size_t  tt_index = background.size();
   background.push_back( new SampleHistMgr( "TTJets"          , total_lumi ) );

   const size_t  singletop_index = background.size();
   background.push_back( new SampleHistMgr( "SingleT_S"       , total_lumi ) );
   background.push_back( new SampleHistMgr( "SingleT_T"       , total_lumi ) );
   background.push_back( new SampleHistMgr( "SingleT_TW"      , total_lumi ) );
   background.push_back( new SampleHistMgr( "SingleTbar_TW"   , total_lumi ) );

   const size_t  tt_boson_index = background.size();
   background.push_back( new SampleHistMgr( "TTW_Lepton"      , total_lumi ) );
   background.push_back( new SampleHistMgr( "TTW_Quark"       , total_lumi ) );
   background.push_back( new SampleHistMgr( "TTZ_Lepton"      , total_lumi ) );
   background.push_back( new SampleHistMgr( "TTZ_Quark"       , total_lumi ) );

   const size_t  singleboson_index = background.size();
   background.push_back( new SampleHistMgr( "WJets_100_200"   , total_lumi ) );
   background.push_back( new SampleHistMgr( "WJets_200_400"   , total_lumi ) );
   background.push_back( new SampleHistMgr( "WJets_400_600"   , total_lumi ) );
   background.push_back( new SampleHistMgr( "WJets_600_800"   , total_lumi ) );
   background.push_back( new SampleHistMgr( "WJets_800_1200"  , total_lumi ) );
   background.push_back( new SampleHistMgr( "WJets_1200_2500" , total_lumi ) );
   background.push_back( new SampleHistMgr( "WJets_2500_Inf"  , total_lumi ) );
   background.push_back( new SampleHistMgr( "ZJets_100_200"   , total_lumi ) );
   background.push_back( new SampleHistMgr( "ZJets_200_400"   , total_lumi ) );
   background.push_back( new SampleHistMgr( "ZJets_400_600"   , total_lumi ) );
   background.push_back( new SampleHistMgr( "ZJets_600_Inf"   , total_lumi ) );

   const size_t  di_boson_index = background.size();
   background.push_back( new SampleHistMgr( "WW"              , total_lumi ) );
   background.push_back( new SampleHistMgr( "WZ"              , total_lumi ) );
   background.push_back( new SampleHistMgr( "ZZ"              , total_lumi ) );

   // Setting plotting colors
   for( unsigned i = tt_index ; i < singletop_index ; ++i ){
      background[i]->SetColor( TColor::GetColor( "#FFCC88" ) );
   }
   for( unsigned i = singletop_index ; i < tt_boson_index ; ++i ){
      background[i]->SetColor( TColor::GetColor("#996600") );
   }
   for( unsigned i = tt_boson_index ; i < singleboson_index ; ++i ){
      background[i]->SetColor( TColor::GetColor("#FF3333") );
   }
   for( unsigned i = singleboson_index ; i < di_boson_index ; ++i ){
      background[i]->SetColor( TColor::GetColor("#33EEEE"));
   }
   for( unsigned i = di_boson_index ; i < background.size() ; ++i ){
      background[i]->SetColor( TColor::GetColor("#0066EE") );
   }

   // Declaring sample sample
   SampleHistMgr* signal_mgr = new SampleHistMgr( "tstar_M700", total_lumi );

   // Making combined stack plots
   for( const auto& hist_name : background.front()->AvailableHistList() ){

      // Caching stuff
      const unsigned bins = background.front()->GetHist(hist_name)->GetXaxis()->GetNbins();
      const double   xmin = background.front()->GetHist(hist_name)->GetXaxis()->GetXmin();
      const double   xmax = background.front()->GetHist(hist_name)->GetXaxis()->GetXmax();

      THStack* stack      = new THStack("stack","");
      TH1D*    error_hist = new TH1D("error","",bins,xmin,xmax);
      TH1D*    data_hist =  new TH1D("data" ,"",bins,xmin,xmax);
      Parameter error(0,0,0);

      // Making stack and total error hist
      for( auto& sample : boost::adaptors::reverse(background) ){
         stack->Add( sample->GetHist( hist_name ), "HIST" );
         error_hist->Add( sample->GetHist(hist_name) );
         error += sample->ExpectedYield(total_lumi);
      }

      // Making data hist
      for( auto& dat : all_data ){
         data_hist->Add( dat->GetHist(hist_name) );
      }

      // Making relative error plot
      TH1D*  data_rel_hist = (TH1D*)data_hist->Clone("data_rel");
      TH1D*  rel_error     = (TH1D*)error_hist->Clone("rel_error");
      data_rel_hist->Divide( error_hist );
      double avgError = error.RelAvgError();
      for( int i = 0 ; i < error_hist->GetSize() ; ++i ){
         double binContent   = error_hist->GetBinContent(i);
         double binError     = error_hist->GetBinError(i);
         double dataBinError = data_hist->GetBinError(i);
         binError += binContent*avgError;
         error_hist->SetBinError(i,binError);

         rel_error->SetBinContent(i,1.0);
         if( binContent != 0 ){
            rel_error->SetBinError(i, binError/binContent);
            data_rel_hist->SetBinError(i,dataBinError/binContent);
         } else {
            rel_error->SetBinError(i,0.0);
            data_rel_hist->SetBinError(i,0.0);
         }
      }
      // Making Signal plots
      TH1D*    signal_hist   = signal_mgr->GetHist(hist_name);

      // Legend settings
      TLegend* l = new TLegend(0.62,0.3,0.90,0.90);
      char data_entry[128];
      sprintf( data_entry , "Data (%lg fb^{-1})" , total_lumi/1000. );
      l->AddEntry( data_hist , data_entry , "lp" );
      l->AddEntry( background[tt_index]->GetHist(hist_name)          , "t#bar{t} + Jets"  , "f" );
      l->AddEntry( background[singletop_index]->GetHist(hist_name)   , "Single top"       , "f" );
      l->AddEntry( background[tt_boson_index]->GetHist(hist_name)    , "t#bar{t} + Boson" , "f");
      l->AddEntry( background[singleboson_index]->GetHist(hist_name) , "Single Boson"     , "f" );
      l->AddEntry( background[di_boson_index]->GetHist(hist_name)    , "Di-Boson"         , "f" );
      l->AddEntry( error_hist , "Background error" , "fl" );
      l->AddEntry( signal_hist, "t^{*}#bar{t}^{*} {}_{M_{t^{*}}=700GeV} (10 pb)" , "fl");


      // The Plotting commands
      TCanvas* c = new TCanvas("c","c",650,500);

      // Drawing the top canvas
      TPad* pad1 = new TPad( "pad1" , "pad1" , 0 , 0.30 , 1., 1.0 );
      pad1->SetBottomMargin(0.050);
      pad1->Draw();
      pad1->cd();
      stack->Draw("HIST");
      data_hist->Draw("same LPE1");
      error_hist->Draw("same E2");
      signal_hist->Draw("same HIST");
      l->Draw("same");
      c->cd();

      // Drawing bottom canvas
      TPad*   pad2 = new TPad( "pad2" , "pad2" , 0 , 0.10, 1, 0.30 );
      TLine*  line = new TLine( error_hist->GetXaxis()->GetXmin() , 1 , error_hist->GetXaxis()->GetXmax() , 1 );
      pad2->SetTopMargin(0.025);
      pad2->SetBottomMargin(0.010);
      pad2->Draw();
      pad2->cd();
      data_rel_hist->Draw("axis");
      data_rel_hist->Draw("same LPE1");
      rel_error->Draw("same E2");
      line->Draw("same");
      c->cd();


      // Plot Settings
      error_hist->SetFillColor(kBlack);
      error_hist->SetLineColor(kBlack);
      error_hist->SetFillStyle(3004);
      rel_error->SetFillColor(kBlack);
      rel_error->SetLineColor(kBlack);
      rel_error->SetFillStyle(3004);
      rel_error->SetStats(0);
      data_hist->SetMarkerStyle(21);
      data_hist->SetLineColor(kBlack);
      data_hist->SetStats(0);
      data_rel_hist->SetMarkerStyle(21);
      data_rel_hist->SetLineColor(kBlack);
      data_rel_hist->SetStats(0);
      signal_hist->SetLineColor(kRed);
      signal_hist->SetFillColor(kRed);
      signal_hist->SetFillStyle(3005);

      // Font and title settings
      const size_t font_size = 14;
      stack->GetXaxis()->SetLabelSize(0);
      stack->GetYaxis()->SetLabelFont(43);
      stack->GetYaxis()->SetLabelSize( font_size );
      stack->GetYaxis()->SetTitleFont(43);
      stack->GetYaxis()->SetTitleSize( font_size );
      stack->GetYaxis()->SetTitle( signal_hist->GetYaxis()->GetTitle() );
      stack->GetYaxis()->SetTitleOffset(1.2);
      data_rel_hist->GetXaxis()->SetTitle( signal_hist->GetXaxis()->GetTitle() );
      data_rel_hist->GetXaxis()->SetLabelFont(43);
      data_rel_hist->GetXaxis()->SetLabelSize( font_size );
      data_rel_hist->GetYaxis()->SetLabelFont(43);
      data_rel_hist->GetXaxis()->SetTitleFont(43);
      data_rel_hist->GetXaxis()->SetTitleSize( font_size );
      data_rel_hist->GetXaxis()->SetTitleOffset(5.5);
      data_rel_hist->GetYaxis()->SetLabelSize( font_size );
      data_rel_hist->GetYaxis()->SetTitle( "#frac{Data}{MC}");
      data_rel_hist->GetYaxis()->SetTitleFont(43);
      data_rel_hist->GetYaxis()->SetTitleSize( font_size );
      data_rel_hist->GetYaxis()->SetTitleOffset(1.2);
      data_rel_hist->SetMaximum(2.0);
      data_rel_hist->SetMinimum(0.0);
      l->SetTextSizePixels( font_size );

      // Additional captions
      TLatex tl;
      tl.SetTextFont(43);
      tl.SetTextSize( font_size + 4 );

      tl.SetTextAlign(11);
      tl.DrawLatex( 0.1, 0.95 , "CMS at #sqrt{s} = 13TeV");

      tl.SetTextAlign(31);
      string channel_msg = "";
      if( GetChannel().find("Muon") != string::npos ){
         channel_msg="(#mu channel)";
      } else if( GetChannel().find("Electron") != string::npos ){
         channel_msg="(e channel)";
      }
      tl.DrawLatex( 0.9, 0.95 , channel_msg.c_str() );

      c->SaveAs( GetKinematicPlotFile(hist_name).c_str() );
      delete pad1;
      delete pad2;
      delete c;
      delete l;
      delete data_hist;
      delete error_hist;
      delete stack;
      delete line;
   }

   // Cleaning up
   for( auto& histmgr : background ){
      delete histmgr;
   }
   for( auto& histmgr : all_data ){
      delete histmgr;
   }
   return 0;
}
