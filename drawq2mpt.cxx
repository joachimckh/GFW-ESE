void drawq2mpt() {

  const char* sys= "Ne-Ne";
  int c1 = 1;
  int c2 = 1;

  auto f = TFile::Open("/Users/joachimcarlokristianhansen/hyperloop_data/LHC25af_pass2/flow/558697/AnalysisResults.root");
  auto dir = dynamic_cast<TDirectory*>(f->Get("flow-gfw-ese"));
  auto ptdir = dynamic_cast<TDirectory*>(dir->Get("mptcorr"));
  auto h3 = dynamic_cast<TH3D*>(ptdir->Get("h3_cent_q2_meanptperc"));

  h3->GetXaxis()->SetRange(c1,c2);
  auto h2 = dynamic_cast<TH2D*>(h3->Project3D("zy"));
  //h2->Scale(1.0/h2->Integral());

  auto c = new TCanvas("c1","",800,600);
  c->cd();

  // h2->Rebin2D(10,10);
  h2->Draw("");
  //gPad->SetLogz();

  TLatex l1;
  l1.SetTextSize(0.04);
  l1.DrawLatexNDC(0.18,0.8,Form("%s #sqrt{#it{s}_{NN}} = 5.36 TeV, Centrality %i-%i%%",sys,c1-1,c2));


  c->SaveAs(Form("figs/mpt/mptq2sel_%.0d_%.0d.pdf",c1-1,c2));
  
  








}
