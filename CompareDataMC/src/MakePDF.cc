/*******************************************************************************
 *
 *  Filename    : MakePDF.cc
 *  Description : Declaring pdfs objects
 *  Author      : Yi-Mu "Enoch" Chen [ ensc@hep1.phys.ntu.edu.tw ]
 *
*******************************************************************************/
#include "TstarAnalysis/CompareDataMC/interface/SampleRooFitMgr.hh"
#include "TstarAnalysis/CompareDataMC/interface/VarMgr.hh"

#include "RooKeysPdf.h"
#include "RooGenericPdf.h"
#include <string>

using namespace std;

//------------------------------------------------------------------------------
//   Making pdfs
//------------------------------------------------------------------------------
RooKeysPdf* MakeKeysPdf(SampleRooFitMgr* sample,const string& name = "")
{
   const string pdf_name = sample->MakePdfAlias( name );
   RooKeysPdf*  pdf      = new RooKeysPdf(
      pdf_name.c_str(), pdf_name.c_str(),
      SampleRooFitMgr::x(),
      *(sample->OriginalDataSet())
   );
   sample->AddPdf( pdf );
   return pdf;
}

RooGenericPdf* MakeFermi(SampleRooFitMgr* sample,const string& name = "fermi" )
{
   char formula[1024];
   const string pdf_name = sample->MakePdfAlias( name );
   RooRealVar* m  = var_mgr.NewVar( "m" + pdf_name , 100, -1000,1000);
   RooRealVar* b  = var_mgr.NewVar( "b" + pdf_name , 100, 1,    1000);

   sprintf( formula, "1/(1+exp((x-%s)/%s))", m->GetName() , b->GetName() );

   RooGenericPdf* pdf = new RooGenericPdf(
      pdf_name.c_str() , pdf_name.c_str(),
      formula,
      RooArgSet(SampleRooFitMgr::x(),*m,*b)
   );

   sample->AddPdf( pdf );
   return pdf;
}

RooGenericPdf* MakeExo( SampleRooFitMgr* sample, const string& name="exo" )
{
   char formula[1024];
   const string pdf_name = sample->MakePdfAlias( name );
   RooRealVar* f  = var_mgr.NewVar( "f" + pdf_name , -10, -100, +100);
   RooRealVar* g  = var_mgr.NewVar( "g" + pdf_name , -40, -100, +100);
   RooRealVar* h  = var_mgr.NewVar( "h" + pdf_name ,   2,  -10, +100);

   sprintf( formula, "(TMath::Power((x-1),(%s)))/(TMath::Power(x,(%s+%s*TMath::Log(x))))",
      f->GetName(),
      g->GetName(),
      h->GetName()
   );

   RooGenericPdf* pdf = new RooGenericPdf(
      pdf_name.c_str() , pdf_name.c_str(),
      formula,
      RooArgSet(SampleRooFitMgr::x(),*f,*g,*h)
   );

   sample->AddPdf( pdf );
   return pdf;
}
