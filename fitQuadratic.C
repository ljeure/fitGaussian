//Example for fitting signal/background.
// This example can be executed with:
// root > .x FittingDemo.C  (using the CINT interpreter)
// root > .x FittingDemo.C+ (using the native complier via ACLIC)
//Author: Rene Brun

#include "TH1.h"
#include "TMath.h"
#include "TF1.h"
#include "TLegend.h"
#include "TCanvas.h"

// Quadratic background function
Double_t background(Double_t *x, Double_t *par) {
  return par[3] + par[4]*x[0]; //par[2]*x[0]*x[0];
}


// Lorenzian Peak function
// par[1] and par[0] are width
// par[2] is center
Double_t gaussianPeak(Double_t *x, Double_t *p) {
  return p[0]*TMath::Exp(-0.5*((x[0]-p[1])/p[2])* ((x[0]-p[1])/p[2]));
}

Double_t linearGauss(Double_t *x, Double_t *p) {
  return background(x, p) + gaussianPeak(x, p); 
}

void demo() {
  //Bevington Exercise by Peter Malzacher, modified by Rene Brun

  TFile* f = new TFile("hMassFitPP.root");
  TH1F* hPP = (TH1F*) f->Get("h-1");

  std::vector<double> *data = new std::vector<double> (hPP->GetNbinsX());
  for (int i = 0; i < hPP->GetNbinsX(); i++) { 
    data->at(i) = hPP->GetBinContent(i + 1);
  }
  
  const int nBins = hPP->GetNbinsX();

  TCanvas *c1 = new TCanvas("c1","Fitting Demo",10,10,700,500);
  c1->SetFillColor(33);
  c1->SetFrameFillColor(41);
  c1->SetGrid();

  TH1F *histo = new TH1F("histo",
            "Gaussian Peak on Linear Background",60,0,3);
  histo->SetMarkerStyle(21);
  histo->SetMarkerSize(0.8);
  histo->SetStats(0);

  for(int i=0; i < nBins;  i++) histo->SetBinContent(i+1,data->at(i));

  // create a TF1 with the range from 0 to 3 and 6 parameters
  TF1 *fitFcn = new TF1("linearGauss",linearGauss,0,3,6);
  fitFcn->SetNpx(500);
  fitFcn->SetLineWidth(4);
  fitFcn->SetLineColor(kMagenta);

  // first try without starting values for the parameters
  // This defaults to 1 for each param.
  // this results in an ok fit for the polynomial function
  // however the non-linear part (lorenzian) does not
  // respond well.
  fitFcn->SetParameters(1,1,1,1,1,1);
  //histo->Fit("linearGauss","0");

  // second try: set start values for some parameters
  fitFcn->SetParameter(4,0.2); // width
  fitFcn->SetParameter(5,1);   // peak

  histo->Fit("linearGauss","V+","ep");

  // improve the picture:
  TF1 *backFcn = new TF1("backFcn",background,0,3,3);
  backFcn->SetLineColor(kRed);
  TF1 *signalFcn = new TF1("signalFcn",gaussianPeak,0,3,3);
  signalFcn->SetLineColor(kBlue);
  signalFcn->SetNpx(500);
  Double_t par[6];

  // writes the fit results into the par array
  fitFcn->GetParameters(par);

  backFcn->SetParameters(par);
  backFcn->Draw("same");

  signalFcn->SetParameters(&par[3]);
  signalFcn->Draw("same");

  // draw the legend
  TLegend *legend=new TLegend(0.6,0.65,0.88,0.85);
  legend->SetTextFont(72);
  legend->SetTextSize(0.04);
  legend->AddEntry(histo,"Data","lpe");
//  legend->AddEntry(backFcn,"Background fit","l");
//  legend->AddEntry(signalFcn,"Signal fit","l");
  legend->AddEntry(fitFcn,"Global Fit","l");
  legend->Draw();
}
