
{
  //std::vector<int> colors = {kRed, kBlue, kGreen+2, kMagenta, kCyan+2, kOrange+7, kViolet+3, kTeal+3};
  std::array<Int_t,8> colors = {kBlack, kRed+1 , kBlue+1, kGreen+3, kMagenta+1, kOrange-1,kCyan+2,kYellow+2};
  auto f = TFile::Open("/Users/joachimcarlokristianhansen/hyperloop_data/LHC25ae_pass1/flow/540641/AnalysisResults.root","READ"); // OO

  auto dir = dynamic_cast<TDirectory*>(f->Get("flow-gfw-ese"));
  auto fc = static_cast<FlowContainer*>(dir->Get("FlowContainer"));
  const char* sys = "OO";

  auto fce = gfwe::EseFlowContainer(fc);
  // std::vector<double> eseBins = {0,30,50,70,80,90,100};
  std::vector<double> eseBins = {0,50,75,100};
  fce.SetIDName("ChGap22");
  fce.SetEseRebin(eseBins);
  fce.EseRebin();

  fce.SetIDName("ChGap32");
  fce.EseRebin();

  auto arr_ese = fce.GetEseProf(); 

  std::vector<TH1D*> v2;
  std::vector<TH1D*> v3;
  auto c = new TCanvas("c","c",800,600);
  c->cd();
  auto prof2 = dynamic_cast<TProfile2D*>(arr_ese->At(0));
  //prof2->RebinX(10);
  fce.SetProf(prof2);
  fce.RebinMult(10);
  // fce.SetIDName("ese_0_99_ChGap");
  // auto tv2 = fce.GetVNN(2, 2, false);
  // tv2->Draw();

  // only do below when errors are fixed
  //for (int i=0; i<5; i++) {
  for (int i = 0; i < eseBins.size() - 1; ++i) {
    fce.SetIDName(Form("ese_%d_%d_ChGap", static_cast<int>(eseBins[i]), static_cast<int>(eseBins[i+1]-1)));
    auto vn = fce.GetVNN(2, 2, false);
    v2.push_back(vn);
  }

  auto prof_v3 = dynamic_cast<TProfile2D*>(arr_ese->At(1));
  prof_v3->RebinX(10);
  fce.SetProf(prof_v3);
  for (int i = 0; i < eseBins.size() - 1; ++i) {
    fce.SetIDName(Form("ese_%d_%d_ChGap", static_cast<int>(eseBins[i]), static_cast<int>(eseBins[i+1]-1)));
    auto vn = fce.GetVNN(3, 2, false);
    v3.push_back(vn);
  }

  
  std::vector<std::pair<double,double>> centBins = {
        //{0,5}, {5,10}, {10,15}, {15,20}, {20,25}, {25,30}, {30,35}, {35,40}, {40,45}, {45,50}
    {0,10}, {10,20}, {20,30}, {30,40}, {40,50}//, {50,60}
  };
  std::vector<double> shift = { 0.004, 0.002, -0.002, -0.004 , -0.008};


  bool firstDraw = true;
  auto leg = new TLegend(0.68,0.65,0.88,0.88);
  leg->SetBorderSize(0);

  for (size_t ic = 0; ic < centBins.size(); ++ic) {
    int color = colors[ic];

    std::vector<double> x, y, ex, ey;


    for (size_t iq = 0; iq < v2.size(); ++iq) {
        if (!v2[iq] || !v3[iq]) continue;

        int bin = ic + 1;
        double v2val = v2[iq]->GetBinContent(bin) - shift[ic];
        double v3val = v3[iq]->GetBinContent(bin) + shift[ic];
        double v2err = v2[iq]->GetBinError(bin);
        double v3err = v3[iq]->GetBinError(bin);

        x.push_back(v2val);
        y.push_back(v3val);
        ex.push_back(v2err);
        ey.push_back(v3err);
    }

    TGraphErrors *gr = new TGraphErrors(x.size(), x.data(), y.data(), ex.data(), ey.data());
    gr->SetMarkerStyle(20);
    gr->SetMarkerColor(color);
    gr->SetLineColor(color);
    gr->SetTitle(Form("Cent: %.0f-%.0f%%", centBins[ic].first, centBins[ic].second));

    // gr->Fit("pol1");

    if (firstDraw) {
        gr->GetXaxis()->SetLimits(0.04, 0.1);
        gr->GetYaxis()->SetRangeUser(0, 0.04);
        gr->Draw("APL");
        gr->GetXaxis()->SetTitle("v_{2}");
        gr->GetYaxis()->SetTitle("v_{3}");
        firstDraw = false;
    } else {
        gr->Draw("PL SAME");
    }

    leg->AddEntry(gr, gr->GetTitle(), "ep");
  }
  leg->Draw();
  TLatex latex2;
  latex2.SetTextSize(0.04);
  latex2.DrawLatexNDC(0.2,0.25,Form("%s #sqrt{#it{s}_{NN}} = 5.36 TeV",sys));
  latex2.DrawLatexNDC(0.2,0.2,"q_{2}^{FT0C} classes" );

  c->SaveAs(Form("figs/inc/v2v3_correlation_%s_ChGap22_32.pdf",sys));


}

