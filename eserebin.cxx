{
   auto f = TFile::Open("/Users/joachimcarlokristianhansen/hyperloop_data/LHC25ae_pass1/flow/501800/AnalysisResults.root","READ"); // OO
  // auto f = TFile::Open("/Users/joachimcarlokristianhansen/hyperloop_data/LHC25af_pass1/flow/499634/AnalysisResults.root","READ"); // Ne-Ne
  auto dir = dynamic_cast<TDirectory*>(f->Get("flow-gfw-ese"));
  auto fc = static_cast<FlowContainer*>(dir->Get("FlowContainer"));
  auto fce = gfwe::EseFlowContainer(fc);
  // auto prof = fc->GetProfile();
  // auto fce = gfwe::EseFlowContainer(prof);
  fce.SetIDName("ChGap22");
  auto vec = std::vector<double>{0,20,40,60,80,100};
  fce.SetEseRebin(vec);


  fce.EseRebin();

  auto arr_ese = fce.GetEseProf();
  // arr_ese->Print();

  auto rand_ese = fce.GetEseRandProf();
  rand_ese->Print();
  
  TCanvas* c1 = new TCanvas("c1","c1",800,600);

  auto prof2 = dynamic_cast<TProfile2D*>(arr_ese->At(0));
  prof2->Draw();


  TFile* fout = new TFile("files/fce_output.root", "RECREATE");
  arr_ese->Write("EseFlow", TObject::kSingleKey);

  // Write rebinned random profiles
  if (fce.GetEseRandProf()) {
    fce.GetEseRandProf()->Write("EseFlowRand", TObject::kSingleKey);
  }

  fout->Close();
}
