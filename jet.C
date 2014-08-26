{
  TFile f1("yajem.root");			
  TFile f0("pythia/pythia.root");			
  TCanvas *c = new TCanvas("c","c",800,600);
  //  c->SetLogy();
  gStyle->SetOptStat(0);
  const int N = 6;
  int colors[N] = {kBlack, kRed, kBlue, kViolet, kOrange+7, kGreen+2};
  TH1 *a[6];
  TH1 *h1[6];
  TH1 *h0[6];
  TH2 *h2 = new TH2F("h2", "Inclusive jets", 2, 0.0, 20.0, 2, 0.0, 2);
  h2->SetTitle("");
  h2->SetXTitle("jet energy GeV");
  h2->SetYTitle("R_{AA}");
  TLegend legend(0.75, 0.45, 0.89, 0.89);
  for (int i = 7; i > 1; --i)
    {
      TString s = "inclusive_jets_0";
      s += i;
      TString t = "R = 0.";
      t += i;
      f1.GetObject(s.Data(), h1[i-2]);
      f0.GetObject(s.Data(), h0[i-2]);
      h1[i-2]->Divide(h1[i-2], h0[i-2], 1000000.0, 1164.0*10.0);
      // 1164 is the mean number of binary collisions in the Au+Au sample I'm using
      a[i-2] = h1[i-2];
      h1[i-2]->SetLineColor(colors[i-2]);
      legend->AddEntry(h1[i-2],t.Data(),"l");
    }
  TH1 *p0,*p1;
  f0.GetObject("pions",p0);
  f1.GetObject("pions",p1);
  p1->Divide(p1, p0, 1000000.0, 1164.0*10.0);
  h2->Draw();
  legend->Draw();
  for (int i = 2; i < 8; ++i)
    {
      h1[i-2]->Draw("SAME");
    }
  p1->Draw("SAME");
}
