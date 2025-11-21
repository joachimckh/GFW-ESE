{
  auto f = TFile::Open("/Users/joachimcarlokristianhansen/hyperloop_data/LHC25af_pass1/flow/499634/AnalysisResults.root","READ"); // Ne-Ne
  auto dir = dynamic_cast<TDirectory*>(f->Get("flow-gfw-ese"));
  auto fc = static_cast<FlowContainer*>(dir->Get("FlowContainer"));
  // auto fce = gfwe::EseFlowContainer(fc);
  auto prof = fc->GetProfile();
  auto fce = gfwe::EseFlowContainer(prof);
  fce.SetIDName("ChGap22");

  int cent = 1; 
  int cent2 = 15;
  auto h1 = fce.GetEseProj(cent, cent2, "hESE");

  fce.SetEseRebin();
  fce.EseRebin();
  auto arr_ese = fce.GetEseProf(); // 0-20%
  auto prof2 = dynamic_cast<TProfile2D*>(arr_ese->At(0));
  fce.SetProf(prof2);
  fce.SetIDName("ese_0_99_ChGap");
  
  TH1D* vn2 = fce.GetVNN(2, 2, false);
  vn2->Print(); // something is wrong above
  // double bra_v2_ket = vn2->GetBinContent(cent+1);
  double bra_v2_ket = vn2->Integral(cent, cent2);

  auto f2 = TFile::Open("/Users/joachimcarlokristianhansen/hyperloop_data/LHC25ae_pass1/flow/505771/AnalysisResults.root","READ"); // OO
  auto dir2 = dynamic_cast<TDirectory*>(f2->Get("flow-gfw-ese"));
  auto fc2 = static_cast<FlowContainer*>(dir2->Get("FlowContainer"));
  auto prof21 = fc2->GetProfile();
  auto fce2 = gfwe::EseFlowContainer(prof21);
  fce2.SetIDName("ChGap22");
  auto h2 = fce2.GetEseProj(cent, cent2, "hESE");

  fce2.SetEseRebin();
  fce2.EseRebin();
  auto arr_ese2 = fce2.GetEseProf();
  auto prof22 = dynamic_cast<TProfile2D*>(arr_ese2->At(0));
  fce2.SetProf(prof22);
  fce2.SetIDName("ese_0_99_ChGap");
  TH1D* vn22 = fce2.GetVNN(2, 2, false);
  // double bra_v2_ket2 = vn22->GetBinContent(cent+1);
  double bra_v2_ket2 = vn22->Integral(cent, cent2);

  std::cout << "<v_2>(Ne-Ne)/<v_2>(OO) = " << bra_v2_ket/bra_v2_ket2 << std::endl;
  h1->Scale(1/bra_v2_ket);
  h2->Scale(1/bra_v2_ket2);
  h2->SetLineColor(kRed);
  h1->SetLineColor(kBlack);


  auto c1 = new TCanvas("c1","c1",800,600);
  // h1->Draw("HIST");
  // h2->Draw("HIST SAME");
  auto ratio = (TH1D*)h1->Clone("ratio");
  ratio->GetXaxis()->SetTitle("#it{q}_{2}^{FT0C} (%)");
  ratio->GetYaxis()->SetTitle("#frac{<v_{2}>_{ESE}^{Ne-Ne}/<v_{2}>_{inc}^{Ne-Ne}}{<v_{2}>_{ESE}^{OO}/<v_{2}>_{inc}^{OO}}");
  ratio->SetLineColor(kBlack);
  ratio->SetMarkerColor(kBlack);
  ratio->SetMarkerStyle(20);
  ratio->Divide(h2);
  ratio->Rebin(10);
  ratio->GetYaxis()->SetRangeUser(0.9,1.3);
  for (int ib=1; ib<=ratio->GetNbinsX(); ++ib) {
    ratio->SetBinContent(ib, ratio->GetBinContent(ib)/10.0);
    ratio->SetBinError(ib,    ratio->GetBinError(ib)/10.0);
  }
  ratio->Draw("EP");

  gStyle->SetOptFit(1111);

  // make an explicit TF1 over the histogram x range
  // TF1 *f = new TF1("f_const","pol0",
  //                  ratio->GetXaxis()->GetXmin(),
  //                  ratio->GetXaxis()->GetXmax());

  // // fit but don't auto-draw (option "0")
  // ratio->Fit(f, "R0");

  // // style and draw on the same pad
  // f->SetLineColor(kRed);
  // f->SetLineWidth(2);
  // f->Draw("SAME");

  TLatex latex;
  latex.SetTextSize(0.04);
  latex.DrawLatexNDC(0.2,0.85,"OO and Ne-Ne #sqrt{#it{s}_{NN}} = 5.36 TeV");
  latex.DrawLatexNDC(0.2,0.2,Form("Centrality %i-%i%%",cent-1,cent2));

  c1->SaveAs(Form("figs/inc/v2vsESE_%i.pdf",cent));
}
