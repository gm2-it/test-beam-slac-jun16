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
 	FC7_clockcounter = new Double_t[nentries];

 	pmt_amplitude = new Double_t[nentries];
 	pin1_amplitude = new Double_t[nentries];
 	pin2_amplitude = new Double_t[nentries];

 	pmt_clockCounter = new Double_t[nentries];
 	pin1_clockCounter = new Double_t[nentries];
 	pin2_clockCounter = new Double_t[nentries];

 	pmt_time = new Double_t[nentries];
 	pin1_time = new Double_t[nentries];
 	pin2_time = new Double_t[nentries];

	anatree = new TTree("anatree","antree");
	anatree->Branch("FC7_clockcounter",&tFC7_clockcounter,"FC7_clockcounter/D");
	anatree->Branch("pmt_amplitude",&tpmt_amplitude,"pmt_amplitude/D");
	anatree->Branch("pin1_amplitude",&tpin1_amplitude,"pin1_amplitude/D");
	anatree->Branch("pin2_amplitude",&tpin2_amplitude,"pin2_amplitude/D");

	cout << "initialize()" << endl;
	cout << "N. entries: " << nentries << endl;
}

void RiderAna::execute(){
	FC7_clockcounter[iter] = 25E-9*FC7_ClockCounter->at(0);

	pmt_amplitude[iter] = Italiano_Amplitude->at(0);
	pin1_amplitude[iter] = Italiano_Amplitude->at(2);
	pin2_amplitude[iter] = Italiano_Amplitude->at(4);

	tFC7_clockcounter = 25E-9*FC7_ClockCounter->at(0);
	tpmt_amplitude = Italiano_Amplitude->at(0);
	tpin1_amplitude = Italiano_Amplitude->at(2);
	tpin2_amplitude = Italiano_Amplitude->at(4);

	pmt_time[iter] = Italiano_Time->at(0);
	pin1_time[iter] = Italiano_Time->at(2);
	pin2_time[iter] = Italiano_Time->at(4);

	pmt_clockCounter[iter] = Italiano_ClockCounter->at(0);
	pin1_clockCounter[iter] = Italiano_ClockCounter->at(2);
	pin2_clockCounter[iter] = Italiano_ClockCounter->at(4);

	anatree->Fill();

	cout << "--> execute(), eventNum: "<< EventNum <<endl;

/*
    for(size_t iC=0; iC<Cluster_EventNum->size();iC++){
	if(Cluster_Time->at(iC)>2000 && Cluster_Time->at(iC)<3000){
	    energy_->Fill(Cluster_Energy->at(iC));}
    }
*/
	iter++;
	//cout << "execute()" << endl;
}

void RiderAna::finalize(string const &filename){
/*
	//histograms for LM analysis
	Int_t nbins = 100;
	Double_t time = TMath::MaxElement(iter,FC7_clockcounter) - FC7_clockcounter[0];
	Double_t pmt_mean = TMath::Mean(iter,pmt_amplitude);

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
	h2_pmt_vs_time = new TH2D("h2_pmt_vs_time",title,100,0,time, 400,0,5000);
	sprintf(title,"H2 Time profile for pin1 - file %s",filename.c_str());
	h2_pin1_vs_time = new TH2D("h2_pin1_vs_time",title,100,0,time, 200,0,700);
	sprintf(title,"H2 Time profile for pin2 - file %s",filename.c_str());
	h2_pin2_vs_time = new TH2D("h2_pin2_vs_time",title,1000,0,time, 200,0,700);

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
		prof_pmt_vs_time->Fill(FC7_clockcounter[j] - FC7_clockcounter[0],pmt_amplitude[j]/pmt_mean);
		prof_pin1_vs_time->Fill(FC7_clockcounter[j] - FC7_clockcounter[0],pin1_amplitude[j]/pin1_amplitude[0]);
		prof_pin2_vs_time->Fill(FC7_clockcounter[j] - FC7_clockcounter[0],pin2_amplitude[j]/pin2_amplitude[0]);
		prof_pin1_vs_pin2->Fill(pin1_amplitude[j],pin2_amplitude[j]);
		if (pin2_amplitude[j]!=0)prof_ratio_pins->Fill(FC7_clockcounter[j] - FC7_clockcounter[0],pin1_amplitude[j]/pin2_amplitude[j]);
		prof_sum_pins->Fill(FC7_clockcounter[j] - FC7_clockcounter[0],(pin1_amplitude[j] + pin2_amplitude[j])/(pin1_amplitude[0] + pin2_amplitude[0]));

		h2_pmt_vs_time->Fill(j,pmt_amplitude[j]);
		h2_pin1_vs_time->Fill(j,pin1_amplitude[j]);
		h2_pin2_vs_time->Fill(j,pin2_amplitude[j]);

		h2_pin1_vs_pin2->Fill(pin1_amplitude[j],pin2_amplitude[j]);
		if (pin2_amplitude[j]!=0)h2_ratio_pins->Fill(j,pin1_amplitude[j]/pin2_amplitude[j]);
		h2_sum_pins->Fill(j,pin1_amplitude[j] + pin2_amplitude[j]);

		h1_pmt_charge->Fill(pmt_amplitude[j]);
		h1_pin1_charge->Fill(pin1_amplitude[j]);
		h1_pin2_charge->Fill(pin2_amplitude[j]);
		if (pin2_amplitude[j]!=0)h1_ratio_pins->Fill(pin1_amplitude[j]/pin2_amplitude[j]);
		h1_sum_pins->Fill(pin1_amplitude[j] + pin2_amplitude[j]);
	}
*/
	// Write out TTree, close output file
	// Prepare output file
	char* cFile  = new char[100];
	sprintf(cFile,"analyzed_%s",filename.c_str());

	TFile *OutF = new TFile(cFile,"recreate");
	OutF->cd();
	gROOT->GetList()->Write();
	anatree->Write();
	OutF->Close();
	delete OutF;
	delete [] cFile;
	useDelete();
	cout << "finalize()" << endl;
}

void RiderAna::useDelete(){
	/*
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
*/
	cout << "delete()" << endl;
}

void RiderAna::clear(){
	//cout << "clear()" << endl;
}
