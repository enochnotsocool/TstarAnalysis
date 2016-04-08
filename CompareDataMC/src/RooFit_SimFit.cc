/*******************************************************************************
 *
 *  Filename    : RooFit_SimFit.cc
 *  Description : Making simultaneous fit objects
 *  Author      : Yi-Mu "Enoch" Chen [ ensc@hep1.phys.ntu.edu.tw ]
 *
*******************************************************************************/
#include "TstarAnalysis/CompareDataMC/interface/SampleRooFitMgr.hh"

#include "RooRealVar.h"
#include "RooGenericPdf.h"
#include "RooKeysPdf.h"
#include "RooAddPdf.h"

//------------------------------------------------------------------------------
//   Static helper variables
//------------------------------------------------------------------------------
static RooRealVar m("m","m",100,1,1000);
static RooRealVar b("b","b",100,1,1000);
static RooRealVar nb("nb","nb",0.99,0,1); // Background strength
static RooRealVar ns("ns","ns",0.01,0,1); // Background strength

void MakeSimFit( SampleRooFitMgr* data , SampleRooFitMgr* sig )
{
   RooGenericPdf* bg_pdf = new RooGenericPdf("bg","bg",
      "1/(1+exp((x-m)/b))",
      RooArgSet(SampleRooFitMgr::x(),m,b) );
   RooKeysPdf* sig_pdf   = new RooKeysPdf( "sig", "sig",
      SampleRooFitMgr::x(),
      *(sig->OriginalDataSet()) );
   RooAddPdf* model = new RooAddPdf( "model" , "model" ,
      RooArgList(*bg_pdf,*sig_pdf),
      RooArgList(nb,ns) );

   model->fitTo(
      *(data->GetReduceDataSet("FitRange")),
      RooFit::Range("FitRange") );


   delete bg_pdf;
   delete sig_pdf;
   delete model;
   return;
}
