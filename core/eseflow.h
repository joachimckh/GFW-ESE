#pragma once

#include "FlowContainer.h"

namespace gfwe {

class EseFlowContainer : public FlowContainer {
  public:
    EseFlowContainer() : FlowContainer() {}
    explicit EseFlowContainer(const char* name) : FlowContainer(name) {}
    explicit EseFlowContainer(TProfile2D* prof);
    ~EseFlowContainer();
    
    void SetEseRebin() {
      fEseRebin = 10; // default value
      fEseRebinEdges = new double[11]{0, 10, 20, 30, 40, 50, 60, 70, 80, 90, 100};
    }
    void SetEseRebin(int nbins, double* binedges) {
      fEseRebin = nbins;
      fEseRebinEdges = binedges;
    }

    void EseRebin(bool ptint = 1);
    TObjArray* GetEseProf() const { return fProfEse; }
    void SetMainProf() { fProf = fMainProf; }
    TProfile2D* GetMainProf() const { return fMainProf; }
    void SetProf(TProfile2D* prof) { 
      fProf = prof;
    }


  private:
    int fEseRebin = -1; //! do not store
    double *fEseRebinEdges = nullptr; //! do not store
    TObjArray* fProfEse = nullptr; 
    TProfile2D* fMainProf = nullptr;
    const int fNPtBins = 28;
    
    void EFBinner(int ir, int ese_lo, int ese_hi, const TString& l_name, TProfile2D* newProf);
    void EFBinnerPt(int ir, int ese_lo, int ese_hi, const TString& l_name, int ptbin, TProfile2D* newProf);

  ClassDef(EseFlowContainer, 1);
};

} // namespace gfwe

