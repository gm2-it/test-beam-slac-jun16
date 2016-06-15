//
// the logic of this code is bad, it stores all data in arrays and then makes
// plots with the correct limits
// should be changed to produce Root auto-ranging histograms
//

#include "RiderAna.h"
#include <TMath.h>
#include <TF1.h>
#include <TF2.h>
#include <TCanvas.h>

#include <iostream>

void RiderAna::Loop(string const &filename){
    cout << "Loop()" << endl;

    //initialize(filename);

    if (fChain == 0) return;

    //nentries = fChain->GetEntriesFast();
    nentries = fChain->GetEntries();
    initialize(filename);

    Int_t nbytes = 0, nb = 0;
    for (Long64_t jentry=0; jentry<nentries;jentry++) {
        Long64_t ientry = LoadTree(jentry);
        if (ientry < 0) break;
        nb = fChain->GetEntry(jentry);   nbytes += nb;

        clear();
        execute();
    }

    finalize(filename);
}

void RiderAna::initialize(string const &filename){
 	iter = 0;
 	aFC7_clockcounter = new Double_t[nentries];

 	apmt_amplitude = new Double_t[nentries];
 	apin1_amplitude = new Double_t[nentries];
 	apin2_amplitude = new Double_t[nentries];

	cout << "initialize()" << endl;
	cout << "N. entries: " << nentries << endl;
}

void RiderAna::execute(){
	aFC7_clockcounter[iter] = FC7_clockcounter;

	apmt_amplitude[iter] = pmt_amplitude;
	apin1_amplitude[iter] = pin1_amplitude;
	apin2_amplitude[iter] = pin2_amplitude;

	iter++;
	//cout << "execute()" << endl;
}

void RiderAna::finalize(string const &filename){
	//histograms for LM analysis
	Int_t nbins = 10;
	Double_t time = aFC7_clockcounter[iter-1] - aFC7_clockcounter[0];
	Double_t pmt_mean = TMath::Mean(iter,apmt_amplitude);
	cout << pmt_mean<<" "<<time << endl;

	char title[100];
	sprintf(title,"Time profile for pmt - file %s",filename.c_str());
	prof_pmt_vs_time = new TProfile("prof_pmt_vs_time",title,time/nbins,0,time);
	sprintf(title,"Time profile for pin1 - file %s",filename.c_str());
	prof_pin1_vs_time = new TProfile("prof_pin1_vs_time",title,time/nbins,0,time);
	sprintf(title,"Time profile for pin2 - file %s",filename.c_str());
	prof_pin2_vs_time = new TProfile("prof_pin2_vs_time",title,time/nbins,0,time);

	sprintf(title,"TProfile for pin1 Vs pin2 - file %s",filename.c_str());
	prof_pin1_vs_pin2 = new TProfile("prof_pin1_vs_pin2",title,100,0,700);
	sprintf(title,"TProfile for pins ratio - file %s",filename.c_str());
	prof_ratio_pins = new TProfile("prof_ratio_pins",title,time/nbins,0,time);
	sprintf(title,"TProfile for pins sum - file %s",filename.c_str());
	prof_sum_pins = new TProfile("prof_sum_pins",title,time/nbins,0,time);

	sprintf(title,"H2 Time profile for pmt - file %s",filename.c_str());
	h2_pmt_vs_time = new TH2D("h2_pmt_vs_time",title,50,0,time, 400,0,5000);
	sprintf(title,"H2 Time profile for pin1 - file %s",filename.c_str());
	h2_pin1_vs_time = new TH2D("h2_pin1_vs_time",title,50,0,time, 200,0,700);
	sprintf(title,"H2 Time profile for pin2 - file %s",filename.c_str());
	h2_pin2_vs_time = new TH2D("h2_pin2_vs_time",title,50,0,time, 200,0,700);

	sprintf(title,"Pin1 Vs Pin2 Correlation - file %s",filename.c_str());
	h2_pin1_vs_pin2 = new TH2D("h2_pin1_vs_pin2",title,400,0,700, 400,0,700);
	sprintf(title,"Pins Ratio Correlation - file %s",filename.c_str());
	h2_ratio_pins = new TH2D("h2_ratio_pins",title,100,0,time, 40,0,2);
	sprintf(title,"Pins Sum Correlation - file %s",filename.c_str());
	h2_sum_pins = new TH2D("h2_sum_pins",title,100,0,time, 400,0,2000);

	sprintf(title,"Histogram for pmt charge - file %s",filename.c_str());
	h1_pmt_charge = new TH1D("h1_pmt_charge",title,200,0,5000);
	sprintf(title,"Histogram for pin1 charge - file %s",filename.c_str());
	h1_pin1_charge = new TH1D("h1_pin1_charge",title,200,0,700);
	sprintf(title,"Histogram for pin2 charge - file %s",filename.c_str());
	h1_pin2_charge = new TH1D("h1_pin2_charge",title,200,0,700);
	sprintf(title,"Histogram for pins ratio - file %s",filename.c_str());
	h1_ratio_pins = new TH1D("h1_ratio_pinse",title,200,0,2);
	sprintf(title,"Histogram for pins sum - file %s",filename.c_str());
	h1_sum_pins = new TH1D("h1_sum_pins",title,200,0,2000);

	for (int j = 0; j < iter; ++j){
		if (apmt_amplitude[j] > 100 && apin1_amplitude[j] > 100 && apin2_amplitude[j] > 100){
			prof_pmt_vs_time->Fill(aFC7_clockcounter[j] - aFC7_clockcounter[0],apmt_amplitude[j]/pmt_mean);
			prof_pin1_vs_time->Fill(aFC7_clockcounter[j] - aFC7_clockcounter[0],apin1_amplitude[j]/apin1_amplitude[0]);
			prof_pin2_vs_time->Fill(aFC7_clockcounter[j] - aFC7_clockcounter[0],apin2_amplitude[j]/apin2_amplitude[0]);
			prof_pin1_vs_pin2->Fill(apin1_amplitude[j],apin2_amplitude[j]);
			if (apin2_amplitude[j]!=0)prof_ratio_pins->Fill(aFC7_clockcounter[j] - aFC7_clockcounter[0],apin1_amplitude[j]/apin2_amplitude[j]);
			prof_sum_pins->Fill(aFC7_clockcounter[j] - aFC7_clockcounter[0],(apin1_amplitude[j] + apin2_amplitude[j])/(apin1_amplitude[0] + apin2_amplitude[0]));

			h2_pmt_vs_time->Fill(j,apmt_amplitude[j]);
			h2_pin1_vs_time->Fill(j,apin1_amplitude[j]);
			h2_pin2_vs_time->Fill(j,apin2_amplitude[j]);

			h2_pin1_vs_pin2->Fill(apin1_amplitude[j],apin2_amplitude[j]);
			if (apin2_amplitude[j]!=0)h2_ratio_pins->Fill(j,apin1_amplitude[j]/apin2_amplitude[j]);
			h2_sum_pins->Fill(j,apin1_amplitude[j] + apin2_amplitude[j]);

			h1_pmt_charge->Fill(apmt_amplitude[j]);
			h1_pin1_charge->Fill(apin1_amplitude[j]);
			h1_pin2_charge->Fill(apin2_amplitude[j]);
			if (apin2_amplitude[j]!=0)h1_ratio_pins->Fill(apin1_amplitude[j]/apin2_amplitude[j]);
			h1_sum_pins->Fill(apin1_amplitude[j] + apin2_amplitude[j]);
		}
	}

	// Write out TTree, close output file
	// Prepare output file
	char* cFile  = new char[100];
	sprintf(cFile,"analyzed_%s",filename.c_str());

	TFile *OutF = new TFile(cFile,"recreate");
	OutF->cd();
	gROOT->GetList()->Write();
	OutF->Close();
	delete OutF;
	delete [] cFile;
	useDelete();
	cout << "finalize()" << endl;
}

void RiderAna::useDelete(){
	delete prof_pmt_vs_time;
	delete prof_pin1_vs_time;
	delete prof_pin2_vs_time;

	delete prof_pin1_vs_pin2;
	delete prof_ratio_pins;
	delete prof_sum_pins;

	delete h2_pmt_vs_time;
	delete h2_pin1_vs_time;
	delete h2_pin2_vs_time;

	delete h2_pin1_vs_pin2;
	delete h2_ratio_pins;
	delete h2_sum_pins;

	delete h1_pmt_charge;
	delete h1_pin1_charge;
	delete h1_pin2_charge;
	delete h1_ratio_pins;
	delete h1_sum_pins;

	cout << "delete()" << endl;
}

void RiderAna::clear(){
	//cout << "clear()" << endl;
}
