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
  pmt_iter = 0;
  p1_iter = 0;
  p2_iter = 0;

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

  if (pmt_fired==1) {
    pmt_Fired[pmt_iter] = pmt_fired;
    pmt_adcval[pmt_iter] = pmt_ADCVal*1.;
    pmt_nbof[pmt_iter] = pmt_NBOF*1.;
    pmt_ntimetrgbof[pmt_iter] = pmt_NTimeTrgBOF*1.;
    pmt_vbias[pmt_iter] = pmt_Vbias;//0.437*
    pmt_extemp[pmt_iter] = pmt_extTemp*200./2047.-50.;
    pmt_boardtemp[pmt_iter] = pmt_boardTemp*200./2047.-50.;
    pmt_csptemp[pmt_iter] = pmt_cspTemp*1.;
    pmt_time[pmt_iter] = (pmt_t_day*86400. + pmt_t_secday)/3600.-72.;
    pmt_iter++;
  }

  if (pin1_fired==1) {
    p1_Fired[p1_iter] = pin1_fired;
    p1_adcval[p1_iter] = pin1_ADCVal*1.;
    p1_nbof[p1_iter] = pin1_NBOF*1.;
    p1_ntimetrgbof[p1_iter] = pin2_NTimeTrgBOF*1.;
    p1_vbias[p1_iter] = pin1_Vbias;
    p1_extemp[p1_iter] = pin1_extTemp*200./2047.-50.;
    p1_boardtemp[p1_iter] = pin1_boardTemp*200./2047.-50.;
    p1_csptemp[p1_iter] = pin1_cspTemp*1.;
    p1_time[p1_iter] = (pin1_t_day*86400. + pin1_t_secday)/3600. -72.;
    p1_iter++;
  }

  if (pin2_fired==1) {
    p2_Fired[p2_iter] = pin2_fired;
    p2_adcval[p2_iter] = pin2_ADCVal*1.;
    p2_nbof[p2_iter] = pin1_NBOF*1.;
    p2_ntimetrgbof[p2_iter] = pin1_NTimeTrgBOF*1.;
    p2_vbias[p2_iter] = pin2_Vbias;
    p2_extemp[p2_iter] = pin2_extTemp*200./2047.-50.;
    p2_boardtemp[p2_iter] = pin2_boardTemp*200./2047.-50.;
    p2_csptemp[p2_iter] = pin2_cspTemp*1.;
    p2_time[p2_iter] = (pin2_t_day*86400. + pin2_t_secday)/3600. -72.;
    p2_iter++;
  }

  // cout << "execute()" << endl;
}

void NaDaqAna::finalize(string const &filename) {

  //histograms for LM analysis
  Double_t max_pmt_time = TMath::MaxElement(pmt_iter, pmt_time) +1;
  Double_t max_p1_time = TMath::MaxElement(p1_iter, p1_time) +1;
  Double_t max_p2_time = TMath::MaxElement(p2_iter, p2_time) +1;

  //--- n. of hours in period 3-15 June
  max_pmt_time = 24*(15-3+1);
  max_p1_time = 24*(15-3+1);
  max_p2_time = 24*(15-3+1);

  Double_t nbof_max_pmt = TMath::MaxElement(pmt_iter, pmt_nbof);
  Double_t nbof_max_p1 = TMath::MaxElement(p1_iter, p1_nbof);
  Double_t nbof_max_p2 = TMath::MaxElement(p2_iter, p2_nbof);

  Double_t ntimetrgbof_max_pmt = TMath::MaxElement(pmt_iter, pmt_ntimetrgbof);
  Double_t ntimetrgbof_max_p1 = TMath::MaxElement(p1_iter, p1_ntimetrgbof);
  Double_t ntimetrgbof_max_p2 = TMath::MaxElement(p2_iter, p2_ntimetrgbof);

  Double_t pmt_vbias_mean = TMath::Mean(pmt_iter, pmt_vbias);
  Double_t p1_vbias_mean = TMath::Mean(p1_iter, p1_vbias);
  Double_t p2_vbias_mean = TMath::Mean(p2_iter, p2_vbias);

  Int_t nbins=2;

  TString title;

  title = "Laser PMT value vs. time";
  prof_pmt_ls_adcval = new TProfile("prof_pmt_laser_adcval", title, max_pmt_time*4, 0, max_pmt_time);
  prof_pmt_ls_adcval->SetStats(0);

  title = "Am PMT value vs. time";
  prof_pmt_am_adcval = new TProfile("prof_pmt_am_adcval", title, max_pmt_time*4, 0, max_pmt_time);
  prof_pmt_am_adcval->SetStats(0);

  title = "Laser PMT peak time vs. time";
  h2_pmt_nbof = new TH2D("h2_pmt_nbof", title, 1000, 0, max_pmt_time,  1000, 0, nbof_max_pmt);

  title = "Histogram for pmt NTimeTrgBOF Value";
  h1_pmt_ntimetrgbof = new TH1D("h1_pmt_ntimetrgbof", title, 1000, 0, ntimetrgbof_max_pmt);

  title = "Laser PMT NTimeTrgBOF vs. time scatter plot";
  h2_pmt_ntimetrgbof = new TH2D("h2_pmt_ntimetrgbof", title, 2000, 0, max_pmt_time,  1000, 0, ntimetrgbof_max_pmt);

  title = "PMT V_bias vs. time";
  prof_pmt_vbias = new TProfile("prof_pmt_vbias", title, max_pmt_time/nbins, 0, max_pmt_time);
  prof_pmt_vbias->SetStats(0);

  title = "PMT external temperature vs. time";
  prof_pmt_extemp = new TProfile("prof_pmt_extemp", title, max_pmt_time/nbins, 0, max_pmt_time);
  prof_pmt_extemp->SetStats(0);

  title = "PMT board temperature vs. time";
  prof_pmt_boardtemp = new TProfile("prof_pmt_boardtemp", title, max_pmt_time/nbins, 0, max_pmt_time);
  prof_pmt_boardtemp->SetStats(0);

  title = "PMT preamp temperature vs. time";
  prof_pmt_csptemp = new TProfile("prof_pmt_csptemp", title, max_pmt_time/nbins, 0, max_pmt_time);
  prof_pmt_csptemp->SetStats(0);

  title = "Histogram for pmt ADC Value";
  h_pmt_adcval = new TH1D("h_pmt_adcval", title, 200, 0, 10000);

  title = "Time profile for pin1 ADC Value";
  prof_pin1_adcval = new TProfile("prof_pin1_adcval", title, max_p1_time/nbins, 0, max_p1_time);
  prof_pin1_adcval->SetStats(0);
  prof_pin1_adcval->Sumw2();

  title = "Time profile for pin1 NBOF Value";
  h2_pin1_nbof = new TH2D("h2_pin1_nbof", title, 1000, 0, max_p1_time,  1000, 0, nbof_max_p1);

  title = "Histogram for pin1 NTimeTrgBOF Value";
  h1_pin1_ntimetrgbof = new TH1D("h1_pin1_ntimetrgbof", title, 2000, 0, ntimetrgbof_max_p1);

  title = "Time profile for pin1 NTimeTrgBOF Value";
  h2_pin1_ntimetrgbof = new TH2D("h2_pin1_ntimetrgbof", title, 1000, 0, max_p1_time, 1000, 0, ntimetrgbof_max_p1);

  title = "Time profile for pin1 Vbias Value";
  prof_pin1_vbias = new TProfile("prof_pin1_vbias", title, max_p1_time/nbins, 0, max_p1_time);
  prof_pin1_vbias->SetStats(0);

  title = "Time profile for pin1 external Temperature Value";
  prof_pin1_extemp = new TProfile("prof_pin1_extemp", title, max_p1_time/nbins, 0, max_p1_time);
  prof_pin1_extemp->SetStats(0);

  title = "Time profile for pin1 board Temperature Value";
  prof_pin1_boardtemp = new TProfile("prof_pin1_boardtemp", title, max_p1_time/nbins, 0, max_p1_time);
  prof_pin1_boardtemp->SetStats(0);

  title = "Time profile for pin1 csp Temperature Value";
  prof_pin1_csptemp = new TProfile("prof_pin1_csptemp", title, max_p1_time/nbins, 0, max_p1_time);
  prof_pin1_csptemp->SetStats(0);

  title = "Histogram for pin1 ADC Value";
  h_pin1_adcval = new TH1D("h_pin1_adcval", title, 1000, 0, 2000);

  title = "Time profile for pin2 ADC Value";
  prof_pin2_adcval = new TProfile("prof_pin2_adcval", title, max_p1_time/nbins, 0, max_p2_time);
  prof_pin2_adcval->SetStats(0);
  prof_pin2_adcval->Sumw2();

  title = "Time profile for pin2 NBOF Value";
  h2_pin2_nbof = new TH2D("h2_pin2_nbof", title, 1000, 0, max_p2_time, 1000, 0, nbof_max_p2);
  h2_pin2_nbof->SetStats(0);

  title = "Histogram for pin2 NTimeTrgBOF Value";
  h1_pin2_ntimetrgbof = new TH1D("h1_pin2_ntimetrgbof", title, 1000, 0, ntimetrgbof_max_p2);

  title = "Time profile for pin2 NTimeTrgBOF Value";
  h2_pin2_ntimetrgbof = new TH2D("h2_pin2_ntimetrgbof", title, 2000, 0, max_p2_time, 1000, 0, ntimetrgbof_max_p2);
  h2_pin2_ntimetrgbof->SetStats(0);

  title = "Time profile for pin2 Vbias Value";
  prof_pin2_vbias = new TProfile("prof_pin2_vbias", title, max_p2_time/nbins, 0, max_p2_time);

  title = "Time profile for pin2 external Temperature Value";
  prof_pin2_extemp = new TProfile("prof_pin2_extemp", title, max_p2_time/nbins, 0, max_p2_time);
  prof_pin2_extemp->SetStats(0);

  title = "Time profile for pin2 board Temperature Value";
  prof_pin2_boardtemp = new TProfile("prof_pin2_boardtemp", title, max_p2_time/nbins, 0, max_p2_time);
  prof_pin2_boardtemp->SetStats(0);

  title = "Time profile for pin2 csp Temperature Value";
  prof_pin2_csptemp = new TProfile("prof_pin2_csptemp", title, max_p2_time/nbins, 0, max_p2_time);
  prof_pin2_csptemp->SetStats(0);

  title = "Time profile for pin ratio ADC Value";
  prof_pin1Dpin2_adcval = new TProfile("prof_pin1Dpin2_adcval", title, max_p2_time/nbins, 0, max_p2_time);
  prof_pin1Dpin2_adcval->SetStats(0);
  prof_pin1Dpin2_adcval->Sumw2();

  title = "Pins ratio ADC Value";
  h1_pin1Dpin2_adcval = new TH1D("h1_pin1Dpin2_adcval", title, max_p2_time/nbins, 0, max_p2_time);
  h1_pin1Dpin2_adcval->Sumw2();

  title = "Pin1 Vs Pin2 Correlation ADC Value";
  h2_pin1Vpin2_adcval = new TH2D("h2_pin1Vpin2_adcval", title, 1000, 0, 2000, 1000, 0, 2000);

  title = "Histogram for pin2 ADC Value";
  h_pin2_adcval = new TH1D("h_pin2_adcval", title, 1000, 0, 2000);

  for (int j = 0; j < pmt_iter; ++j) {
    if (pmt_adcval[j]>5300 && pmt_adcval[j]<10000)prof_pmt_ls_adcval->Fill(pmt_time[j],pmt_adcval[j]);
    if (pmt_adcval[j]>3000 && pmt_adcval[j]<5300)prof_pmt_am_adcval->Fill(pmt_time[j],pmt_adcval[j]);
    h2_pmt_nbof->Fill(pmt_time[j],pmt_nbof[j]);
    h1_pmt_ntimetrgbof->Fill(pmt_ntimetrgbof[j]/10);
    h2_pmt_ntimetrgbof->Fill(pmt_time[j],pmt_ntimetrgbof[j]);
    prof_pmt_extemp->Fill(pmt_time[j],pmt_extemp[j]);
    prof_pmt_boardtemp->Fill(pmt_time[j],pmt_boardtemp[j]);
    prof_pmt_csptemp->Fill(pmt_time[j],pmt_csptemp[j]);
    prof_pmt_vbias->Fill(pmt_time[j],pmt_vbias[j]*0.437/pmt_vbias_mean);
    h_pmt_adcval->Fill(pmt_adcval[j]);
  }

  for (int j = 0; j < p1_iter; ++j){
    if(p1_adcval[j]>920 && p1_adcval[j]<1020){
      prof_pin1_adcval->Fill(p1_time[j],p1_adcval[j]);
      h2_pin1_nbof->Fill(p1_time[j],p1_nbof[j]);
      h1_pin1_ntimetrgbof->Fill(p1_ntimetrgbof[j]/10);
      h2_pin1_ntimetrgbof->Fill(p1_time[j],p1_ntimetrgbof[j]);
      prof_pin1_extemp->Fill(p1_time[j],p1_extemp[j]);
      prof_pin1_boardtemp->Fill(p1_time[j],p1_boardtemp[j]);
      prof_pin1_csptemp->Fill(p1_time[j],p1_csptemp[j]);
      prof_pin1_vbias->Fill(p1_time[j],p1_vbias[j]*(-30.)/p1_vbias_mean);
      h_pin1_adcval->Fill(p1_adcval[j]);
    }
  }

  for (int j = 0; j < p2_iter; ++j){
    if(p2_adcval[j]>700 && p2_adcval[j]<780){
      prof_pin2_adcval->Fill(p2_time[j],p2_adcval[j]);
      h2_pin2_nbof->Fill(p2_time[j],p2_nbof[j]);
      h1_pin2_ntimetrgbof->Fill(p2_ntimetrgbof[j]/10);
      h2_pin2_ntimetrgbof->Fill(p2_time[j],p2_ntimetrgbof[j]);
      prof_pin2_extemp->Fill(p2_time[j],p2_extemp[j]);
      prof_pin2_boardtemp->Fill(p2_time[j],p2_boardtemp[j]);
      prof_pin2_csptemp->Fill(p2_time[j],p2_csptemp[j]);
      prof_pin2_vbias->Fill(p2_time[j],p2_vbias[j]*(-30.)/p2_vbias_mean);
      h_pin2_adcval->Fill(p2_adcval[j]);
    }
  }

  for (int b = 1; b <= max_p1_time/nbins; ++b) {

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
