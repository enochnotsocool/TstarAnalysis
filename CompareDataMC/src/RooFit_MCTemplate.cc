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
static RooRealVar m("m","m", 500 , 0, 1000 );
static RooRealVar b("b","b", 200 , 0 ,1000 );
static string roofit_file = GetRooFitObj_Template_File();
static const string ws_name = "wspace";

//------------------------------------------------------------------------------
//   Helper variables
//------------------------------------------------------------------------------
void MakeBGFromMC( SampleRooFitMgr* );
void MakeSignalPdf( SampleRooFitMgr* );
void MakeTemplatePlots( SampleRooFitMgr* , SampleRooFitMgr*, SampleRooFitMgr* );
void MakeCardFile(SampleRooFitMgr*, SampleRooFitMgr*, SampleRooFitMgr*);

//------------------------------------------------------------------------------
//   Function implemetations
//------------------------------------------------------------------------------
void MakeTemplate( SampleRooFitMgr* data, SampleRooFitMgr* bg, vector<SampleRooFitMgr*> signal_list )
{
   MakeBGFromMC( bg );
   for( auto& signal : signal_list ){
      MakeSignalPdf( signal );
   }
   MakeTemplatePlots( data, bg, signal_list.front() );

   for( auto& signal : signal_list ){
      MakeCardFile(data,bg,signal);
   }
   //------------------------------------------------------------------------------
   //   Saving all relavent obejcts by RooWorkSpace
   //------------------------------------------------------------------------------
   roofit_file = GetRooFitObj_Template_File();
   RooWorkspace ws( ws_name.c_str() , ws_name.c_str() );
   ws.import( *(data->GetReduceDataSet("FitRange")) );
   ws.import( *(bg->GetPdfFromAlias("fit")) );
   for( auto& signal : signal_list ){
      ws.import( *(signal->GetPdfFromAlias("fit")) );
   }
   ws.writeToFile( roofit_file.c_str() );


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


void MakeBGFromMC( SampleRooFitMgr* bg )
{
   const string bg_pdf_name = bg->MakePdfAlias( "fit" );
   RooGenericPdf* bg_pdf = new RooGenericPdf(
      bg_pdf_name.c_str(), bg_pdf_name.c_str(),
      "1/(1+exp((x-m)/b))",
      RooArgSet(SampleRooFitMgr::x(),m,b) );
   bg_pdf->fitTo( *(bg->OriginalDataSet()) ,
      RooFit::Save() ,            // Suppressing output
      RooFit::SumW2Error(kTRUE),  // For Weighted dataset
      RooFit::Range("FitRange")  // Fitting range
   );
   bg->AddPdf( bg_pdf );
   m.setConstant(kTRUE);
   b.setConstant(kTRUE);
   return ;
}

void MakeSignalPdf( SampleRooFitMgr* signal )
{
   const string pdf_name = signal->MakePdfAlias( "fit" );
   RooDataSet* sig_selc  = signal->MakeReduceDataSet( "FitRange" , RooFit::CutRange("FitRange") );
   RooKeysPdf* sig_pdf   = new RooKeysPdf( pdf_name.c_str() , pdf_name.c_str() , SampleRooFitMgr::x() , *sig_selc );
   signal->AddPdf( sig_pdf );
   return;
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
   TGraph* bg_pdf_plot  = PlotOn( frame , bg_pdf );
   TGraph* sig_set_plot = PlotOn( frame , signal_dataset , RooFit::DrawOption("B") );
   TGraph* sig_pdf_plot = PlotOn( frame , signal_pdf );
   TGraph* data_plot    = PlotOn( frame , data_dataset );
   TGraph* bg_pdf_norm_plot = PlotOn( frame , bg_pdf );

   TCanvas* c1 = new TCanvas("c","c" , 650, 500 );
   frame->Draw();
   frame->SetMinimum(0.0001);

   frame->GetXaxis()->SetLabelFont(43);
   frame->GetXaxis()->SetLabelSize( FONT_SIZE );
   frame->GetXaxis()->SetTitleFont(43);
   frame->GetXaxis()->SetTitleSize( FONT_SIZE );
   frame->GetXaxis()->SetTitleOffset( 1.2 );
   frame->GetYaxis()->SetLabelFont(43);
   frame->GetYaxis()->SetLabelSize( FONT_SIZE);
   frame->GetYaxis()->SetTitleFont(43);
   frame->GetYaxis()->SetTitleSize( FONT_SIZE );
   frame->GetYaxis()->SetTitleOffset( 1.2 );
   frame->SetTitle("");

   bg_set_plot->SetFillStyle(3003);
   bg_set_plot->SetFillColorAlpha( kBlue , 1.0 );
   bg_set_plot->SetLineColor( kBlue );
   bg_pdf_plot->SetLineColor( kBlue );

   sig_set_plot->SetFillStyle( 3004 );
   sig_set_plot->SetFillColorAlpha( kRed , 0.4 );
   sig_set_plot->SetLineColor( kRed );
   sig_pdf_plot->SetLineColor( kRed );
   bg_pdf_norm_plot->SetLineColor( kBlack );

   TLegend* leg = new TLegend(0.55,0.5,0.9,0.9);
   leg->AddEntry( data_plot        , "Data (2.256 fb^{-1})" , "lp" );
   leg->AddEntry( bg_set_plot      , "MC Background (Not Normalized)" , "f" );
   leg->AddEntry( bg_pdf_plot      , "Fitted (Yield from MC)" , "l" );
   leg->AddEntry( bg_pdf_norm_plot , "Fitted (Normalized to Data)" , "l");
   leg->AddEntry( sig_set_plot     , "t^{*} {}_{M_{t^{*}}=700GeV} (30 pb)" , "f" );
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
   tl->DrawLatex( 0.9, 0.93 , "(#mu channel)");
   tl->SetTextAlign(33);
   tl->DrawLatex(0.53,0.85 , "f(m) = N #left(1 + exp#left( #frac{m-a}{b} #right)#right)^{-1}" );


   c1->SaveAs( GetRooFitPlot_Template_File().c_str() );
   delete leg;
   delete tl;
   delete c1;
}


void MakeCardFile( SampleRooFitMgr* data, SampleRooFitMgr* bg, SampleRooFitMgr* signal )
{
   const string cardfile_name = GetTemplate_CardFile( signal->Name() );
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
      data_obs->sumEntries() );

   // Listing Nuisance parameters
   fprintf( cardfile , "----------------------------------------\n" );
   const string sig_unc = "1.05";
   fprintf( cardfile, "%8s lnN %15s %15s\n" , "Lumi"   , "1.05"          , "1.05" );
   fprintf( cardfile, "%8s lnN %15s %15s\n" , "sig_unc", sig_unc.c_str() , "--"   );
   fprintf( cardfile, "%8s lnN %15s %15s\n" , "bg_unc" , "--"            , "1.05" );
   fclose( cardfile );
}
