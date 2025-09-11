
{
  // auto f = TFile::Open("/Users/joachimcarlokristianhansen/hyperloop_data/LHC25af_pass1/flow/498443/AnalysisResults.root","READ");
  auto f = TFile::Open("/Users/joachimcarlokristianhansen/hyperloop_data/LHC25af_pass1/flow/497615/AnalysisResults.root","READ");

  auto dir = dynamic_cast<TDirectory*>(f->Get("flow-gfw-ese"));
  auto fc = static_cast<FlowContainer*>(dir->Get("FlowContainer"));
  auto prof = fc->GetProfile();

  //double ese_edges[] = {0, 20, 40, 60, 80, 100};
  //int fESERebin = 5; 

  TString fIDName = "ChGap22";
  auto fce = EseFlowContainer(prof);
  fce.SetIDName(fIDName);
  fce.SetEseRebin();
  fce.EseRebin();

  // fce.SetIDName("ChGap32");
  // fce.EseRebin();

  auto arr_ese = fce.GetEseProf(); // 0-20%
  arr_ese->Print();
  
  auto c = new TCanvas("c","c",800,600);
  c->cd();
  auto prof2 = dynamic_cast<TH2*>(arr_ese->At(0));
  prof2->Draw();
  

}
