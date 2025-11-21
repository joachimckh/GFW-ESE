#pragma once

#include "FlowContainer.h"

namespace gfwe {

enum FlowType {
  PTINT,
  PTDIFF
};

class EseFlowContainer : public FlowContainer {
  public:
    EseFlowContainer() : FlowContainer() {}
    explicit EseFlowContainer(const char* name) : FlowContainer(name) {}
    explicit EseFlowContainer(TProfile2D* prof);
    // explicit EseFlowContainer(TProfile2D* prof, TObjArray
    explicit EseFlowContainer(FlowContainer* base);
    ~EseFlowContainer();
    
    void SetEseRebin() {
      fEseRebin = 10; // default value
      fEseRebinEdges = new double[11]{0, 10, 20, 30, 40, 50, 60, 70, 80, 90, 100};
    }
    void SetEseRebin(std::vector<double> binedges) {
      fEseVec = std::move(binedges);
      fEseRebin = fEseVec.size() - 1;
      fEseRebinEdges = fEseVec.data();
    }
    void SetEseRebin(int nbins, double* binedges) {
      fEseRebin = nbins;
      fEseRebinEdges = binedges;
    }
    void SetPtBin() {
      this->SetPtBin({0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0, 1.25, 1.5, 1.75, 2.0, 2.25,2.5,2.75,3,3.25,3.5,3.75,4,4.5,4.5,5,5.5,6,7,8,10});
    }
    void SetPtBin(std::vector<double> binedges) {
      fPtVec = std::move(binedges);
      fNPtBins = fPtVec.size() - 1;
      auto xax = new TAxis(fNPtBins, fPtVec.data());
      SetXAxis(xax);
    }
    void SetPtRebin(std::vector<double> binedges) {
      fPtVec = std::move(binedges);
      fNPtBins = fPtVec.size() - 1;
      FlowContainer::SetPtRebin(fNPtBins, fPtVec.data());
    }

    void EseRebin(bool ptint = 1);
    TObjArray* GetEseProf() const { return fProfEse; }
    void SetMainProf() { fProf = fMainProf; }
    TProfile2D* GetMainProf() const { return fMainProf; }
    void SetProf(TProfile2D* prof) { 
      fProf = prof;
    }

    TH1D* GetVNN(int n = 2, int m = 2, bool onPt = true, double arg1 = -1, double arg2 = -1, bool eseCon = true);

    TH1D* GetEseProj(int cLow, int cHigh, const char* name ="hESE");
    TObjArray* GetEseRandProf() const { return fProfEseRand; }

    void RebinMult(int n) {
      if (fProf) {
        fProf->RebinX(n);
      }
      // rebin all profiles in rand as well
      if (fProfEseRand) {
        for (int i = 0; i < fProfEseRand->GetEntries(); ++i) {
          auto prof = dynamic_cast<TProfile2D*>(fProfEseRand->At(i));
          if (prof) {
            prof->RebinX(n);
          }
        }
      }
    }
    
  private:
    int fEseRebin = -1; //! do not store
    double *fEseRebinEdges = nullptr; //! do not store
    std::vector<double> fEseVec; //!
    TObjArray* fProfEse = nullptr; 
    TObjArray* fProfEseRand = nullptr;
    TProfile2D* fMainProf = nullptr; //!
    std::vector<double> fPtVec; //!
    int fNPtBins = 28; //!
    
    void EFBinner(int ir, int ese_lo, int ese_hi, const TString& l_name, TProfile2D* newProf);
    void EFBinnerPt(int ir, int ese_lo, int ese_hi, const TString& l_name, int ptbin, TProfile2D* newProf);

  ClassDef(EseFlowContainer, 1);
};

} // namespace gfwe

