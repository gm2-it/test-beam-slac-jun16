#include "NaDaqAna.h"

#include <TMath.h>
#include <TF1.h>
#include <TF2.h>
#include <TCanvas.h>

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
    execute();
  }

  finalize(filename);
}

TH1D* new_TH1D(const char* name, const char* title, Int_t nbinsx, const char* xaxis, const char* yaxis) {
  TH1D* h1 = new TH1D(name, title, nbinsx, Double_t(0), Double_t(0));
  // h1->SetStats(0);
  h1->Sumw2();
  h1->SetBit(TH1::kCanRebin);
  h1->GetXaxis()->SetTitle(xaxis);
  h1->GetYaxis()->SetTitle(yaxis);
  return(h1);
}

TProfile* new_tprofile(const char* name, const char* title, Int_t nbinsx, const char* xaxis, const char* yaxis) {
  TProfile* hprof = new TProfile(name, title, nbinsx, Double_t(0), Double_t(0));
  hprof->SetStats(0);
  // hprof->Sumw2();
  hprof->SetBit(TH1::kCanRebin);
  hprof->GetXaxis()->SetTitle(xaxis);
  hprof->GetYaxis()->SetTitle(yaxis);
  return(hprof);
}

TH2D* new_TH2D(const char* name, const char* title, Int_t nbinsx, Int_t nbinsy, const char* xaxis, const char* yaxis) {
  TH2D* h2 = new TH2D(name, title, nbinsx, Double_t(0), Double_t(0), nbinsy, Double_t(0), Double_t(0));
  h2->SetStats(0);
  h2->Sumw2();
  h2->SetBit(TH1::kCanRebin);
  h2->GetXaxis()->SetTitle(xaxis);
  h2->GetYaxis()->SetTitle(yaxis);
  return(h2);
}

void NaDaqAna::initialize(string const &filename) {
  TString title;

  title = "Laser PMT value vs. time";
  prof_pmt_ls_adcval = new_tprofile("prof_pmt_laser_adcval", title, 1000, "[days of June 2016]", "");

  title = "Am PMT value vs. time";
  prof_pmt_am_adcval = new_tprofile("prof_pmt_am_adcval", title, 1000, "[days of June 2016]", "");

  title = "PMT Value";
  h1_pmt_adcval = new_TH1D("h1_pmt_adcval", title, 400, "", "");

  title = "PMT peak time vs. time";
  h2_pmt_nbof = new_TH2D("h2_pmt_nbof", title, 1000, 1000, "[days of June 2016]", "");

  title = "PMT NTimeTrgBOF";
  h1_pmt_ntimetrgbof = new_TH1D("h1_pmt_ntimetrgbof", title, 1000, "", "");

  title = "PMT NTimeTrgBOF vs. time scatter plot";
  h2_pmt_ntimetrgbof = new_TH2D("h2_pmt_ntimetrgbof", title, 1000, 1000, "[days of June 2016]", "");

  title = "PMT V_bias vs. time";
  prof_pmt_vbias = new_tprofile("prof_pmt_vbias", title, 1000, "[days of June 2016]", "");

  title = "PMT external temperature vs. time";
  prof_pmt_extemp = new_tprofile("prof_pmt_extemp", title, 1000, "[days of June 2016]", "");

  title = "PMT board temperature vs. time";
  prof_pmt_boardtemp = new_tprofile("prof_pmt_boardtemp", title, 1000, "[days of June 2016]", "");

  title = "PMT preamp temperature vs. time";
  prof_pmt_csptemp = new_tprofile("prof_pmt_csptemp", title, 1000, "[days of June 2016]", "");

  title = "Pin1 Value vs. time";
  prof_pin1_adcval = new_tprofile("prof_pin1_adcval", title, 1000, "[days of June 2016]", "");

  title = "Pin1 Value";
  h1_pin1_adcval = new TH1D("h1_pin1_adcval", title, 400, 0, 2000);
  h1_pin1_adcval->Sumw2();

  title = "Pin1 NBOF vs. time";
  h2_pin1_nbof = new_TH2D("h2_pin1_nbof", title, 1000, 1000, "[days of June 2016]", "");

  title = "Pin1 NTimeTrgBOF";
  h1_pin1_ntimetrgbof = new_TH1D("h1_pin1_ntimetrgbof", title, 100, "", "");

  title = "Pin1 NTimeTrgBOF vs. time";
  h2_pin1_ntimetrgbof = new_TH2D("h2_pin1_ntimetrgbof", title, 1000, 1000, "[days of June 2016]", "");

  title = "Pin1 Vbias vs. time";
  prof_pin1_vbias = new_tprofile("prof_pin1_vbias", title, 1000, "[days of June 2016]", "");

  title = "Pin1 external Temperature vs. time";
  prof_pin1_extemp = new_tprofile("prof_pin1_extemp", title, 1000, "[days of June 2016]", "");

  title = "Pin1 board Temperature vs. time";
  prof_pin1_boardtemp = new_tprofile("prof_pin1_boardtemp", title, 1000, "[days of June 2016]", "");

  title = "Pin1 preamp Temperature vs. time";
  prof_pin1_csptemp = new_tprofile("prof_pin1_csptemp", title, 1000, "[days of June 2016]", "");

  title = "Pin2 Value vs. time";
  prof_pin2_adcval = new_tprofile("prof_pin2_adcval", title, 1000, "[days of June 2016]", "");

  title = "Pin2 Value";
  h1_pin2_adcval = new TH1D("h1_pin2_adcval", title, 400, 0, 2000);
  h1_pin2_adcval->Sumw2();

  title = "Pin2 NBOF vs. time";
  h2_pin2_nbof = new_TH2D("h2_pin2_nbof", title, 1000, 1000, "[days of June 2016]", "");

  title = "Pin2 NTimeTrgBOF";
  h1_pin2_ntimetrgbof = new_TH1D("h1_pin2_ntimetrgbof", title, 100, "", "");

  title = "Pin2 NTimeTrgBOF vs. time";
  h2_pin2_ntimetrgbof = new_TH2D("h2_pin2_ntimetrgbof", title, 1000, 1000, "[days of June 2016]", "");

  title = "Pin2 Vbias vs. time";
  prof_pin2_vbias = new_tprofile("prof_pin2_vbias", title, 1000, "[days of June 2016]", "");

  title = "Pin2 external Temperature vs. time";
  prof_pin2_extemp = new_tprofile("prof_pin2_extemp", title, 1000, "[days of June 2016]", "");

  title = "Pin2 board Temperature vs. time";
  prof_pin2_boardtemp = new_tprofile("prof_pin2_boardtemp", title, 1000, "[days of June 2016]", "");

  title = "Pin2 preamp Temperature vs. time";
  prof_pin2_csptemp = new_tprofile("prof_pin2_csptemp", title, 1000, "[days of June 2016]", "");

  title = "Pin1/Pin2 ratio vs. time";
  prof_ratio_pin1_pin2_adcval = new_tprofile("prof_ratio_pin1_pin2_adcval", title, 1000, "[days of June 2016]", "");

  title = "Pin1/Pin2 ratio";
  h1_ratio_pin1_pin2_adcval = new TH1D("h1_ratio_pin1_pin2_adcval", title, 400, 0, 4);
  h1_ratio_pin1_pin2_adcval->Sumw2();

  title = "Pin1 Vs Pin2 Correlation";
  h2_pin1_vs_pin2_adcval = new TH2D("h2_pin1_vs_pin2_adcval", title, 400, 0, 2000, 400, 0, 2000);

  pin1_last_valid = false;
  pin2_last_valid = false;

  cout << "initialize()" << endl;
}

void NaDaqAna::execute() {

  if (pmt_fired==1) {
    Double_t pmt_time(pmt_t_day + Double_t(pmt_t_secday)/(60*60*24));

    if (pmt_ADCVal>5300 && pmt_ADCVal<10000) prof_pmt_ls_adcval->Fill(pmt_time, pmt_ADCVal);
    if (pmt_ADCVal>3000 && pmt_ADCVal<5300)  prof_pmt_am_adcval->Fill(pmt_time, pmt_ADCVal);

    h2_pmt_nbof->Fill(pmt_time, pmt_NBOF);
    h1_pmt_ntimetrgbof->Fill(pmt_NTimeTrgBOF);
    h2_pmt_ntimetrgbof->Fill(pmt_time, pmt_NTimeTrgBOF);
    prof_pmt_extemp->Fill(pmt_time, pmt_extTemp*200./2047. -50.);
    prof_pmt_boardtemp->Fill(pmt_time, pmt_boardTemp*200./2047. -50.);
    prof_pmt_csptemp->Fill(pmt_time, pmt_cspTemp*200./2047. -50.);
    // was pmt_vbias[j]*0.437/pmt_vbias_mean
    prof_pmt_vbias->Fill(pmt_time, pmt_Vbias);
    h1_pmt_adcval->Fill(pmt_ADCVal);
  }

  if (pin1_fired==1) {
    Double_t pin1_time(pin1_t_day + Double_t(pin1_t_secday)/(60*60*24));

    prof_pin1_adcval->Fill(pin1_time, pin1_ADCVal);
    h2_pin1_nbof->Fill(pin1_time, pin1_NBOF);
    h1_pin1_ntimetrgbof->Fill(pin1_NTimeTrgBOF);
    h2_pin1_ntimetrgbof->Fill(pin1_time, pin1_NTimeTrgBOF);
    prof_pin1_extemp->Fill(pin1_time, pin1_extTemp*200./2047. -50.);
    prof_pin1_boardtemp->Fill(pin1_time, pin1_boardTemp*200./2047. -50.);
    prof_pin1_csptemp->Fill(pin1_time, pin1_cspTemp*200./2047. -50.);
    // was p1_vbias[j]*(-30.)/p1_vbias_mean
    prof_pin1_vbias->Fill(pin1_time, pin1_Vbias);
    h1_pin1_adcval->Fill(pin1_ADCVal);

    pin1_last_valid = true;
    pin1_adcval_last = pin1_ADCVal;
    pin1_time_last = pin1_time;
  }

  if (pin2_fired==1) {
    Double_t pin2_time(pin2_t_day + Double_t(pin2_t_secday)/(60*60*24));

    prof_pin2_adcval->Fill(pin2_time, pin2_ADCVal);
    h2_pin2_nbof->Fill(pin2_time, pin2_NBOF);
    h1_pin2_ntimetrgbof->Fill(pin2_NTimeTrgBOF);
    h2_pin2_ntimetrgbof->Fill(pin2_time, pin2_NTimeTrgBOF);
    prof_pin2_extemp->Fill(pin2_time, pin2_extTemp*200./2047. -50.);
    prof_pin2_boardtemp->Fill(pin2_time, pin2_boardTemp*200./2047. -50.);
    prof_pin2_csptemp->Fill(pin2_time, pin2_cspTemp*200./2047. -50.);
    // was p1_vbias[j]*(-30.)/p1_vbias_mean
    prof_pin2_vbias->Fill(pin2_time, pin2_Vbias);
    h1_pin2_adcval->Fill(pin2_ADCVal);

    pin2_last_valid = true;
    pin2_adcval_last = pin2_ADCVal;
    pin2_time_last = pin2_time;
  }

  if (pin1_last_valid && pin2_last_valid) {
    prof_ratio_pin1_pin2_adcval-> Fill( (pin1_time_last+pin2_time_last)/2, pin1_adcval_last/pin2_adcval_last);
    h1_ratio_pin1_pin2_adcval-> Fill( pin1_adcval_last/pin2_adcval_last);
    h2_pin1_vs_pin2_adcval-> Fill( pin1_adcval_last, pin2_adcval_last);
    pin1_last_valid = false;
    pin2_last_valid = false;
  }

  // cout << "execute()" << endl;
}

void NaDaqAna::finalize(string const &filename) {

  prof_ratio_pin1_pin2_adcval->SetMaximum(4);
  
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

  delete prof_ratio_pin1_pin2_adcval;
  delete h1_ratio_pin1_pin2_adcval;
  delete h2_pin1_vs_pin2_adcval;

  delete h2_pmt_nbof;
  delete h2_pin1_nbof;
  delete h2_pin2_nbof;

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

  delete h1_pmt_adcval;
  delete h1_pin1_adcval;
  delete h1_pin2_adcval;

  cout << "delete()" << endl;
}
