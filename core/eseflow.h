#pragma once

#include "FlowContainer.h"



class EseFlowContainer : public FlowContainer {
  public:
    EseFlowContainer() : FlowContainer() {}
    explicit EseFlowContainer(const char* name) : FlowContainer(name) {}
    explicit EseFlowContainer(TProfile2D* prof);
    ~EseFlowContainer();
    
    void SetEseRebin() {
      fEseRebin = 5; // default value
      fEseRebinEdges = new double[6]{0, 20, 40, 60, 80, 100};
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


  ClassDef(EseFlowContainer, 1);
};


