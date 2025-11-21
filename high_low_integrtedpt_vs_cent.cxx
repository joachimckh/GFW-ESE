
{
  const char* sys = "OO and Ne-Ne";
  auto f = TFile::Open("/Users/joachimcarlokristianhansen/hyperloop_data/LHC25ae_pass1/flow/501800/AnalysisResults.root","READ"); // OO
  auto fNe = TFile::Open("/Users/joachimcarlokristianhansen/hyperloop_data/LHC25af_pass1/flow/499634/AnalysisResults.root","READ"); // Ne-Ne
  auto dir = dynamic_cast<TDirectory*>(f->Get("flow-gfw-ese"));
  auto fc = static_cast<FlowContainer*>(dir->Get("FlowContainer"));
  // auto prof = fc->GetProfile();

  auto fce = gfwe::EseFlowContainer(fc);
  fce.SetIDName("ChGap22");
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
  vn2->SetLineColor(kBlack);
  vn2->SetMarkerColor(kBlack);
  vn2->Draw("");
  

  fce.SetIDName("ese_90_99_ChGap");
  TH1D* vn2_1 = fce.GetVNN(2, 2, false);
  vn2_1->SetLineColor(kBlack);
  vn2_1->SetMarkerColor(kBlack); 
  vn2_1->Draw("same");
  

  auto dirNe = dynamic_cast<TDirectory*>(fNe->Get("flow-gfw-ese"));
  auto fcNe = static_cast<FlowContainer*>(dirNe->Get("FlowContainer"));
  auto fceNe = gfwe::EseFlowContainer(fcNe);
  fceNe.RebinMult(5);
  fceNe.SetIDName("ChGap22");
  fceNe.SetEseRebin(eseBins);
  fceNe.EseRebin();
  auto arr_eseNe = fceNe.GetEseProf();
  auto profNe = dynamic_cast<TProfile2D*>(arr_eseNe->At(0));
  fceNe.SetProf(profNe);
  fceNe.SetIDName("ese_0_9_ChGap");
  TH1D* vn2Ne = fceNe.GetVNN(2, 2, false);
  vn2Ne->SetLineColor(kRed);
  vn2Ne->SetMarkerColor(kRed);
  vn2Ne->Draw("same");
  fceNe.SetIDName("ese_90_99_ChGap");
  TH1D* vn2Ne_1 = fceNe.GetVNN(2, 2, false);
  vn2Ne_1->SetLineColor(kBlue);
  vn2Ne_1->SetMarkerColor(kBlue);
  vn2Ne_1->Draw("same");
  vn2Ne_1->Divide(vn2Ne);


  auto ratio = dynamic_cast<TH1D*>(vn2_1->Clone("ratio") );
  ratio->Divide(vn2);

  auto c2 = new TCanvas("c2","c2",800,600);
  c2->cd();

  ratio->SetLineColor(kBlack);
  ratio->SetMarkerColor(kBlack);
  ratio->SetMarkerStyle(20);
  ratio->SetMarkerSize(0.8);
  ratio->GetYaxis()->SetTitle("v_{2}{2}(high-q_{2}^{FT0C}) / v_{2}{2}(low-q_{2}^{FT0C})");
  ratio->GetXaxis()->SetTitle("Centrality (%)");
  ratio->GetXaxis()->SetRangeUser(0,60);
  ratio->GetYaxis()->SetRangeUser(0.7,1.4);
  ratio->Draw("");

  vn2Ne_1->SetLineColor(kRed);
  vn2Ne_1->SetMarkerColor(kRed);
  vn2Ne_1->SetMarkerStyle(21);
  vn2Ne_1->SetMarkerSize(0.8);
  vn2Ne_1->Draw("EPsame");

  // horizontal line at 1
  auto line = new TLine(0,1,60,1);
  line->SetLineStyle(2);
  line->Draw("same");


  auto leg2 = new TLegend(0.6,0.7,0.88,0.88);
  leg2->AddEntry(ratio,"OO","ep");
  leg2->AddEntry(vn2Ne_1,"Ne-Ne","ep");
  leg2->Draw();
  

  TLatex latex3;
  latex3.SetTextSize(0.04);
  latex3.DrawLatexNDC(0.2,0.2,Form("%s #sqrt{#it{s}_{NN}} = 5.36 TeV",sys));


  c2->SaveAs(Form("figs/inc/%s_diffcent_vn2_ratio_90-99_vs_0-9.pdf",sys));


}
