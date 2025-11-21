
{
  //auto f = TFile::Open("/Users/joachimcarlokristianhansen/hyperloop_data/LHC25ae_pass1/flow/505771/AnalysisResults.root","READ"); // OO
  //const char* sys = "OO";
  auto f = TFile::Open("/Users/joachimcarlokristianhansen/hyperloop_data/LHC25af_pass1/flow/499634/AnalysisResults.root","READ"); // Ne-Ne 
  const char* sys = "Ne-Ne";
  double minc = 0;
  double maxc = 5;
  
  auto dir = dynamic_cast<TDirectory*>(f->Get("flow-gfw-ese"));
  auto fc = static_cast<FlowContainer*>(dir->Get("FlowContainer"));
  // auto prof = fc->GetProfile();
  // auto fce = gfwe::EseFlowContainer(prof);
  auto fce = gfwe::EseFlowContainer(fc);
  //fce.SetPropagateErrors(true);
  fce.SetIDName("ChGap22");
  std::vector<double> vec = {0, 10,20, 30,40, 50,60, 70,80, 90,100};
  fce.SetEseRebin(vec);
  // fce.SetEseRebin();
  fce.EseRebin(false); // false ust means for pT, change this its so confusing


  // fce.SetIDName("ChGap32");
  // fce.EseRebin();

  auto arr_ese = fce.GetEseProf(); // 0-20%
  //fce.SetPtBin();
  fce.SetPtRebin({0.2, 0.4, 0.6, 0.8, 1.0, 1.5, 2.0, 2.5,3,3.5,4,5,6,7,8,9,10});
  
  auto c = new TCanvas("c","c",800,600);
  c->cd();
  auto prof2 = dynamic_cast<TProfile2D*>(arr_ese->At(0));
  // prof2->Draw("");
  fce.SetProf(prof2);
  fce.SetIDName("ese_0_9_ChGap");
  TH1D* vn2_low = fce.GetVNN(2, 2, true, minc, maxc);
  vn2_low->SetLineColor(kCyan+1);
  vn2_low->SetMarkerColor(kCyan+1);
  vn2_low->SetMarkerStyle(20);
  vn2_low->SetMarkerSize(0.8);


  fce.SetIDName("ese_90_99_ChGap");
  TH1D* vn2_high = fce.GetVNN(2, 2, true, minc, maxc);
  vn2_high->SetLineColor(kRed);
  vn2_high->SetMarkerColor(kRed);
  vn2_high->SetMarkerStyle(21);
  vn2_high->SetMarkerSize(0.8);

  fce.SetIDName("ese_0_99_ChGap");
  TH1D* vn2 = fce.GetVNN(2, 2, true, minc, maxc);
  vn2->SetLineColor(kBlack);
  vn2->SetMarkerColor(kBlack);
  vn2->SetMarkerStyle(22);
  vn2->SetMarkerSize(0.8);

  vn2_high->GetYaxis()->SetRangeUser(0,0.4);
  vn2_high->Draw("EP");
  vn2_low->Draw("EPsame");
  vn2->Draw("EPsame");

  TLatex latex;
  latex.SetTextSize(0.04);
  latex.DrawLatexNDC(0.2,0.2,Form("%s #sqrt{#it{s}_{NN}} = 5.36 TeV, Centrality %.0f-%.0f%%",sys,minc,maxc));
  
  auto leg = new TLegend(0.68,0.2,0.88,0.4);
  leg->AddEntry(vn2,"Inclusive","ep");
  leg->AddEntry(vn2_low,"0-10%% low-q_{2}^{FT0C}","ep");
  leg->AddEntry(vn2_high,"90-100%% high-q_{2}^{FT0C}","ep");
  leg->Draw();

  c->SaveAs(Form("figs/pt/%s_diffpt_vn2_cent_%.0f_%.0f.pdf",sys,minc,maxc));

  auto c2 = new TCanvas("c2","c2",800,600);
  c2->cd();

  // ratios between vn2 and low and high

  auto r_low = dynamic_cast<TH1D*>(vn2_low->Clone("r_low"));
  r_low->Divide(vn2);

  r_low->SetLineColor(kCyan+1);
  r_low->SetMarkerColor(kCyan+1);
  r_low->SetMarkerStyle(20);
  r_low->SetMarkerSize(0.8);

  auto r_high = dynamic_cast<TH1D*>(vn2_high->Clone("r_high"));
  r_high->GetYaxis()->SetTitle("v_{2}{2}(ESE-selected) / v_{2}{2}(Inclusive)");
  r_high->Divide(vn2);
  r_high->SetLineColor(kRed);
  r_high->SetMarkerColor(kRed);
  r_high->SetMarkerStyle(21);
  r_high->SetMarkerSize(0.8);

  r_high->Draw("EP");
  r_low->Draw("EPsame");

  r_high->GetYaxis()->SetRangeUser(0,2);

  auto leg2 = new TLegend(0.6,0.7,0.88,0.88);
  leg2->AddEntry(r_low,"0-10%% low-q_{2}^{FT0C} / Inclusive","ep");
  leg2->AddEntry(r_high,"90-100%% high-q_{2}^{FT0C} / Inclusive","ep");
  leg2->Draw();

  TLatex latex2;
  latex2.SetTextSize(0.04);
  latex2.DrawLatexNDC(0.2,0.2,Form("%s #sqrt{#it{s}_{NN}} = 5.36 TeV, Centrality %.0f-%.0f%%",sys,minc,maxc));
  

  c2->SaveAs(Form("figs/pt/%s_diffpt_vn2_ratio_cent_%.0f_%.0f.pdf",sys,minc,maxc));

  auto c3 = new TCanvas("c3","c3",800,600);
  c3->cd();
  // ratio of high and low
  //
  auto r3_ratio = dynamic_cast<TH1D*>(vn2_high->Clone("r3_ratio"));
  r3_ratio->Divide(vn2_low);

  r3_ratio->SetLineColor(kBlack);
  r3_ratio->SetMarkerColor(kBlack);
  // makers for closed circles in root? 
  r3_ratio->SetMarkerStyle(20);
  r3_ratio->SetMarkerSize(0.8);
  r3_ratio->GetYaxis()->SetTitle("v_{2}{2}(high-q_{2}^{FT0C}) / v_{2}{2}(low-q_{2}^{FT0C})");
  r3_ratio->GetYaxis()->SetRangeUser(0.8,1.5);
  r3_ratio->GetXaxis()->SetTitle("p_{T} (GeV/c)");
  r3_ratio->GetXaxis()->SetRangeUser(0.2,3);
  r3_ratio->Draw("EP");

  TLatex latex3;
  latex3.SetTextSize(0.04);
  latex3.DrawLatexNDC(0.2,0.2,Form("%s #sqrt{#it{s}_{NN}} = 5.36 TeV, Centrality %.0f-%.0f%%",sys,minc,maxc));

  c3->SaveAs(Form("figs/pt/%s_diffpt_vn2_high_low_ratio_cent_%.0f_%.0f.pdf",sys,minc,maxc));
}
