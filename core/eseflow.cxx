#include "eseflow.h"


namespace gfwe {

EseFlowContainer::EseFlowContainer(TProfile2D* prof) :
  FlowContainer()
{
  fProfEse = new TObjArray(); 
  fProfEse->SetName("EseFlow");
  fProfEse->SetOwner(kTRUE);
  fProf = prof;
  fMainProf = dynamic_cast<TProfile2D*>(prof->Clone("mainClone"));
}

EseFlowContainer::~EseFlowContainer() {
  
  SetMainProf();
  if (fProfEse)
    delete fProfEse;
};

void EseFlowContainer::EseRebin(bool ptint) {
  if (!fEseRebin || !fEseRebinEdges || !fProf)
    return;

  const int nX = fProf->GetNbinsX();
  const double xMin = fProf->GetXaxis()->GetXmin();
  const double xMax = fProf->GetXaxis()->GetXmax();

  TString l_name("");
  Ssiz_t l_pos = 0;

  if (ptint) {
    auto* newProf = new TProfile2D(Form("ese_%s", fIDName.Data()), "",
                                   nX, xMin, xMax,
                                   fEseRebin + 1, 0.5, fEseRebin + 1 + 0.5);
    newProf->SetDirectory(0);

    while (fIDName.Tokenize(l_name, l_pos)) {
      // Regular ESE bins
      for (int ir = 0; ir < fEseRebin; ++ir) {
        const int ese_lo = fEseRebinEdges[ir];
        const int ese_hi = fEseRebinEdges[ir + 1] - 1;
        this->EFBinner(ir, ese_lo, ese_hi, l_name, newProf);
      }
      // Inclusive bin
      this->EFBinner(fEseRebin, fEseRebinEdges[0], fEseRebinEdges[fEseRebin] - 1, l_name, newProf);
    }

    fProfEse->Add(newProf);
  } else {
    auto* newProf = new TProfile2D(Form("ese_%s", fIDName.Data()), "",
                                   fProf->GetNbinsX(),
                                   fProf->GetXaxis()->GetXmin(),
                                   fProf->GetXaxis()->GetXmax(),
                                   (fEseRebin + 1) * fNPtBins, 0.5, (fEseRebin + 1) * fNPtBins + 0.5);
    newProf->SetDirectory(0);

    TString l_name("");
    Ssiz_t l_pos = 0;
    while (fIDName.Tokenize(l_name, l_pos)) {
      for (int j = 1; j <= fNPtBins; ++j) {
        for (int ir = 0; ir < fEseRebin; ++ir) {
          const int ese_lo = fEseRebinEdges[ir];
          const int ese_hi = fEseRebinEdges[ir + 1] - 1;
          const int yIndex = (j - 1) * (fEseRebin + 1) + ir;
          this->EFBinnerPt(yIndex, ese_lo, ese_hi, l_name, j, newProf);
        }
        // Inclusive bin
        const int yIndex = (j - 1) * (fEseRebin + 1) + fEseRebin;
        this->EFBinnerPt(yIndex, fEseRebinEdges[0], fEseRebinEdges[fEseRebin] - 1, l_name, j, newProf);
      }
    }

    fProfEse->Add(newProf);
  }
}

void EseFlowContainer::EFBinner(int ir, int ese_lo, int ese_hi, const TString& l_name, TProfile2D* newProf) {
  const TString label = Form("ese_%i_%i_%s", ese_lo, ese_hi, l_name.Data());
  const int nX = fProf->GetNbinsX();
  TAxis* yax = fProf->GetYaxis();
  const int targetYBin = ir + 1;
  newProf->GetYaxis()->SetBinLabel(targetYBin, label);

  const TString ybl1 = Form("ese_%i_%s", ese_lo, l_name.Data());
  const TString ybl2 = Form("ese_%i_%s", ese_hi, l_name.Data());
  const int ybn1 = yax->FindBin(ybl1.Data());
  const int ybn2 = yax->FindBin(ybl2.Data());

  if (ybn1 <= 0 || ybn2 <= 0 || ybn1 > fProf->GetNbinsY() || ybn2 > fProf->GetNbinsY())
    return;

  for (int ix = 1; ix <= nX; ++ix) {
    double xval = fProf->GetXaxis()->GetBinCenter(ix);
    double sumV = 0, sumW = 0, sumE2 = 0;

    for (int iy = ybn1; iy <= ybn2; ++iy) {
      const int bin = fProf->GetBin(ix, iy);
      const double v = fProf->GetBinContent(bin);
      const double w = fProf->GetBinEntries(bin);
      const double e = fProf->GetBinError(bin);

      if (w > 0) {
        sumV += v * w;
        sumW += w;
        sumE2 += e * e;
      }
    }

    if (sumW > 0) {
      const double avgV = sumV / sumW;
      newProf->Fill(xval, targetYBin, avgV, sumW);
      newProf->SetBinError(ix, targetYBin, std::sqrt(sumE2));
    }
  }
};


void EseFlowContainer::EFBinnerPt(int yIndex, int ese_lo, int ese_hi, const TString& l_name, int ptbin, TProfile2D* newProf) {
  const TString label = Form("ese_%d_%d_%s_pt_%d", ese_lo, ese_hi, l_name.Data(), ptbin);
  const int targetYBin = yIndex + 1;
  newProf->GetYaxis()->SetBinLabel(targetYBin, label);

  const int nX = fProf->GetNbinsX();
  TAxis* yax = fProf->GetYaxis();

  for (int ix = 1; ix <= nX; ++ix) {
    double xval = fProf->GetXaxis()->GetBinCenter(ix);
    double sumV = 0, sumW = 0, sumE2 = 0;

    for (int i = ese_lo; i <= ese_hi; ++i) {
      const TString yLabel = Form("ese_%d_%s_pt_%d", i, l_name.Data(), ptbin);
      const int ybin = yax->FindBin(yLabel.Data());
      if (ybin <= 0 || ybin > fProf->GetNbinsY()) continue;

      const int bin = fProf->GetBin(ix, ybin);
      const double v = fProf->GetBinContent(bin);
      const double w = fProf->GetBinEntries(bin);
      const double e = fProf->GetBinError(bin);

      if (w > 0) {
        sumV += v * w;
        sumW += w;
        sumE2 += e * e;
      }
    }

    if (sumW > 0) {
      const double avgV = sumV / sumW;
      newProf->Fill(xval, targetYBin, avgV, sumW);
      newProf->SetBinError(ix, targetYBin, std::sqrt(sumE2));
    }
  }
}

} // namespace gfwe

