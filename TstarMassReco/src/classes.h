#include "TstarAnalysis/TstarMassReco/interface/ChiSquareResult.h"
#include "TstarAnalysis/TstarMassReco/interface/HitFitResult.h"
#include "DataFormats/Common/interface/Wrapper.h"

namespace TstarAnalysis_TstarMassReco {
   struct dictionaryobjects {
      ChiSquareResult csr;
      HitFitResult    hfr;
      edm::Wrapper<ChiSquareResult>  csr_wp;
      edm::Wrapper<HitFitResult>     hfr_wp;
   };
}
