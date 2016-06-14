#include <iostream>
#include <string>
#include "TFile.h"
#include "TTree.h"
#include "TMath.h"
#include "Riostream.h"
#include <vector>
#include "TGraphErrors.h"
#include "TNtupleD.h"
#include "TNtuple.h"
// per gStyle ...
#include <TCanvas.h>
#include <TStyle.h>
#include <TProfile.h>



void AnalyzerMB( Int_t NBINS=10){


  Double_t FC7_clockcounter;
  Double_t pmt_amplitude;
  Double_t pin1_amplitude;
  Double_t pin2_amplitude;
  

 TChain * data = new TChain("anatree");
 data->Add("out/analyzed_gm2slac_run02122.root");
 data->Add("out/analyzed_gm2slac_run02123.root");
 data->Add("out/analyzed_gm2slac_run02124.root");
 data->Add("out/analyzed_gm2slac_run02125.root");
 data->Add("out/analyzed_gm2slac_run02126.root");
 data->Add("out/analyzed_gm2slac_run02127.root");
 data->Add("out/analyzed_gm2slac_run02128.root");
 data->Add("out/analyzed_gm2slac_run02129.root");
 data->Add("out/analyzed_gm2slac_run02130.root");
 data->Add("out/analyzed_gm2slac_run02131.root");
 data->Add("out/analyzed_gm2slac_run02132.root");
 data->Add("out/analyzed_gm2slac_run02133.root");
 data->Add("out/analyzed_gm2slac_run02134.root");
 data->Add("out/analyzed_gm2slac_run02135.root");
 data->Add("out/analyzed_gm2slac_run02136.root");
 data->Add("out/analyzed_gm2slac_run02137.root");
 data->Add("out/analyzed_gm2slac_run02138.root");
 data->Add("out/analyzed_gm2slac_run02139.root");
 data->Add("out/analyzed_gm2slac_run02140.root");
 data->Add("out/analyzed_gm2slac_run02141.root");
 data->Add("out/analyzed_gm2slac_run02142.root");
 data->Add("out/analyzed_gm2slac_run02143.root");
 data->Add("out/analyzed_gm2slac_run02144.root");
 data->Add("out/analyzed_gm2slac_run02145.root");
 data->Add("out/analyzed_gm2slac_run02146.root");
 data->Add("out/analyzed_gm2slac_run02147.root");
 data->Add("out/analyzed_gm2slac_run02148.root");
 data->Add("out/analyzed_gm2slac_run02149.root");
 data->Add("out/analyzed_gm2slac_run02150.root");
 data->Add("out/analyzed_gm2slac_run02151.root");
 data->Add("out/analyzed_gm2slac_run02152.root");
 data->Add("out/analyzed_gm2slac_run02153.root");
 data->Add("out/analyzed_gm2slac_run02154.root");
 data->Add("out/analyzed_gm2slac_run02155.root");
 data->Add("out/analyzed_gm2slac_run02156.root");
 data->Add("out/analyzed_gm2slac_run02157.root");
 data->Add("out/analyzed_gm2slac_run02158.root");
 data->Add("out/analyzed_gm2slac_run02159.root");
 data->Add("out/analyzed_gm2slac_run02160.root");
 data->Add("out/analyzed_gm2slac_run02161.root");
 data->Add("out/analyzed_gm2slac_run02162.root");
 data->Add("out/analyzed_gm2slac_run02163.root");
 data->Add("out/analyzed_gm2slac_run02164.root");
 data->Add("out/analyzed_gm2slac_run02165.root");
 data->Add("out/analyzed_gm2slac_run02166.root");
 data->Add("out/analyzed_gm2slac_run02167.root");
 data->Add("out/analyzed_gm2slac_run02168.root");
 data->Add("out/analyzed_gm2slac_run02169.root");
 data->Add("out/analyzed_gm2slac_run02170.root");
 data->Add("out/analyzed_gm2slac_run02171.root");


 data->SetBranchAddress("FC7_clockcounter", &FC7_clockcounter);
 data->SetBranchAddress("pmt_amplitude",&pmt_amplitude);
 data->SetBranchAddress("pin1_amplitude",&pin1_amplitude);
 data->SetBranchAddress("pin2_amplitude",&pin2_amplitude);


   

 Int_t nev=data->GetEntries();
 cout << "entries "<< data->GetEntries() << endl;

 data->GetEntry(0);
 Double_t FC7_clockcounter0 =FC7_clockcounter;
 data->GetEntry(nev-1);
 Double_t FC7_clockcounterM= FC7_clockcounter-FC7_clockcounter0;


 cout << FC7_clockcounterM << endl;

 //Int_tNBINS=10;
 
 TProfile *p1 = new TProfile("p1","",NBINS,0.,FC7_clockcounterM);
 TProfile *p2 = new TProfile("p2","",NBINS,0.,FC7_clockcounterM);
 TProfile *p3 = new TProfile("p3","",NBINS,0.,FC7_clockcounterM);
 TProfile *p4 = new TProfile("p4","",NBINS,0.,FC7_clockcounterM);
 TProfile *p5 = new TProfile("p5","",NBINS,0.,FC7_clockcounterM);

 TH2D *h2_1 = new TH2D("h2_1","",NBINS,0.,FC7_clockcounterM,1000,0.,5000);
 TH2D *h2_2 = new TH2D("h2_2","",NBINS,0.,FC7_clockcounterM,1000,0.,2000);
 TH2D *h2_3 = new TH2D("h2_3","",NBINS,0.,FC7_clockcounterM,1000,0.,2000);
 TH2D *h2_4 = new TH2D("h2_4","",NBINS,0.,FC7_clockcounterM,1000,0.,2000);
 TH2D *h2_5 = new TH2D("h2_5","",NBINS,0.,FC7_clockcounterM,1000,0.,2000);

  for (Int_t  k =0; k < data->GetEntries(); k++){
    data->GetEntry(k);
if( pmt_amplitude>100 && pin1_amplitude>100 && pin2_amplitude>100){
    p1->Fill(FC7_clockcounter-FC7_clockcounter0,pmt_amplitude);
    p2->Fill(FC7_clockcounter-FC7_clockcounter0,pin1_amplitude);
    p3->Fill(FC7_clockcounter-FC7_clockcounter0,pin2_amplitude);
    p4->Fill(FC7_clockcounter-FC7_clockcounter0,pin1_amplitude+pin2_amplitude);
    p5->Fill(FC7_clockcounter-FC7_clockcounter0,pin1_amplitude/pin2_amplitude);

    h2_1->Fill(FC7_clockcounter-FC7_clockcounter0,pmt_amplitude);
    h2_2->Fill(FC7_clockcounter-FC7_clockcounter0,pin1_amplitude);
    h2_3->Fill(FC7_clockcounter-FC7_clockcounter0,pin2_amplitude);
    h2_4->Fill(FC7_clockcounter-FC7_clockcounter0,pin1_amplitude+pin2_amplitude);
    h2_5->Fill(FC7_clockcounter-FC7_clockcounter0,pin1_amplitude/pin2_amplitude);
    }
  }

/* Double_t p1_mean= p1->GetMean(2);
 Double_t p2_mean= p2->GetMean(2);
 Double_t p3_mean= p3->GetMean(2);
 Double_t p4_mean= p4->GetMean(2);
 Double_t p5_mean= p5->GetMean(2);
*/

  Double_t p1_mean= p1->GetBinContent(1);
  Double_t p2_mean= p2->GetBinContent(1);
  Double_t p3_mean= p3->GetBinContent(1);
  Double_t p4_mean= p4->GetBinContent(1);
  Double_t p5_mean= p5->GetBinContent(1);

  
  p1->Scale(1./p1_mean);
  p2->Scale(1./p2_mean);
  p3->Scale(1./p3_mean);
  p4->Scale(1./p4_mean);
  p5->Scale(1./p5_mean);

  p1->SetLineColor(1);
  p2->SetLineColor(2);
  p3->SetLineColor(3);
  p4->SetLineColor(4);
  p5->SetLineColor(8);

  p1->SetMarkerColor(1);
  p2->SetMarkerColor(2);
  p3->SetMarkerColor(3);
  p4->SetMarkerColor(4);
  p5->SetMarkerColor(8);

  p1->SetMarkerSize(0.6);
  p2->SetMarkerSize(0.6);
  p3->SetMarkerSize(0.6);
  p4->SetMarkerSize(0.6);
  p5->SetMarkerSize(0.6);

  p1->SetMarkerStyle(21);
  p2->SetMarkerStyle(20);
  p3->SetMarkerStyle(4);
  p4->SetMarkerStyle(20);
  p5->SetMarkerStyle(4);

  

  gStyle->SetOptStat(kFALSE);
 
  
  TCanvas *c1 = new TCanvas("c1","c1",10,10,800,600);
  c1->cd();
  p1->Draw();
  p2->Draw("same");
  p3->Draw("same");

  p1->GetYaxis()->SetTitle("Normalized Response");
  p1->GetXaxis()->SetTitle("Time (FC7_clockcounter)");
  Double_t xl1=.05, yl1=0.75, xl2=xl1+.3, yl2=yl1+.125;
  TLegend *leg = new TLegend(xl1,yl1,xl2,yl2);
  leg->AddEntry(p1,"pmt","pl");   
  leg->AddEntry(p2,"pin1","pl");
  leg->AddEntry(p3,"pin2","pl");
  leg->Draw();

  TCanvas *c2 = new TCanvas("c2","c2",10,10,800,600);
  c2->cd();
  p1->Draw();
  p4->Draw("same");
  p5->Draw("same");
  //Double_t xl1=.05, yl1=0.75, xl2=xl1+.3, yl2=yl1+.125;
  TLegend *leg2 = new TLegend(xl1,yl1,xl2,yl2);
  leg2->AddEntry(p1,"pmt","pl");   
  leg2->AddEntry(p4,"pin1+pin2","pl");
  leg2->AddEntry(p5,"pin1/pin2","pl");
  leg2->Draw();


	TCanvas *c3 = new TCanvas("c3","c3",10,10,800,600);
	  c3->cd();
	  h2_1->Draw("COLZ");  

TCanvas *c4= new TCanvas("c4","c4",10,10,800,600);
  c4->cd();
  h2_2->Draw("COLZ");

TCanvas *c5= new TCanvas("c5","c5",10,10,800,600);
  c5->cd();
  h2_3->Draw("COLZ");
}



