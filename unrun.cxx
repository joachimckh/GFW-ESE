
{
  auto f = TFile::Open("/Users/joachimcarlokristianhansen/gfwO2/local-dev/localrun_lightion/ptintv2v3.root","READ");

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

  fce.SetIDName("ChGap32");
  fce.EseRebin();

  auto prof2 = fce.GetProfile();
  auto c = new TCanvas("c","c",800,600);
  c->cd();
  prof->Draw();
  

}
