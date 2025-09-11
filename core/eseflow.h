#pragma once

#include "FlowContainer.h"



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

  private:
    int fEseRebin = -1; //! do not store, used for ESE analysis
    double *fEseRebinEdges = nullptr; //! do not store, used for ESE analysis
    TObjArray* fProfEse = nullptr; 
    TProfile2D* fMainProf = nullptr;
    const int fNPtBins = 27;
    
    void EFBinner(int ir, int ese_lo, int ese_hi, const TString& label, TProfile2D* newProf);

  ClassDef(EseFlowContainer, 1);
};


