
{
  auto f = TFile::Open("/Users/joachimcarlokristianhansen/hyperloop_data/LHC25ae_pass1/flow/501800/AnalysisResults.root","READ"); // OO
  // auto f = TFile::Open("/Users/joachimcarlokristianhansen/hyperloop_data/LHC25af_pass1/flow/499634/AnalysisResults.root","READ"); // Ne-Ne
  auto dir = dynamic_cast<TDirectory*>(f->Get("flow-gfw-ese"));
  auto fc = static_cast<FlowContainer*>(dir->Get("FlowContainer"));
  auto prof = fc->GetProfile();

  auto fce = gfwe::EseFlowContainer(prof);
  fce.SetIDName("ChGap22");
  fce.SetEseRebin();
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
  vn2->Draw("");
  

  fce.SetIDName("ese_90_99_ChGap");
  TH1D* vn2_1 = fce.GetVNN(2, 2, false);
  vn2_1->Draw("same");


}
