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

#include "RooFit.h"
#include "RooRealVar.h"
#include "RooDataSet.h"
#include "RooGenericPdf.h"
#include "RooKeysPdf.h"
#include "RooPlot.h"

#include "TAxis.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TLatex.h"
#include <string>

using namespace std;

//------------------------------------------------------------------------------
//   Static variables
//------------------------------------------------------------------------------
RooRealVar m("m","m", 500 , 0, 1000 );
RooRealVar b("b","b", 200 , 0 ,1000 );

//------------------------------------------------------------------------------
//   Function implemetations
//------------------------------------------------------------------------------
void MakeBGFromMC( SampleRooFitMgr& bg )
{
   const string bg_pdf_name = bg.MakePdfAlias( "fit" );
   RooGenericPdf* bg_pdf = new RooGenericPdf(
      bg_pdf_name.c_str(), bg_pdf_name.c_str(),
      "1/(1+exp((x-m)/b))",
      RooArgSet(SampleRooFitMgr::x(),m,b) );
   bg_pdf->fitTo( *(bg.OriginalDataSet()) ,
      RooFit::Save() ,            // Suppressing output
      RooFit::SumW2Error(kTRUE),  // For Weighted dataset
      RooFit::Range("FitRange")  // Fitting range
   );
   bg.AddPdf( bg_pdf );
   return ;
}

void MakeSignalPdf( SampleRooFitMgr& signal )
{
   const string pdf_name = signal.MakePdfAlias( "fit" );
   RooDataSet* sig_selc  = signal.MakeReduceDataSet( "selc" , RooFit::CutRange("FitRange") );
   RooKeysPdf* sig_pdf   = new RooKeysPdf( pdf_name.c_str() , pdf_name.c_str() , SampleRooFitMgr::x() , *sig_selc );
   signal.AddPdf( sig_pdf );
   return;
}


void MakeTemplatePlots( SampleRooFitMgr& data, SampleRooFitMgr& bg, SampleRooFitMgr& signal )
{
   RooPlot* frame = SampleRooFitMgr::x().frame();
   RooDataSet* data_dataset   = data.OriginalDataSet();
   RooDataSet* bg_dataset     = bg.OriginalDataSet();
   RooDataSet* signal_dataset = signal.OriginalDataSet();
   RooAbsPdf*  signal_pdf     = signal.GetPdfFromAlias("fit");
   RooAbsPdf*  bg_pdf         = bg.GetPdfFromAlias("fit");

   bg_dataset->plotOn( frame ,  RooFit::DrawOption("B"));
   TGraph* bg_set_plot = (TGraph*)frame->getObject( frame->numItems() -1 );

   bg_pdf->plotOn(frame);
   TGraph* bg_pdf_plot = (TGraph*)frame->getObject( frame->numItems() -1 );

   signal_dataset->plotOn( frame , RooFit::DrawOption("B"));
   TGraph* sig_set_plot = (TGraph*)frame->getObject( frame->numItems() -1 );

   signal_pdf->plotOn( frame );
   TGraph* sig_pdf_plot = (TGraph*)frame->getObject( frame->numItems() -1 );

   data_dataset->plotOn( frame );
   TGraph* data_plot = (TGraph*)frame->getObject( frame->numItems() -1 );

   bg_pdf->plotOn(frame);
   TGraph* bg_pdf_norm_plot = (TGraph*)frame->getObject( frame->numItems() -1 );

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
