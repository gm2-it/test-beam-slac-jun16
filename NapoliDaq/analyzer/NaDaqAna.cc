#include "NaDaqAna.h"
#include <TMath.h>
#include <TF1.h>
#include <TF2.h>
#include <TCanvas.h>

#include <iostream>

void NaDaqAna::Loop(string const &filename) {
  cout << "Loop()" << endl;
  if (fChain == 0) return;
  
  // nentries = fChain->GetEntriesFast();
  nentries = fChain->GetEntries();
  cout << "files contain " << nentries << " entries" << endl;
  
  initialize(filename);

  Int_t nbytes = 0, nb = 0;
  for (Long64_t jentry=0; jentry < nentries; jentry++) {
    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;
    nb = fChain->GetEntry(jentry); nbytes += nb;
    clear();
    execute();
  }

  finalize(filename);
}

void NaDaqAna::initialize(string const &filename) {
  iter = 0;

  pmt_Fired = new Double_t[nentries];
  p1_Fired = new Double_t[nentries];
  p2_Fired = new Double_t[nentries];
	
  pmt_adcval = new Double_t[nentries];
  p1_adcval = new Double_t[nentries];
  p2_adcval = new Double_t[nentries];

  pmt_nbof = new Double_t[nentries];
  p1_nbof = new Double_t[nentries];
  p2_nbof = new Double_t[nentries];

  pmt_ntimetrgbof = new Double_t[nentries];
  p1_ntimetrgbof = new Double_t[nentries];
  p2_ntimetrgbof = new Double_t[nentries];

  pmt_vbias = new Double_t[nentries];
  p1_vbias = new Double_t[nentries];
  p2_vbias = new Double_t[nentries];

  pmt_extemp = new Double_t[nentries];
  p1_extemp = new Double_t[nentries];
  p2_extemp = new Double_t[nentries];

  pmt_csptemp = new Double_t[nentries];
  p1_csptemp = new Double_t[nentries];
  p2_csptemp = new Double_t[nentries];

  pmt_boardtemp = new Double_t[nentries];
  p1_boardtemp = new Double_t[nentries];
  p2_boardtemp = new Double_t[nentries];

  pmt_time = new Double_t[nentries];
  p1_time = new Double_t[nentries];
  p2_time = new Double_t[nentries];

  cout << "initialize()" << endl;
}

void NaDaqAna::execute() {

  pmt_Fired[iter] = pmt_fired;
  p1_Fired[iter] = pin1_fired;
  p2_Fired[iter] = pin2_fired;
  
  pmt_adcval[iter] = pmt_ADCVal*1.;
  p1_adcval[iter] = pin1_ADCVal*1.;
  p2_adcval[iter] = pin2_ADCVal*1.;

  pmt_nbof[iter] = pmt_NBOF*1.;
  p1_nbof[iter] = pin1_NBOF*1.;
  p2_nbof[iter] = pin1_NBOF*1.;

  pmt_ntimetrgbof[iter] = pmt_NTimeTrgBOF*1.;
  p2_ntimetrgbof[iter] = pin1_NTimeTrgBOF*1.;
  p1_ntimetrgbof[iter] = pin2_NTimeTrgBOF*1.;

  pmt_vbias[iter] = pmt_Vbias; //0.437*
  p1_vbias[iter] = pin1_Vbias;
  p2_vbias[iter] = pin2_Vbias;

  pmt_extemp[iter] = pmt_extTemp*200./2047.-50.;
  p1_extemp[iter] = pin1_extTemp*200./2047.-50.;
  p2_extemp[iter] = pin2_extTemp*200./2047.-50.;

  pmt_boardtemp[iter] = pmt_boardTemp*200./2047.-50.;
  p1_boardtemp[iter] = pin1_boardTemp*200./2047.-50.;
  p2_boardtemp[iter] = pin2_boardTemp*200./2047.-50.;

  pmt_csptemp[iter] = pmt_cspTemp*1.;
  p1_csptemp[iter] = pin1_cspTemp*1.;
  p2_csptemp[iter] = pin2_cspTemp*1.;

  pmt_time[iter] = (pmt_t_day*86400. + pmt_t_secday)/3600.-72.;
  p1_time[iter] = (pin1_t_day*86400. + pin1_t_secday)/3600. -72.;
  p2_time[iter] = (pin2_t_day*86400. + pin2_t_secday)/3600. -72.;

  // pmt_time[iter] = (pmt_t_day*86400. + pmt_t_secday)/3600.-pmt_t_day*24.;
  // p1_time[iter] = (pin1_t_day*86400. + pin1_t_secday)/3600.-pin1_t_day*24.;
  // p2_time[iter] = (pin2_t_day*86400. + pin2_t_secday)/3600.-pin2_t_day*24.;

  // pmt_time[iter] = pmt_t_secday;
  // p1_time[iter] =  pin1_t_secday;
  // p2_time[iter] =  pin2_t_secday;
	
  iter++;

  //cout << "execute()" << endl;
}

void NaDaqAna::finalize(string const &filename) {

  //histograms for LM analysis
  Double_t t_pmt = TMath::MaxElement(iter, pmt_time) +1;
  Double_t t_p1 = TMath::MaxElement(iter, p1_time) +1;
  Double_t t_p2 = TMath::MaxElement(iter, p2_time) +1;

  Double_t nbof_max_pmt = TMath::MaxElement(iter, pmt_nbof);
  Double_t nbof_max_p1 = TMath::MaxElement(iter, p1_nbof);
  Double_t nbof_max_p2 = TMath::MaxElement(iter, p2_nbof);

  Double_t ntimetrgbof_max_pmt = TMath::MaxElement(iter, pmt_ntimetrgbof);
  Double_t ntimetrgbof_max_p1 = TMath::MaxElement(iter, p1_ntimetrgbof);
  Double_t ntimetrgbof_max_p2 = TMath::MaxElement(iter, p2_ntimetrgbof);

  Double_t pmt_vbias_mean = TMath::Mean(iter, pmt_vbias);
  Double_t p1_vbias_mean = TMath::Mean(iter, p1_vbias);
  Double_t p2_vbias_mean = TMath::Mean(iter, p2_vbias);

  Int_t nbins=2;
  
  TString title;
  
  title = "Time profile for pmt: laser ADC Value file";
  prof_pmt_ls_adcval = new TProfile("prof_pmt_ls_adcval", title, t_pmt/nbins, 0, t_pmt);

  title = "Time profile for pmt: Americium ADC Value file";
  prof_pmt_am_adcval = new TProfile("prof_pmt_am_adcval", title, t_pmt/nbins, 0, t_pmt);

  title = "Time profile for pmt NBOF Value file";
  h2_pmt_nbof = new TH2D("h2_pmt_nbof", title, 1000, 0, t_pmt,  1000, 0, nbof_max_pmt);

  title = "Histogram for pmt NTimeTrgBOF Value file";
  h1_pmt_ntimetrgbof = new TH1D("h1_pmt_ntimetrgbof", title, 1000, 0, ntimetrgbof_max_pmt);

  title = "Time profile for pmt NTimeTrgBOF Value file";
  h2_pmt_ntimetrgbof = new TH2D("h2_pmt_ntimetrgbof", title, 2000, 0, t_pmt,  1000, 0, ntimetrgbof_max_pmt);

  title = "Time profile for pmt Vbias Value file";
  prof_pmt_vbias = new TProfile("prof_pmt_vbias", title, t_pmt/nbins, 0, t_pmt);

  title = "Time profile for pmt external Temperature Value file";
  prof_pmt_extemp = new TProfile("prof_pmt_extemp", title, t_pmt/nbins, 0, t_pmt);

  title = "Time profile for pmt board Temperature Value file";
  prof_pmt_boardtemp = new TProfile("prof_pmt_boardtemp", title, t_pmt/nbins, 0, t_pmt);

  title = "Time profile for pmt csp Temperature Value file";
  prof_pmt_csptemp = new TProfile("prof_pmt_csptemp", title, t_pmt/nbins, 0, t_pmt);

  title = "Histogram for pmt ADC Value file";
  h_pmt_adcval = new TH1D("h_pmt_adcval", title, 200, 0, 10000);

  title = "Time profile for pin1 ADC Value file";
  prof_pin1_adcval = new TProfile("prof_pin1_adcval", title, t_p1/nbins, 0, t_p1);
  prof_pin1_adcval->Sumw2();

  title = "Time profile for pin1 NBOF Value file";
  h2_pin1_nbof = new TH2D("h2_pin1_nbof", title, 1000, 0, t_p1,  1000, 0, nbof_max_p1);

  title = "Histogram for pin1 NTimeTrgBOF Value file";
  h1_pin1_ntimetrgbof = new TH1D("h1_pin1_ntimetrgbof", title, 2000, 0, ntimetrgbof_max_p1);

  title = "Time profile for pin1 NTimeTrgBOF Value file";
  h2_pin1_ntimetrgbof = new TH2D("h2_pin1_ntimetrgbof", title, 1000, 0, t_p1, 1000, 0, ntimetrgbof_max_p1);

  title = "Time profile for pin1 Vbias Value file";
  prof_pin1_vbias = new TProfile("prof_pin1_vbias", title, t_p1/nbins, 0, t_p1);

  title = "Time profile for pin1 external Temperature Value file";
  prof_pin1_extemp = new TProfile("prof_pin1_extemp", title, t_p1/nbins, 0, t_p1);

  title = "Time profile for pin1 board Temperature Value file";
  prof_pin1_boardtemp = new TProfile("prof_pin1_boardtemp", title, t_p1/nbins, 0, t_p1);

  title = "Time profile for pin1 csp Temperature Value file";
  prof_pin1_csptemp = new TProfile("prof_pin1_csptemp", title, t_p1/nbins, 0, t_p1);

  title = "Histogram for pin1 ADC Value file";
  h_pin1_adcval = new TH1D("h_pin1_adcval", title, 1000, 0, 2000);

  title = "Time profile for pin2 ADC Value file";
  prof_pin2_adcval = new TProfile("prof_pin2_adcval", title, t_p1/nbins, 0, t_p2);
  prof_pin2_adcval->Sumw2();

  title = "Time profile for pin2 NBOF Value file";
  h2_pin2_nbof = new TH2D("h2_pin2_nbof", title, 1000, 0, t_p2, 1000, 0, nbof_max_p2);

  title = "Histogram for pin2 NTimeTrgBOF Value file";
  h1_pin2_ntimetrgbof = new TH1D("h1_pin2_ntimetrgbof", title, 1000, 0, ntimetrgbof_max_p2);

  title = "Time profile for pin2 NTimeTrgBOF Value file";
  h2_pin2_ntimetrgbof = new TH2D("h2_pin2_ntimetrgbof", title, 2000, 0, t_p2, 1000, 0, ntimetrgbof_max_p2);

  title = "Time profile for pin2 Vbias Value file";
  prof_pin2_vbias = new TProfile("prof_pin2_vbias", title, t_p2/nbins, 0, t_p2);

  title = "Time profile for pin2 external Temperature Value file";
  prof_pin2_extemp = new TProfile("prof_pin2_extemp", title, t_p2/nbins, 0, t_p2);

  title = "Time profile for pin2 board Temperature Value file";
  prof_pin2_boardtemp = new TProfile("prof_pin2_boardtemp", title, t_p2/nbins, 0, t_p2);

  title = "Time profile for pin2 csp Temperature Value file";
  prof_pin2_csptemp = new TProfile("prof_pin2_csptemp", title, t_p2/nbins, 0, t_p2);

  title = "Time profile for pin ratio ADC Value file";
  prof_pin1Dpin2_adcval = new TProfile("prof_pin1Dpin2_adcval", title, t_p2/nbins, 0, t_p2);
  prof_pin1Dpin2_adcval->Sumw2();

  title = "Pins ratio ADC Value file";
  h1_pin1Dpin2_adcval = new TH1D("h1_pin1Dpin2_adcval", title, t_p2/nbins, 0, t_p2);
  h1_pin1Dpin2_adcval->Sumw2();

  title = "Pin1 Vs Pin2 Correlation ADC Value file";
  h2_pin1Vpin2_adcval = new TH2D("h2_pin1Vpin2_adcval", title, 1000, 0, 2000, 1000, 0, 2000);
  
  title = "Histogram for pin2 ADC Value file";
  h_pin2_adcval = new TH1D("h_pin2_adcval", title, 1000, 0, 2000);
  
  for (int j = 0; j < iter; ++j) {
    
    //if (pmt_time[j] > 7 && pmt_time[j] < 14 && p1_time[j] > 7 && p1_time[j] <14 && p2_time[j] > 7 && p2_time[j] <14) {
    if (pmt_adcval[j]>5300 && pmt_adcval[j]<10000) prof_pmt_ls_adcval->Fill(pmt_time[j], pmt_adcval[j]);
    if (pmt_adcval[j]>3000 && pmt_adcval[j]<5300) prof_pmt_am_adcval->Fill(pmt_time[j], pmt_adcval[j]);
    if (p1_Fired[j]>0&&p1_adcval[j]>920&&p1_adcval[j]<1020) prof_pin1_adcval->Fill(p1_time[j], p1_adcval[j]);
    if (p2_Fired[j]>0&&p2_adcval[j]>700&&p2_adcval[j]<780) prof_pin2_adcval->Fill(p2_time[j], p2_adcval[j]);
    //if (p2_adcval[j]>0&&p1_adcval[j]>0) prof_pin1Dpin2_adcval->Fill(p2_time[j], p1_adcval[j]/p2_adcval[j]);

    if (pmt_Fired[j]>0) h2_pmt_nbof->Fill(pmt_time[j], pmt_nbof[j]);
    if (p1_Fired[j]>0&&p1_adcval[j]>920&&p1_adcval[j]<1020) h2_pin1_nbof->Fill(p1_time[j], p1_nbof[j]);
    if (p2_Fired[j]>0&&p2_adcval[j]>700&&p2_adcval[j]<780) h2_pin2_nbof->Fill(p2_time[j], p2_nbof[j]);
    //h2_pin1Vpin2_adcval->Fill(p1_adcval[j], p2_adcval[j]);

    if (pmt_Fired[j]>0) h1_pmt_ntimetrgbof->Fill(pmt_ntimetrgbof[j]/10);
    if (p1_Fired[j]>0&&p1_adcval[j]>920&&p1_adcval[j]<1020) h1_pin1_ntimetrgbof->Fill(p1_ntimetrgbof[j]/10);
    if (p2_Fired[j]>0&&p2_adcval[j]>700&&p2_adcval[j]<780) h1_pin2_ntimetrgbof->Fill(p2_ntimetrgbof[j]/10);

    if (pmt_Fired[j]>0) h2_pmt_ntimetrgbof->Fill(pmt_time[j], pmt_ntimetrgbof[j]);
    if (p1_Fired[j]>0&&p1_adcval[j]>920&&p1_adcval[j]<1020) h2_pin1_ntimetrgbof->Fill(p1_time[j], p1_ntimetrgbof[j]);
    if (p2_Fired[j]>0&&p2_adcval[j]>700&&p2_adcval[j]<780) h2_pin2_ntimetrgbof->Fill(p2_time[j], p2_ntimetrgbof[j]);

    if (pmt_Fired[j]>0) prof_pmt_extemp->Fill(pmt_time[j], pmt_extemp[j]);
    if (p1_Fired[j]>0&&p1_adcval[j]>920&&p1_adcval[j]<1020) prof_pin1_extemp->Fill(p1_time[j], p1_extemp[j]);
    if (p2_Fired[j]>0&&p2_adcval[j]>700&&p2_adcval[j]<780) prof_pin2_extemp->Fill(p2_time[j], p2_extemp[j]);

    if (pmt_Fired[j]>0) prof_pmt_boardtemp->Fill(pmt_time[j], pmt_boardtemp[j]);
    if (p1_Fired[j]>0&&p1_adcval[j]>920&&p1_adcval[j]<1020) prof_pin1_boardtemp->Fill(p1_time[j], p1_boardtemp[j]);
    if (p2_Fired[j]>0&&p2_adcval[j]>700&&p2_adcval[j]<780) prof_pin2_boardtemp->Fill(p2_time[j], p2_boardtemp[j]);

    if (pmt_Fired[j]>0) prof_pmt_csptemp->Fill(pmt_time[j], pmt_csptemp[j]);
    if (p1_Fired[j]>0&&p1_adcval[j]>920&&p1_adcval[j]<1020) prof_pin1_csptemp->Fill(p1_time[j], p1_csptemp[j]);
    if (p2_Fired[j]>0&&p2_adcval[j]>700&&p2_adcval[j]<780) prof_pin2_csptemp->Fill(p2_time[j], p2_csptemp[j]);

    if (pmt_Fired[j]>0) prof_pmt_vbias->Fill(pmt_time[j], pmt_vbias[j]*0.437/pmt_vbias_mean);
    if (p1_Fired[j]>0&&p1_adcval[j]>920&&p1_adcval[j]<1020) prof_pin1_vbias->Fill(p1_time[j], p1_vbias[j]*(-30.)/p1_vbias_mean);
    if (p2_Fired[j]>0&&p2_adcval[j]>700&&p2_adcval[j]<780) prof_pin2_vbias->Fill(p2_time[j], p2_vbias[j]*(-30.)/p2_vbias_mean);

    if (pmt_Fired[j]>0) h_pmt_adcval->Fill(pmt_adcval[j]);
    if (p1_Fired[j]>0&&p1_adcval[j]>920&&p1_adcval[j]<1020) h_pin1_adcval->Fill(p1_adcval[j]);
    if (p2_Fired[j]>0&&p2_adcval[j]>700&&p2_adcval[j]<780) h_pin2_adcval->Fill(p2_adcval[j]);
  }

  for (int b = 1; b <= t_p1/nbins; ++b) {
    
    if (prof_pin1_adcval->GetBinContent(b)!=0) {
      
      h1_pin1Dpin2_adcval->SetBinContent(b, prof_pin1_adcval->GetBinContent(b)/prof_pin2_adcval->GetBinContent(b));
      
      h1_pin1Dpin2_adcval->SetBinError(b,
        sqrt(pow(prof_pin1_adcval->GetBinError(b)/prof_pin1_adcval->GetBinContent(b), 2)+
	     pow(prof_pin2_adcval->GetBinError(b)/prof_pin2_adcval->GetBinContent(b), 2))*			               			      (prof_pin1_adcval->GetBinContent(b)/prof_pin2_adcval->GetBinContent(b)));

    } else {
      
      h1_pin1Dpin2_adcval->SetBinContent(b, 0);
      h1_pin1Dpin2_adcval->SetBinError(b, 0);
      
    }
  }

  cout << "create  file " << filename << endl;
  TFile *OutF = new TFile(filename.c_str(), "recreate");
  OutF->cd();
  gROOT->GetList()->Write();
  OutF->Close();
  cout << "written file " << filename << endl;  
  delete OutF;
  useDelete();

  cout << "finalize()" << endl;
}

void NaDaqAna::useDelete() {

  delete prof_pmt_ls_adcval;
  delete prof_pmt_am_adcval;
  delete prof_pin1_adcval;
  delete prof_pin2_adcval;
  delete prof_pin1Dpin2_adcval;

  delete h2_pmt_nbof;
  delete h2_pin1_nbof;
  delete h2_pin2_nbof;
  delete h2_pin1Vpin2_adcval;
  
  delete h1_pin1Dpin2_adcval;

  delete h1_pmt_ntimetrgbof;
  delete h1_pin1_ntimetrgbof;
  delete h1_pin2_ntimetrgbof;

  delete h2_pmt_ntimetrgbof;
  delete h2_pin1_ntimetrgbof;
  delete h2_pin2_ntimetrgbof;

  delete prof_pmt_vbias;
  delete prof_pin1_vbias;
  delete prof_pin2_vbias;

  delete prof_pmt_extemp;
  delete prof_pin1_extemp;
  delete prof_pin2_extemp;

  delete prof_pmt_boardtemp;
  delete prof_pin1_boardtemp;
  delete prof_pin2_boardtemp;

  delete prof_pmt_csptemp;
  delete prof_pin1_csptemp;
  delete prof_pin2_csptemp;

  delete h_pmt_adcval;
  delete h_pin1_adcval;
  delete h_pin2_adcval;

  cout << "delete()" << endl;
}

void NaDaqAna::clear() {
  //cout << "clear()" << endl;
}
