/*******************************************************************************
 *
 *  Filename    : KinematicCompare.cc
 *  Description : Simple comparison of kinematic distribution of various
 *  Author      : Yi-Mu "Enoch" Chen [ ensc@hep1.phys.ntu.edu.tw ]
 *
*******************************************************************************/
#include "TstarAnalysis/Utils/interface/SampleMgr.hh"
#include "TstarAnalysis/CompareDataMC/interface/KinematicCompareHist.hh"
#include "TCanvas.h"
#include "TLegend.h"
#include "THStack.h"
#include "TColor.h"

#include <boost/range/adaptor/reversed.hpp>
#include <iostream>
using namespace std;

int main(int argc, char* argv[])
{
   SampleMgr  data("data/MuonSignal_SignalMC.json", "Data_1" );
   SampleMgr  data_2("data/MuonSignal_SignalMC.json", "Data_2" );
   KinematicCompareHist* data_mgr = new KinematicCompareHist( data , -1 );
   KinematicCompareHist* data_mgr_2 = new KinematicCompareHist( data_2 , -1 ); // Filling without weight

   // Defining out channels see data/MuonSignal.json for sampleSettings
   cout<< "Defining Background Samples..." << endl;
   SampleMgr  ttjets         ( "data/MuonSignal.json", "TTJets" );
   SampleMgr  singlet_s      ( "data/MuonSignal.json", "SingleT_S" );
   SampleMgr  singlet_t      ( "data/MuonSignal.json", "SingleT_T" );
   SampleMgr  singlet_tw     ( "data/MuonSignal.json", "SingleT_TW" );
   SampleMgr  singletbar_tw  ( "data/MuonSignal.json", "SingleTbar_TW" );
   SampleMgr  wjets_100_200  ( "data/MuonSignal.json", "WJets_100_200" );
   SampleMgr  wjets_200_400  ( "data/MuonSignal.json", "WJets_200_400" );
   SampleMgr  wjets_400_600  ( "data/MuonSignal.json", "WJets_400_600" );
   SampleMgr  wjets_600_800  ( "data/MuonSignal.json", "WJets_600_800" );
   SampleMgr  wjets_800_1200 ( "data/MuonSignal.json", "WJets_800_1200" );
   SampleMgr  wjets_1200_2500( "data/MuonSignal.json", "WJets_1200_2500" );
   SampleMgr  wjets_2500_inf ( "data/MuonSignal.json", "WJets_2500_Inf" );
   SampleMgr  zjets_100_200  ( "data/MuonSignal.json", "ZJets_100_200" );
   SampleMgr  zjets_200_400  ( "data/MuonSignal.json", "ZJets_200_400" );
   SampleMgr  zjets_400_600  ( "data/MuonSignal.json", "ZJets_400_600" );
   SampleMgr  zjets_600_800  ( "data/MuonSignal.json", "ZJets_600_Inf" );
   SampleMgr  diboson_ww     ( "data/MuonSignal.json", "WW" );
   SampleMgr  diboson_wz     ( "data/MuonSignal.json", "WZ" );
   SampleMgr  diboson_zz     ( "data/MuonSignal.json", "ZZ" );
   SampleMgr  ttw_lepton     ( "data/MuonSignal.json", "TTW_Lepton");
   SampleMgr  ttw_quark      ( "data/MuonSignal.json", "TTW_Quark");
   SampleMgr  ttz_lepton     ( "data/MuonSignal.json", "TTZ_Lepton");
   SampleMgr  ttz_quark      ( "data/MuonSignal.json", "TTZ_Quark");


   // Filling Histograms and defining colors
   cout << "Filling histgrams" << endl;
   const double total_lumi = 2257;
   vector<KinematicCompareHist*>  back_ground;

   const Color_t ttcolor = TColor::GetColor( "#FFCC88" );
   const size_t  tt_index = back_ground.size();
   back_ground.push_back( new KinematicCompareHist( ttjets , total_lumi) );
   back_ground.back()->SetColor( ttcolor );

   const Color_t singletop = TColor::GetColor("#996600");
   const size_t  singletop_index = back_ground.size();
   back_ground.push_back( new KinematicCompareHist( singlet_s , total_lumi ) );
   back_ground.back()->SetColor( singletop );
   back_ground.push_back( new KinematicCompareHist( singlet_t , total_lumi ) );
   back_ground.back()->SetColor( singletop );
   back_ground.push_back( new KinematicCompareHist( singlet_tw , total_lumi ) );
   back_ground.back()->SetColor( singletop );
   back_ground.push_back( new KinematicCompareHist( singletbar_tw , total_lumi ) );
   back_ground.back()->SetColor( singletop );

   const Color_t tt_boson = TColor::GetColor("#993333");
   const size_t  tt_boson_index = back_ground.size();
   back_ground.push_back( new KinematicCompareHist( ttw_quark , total_lumi ));
   back_ground.back()->SetColor( tt_boson );
   back_ground.push_back( new KinematicCompareHist( ttw_lepton , total_lumi ));
   back_ground.back()->SetColor( tt_boson );
   back_ground.push_back( new KinematicCompareHist( ttz_quark , total_lumi ));
   back_ground.back()->SetColor( tt_boson );
   back_ground.push_back( new KinematicCompareHist( ttz_lepton , total_lumi ));
   back_ground.back()->SetColor( tt_boson );

   const Color_t singleboson = TColor::GetColor("#33EEEE");
   const size_t  singleboson_index = back_ground.size();
   back_ground.push_back( new KinematicCompareHist( wjets_100_200, total_lumi ));
   back_ground.back()->SetColor( singleboson );
   back_ground.push_back( new KinematicCompareHist( wjets_200_400, total_lumi ));
   back_ground.back()->SetColor( singleboson );
   back_ground.push_back( new KinematicCompareHist( wjets_400_600, total_lumi ));
   back_ground.back()->SetColor( singleboson );
   back_ground.push_back( new KinematicCompareHist( wjets_600_800, total_lumi ));
   back_ground.back()->SetColor( singleboson );
   back_ground.push_back( new KinematicCompareHist( wjets_800_1200, total_lumi ));
   back_ground.back()->SetColor( singleboson );
   back_ground.push_back( new KinematicCompareHist( wjets_1200_2500, total_lumi ));
   back_ground.back()->SetColor( singleboson );
   back_ground.push_back( new KinematicCompareHist( wjets_2500_inf, total_lumi ));
   back_ground.back()->SetColor( singleboson );

   const Color_t di_boson = TColor::GetColor("#0022AA");
   const size_t  di_boson_index = back_ground.size();
   back_ground.push_back( new KinematicCompareHist( diboson_ww , total_lumi ) );
   back_ground.back()->SetColor( di_boson );
   back_ground.push_back( new KinematicCompareHist( diboson_wz , total_lumi ) );
   back_ground.back()->SetColor( di_boson );
   back_ground.push_back( new KinematicCompareHist( diboson_zz , total_lumi ) );
   back_ground.back()->SetColor( di_boson );




   // Making combined stack plots
   for( const auto& hist_name : back_ground.front()->AvailableHistList() ){
      THStack* stack      = new THStack("stack","");
      TH1D*    error_hist = new TH1D("error","",
         back_ground.front()->GetHist(hist_name)->GetXaxis()->GetNbins(),
         back_ground.front()->GetHist(hist_name)->GetXaxis()->GetXmin(),
         back_ground.front()->GetHist(hist_name)->GetXaxis()->GetXmax() );
      TH1D*    rel_error = (TH1D*)error_hist->Clone("rel_error");
      TH1D*    data_hist = (TH1D*)(data_mgr->GetHist(hist_name)->Clone());
               data_hist->Add( data_mgr_2->GetHist(hist_name) );
      TH1D*    data_rel_hist = (TH1D*)data_mgr->GetHist(hist_name)->Clone("data_rel");
      Parameter error(0,0,0);

      // Stacking stuff
      for( auto& sample : boost::adaptors::reverse(back_ground) ){
         stack->Add( sample->GetHist( hist_name ), "HIST" );
         error_hist->Add( sample->GetHist(hist_name) );
         error += sample->Sample()->ExpectedYield( total_lumi );
      }

      // setting Errors
      data_rel_hist->Add( error_hist , -1 );
      data_rel_hist->Divide( error_hist );
      double avgError = error.RelAvgError();
      for( int i = 0 ; i < error_hist->GetSize() ; ++i ){
         double binContent = error_hist->GetBinContent(i);
         double binError   = error_hist->GetBinError(i);
         double dataBinError = data_hist->GetBinError(i);
         binError += binContent*avgError;
         error_hist->SetBinError(i,binError);

         rel_error->SetBinContent(i,0.0);
         if( binContent != 0 ){
            rel_error->SetBinError(i, binError/binContent);
            data_rel_hist->SetBinError(i,dataBinError/binContent);
         } else {
            rel_error->SetBinError(i,0.0);
            data_rel_hist->SetBinError(i,0.0);
         }
      }

      // Legend settings
      TLegend* l = new TLegend(0.65,0.75,0.95,0.95);
      l->AddEntry( data_hist , "Data", "lp" );
      l->AddEntry( back_ground[tt_index]->GetHist(hist_name)          , "t#bar{t} + Jets"  , "f" );
      l->AddEntry( back_ground[singletop_index]->GetHist(hist_name)   , "Single t/#bar{t}" , "f" );
      l->AddEntry( back_ground[singleboson_index]->GetHist(hist_name) , "Single Boson"     , "f" );
      l->AddEntry( back_ground[di_boson_index]->GetHist(hist_name)    , "Di-Boson"         , "f" );
      l->AddEntry( back_ground[tt_boson_index]->GetHist(hist_name)    , "t#bar{t} + Boson" , "f");
      l->AddEntry( error_hist , "background error" , "fl" );


      // The Plotting commands
      TCanvas* c = new TCanvas("c","c",1600,1000);

      // Drawing the top canvas
      TPad* pad1 = new TPad( "pad1" , "pad1" , 0 , 0.3025 , 1., 1.0 );
      pad1->SetBottomMargin(0.025);
      pad1->Draw();
      pad1->cd();
      stack->Draw();
      data_hist->Draw("same axis");
      data_hist->Draw("same LPE1");
      error_hist->Draw("same E2");
      l->Draw("same");
      c->cd();

      // Drawing bottom canvas
      TPad*   pad2 = new TPad( "pad2" , "pad2" , 0 , 0.05, 1, 0.2975 );
      pad2->SetTopMargin(0.025);
      pad2->Draw();
      pad2->cd();
      rel_error->Draw("axis");
      rel_error->Draw("same E2");
      data_rel_hist->Draw("same LPE1");
      c->cd();


      // Plot Settings
      error_hist->SetFillColor(kBlack);
      error_hist->SetLineColor(kBlack);
      error_hist->SetFillStyle(3004);
      rel_error->SetFillColor(kBlack);
      rel_error->SetLineColor(kBlack);
      rel_error->SetFillStyle(3004);
      data_hist->SetMarkerStyle(21);
      data_hist->SetLineColor(kBlack);
      data_hist->SetStats(0);
      data_rel_hist->SetMarkerStyle(21);
      data_rel_hist->SetLineColor(kBlack);
      data_rel_hist->SetStats(0);


      // Save and clean up
      c->SaveAs( (hist_name + ".png").c_str() );
      delete pad1;
      delete pad2;
      delete c;
      delete error_hist;
      delete l;
      delete stack;
   }

   // Cleaning up
   for( auto* histmgr : back_ground ){
      delete histmgr;
   }
   return 0;
}
