void fit(){



TFile* f = new TFile("hMassFitPP.root");
TH1F* hPP = (TH1F*) f->Get("h-1");

std::vector<double> *v = new std::vector<double> (hPP->GetNbinsX());
for (int i = 0; i < hPP->GetNbinsX(); i++) { 
  v->at(i) = hPP->GetBinContent(i + 1);
  std::cout << "v " << i << " " << v->at(i) << std::endl;
}


hPP->Draw();
}
