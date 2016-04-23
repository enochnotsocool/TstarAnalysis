/*******************************************************************************
 *
 *  Filename    : RooFit_SimFit.cc
 *  Description : Making simultaneous fit objects
 *  Author      : Yi-Mu "Enoch" Chen [ ensc@hep1.phys.ntu.edu.tw ]
 *
*******************************************************************************/
#include "TstarAnalysis/CompareDataMC/interface/SampleRooFitMgr.hh"
#include "TstarAnalysis/CompareDataMC/interface/PlotConfig.hh"
#include "TstarAnalysis/CompareDataMC/interface/FileNames.hh"
#include "TstarAnalysis/Utils/interface/RooFitUtils.hh"


#include <stdio.h>

#include "RooWorkspace.h"
#include "RooRealVar.h"
#include "RooGenericPdf.h"
#include "RooKeysPdf.h"
#include "RooAddPdf.h"

#include "TCanvas.h"
#include "TLegend.h"
#include "TLatex.h"

using namespace std;

//------------------------------------------------------------------------------
//   Static helper variables
//------------------------------------------------------------------------------
static const string ws_name = "wspace";
static vector<RooRealVar*> var_list ;
static RooRealVar* AddVar( const string&, double, double, double); // Self variables management
static void ClearVar();

//------------------------------------------------------------------------------
//   Helper static variables
//------------------------------------------------------------------------------
static void FitPDFs( SampleRooFitMgr*, SampleRooFitMgr* );
static void MakeValidationPlot( SampleRooFitMgr*, SampleRooFitMgr* );
static void MakeCardFile( SampleRooFitMgr*, SampleRooFitMgr* );
static void SaveRooWorkSpace(SampleRooFitMgr*,vector<SampleRooFitMgr*>);

void MakeSimFit( SampleRooFitMgr* data, vector<SampleRooFitMgr*> signal_list )
{
   for( auto& signal : signal_list ){
      FitPDFs( data, signal );
      MakeValidationPlot( data, signal );
      MakeCardFile( data,signal);
   }
   SaveRooWorkSpace(data,signal_list);
   ClearVar();
}

//------------------------------------------------------------------------------
//   RooFit control flow
//------------------------------------------------------------------------------
void FitPDFs( SampleRooFitMgr* data , SampleRooFitMgr* sig )
{
   const string bg_name    = data->MakePdfAlias( "bg" + sig->Name() );
   const string sig_name   = sig->MakePdfAlias( "sig" );
   const string model_name = data->MakePdfAlias( sig->Name() );

   RooRealVar* m  = AddVar("m" +sig->Name(), 100, -1000,1000);
   RooRealVar* b  = AddVar("b" +sig->Name(), 100, 1,    1000);
   RooRealVar* nb = AddVar("nb"+sig->Name(), 4000,0,    10000); // Background strength
   RooRealVar* ns = AddVar("ns"+sig->Name(), 100, 0,    10000); // signale strength

   char formula[1024];
   sprintf( formula, "1/(1+exp((x+%s)/%s))", m->GetName() , b->GetName() );

   RooGenericPdf* bg_pdf = new RooGenericPdf(
      bg_name.c_str() , bg_name.c_str(),
      formula,
      RooArgSet(SampleRooFitMgr::x(),*m,*b)
   );
   RooKeysPdf* sig_pdf   = new RooKeysPdf(
      sig_name.c_str(), sig_name.c_str(),
      SampleRooFitMgr::x(),
      *(sig->OriginalDataSet())
   );
   RooAddPdf* model = new RooAddPdf(
      model_name.c_str(), model_name.c_str(),
      RooArgList(*bg_pdf,*sig_pdf),
      RooArgList(*nb,*ns)
   );
   model->fitTo(
      *(data->GetReduceDataSet("FitRange")),
      RooFit::Range("FitRange"),
      RooFit::Minos(kTRUE)
   );

   sig->AddPdf( sig_pdf );
   data->AddPdf( bg_pdf );
   data->AddPdf( model );

   m->setConstant(kTRUE);
   b->setConstant(kTRUE);
   nb->setConstant(kTRUE);
   ns->setConstant(kTRUE);
}
//------------------------------------------------------------------------------
//   Making data card
//------------------------------------------------------------------------------
void MakeCardFile( SampleRooFitMgr* data, SampleRooFitMgr* sig )
{
   RooAddPdf* model    = (RooAddPdf*)(data->GetPdfFromAlias( sig->Name() )) ;
   const Parameter bg_strength = Parameter(
      ((RooRealVar*)(model->coefList().at(0)))->getVal(),
      ((RooRealVar*)(model->coefList().at(0)))->getErrorHi(),
      ((RooRealVar*)(model->coefList().at(0)))->getErrorLo()
   );
   const Parameter sig_strength = sig->Sample().ExpectedYield();

   RooDataSet* data_obs    = data->GetReduceDataSet("FitRange");
   RooAbsPdf*  bg_pdf      = data->GetPdfFromAlias("bg"+sig->Name());
   RooAbsPdf*  signal_pdf  = sig->GetPdfFromAlias("sig");

   FILE* cardfile = fopen( GetCardFile(sig->Name()).c_str() , "w" );

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
      bg_pdf->GetName()
   );
   fprintf( cardfile , "shapes %10s %15s %30s %s:%s\n" ,
      "sig",
      GetChannel().c_str() ,
      GetRooObjFile().c_str() ,
      ws_name.c_str(),
      signal_pdf->GetName()
   );
   fprintf( cardfile , "shapes %10s %15s %30s %s:%s\n" ,
      "data_obs",
      GetChannel().c_str() ,
      GetRooObjFile().c_str() ,
      ws_name.c_str(),
      data_obs->GetName()
   );
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
      sig_strength.CentralValue() ,
      data_obs->sumEntries()
   );

   // Listing Nuisance parameters
   char sig_unc[1024];
   char bg_unc[1024];
   sprintf( sig_unc , "%lf/%lf" , 1+sig_strength.RelUpperError(),1-sig_strength.RelLowerError() );
   sprintf( bg_unc  , "%lf/%lf" , 1+bg_strength.RelUpperError(),1-bg_strength.RelLowerError() );

   fprintf( cardfile , "----------------------------------------\n" );
   fprintf( cardfile, "%8s lnN %15s %15s\n" , "Lumi"   , "1.05" , "1.05" );
   fprintf( cardfile, "%8s lnN %15s %15s\n" , "sig_unc", sig_unc, "--"   );
   fprintf( cardfile, "%8s lnN %15s %15s\n" , "bg_unc" , "--"   , bg_unc );

   fclose( cardfile );
}

//------------------------------------------------------------------------------
//   Making validation plots
//------------------------------------------------------------------------------
void MakeValidationPlot( SampleRooFitMgr* data, SampleRooFitMgr* sig )
{
   TCanvas* c = new TCanvas("c","c", CANVAS_WIDTH, CANVAS_HEIGHT );
   RooPlot* frame = SampleRooFitMgr::x().frame();

   RooAddPdf* model   = (RooAddPdf*)data->GetPdfFromAlias( sig->Name() );
   RooAbsPdf* bg_pdf  = data->GetPdfFromAlias("bg"+sig->Name() );
   RooAbsPdf* sig_pdf = sig->GetPdfFromAlias("sig");
   double bg_strength  = ((RooRealVar*)(model->coefList().at(0)))->getVal();
   double bg_err       = ((RooRealVar*)(model->coefList().at(0)))->getError();
   double sig_strength = sig->Sample().ExpectedYield();

   TGraph* data_plot = PlotOn( frame, data->OriginalDataSet() );
   TGraph* bg_plot = PlotOn( frame, bg_pdf,
      RooFit::Normalization(bg_strength,RooAbsReal::NumEvent)
   );
   TGraph* sig_plot  = PlotOn( frame, sig_pdf,
      RooFit::Normalization(sig_strength,RooAbsReal::NumEvent),
      RooFit::DrawOption("LB")
   );

   frame->Draw();
   SetFrame(frame,FONT_SIZE); // see Utils/src/RooFitUtils.cc

   sig_plot->SetLineColor(kRed);
   sig_plot->SetFillStyle(3004);
   sig_plot->SetFillColor(kRed);

   TLegend* leg = new TLegend(0.65,0.7,0.9,0.9);
   char data_entry[1024];
   char sig_entry[1024];
   sprintf( data_entry, "Data (%.3lf fb^{-1})" , SampleRooFitMgr::TotalLuminosity()/1000. );
   sprintf( sig_entry,  "t^{*} {}_{M_{t^{*}}=%dGeV} (%.1lf pb)" , GetSignalMass(sig->Name()), sig->Sample().CrossSection().CentralValue() );
   leg->AddEntry( data_plot, data_entry , "lp" );
   leg->AddEntry( bg_plot  , "Fitted Background" , "l" );
   leg->AddEntry( sig_plot , sig_entry , "lf" );
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
   tl->SetTextSize( FONT_SIZE );
   tl->SetTextAlign(33);
   tl->DrawLatex(0.60,0.85 , "f(m) = N #left(1 + exp#left( #frac{m-a}{b} #right)#right)^{-1}" );

   unsigned obs = data->OriginalDataSet()->sumEntries();
   char obs_yield[1024];
   char exp_yield[1024];
   char delta[1024];
   sprintf( obs_yield, "Observed Yield: %d", obs );
   sprintf( exp_yield, "Expected Yield: %.2lf #pm %.2lf" , bg_strength, bg_err );
   sprintf( delta    , "#Delta: %lf%%", 100.* (bg_strength-obs)/obs );
   tl->DrawLatex(0.60,0.65 , obs_yield );
   tl->DrawLatex(0.60,0.57 , exp_yield );
   tl->DrawLatex(0.60,0.49 , delta );


   // Saving and cleaning up
   c->SaveAs( GetRooObjPlot( sig->Name() ).c_str() );
   c->SetLogy(kTRUE);
   c->SaveAs( GetRooObjPlot(sig->Name() + "_log").c_str() );
   delete leg;
   delete tl;
   delete c;
}

//------------------------------------------------------------------------------
//   Save everything to RooWorkspace
//------------------------------------------------------------------------------
void SaveRooWorkSpace( SampleRooFitMgr* data, vector<SampleRooFitMgr*> signal_list )
{
   const string roofit_file = GetRooObjFile();
   cout << "Saving RooFit objects to " << roofit_file << endl;
   RooWorkspace ws( ws_name.c_str() , ws_name.c_str() );
   ws.import( *(data->GetReduceDataSet("FitRange")) );
   for( auto& signal : signal_list ){
      ws.import( *(data->GetPdfFromAlias("bg"+signal->Name())) );
      ws.import( *(signal->GetPdfFromAlias("sig")) );
   }
   ws.writeToFile( roofit_file.c_str() );
}


//------------------------------------------------------------------------------
//   Helper functions
//------------------------------------------------------------------------------
RooRealVar* AddVar( const string& x , double a , double min, double max )
{
   var_list.push_back( new RooRealVar(x.c_str(), x.c_str(), a, min, max) );
   return var_list.back();
}

void ClearVar()
{
   for( auto& var : var_list ){
      delete var;
   }
}
