#include "eseflow.h"

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
  delete fProfEse;
};

void EseFlowContainer::EseRebin(bool ptint) {

  if (!fEseRebin || !fEseRebinEdges || !fProf)
    return;

  int nX = fProf->GetNbinsX();
  double xMin = fProf->GetXaxis()->GetXmin();
  double xMax = fProf->GetXaxis()->GetXmax();

  int nESE = fEseRebin;
  TProfile2D* newProf = new TProfile2D(Form("ese_%s", fIDName.Data()), "",
                                    nX, xMin, xMax,
                                    nESE, 0.5, nESE + 0.5);
  newProf->SetDirectory(0);

  TAxis* yax = fProf->GetYaxis();
  TString l_name("");
  Ssiz_t l_pos = 0;
  while (fIDName.Tokenize(l_name, l_pos)) {
    for (int ir = 0; ir < fEseRebin; ++ir) {
      int ese_lo = fEseRebinEdges[ir];
      int ese_hi = fEseRebinEdges[ir + 1] - 1;

      TString ybl1 = Form("ese_%i_%s", ese_lo, l_name.Data());
      TString ybl2 = Form("ese_%i_%s", ese_hi, l_name.Data());

      int ybn1 = yax->FindBin(ybl1.Data());
      int ybn2 = yax->FindBin(ybl2.Data());

      if (ybn1 <= 0 || ybn2 <= 0 || ybn1 > fProf->GetNbinsY() || ybn2 > fProf->GetNbinsY())
        continue;

      TString label = Form("ese_%i_%i_%s", ese_lo, ese_hi, l_name.Data());
      int targetYBin = ir + 1;
      newProf->GetYaxis()->SetBinLabel(targetYBin, label.Data());


      for (int ix = 1; ix <= fProf->GetNbinsX(); ++ix) {
        double xval = fProf->GetXaxis()->GetBinCenter(ix);
        double sumV = 0;
        double sumW = 0;
        double sumE2 = 0;

        for (int iy = ybn1; iy <= ybn2; ++iy) {
          int bin = fProf->GetBin(ix, iy);
          double v = fProf->GetBinContent(bin);
          double w = fProf->GetBinEntries(bin);
          double e = fProf->GetBinError(bin);

          if (w > 0) {
            sumV += v * w;
            sumW += w;
            sumE2 += e * e;
          }
        }

        if (sumW > 0) {
          double avgV = sumV / sumW;
          newProf->Fill(xval, targetYBin, avgV, sumW);
          newProf->SetBinError(ix, targetYBin, std::sqrt(sumE2));
        }
      }
    }
  }
  
  fProfEse->Add(newProf);
}

