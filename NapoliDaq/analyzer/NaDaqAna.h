#ifndef NaDaqAna_h
#define NaDaqAna_h

#include "NaDaqNtuple.h"

#include <stdio.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TGraph.h>
#include <TProfile.h>

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

class NaDaqAna : public NaDaqNtuple {
 public:
  NaDaqAna(TTree *tree=0):NaDaqNtuple(tree){};
  ~NaDaqAna() {};
  virtual void Loop(string const &filename);
  void initialize(string const &filename);
  void execute();
  void finalize(string const &filename);
  void useDelete();

private:
  Long64_t nentries;

  Bool_t pmt_ls_last_valid;
  Double_t pmt_ls_adcval_last;
  Double_t pmt_ls_time_last;

  Bool_t pmt_am_last_valid;
  Double_t pmt_am_adcval_last;
  Double_t pmt_am_time_last;

  Bool_t pin1_last_valid;
  Double_t pin1_adcval_last;
  Double_t pin1_time_last;

  Bool_t pin2_last_valid;
  Double_t pin2_adcval_last;
  Double_t pin2_time_last;

  // histograms for analysis
  TProfile *prof_pmt_ls_adcval;
  TProfile *prof_pmt_am_adcval;
  TProfile *prof_pmt_ls_am_ratio;
  TProfile *prof_pin1_adcval;
  TProfile *prof_pin2_adcval;
  TProfile *prof_ratio_pin1_pin2_adcval;

  TH1D *h1_pmt_adcval;
  TH1D *h1_pin1_adcval;
  TH1D *h1_pin2_adcval;
  TH1D *h1_ratio_pin1_pin2_adcval;
  TH2D *h2_pin1_vs_pin2_adcval;

  TProfile *prof_pmt_vbias;
  TProfile *prof_pin1_vbias;
  TProfile *prof_pin2_vbias;

  TProfile *prof_pmt_extemp;
  TProfile *prof_pin1_extemp;
  TProfile *prof_pin2_extemp;

  TProfile *prof_pmt_boardtemp;
  TProfile *prof_pin1_boardtemp;
  TProfile *prof_pin2_boardtemp;

  TProfile *prof_pmt_csptemp;
  TProfile *prof_pin1_csptemp;
  TProfile *prof_pin2_csptemp;

  TH2D *h2_pmt_nbof;
  TH2D *h2_pin1_nbof;
  TH2D *h2_pin2_nbof;

  TH2D *h2_pmt_ntimetrgbof;
  TH2D *h2_pin1_ntimetrgbof;
  TH2D *h2_pin2_ntimetrgbof;

  TH1D *h1_pmt_ntimetrgbof;
  TH1D *h1_pin1_ntimetrgbof;
  TH1D *h1_pin2_ntimetrgbof;
};
#endif
