/*******************************************************************************
 *
 *  Filename    : RooFit_MCTemplate.cc
 *  Description : Plotting to file for template methods.cc
 *  Author      : Yi-Mu "Enoch" Chen [ ensc@hep1.phys.ntu.edu.tw ]
 *
*******************************************************************************/
#include "TstarAnalysis/CompareDataMC/interface/SampleRooFitMgr.hh"
#include "TstarAnalysis/CompareDataMC/interface/FileNames.hh"
#include "TstarAnalysis/CompareDataMC/interface/PlotConfig.hh"

#include "TstarAnalysis/Utils/interface/RooFitUtils.hh"

#include "RooFit.h"
#include "RooRealVar.h"
#include "RooDataSet.h"
#include "RooGenericPdf.h"
#include "RooKeysPdf.h"
#include "RooPlot.h"
#include "RooWorkspace.h"

#include "TAxis.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TLatex.h"
#include <string>

using namespace std;

//------------------------------------------------------------------------------
//   Static variables
//------------------------------------------------------------------------------
static RooRealVar m = RooRealVar("m","m", 500 , 0, 1000 );
static RooRealVar b = RooRealVar("b","b", 200 , 0 ,1000 );
static RooRealVar p1 = RooRealVar("p1","p1",500,0,1000);
static RooRealVar p2 = RooRealVar("p2","p2",500,0,1000);
static RooRealVar p3 = RooRealVar("p3","p2",500,0,1000);
static const string ws_name = "wspace";

//------------------------------------------------------------------------------
//   Helper variables
//------------------------------------------------------------------------------
static void MakeBGFromMC( SampleRooFitMgr* );
static void MakeSignalPdf( SampleRooFitMgr* );
static void MakeTemplatePlots( SampleRooFitMgr* , SampleRooFitMgr*, SampleRooFitMgr* );
static void SaveRooWorkSpace( SampleRooFitMgr*, SampleRooFitMgr*, vector<SampleRooFitMgr*> );
static void MakeCardFile(SampleRooFitMgr*, SampleRooFitMgr*, SampleRooFitMgr*);

//------------------------------------------------------------------------------
//   Function implemetations
//------------------------------------------------------------------------------
void MakeTemplate( SampleRooFitMgr* data, SampleRooFitMgr* bg, vector<SampleRooFitMgr*> signal_list )
{
   MakeBGFromMC(bg);
   for( auto& signal : signal_list ){
      MakeSignalPdf( signal );
   }
   MakeTemplatePlots( data, bg, signal_list.front() );

   //------------------------------------------------------------------------------
   //   Saving all relavent obejcts by RooWorkSpace
   //------------------------------------------------------------------------------
   SaveRooWorkSpace( data, bg, signal_list );

   //------------------------------------------------------------------------------
   //   Making Higgs Combine Data Cards
   //------------------------------------------------------------------------------
   for( auto& signal : signal_list ){
      MakeCardFile(data,bg,signal);
   }

   //------------------------------------------------------------------------------
   //   Making RooDataSet only files
   //------------------------------------------------------------------------------
   // RooWorkspace ws_temp("dataset","dataset");
   // ws_temp.import( *(data.OriginalDataSet()) );
   // ws_temp.import( *(bg.OriginalDataSet()) );
   // for( auto& signal: signal_list ){
   //    ws_temp.import( *(signal->OriginalDataSet() )) ;
   // }
   // ws_temp.writeToFile( "DataSetOnly.root" );
}



//------------------------------------------------------------------------------
//  Helper function implementations
//------------------------------------------------------------------------------
void MakeBGFromMC( SampleRooFitMgr* bg )
{
   const string bg_pdf_name = bg->MakePdfAlias( "fit" );
   RooGenericPdf* bg_pdf;

   bg_pdf = new RooGenericPdf(
      bg_pdf_name.c_str(), bg_pdf_name.c_str(),
      "1./(1.+TMath::Exp((x-m)/b))",
      RooArgSet(SampleRooFitMgr::x(),m,b) );

   bg_pdf->fitTo( *(bg->OriginalDataSet()) ,
      RooFit::Save() ,            // Suppressing output
      RooFit::SumW2Error(kTRUE),  // For Weighted dataset
      RooFit::Range("FitRange"),  // Fitting range
      RooFit::Minos(kTRUE)
   );

   bg->AddPdf( bg_pdf );
   m.setConstant(kTRUE);
   b.setConstant(kTRUE);
   return ;
}

void MakeSignalPdf( SampleRooFitMgr* signal )
{
   const string pdf_name = signal->MakePdfAlias( "fit" );
   RooDataSet*  sig_selc = signal->MakeReduceDataSet( "FitRange" , RooFit::CutRange("FitRange") );
   RooKeysPdf*  sig_pdf  = new RooKeysPdf(
      pdf_name.c_str(), pdf_name.c_str(),
      SampleRooFitMgr::x(), *sig_selc,
      RooKeysPdf::NoMirror, 2.
   );
   signal->AddPdf( sig_pdf );
   return;
}

void SaveRooWorkSpace( SampleRooFitMgr* data, SampleRooFitMgr* bg, vector<SampleRooFitMgr*> sig_list )
{
   const string roofit_file = GetRooObjFile();
   cout << "Saving RooFit objects to " << roofit_file << endl;
   RooWorkspace ws( ws_name.c_str() , ws_name.c_str() );
   ws.import( *(data->GetReduceDataSet("FitRange")) );
   ws.import( *(bg->GetPdfFromAlias("fit")) );
   for( auto& signal : sig_list ){
      ws.import( *(signal->GetPdfFromAlias("fit")) );
   }
   ws.writeToFile( roofit_file.c_str() );
}


void MakeTemplatePlots( SampleRooFitMgr* data, SampleRooFitMgr* bg, SampleRooFitMgr* signal )
{
   RooPlot* frame = SampleRooFitMgr::x().frame();
   RooDataSet* data_dataset   = data->OriginalDataSet();
   RooDataSet* bg_dataset     = bg->OriginalDataSet();
   RooDataSet* signal_dataset = signal->OriginalDataSet();
   RooAbsPdf*  signal_pdf     = signal->GetPdfFromAlias("fit");
   RooAbsPdf*  bg_pdf         = bg->GetPdfFromAlias("fit");

   TGraph* bg_set_plot  = PlotOn( frame , bg_dataset , RooFit::DrawOption("B"));
   TGraph* bg_pdf_plot  = PlotOn( frame , bg_pdf , RooFit::Range("FitRange"));
   TGraph* sig_set_plot = PlotOn( frame , signal_dataset , RooFit::DrawOption("B") );
   TGraph* sig_pdf_plot = PlotOn( frame , signal_pdf );
   TGraph* data_plot    = PlotOn( frame , data_dataset );
   TGraph* bg_pdf_norm_plot = PlotOn( frame , bg_pdf, RooFit::Range("FitRange") );

   TCanvas* c1 = new TCanvas("c","c" , 650, 500 );
   frame->Draw();
   frame->SetMinimum(0.0001);

   SetFrame( frame , FONT_SIZE );

   bg_set_plot->SetFillStyle(3003);
   bg_set_plot->SetFillColorAlpha( kBlue , 1.0 );
   bg_set_plot->SetLineColor( kBlue );
   bg_pdf_plot->SetLineColor( kBlue );

   sig_set_plot->SetFillStyle( 3004 );
   sig_set_plot->SetFillColorAlpha( kRed , 0.4 );
   sig_set_plot->SetLineColor( kRed );
   sig_pdf_plot->SetLineColor( kRed );
   bg_pdf_norm_plot->SetLineColor( kBlack );

   char data_entry[1024];
   char sig_entry[1024];
   sprintf( data_entry, "Data (%.3lf fb^{-1})" , SampleRooFitMgr::TotalLuminosity()/1000. );
   sprintf( sig_entry,  "t^{*} {}_{M_{t^{*}}=700GeV} (%.1lf pb)" , signal->Sample().CrossSection().CentralValue() );
   TLegend* leg = new TLegend(0.55,0.5,0.9,0.9);
   leg->AddEntry( data_plot        , data_entry , "lp" );
   leg->AddEntry( bg_set_plot      , "MC Background (Not Normalized)" , "f" );
   leg->AddEntry( bg_pdf_plot      , "Fitted (Yield from MC)" , "l" );
   leg->AddEntry( bg_pdf_norm_plot , "Fitted (Normalized to Data)" , "l");
   leg->AddEntry( sig_set_plot     , sig_entry , "f" );
   leg->AddEntry( sig_pdf_plot     , "Fitted Signal (RooKeysPdf)" , "l");
   leg->Draw();
   leg->SetTextSizePixels(FONT_SIZE);

   TLatex* tl = new TLatex();
   tl->SetNDC(kTRUE);
   tl->SetTextFont(43);
   tl->SetTextSize( FONT_SIZE + 4 );
   tl->SetTextAlign(11);
   tl->DrawLatex( 0.1, 0.93, "CMS at #sqrt{s} = 13TeV");
   tl->SetTextAlign(31);
   tl->DrawLatex( 0.9, 0.93 , GetChannelPlotLabel().c_str() );
   tl->SetTextAlign(33);
   tl->DrawLatex(0.53,0.85 , "f(m) = N #left(1 + exp#left( #frac{m-a}{b} #right)#right)^{-1}" );


   c1->SaveAs( GetRooObjPlot("master").c_str() );
   delete leg;
   delete tl;
   delete c1;
}


void MakeCardFile( SampleRooFitMgr* data, SampleRooFitMgr* bg, SampleRooFitMgr* signal )
{
   const string cardfile_name = GetCardFile( signal->Name() );
   RooDataSet* data_obs       = data->GetReduceDataSet("FitRange");
   RooAbsPdf*  bg_pdf         = bg->GetPdfFromAlias("fit");
   RooDataSet* signal_dataset = signal->GetReduceDataSet("FitRange");
   RooAbsPdf*  signal_pdf     = signal->GetPdfFromAlias("fit");

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
   GetRooObjFile().c_str() ,
   ws_name.c_str(),
   bg_pdf->GetName() );
   fprintf( cardfile , "shapes %10s %15s %30s %s:%s\n" ,
   "sig",
   GetChannel().c_str() ,
   GetRooObjFile().c_str() ,
   ws_name.c_str(),
   signal_pdf->GetName() );
   fprintf( cardfile , "shapes %10s %15s %30s %s:%s\n" ,
   "data_obs",
   GetChannel().c_str() ,
   GetRooObjFile().c_str() ,
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
   signal_dataset->sumEntries(),
   data_obs->sumEntries() );

   // Listing Nuisance parameters
   fprintf( cardfile , "----------------------------------------\n" );
   const string sig_unc = "1.05";
   fprintf( cardfile, "%8s lnN %15s %15s\n" , "Lumi"   , "1.05"          , "1.05" );
   fprintf( cardfile, "%8s lnN %15s %15s\n" , "sig_unc", sig_unc.c_str() , "--"   );
   fprintf( cardfile, "%8s lnN %15s %15s\n" , "bg_unc" , "--"            , "1.05" );

   cout << "Writing to " << cardfile_name << "..." << endl;
   fclose( cardfile );
}

void MakeCheckPlot(SampleRooFitMgr* data, SampleRooFitMgr* mc)
{
   TCanvas* c1 = new TCanvas("c1","c1",CANVAS_WIDTH,CANVAS_HEIGHT);
   TLegend* l1 = new TLegend(0.55,0.5,0.9,0.9);
   RooPlot* frame_1 = SampleRooFitMgr::x().frame();
   TGraph* mc_set_plot = PlotOn( frame_1 , mc->OriginalDataSet() );
   TGraph* pdf_plot    = PlotOn( frame_1 , mc->GetPdfFromAlias("fit") , RooFit::Range("FitRange") );
   frame_1->Draw();
   frame_1->SetMinimum(0.3);
   l1->AddEntry(mc_set_plot,"MC Template Background","lp");
   l1->AddEntry(pdf_plot   ,"MC Template fit","l");
   l1->Draw();
   c1->SaveAs( GetRooObjPlot("fitmc_vs_mc").c_str() );
   c1->SetLogy();
   c1->SaveAs( GetRooObjPlot("fitmc_vs_mc_log").c_str() );

   TCanvas* c2 = new TCanvas("c2","c2",CANVAS_WIDTH,CANVAS_HEIGHT);
   TLegend* l2 = new TLegend(0.55,0.5,0.9,0.9);
   RooPlot* frame_2 = SampleRooFitMgr::x().frame();
   TGraph* data_set_plot = PlotOn( frame_2 , data->OriginalDataSet() );
   TGraph* pdf_plot_2    = PlotOn( frame_2 , mc->GetPdfFromAlias("fit"), RooFit::Range("FitRange") );
   frame_2->Draw();
   frame_2->SetMinimum(0.3);
   l2->AddEntry(data_set_plot,"Data","lp");
   l2->AddEntry(pdf_plot_2 ,"MC Template fit (Norm)","l");
   l2->Draw();
   c2->SaveAs( GetRooObjPlot("fitmc_vs_data").c_str() );
   c2->SetLogy();
   c2->SaveAs( GetRooObjPlot("fitmc_vs_data_log").c_str() );
}
