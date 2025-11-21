#include "eseflow.h"

ClassImp(gfwe::EseFlowContainer)


namespace gfwe {

EseFlowContainer::EseFlowContainer(TProfile2D* prof) :
  FlowContainer()
{
  fProfEse = new TObjArray(); 
  fProfEse->SetName("EseFlow");
  fProfEse->SetOwner(kTRUE);
  fProf = prof;
  fMainProf = dynamic_cast<TProfile2D*>(prof->Clone("mainClone"));

  fProfEseRand = new TObjArray();
  fProfEseRand->SetName("EseFlowRand");
  fProfEseRand->SetOwner(kTRUE);
}
EseFlowContainer::EseFlowContainer(FlowContainer* base) :
  FlowContainer(*base)
{
  fProfEse = new TObjArray(); 
  fProfEse->SetName("EseFlow");
  fProfEse->SetOwner(kTRUE);
  fProf = base->GetProfile();
  fMainProf = dynamic_cast<TProfile2D*>(fProf->Clone("mainClone"));

  fProfEseRand = new TObjArray();
  fProfEseRand->SetName("EseFlowRand");
  fProfEseRand->SetOwner(kTRUE);
}

EseFlowContainer::~EseFlowContainer() {
  
  SetMainProf();
  if (fProfEse)
    delete fProfEse;
  if (fProfEseRand)
    delete fProfEseRand;
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
  int totalYBins = fNPtBins * (fEseRebin + 1) + (fEseRebin + 1); // + pt-integrated bins
  auto* newProf = new TProfile2D(Form("ese_%s", fIDName.Data()), "",
                                 fProf->GetNbinsX(),
                                 fProf->GetXaxis()->GetXmin(),
                                 fProf->GetXaxis()->GetXmax(),
                                 totalYBins, 0.5, totalYBins + 0.5);
  newProf->SetDirectory(0);

  TString l_name("");
  Ssiz_t l_pos = 0;
  while (fIDName.Tokenize(l_name, l_pos)) {
    // Step 1: Fill pt-integrated bins first (same as EFBinner)
    for (int ir = 0; ir < fEseRebin; ++ir) {
      const int ese_lo = fEseRebinEdges[ir];
      const int ese_hi = fEseRebinEdges[ir + 1] - 1;
      this->EFBinner(ir, ese_lo, ese_hi, l_name, newProf);
    }
    // Inclusive
    this->EFBinner(fEseRebin, fEseRebinEdges[0], fEseRebinEdges[fEseRebin] - 1, l_name, newProf);

    for (int ir = 0; ir <= fEseRebin; ++ir) { // include inclusive bin here too
      const int ese_lo = (ir < fEseRebin) ? fEseRebinEdges[ir] : fEseRebinEdges[0];
      const int ese_hi = (ir < fEseRebin) ? fEseRebinEdges[ir + 1] - 1 : fEseRebinEdges[fEseRebin] - 1;

      for (int j = 1; j <= fNPtBins; ++j) {
        int yIndex = (fEseRebin + 1) + ir * fNPtBins + (j - 1);
        this->EFBinnerPt(yIndex, ese_lo, ese_hi, l_name, j, newProf);
      }
    }
  }

  fProfEse->Add(newProf);
} 
  if (fProfRand && fProfRand->GetEntries() > 0) {
      if (!fProfEseRand) {
        fProfEseRand = new TObjArray();
        fProfEseRand->SetName("EseFlowRand");
        fProfEseRand->SetOwner(kTRUE);
      }

      for (int i = 0; i < fProfRand->GetEntries(); ++i) {
        //std::cout << "Rebinning random profile " << i << std::endl;
        auto* prof = dynamic_cast<TProfile2D*>(fProfRand->At(i));
        if (!prof) {
          std::cout << "Warning: random profile " << i << " is not a TProfile2D, skipping..." << std::endl;
          continue;
        }

        auto* tempFlow = new EseFlowContainer(prof);
        tempFlow->SetIDName(fIDName);
        tempFlow->SetEseRebin(fEseVec);
        tempFlow->EseRebin(ptint);

        auto arr_tmp = tempFlow->GetEseProf();

        auto* rebinned = dynamic_cast<TProfile2D*>(tempFlow->GetEseProf()->At(0)->Clone(
          Form("ese_%s_rand_%d", fIDName.Data(), i)));
        rebinned->SetDirectory(0);
        fProfEseRand->Add(rebinned);

        delete tempFlow;
      }
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

TH1D* EseFlowContainer::GetEseProj(int cLow, int cHigh, const char* name ="hESE") {
  if (!fProf) return nullptr;

  TAxis* yAxis = fProf->GetYaxis();
  int nEse = 100; 

  TH1D* hEse = new TH1D(name, name, nEse, 0, nEse);
  hEse->GetXaxis()->SetTitle("ESE index");
  hEse->GetYaxis()->SetTitle("Value");

  std::cout << fIDName.Data() << " ESE projection over centrality " << cLow << "-" << cHigh << std::endl;
  for (int iEse = 1; iEse <= nEse; ++iEse) {
    // project in X over [cLo, cHi] for fixed ESE bin
    const TString yLabel = Form("ese_%d_%s", iEse-1, fIDName.Data());
    const int yBin = yAxis->FindBin(yLabel.Data());
    double sum = 0;
    double err2 = 0;
    int n = 0;

    for (int ic = cLow; ic <= cHigh; ++ic) {
      double val = fProf->GetBinContent(ic, yBin);
      double err = fProf->GetBinError(ic, yBin);
      if (val == 0 && err == 0) continue; // skip empty

      sum += val;
      err2 += err * err;
      n++;
    }

    if (n > 0) {
      hEse->SetBinContent(iEse, sum / n);
      hEse->SetBinError(iEse, std::sqrt(err2) / n);
    }
  }

  return hEse;
}


TH1D* EseFlowContainer::GetVNN(int n = 2, int m = 2, bool onPt = true, double arg1 = -1, double arg2 = -1, bool eseCon = true) {
if (!fProf) return nullptr;

  if (eseCon) {
  // Get central value from main profile
  TH1D* hMain = FlowContainer::GetVNN(n, m, onPt, arg1, arg2);
  if (!hMain || !fProfEseRand || fProfEseRand->GetEntries() < 1) return hMain;

  const int nBins = hMain->GetNbinsX();
  std::vector<std::vector<double>> bootVals(nBins); // [bin] -> [bootstrap values]

  // Loop over bootstrap subsamples
  if (!fProfEseRand) {std::cout << "no fProfEseRand" << std::endl;} 
  for (int i = 0; i < fProfEseRand->GetEntries(); ++i) {
    auto* prof = dynamic_cast<TProfile2D*>(fProfEseRand->At(i));
    if (!prof) continue;
    TString name = prof->GetName();
    if (!name.Contains(Form("ChGap%i%i",n,m))) continue; // make this smarter at some point

    EseFlowContainer tmpFC;
    tmpFC.SetProf(prof);
    tmpFC.SetIDName(fIDName);
    if (onPt) tmpFC.SetPtBin(fPtVec);
    TH1D* hSub = tmpFC.GetVNN(n, m, onPt, arg1, arg2, false);

    if (!hSub) continue;

    for (int bin = 1; bin <= nBins; ++bin) {
      double val = hSub->GetBinContent(bin);
      bootVals[bin - 1].push_back(val);
    }

    delete hSub; // Clean up
  }

  // Compute standard deviation for each bin
  for (int bin = 1; bin <= nBins; ++bin) {
    const std::vector<double>& vals = bootVals[bin - 1];
    if (vals.size() < 2) continue;

    double mean = std::accumulate(vals.begin(), vals.end(), 0.0) / vals.size();
    double sq_sum = std::inner_product(vals.begin(), vals.end(), vals.begin(), 0.0);
    double stdev = std::sqrt((sq_sum / vals.size()) - (mean * mean));

    hMain->SetBinError(bin, stdev);
  }

  return hMain;
  } else {
    return FlowContainer::GetVNN(n, m, onPt, arg1, arg2);
  }
}

} // namespace gfwe

