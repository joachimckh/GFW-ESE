
void high_low_v2_mpt_cent() {
  const char* sys = "LHC25af_pass2(Ne-Ne)";
  const char* selection = "#delta#it{p}_{T}/<<#it{p}_{T}>>";
  auto f = TFile::Open("/Users/joachimcarlokristianhansen/hyperloop_data/LHC25af_pass2/flow/558697/AnalysisResults.root","READ");  // Ne-Ne
  auto dir = dynamic_cast<TDirectory*>(f->Get("flow-gfw-ese_mpt"));
  auto fc = static_cast<FlowContainer*>(dir->Get("FlowContainer"));
  // auto prof = fc->GetProfile();

  auto fce = gfwe::EseFlowContainer(fc);
  fce.SetIDName("ChGap22");
  fce.RebinMult(2);
  std::vector<double> eseBins = {0,10,50,75,90,100};
  fce.SetEseRebin(eseBins);
  fce.EseRebin();

  // fce.SetIDName("ChGap32");
  // fce.EseRebin();

  auto arr_ese = fce.GetEseProf(); // 0-20%
  arr_ese->Print();
  
  auto c = new TCanvas("c","c",800,600);
  c->cd();
  auto prof2 = dynamic_cast<TProfile2D*>(arr_ese->At(0));
  fce.SetProf(prof2);
  fce.SetIDName("ese_0_9_ChGap");
  TH1D* vn2 = fce.GetVNN(2, 2, false);
  vn2->SetLineColor(kBlue);
  vn2->SetMarkerColor(kBlue);
  vn2->SetMarkerStyle(20);
  vn2->GetXaxis()->SetTitle("Centrality (%)");
  vn2->GetYaxis()->SetTitle("v_{2}{2}");
  vn2->GetXaxis()->SetRangeUser(0,60);
  vn2->GetYaxis()->SetRangeUser(0,0.13);
  vn2->Draw("");
  

  fce.SetIDName("ese_90_99_ChGap");
  TH1D* vn2_1 = fce.GetVNN(2, 2, false);
  vn2_1->SetMarkerStyle(20);
  vn2_1->SetLineColor(kRed);
  vn2_1->SetMarkerColor(kRed); 
  vn2_1->Draw("same");

  fce.SetIDName("ese_0_99_ChGap");
  TH1D* vn2_all = fce.GetVNN(2, 2, false);
  vn2_all->SetMarkerStyle(20);
  vn2_all->SetLineColor(kBlack);
  vn2_all->SetMarkerColor(kBlack);
  vn2_all->Draw("same");

  auto leg = new TLegend(0.6,0.7,0.88,0.88);
  leg->AddEntry(vn2,Form("10%%-lowest %s",selection),"ep");
  leg->AddEntry(vn2_1,Form("10%%-highest %s",selection),"ep");
  leg->AddEntry(vn2_all,"Inclusive","ep");
  leg->Draw(); 

  TLatex latex3;
  latex3.SetTextSize(0.04);
  latex3.DrawLatexNDC(0.2,0.2,Form("%s #sqrt{#it{s}_{NN}} = 5.36 TeV",sys));

  
  auto ratio = dynamic_cast<TH1D*>(vn2_1->Clone("ratio") );
  ratio->Divide(vn2);

  auto c2 = new TCanvas("c2","c2",800,600);
  c2->cd();

  ratio->SetLineColor(kBlack);
  ratio->SetMarkerColor(kBlack);
  ratio->SetMarkerStyle(20);
  ratio->SetMarkerSize(0.8);
  //ratio->GetYaxis()->SetTitle(Form("v_{2}{2}(high-%s) / v_{2}{2}(low-%s)",selection,selection));
  ratio->GetYaxis()->SetTitle("Ratio");
  ratio->GetXaxis()->SetTitle("Centrality (%)");
  ratio->GetXaxis()->SetRangeUser(0,60);
  ratio->GetYaxis()->SetRangeUser(0.8,1.8);
  ratio->Draw("");

  // horizontal line at 1
  auto line = new TLine(0,1,60,1);
  line->SetLineStyle(2);
  line->Draw("same");

  

  c->SaveAs(Form("figs/mpt/%s_vn2_0-9_vs_90-99.pdf",sys) );
  c2->SaveAs(Form("figs/mpt/%s_diffcent_vn2_ratio_90-99_vs_0-9.pdf",sys) );


}
