
{

  // auto f = TFile::Open("/Users/joachimcarlokristianhansen/hyperloop_data/LHC25af_pass1/flow/498443/AnalysisResults.root","READ"); 
  auto f = TFile::Open("/Users/joachimcarlokristianhansen/hyperloop_data/LHC25ae_pass1_small/499544/AnalysisResults.root");

  auto dir = dynamic_cast<TDirectory*>(f->Get("flow-gfw-ese"));
  auto fc = static_cast<FlowContainer*>(dir->Get("FlowContainer"));

  
  TString fIDName = "ese_10_ChGap";
  fc->SetIDName(fIDName);

  auto vn2 = fc->GetVNN(2, 2, true, 0, 20);


}
