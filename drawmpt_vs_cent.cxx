
TProfile* getProf(TH3* h3, int low, int high) {
  h3->GetYaxis()->SetRange(low,high);
  auto h2 = dynamic_cast<TH2D*>(h3->Project3D("zx"));
  //h2->Rebin2D(10,1);
  auto prof = h2->ProfileX(Form("prof_%d_%d",low,high));
  prof->SetMarkerStyle(20);
  return prof;
}

void drawmpt_vs_cent() {
  int r1 = TColor::GetColor("#ffcccc");
  int r2 = TColor::GetColor("#ff9999");
  int r3 = TColor::GetColor("#ff6666");
  int r4 = TColor::GetColor("#ff3333");
  int r5 = TColor::GetColor("#cc0000");

  int b1 = TColor::GetColor("#ccccff");
  int b2 = TColor::GetColor("#9999ff");
  int b3 = TColor::GetColor("#6666ff");
  int b4 = TColor::GetColor("#3333ff");
  int b5 = TColor::GetColor("#0000cc");

  const char* sys= "Ne-Ne";

  auto f = TFile::Open("/Users/joachimcarlokristianhansen/hyperloop_data/LHC25af_pass2/flow/558697/AnalysisResults.root");
  auto dir = dynamic_cast<TDirectory*>(f->Get("flow-gfw-ese"));
  auto ptdir = dynamic_cast<TDirectory*>(dir->Get("mptcorr"));
  auto h3 = dynamic_cast<TH3D*>(ptdir->Get("h3_cent_q2_mptvalue"));

  auto prof_1 = getProf(h3,0,9);
  prof_1->SetLineColor(r1);
  prof_1->SetMarkerColor(r1);
  prof_1->GetXaxis()->SetRangeUser(0,60);
  prof_1->GetYaxis()->SetRangeUser(-0.03,.01);

  auto prof_2 = getProf(h3,10,19);
  prof_2->SetLineColor(r2);
  prof_2->SetMarkerColor(r2);
  auto prof_3 = getProf(h3,20,29);
  prof_3->SetLineColor(r3);
  prof_3->SetMarkerColor(r3);
  auto prof_4 = getProf(h3,30,39);
  prof_4->SetLineColor(r4);
  prof_4->SetMarkerColor(r4);
  auto prof_5 = getProf(h3,40,49);
  prof_5->SetLineColor(r5);
  prof_5->SetMarkerColor(r5);
  auto prof_6 = getProf(h3,50,59);
  prof_6->SetLineColor(b1);
  prof_6->SetMarkerColor(b1);
  auto prof_7 = getProf(h3,60,69);
  prof_7->SetLineColor(b2);
  prof_7->SetMarkerColor(b2);
  auto prof_8 = getProf(h3,70,79);
  prof_8->SetLineColor(b3);
  prof_8->SetMarkerColor(b3);
  auto prof_9 = getProf(h3,80,89);
  prof_9->SetLineColor(b4);
  prof_9->SetMarkerColor(b4);
  auto prof_10 = getProf(h3,99,99);
  prof_10->SetLineColor(b5);
  prof_10->SetMarkerColor(b5);

  auto prof_full = getProf(h3,0,99);
  prof_full->SetLineColor(kBlack);
  prof_full->SetMarkerColor(kBlack);

  auto c = new TCanvas("c1","",800,600);
  c->cd();
  prof_1->SetTitle(";Centrality (%);#delta#it{p}_{T}/<<#it{p}_{T}>>");
  prof_1->Draw("");
  prof_2->Draw("same");
  prof_3->Draw("same");
  prof_4->Draw("same");
  prof_5->Draw("same");
  prof_6->Draw("same");
  prof_7->Draw("same");
  prof_8->Draw("same");
  prof_9->Draw("same");
  prof_10->Draw("same");
  prof_full->Draw("same");

  //prof_10->Divide(prof_1);
  //prof_10->Draw("");

  TLatex l1;
  l1.SetTextSize(0.04);
  l1.DrawLatexNDC(0.18,0.38,Form("%s #sqrt{#it{s}_{NN}} = 5.36 TeV",sys));

  auto leg = new TLegend(0.18,0.18,0.38,0.33);
  leg->SetBorderSize(0);
  leg->AddEntry(prof_1,"0-10% q_{2}","lep");
  leg->AddEntry(prof_2,"10-20% q_{2}","lep");
  leg->AddEntry(prof_3,"20-30% q_{2}","lep");
  leg->AddEntry(prof_4,"30-40% q_{2}","lep");
  leg->AddEntry(prof_5,"40-50% q_{2}","lep");
  leg->AddEntry(prof_6,"50-60% q_{2}","lep");
  leg->AddEntry(prof_7,"60-70% q_{2}","lep");
  leg->AddEntry(prof_8,"70-80% q_{2}","lep");
  leg->AddEntry(prof_9,"80-90% q_{2}","lep");
  leg->AddEntry(prof_10,"90-100% q_{2}","lep");
  leg->Draw();

  //c->SaveAs(Form("mptq2sel_%.0d_%.0d.pdf",c1-1,c2));
  
  








}
