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

typedef struct {
  Int_t NBOF;
  Int_t NTimeTrgBOF;
  Int_t NtrgBOF;
  Int_t BoardAdr;
  Int_t boardTemp;
  Int_t cspTemp;
  Int_t extTemp;
  Int_t Vbias;
  Int_t ADCVal;
  Int_t PulseType;
  Int_t t_year;
  Int_t t_mon;
  Int_t t_day;
  Int_t t_secday;
  Int_t fired;
} DEVICE;

void DecodeMB_v4(TString filen1)
{
	int  NofEvt, NtrgBOF, Nframe, Nbytes, NBOF, BoardAdr, ChBoard, PulseType, NTimeTrgBOF, FlagType, StartW, StopW, StopW2;
	int  boardTemp, cspTemp, extTemp, Vbias, ADCVal, channel;  
	int t_year, t_mon, t_day, t_secday, StatusErr, StatusDFifo;
	Double_t trgdtime[4000], time[4000], scale_time;
	int trg[4000], Ntime[4000];
	int ErrorCode, frameloc;
    int byteRD;
	DEVICE pmt,pin1,pin2;

	//==============================
	// Output File
	char output_ROOT[200], sTemp[200], myofile[200], sfile[200], nowtemp[100];
	int qq, qqq, l, ptrStart, ptrStop, Nhit, StartNewFrame, MyCount=0;
	strcpy(myofile,filen1.Data());
	strcpy(sfile,filen1.Data());

	cout << " FileIN " << filen1.Data() << endl;
	strcpy(output_ROOT,filen1.Data());
	sprintf(sTemp,".root");
	strcat(output_ROOT,sTemp);
	TFile *hfile = new TFile(output_ROOT,"RECREATE");

	TH1F *hADC=new TH1F("hADC","",1000,0.,16000.);
	TH1F *hbTemp=new TH1F("hbTemp","",100,0.,16000.);
	TH1F *hcspTemp=new TH1F("hcspTemp","",100,0.,16000.);
	TH1F *hextTemp=new TH1F("hextTemp","",100,0.,16000.);
	TH1F *hbias=new TH1F("hbias","",100,0.,16000.);

	// Output Tree
	TTree *ntMonFrame;

	ntMonFrame = new TTree ("ntMonFrame","MonitorFrame");

	ntMonFrame->Branch("pmt",&pmt,  "NBOF/I:NTimeTrgBOF/I:NtrgBOF/I:BoardAdr/I:boardTemp/I:cspTemp/I:extTemp/I:Vbias/I:ADCVal/I:PulseType/I:t_year/I:t_mon/I:t_day/I:t_secday/I:fired/I");
	ntMonFrame->Branch("pin1",&pin1,"NBOF/I:NTimeTrgBOF/I:NtrgBOF/I:BoardAdr/I:boardTemp/I:cspTemp/I:extTemp/I:Vbias/I:ADCVal/I:PulseType/I:t_year/I:t_mon/I:t_day/I:t_secday/I:fired/I");
	ntMonFrame->Branch("pin2",&pin2,"NBOF/I:NTimeTrgBOF/I:NtrgBOF/I:BoardAdr/I:boardTemp/I:cspTemp/I:extTemp/I:Vbias/I:ADCVal/I:PulseType/I:t_year/I:t_mon/I:t_day/I:t_secday/I:fired/I");

/*	
	ntMonFrame->Branch("ErrorCode",&ErrorCode,"ErrorCode/I"); 	//Error Flag: is 1 if there is an error 
	ntMonFrame->Branch("frameloc",&frameloc,"frameloc/I");		//Numbers of Byte: For check 28 is the aspected right value. If different -> error is present  
	ntMonFrame->Branch("NBOF",&NBOF,"NBOF/I");					//Time reference Number
	ntMonFrame->Branch("NTimeTrgBOF",&NTimeTrgBOF,"NTimeTrgBOF/I");		//Number: time of trigger rispect to BOF
	ntMonFrame->Branch("NtrgBOF",&NtrgBOF,"NtrgBOF/I");			//Numbers of triggers in the BOF

	ntMonFrame->Branch("PulseType",&PulseType,"PulseType/I");	// Pulse Type: 1 for Laser and 2 for Americium
	ntMonFrame->Branch("BoardAdr",&BoardAdr,"BoardAdr/I");		//Board Address from 0 to 11
	ntMonFrame->Branch("ChBoard",&ChBoard,"ChBoard/I");			//Channals board: 3, A B C corresponding to 10 11 12 decimal value 
        
	ntMonFrame->Branch("boardTemp",&boardTemp,"boardTemp/I");	//Board Temperature for each channel
	ntMonFrame->Branch("cspTemp",&cspTemp,"cspTemp/I");			//Preamplifier Temperature for each channel
	ntMonFrame->Branch("extTemp",&extTemp,"extTemp/I");			//External Temperature  for each channel: one of nine is 0
    ntMonFrame->Branch("Vbias",&Vbias,"Vbias/I");				//Bias Voltage for each channel
    ntMonFrame->Branch("ADCVal",&ADCVal,"ADCVal/I");			//ADC Value  for each channel (charge)
	ntMonFrame->Branch("t_year",&t_year,"t_year/I");			//Temporal informations from rs232 board: Year
	ntMonFrame->Branch("t_mon",&t_mon,"t_mon/I");				//Temporal informations from rs232 board: Month
	ntMonFrame->Branch("t_day",&t_day,"t_day/I");				//Temporal informations from rs232 board: Day
	ntMonFrame->Branch("t_secday",&t_secday,"t_secday/I");		//Temporal informations from rs232 board: seconds of day
*/  
	Int_t i, j, k, jcnt, jbyte, jword ;

	int Readlocal, WordMSB, WordLSB, DataFrame[20], NTime1, NTime2;
	int FifoVal, cntbytes;
	int numpmt=0;
	int numpin1=0;
	int numpin2=0;
	
	FILE *fpIN, *fpIN2;

	fpIN=fopen(sfile,"r");	
	Nframe=0;
	int nerr=0, ch10err=0, ch11err=0, ch12err=0;
      
	if (fpIN!=NULL)
	  {
	    cout << " open file " << sfile << endl;
	    qq=1;
	    while (qq>0)
	      //while (MyCount < 30000)
	      {
		//2016 4 29 82750 
		//qq=fscanf(fpIN,"%d %d ",&Nframe, &Nbytes);
		qq=fscanf(fpIN,"%d %d %d %d ",&t_year, &t_mon, &t_day, &t_secday);
		//cout << " frame " << " day " << t_day << " sec_day " << t_secday  << endl;
		//00000000 00000000 8475 02C0 03D2 004F 02E9 009E 023C 
		//qq=fscanf(fpIN,"%d %d %x %x %x %x %x %x %x %x %x\n",&ErrorCode, &frameloc, &NBOF, &NTimeBOF, &NtrgBOF, &FlagType, &boardTemp, &cspTemp, &extTemp, &Vbias, &ADCVal);
		qq=fscanf(fpIN,"%d %d ",&ErrorCode, &frameloc);
		if (ErrorCode==0)
		  {
		    qq=fscanf(fpIN,"%x %x %x %x %x %x %x %x",&NBOF, &NTimeTrgBOF, &NtrgBOF, &FlagType, &boardTemp, &cspTemp, &extTemp, &Vbias);
		    qq=fscanf(fpIN," %x\n", &ADCVal);
		    //printf("%x %x %x %x %x %x %x %x %x\n",NBOF, NTimeTrgBOF, NtrgBOF, FlagType, boardTemp, cspTemp, extTemp, Vbias, ADCVal);

		    BoardAdr= (FlagType&0x000F);
		    ChBoard=  (FlagType&0x00F0)>>4;
		    PulseType= (FlagType&0xFF00)>>8;


		    	pmt.fired=-1;
			pin1.fired=-1;
			pin2.fired=-1;
		    //getchar();

		    //
		    // Fill NTPle
		    //
		    switch (ChBoard){
		    case 10:
		      {
			pmt.NBOF = NBOF;
			pmt.NTimeTrgBOF = NTimeTrgBOF;
			pmt.NtrgBOF = NtrgBOF;
			pmt.BoardAdr = BoardAdr;
			pmt.boardTemp = boardTemp;
			pmt.cspTemp = cspTemp;
			pmt.extTemp = extTemp;
			pmt.Vbias = Vbias;
			pmt.ADCVal = ADCVal;
			pmt.PulseType = PulseType;
			pmt.t_year = t_year;
			pmt.t_mon = t_mon;
			pmt.t_day = t_day;
			pmt.t_secday = t_secday;
			pmt.fired=1;


			pin1.NBOF = -999;
			pin1.NTimeTrgBOF = -999;
			pin1.NtrgBOF = -999;
			pin1.BoardAdr = -999;
			pin1.boardTemp = -999;
			pin1.cspTemp = -999;
			pin1.extTemp = -999;
			pin1.Vbias = -999;
			pin1.ADCVal = -999;
			pin1.PulseType = -999;
			pin1.t_year = -999;
			pin1.t_mon = -999;
			pin1.t_day = -999;
			pin1.t_secday = -999;
			
			
			pin2.NBOF = -999;
			pin2.NTimeTrgBOF = -999;
			pin2.NtrgBOF = -999;
			pin2.BoardAdr = -999;
			pin2.boardTemp = -999;
			pin2.cspTemp = -999;
			pin2.extTemp = -999;
			pin2.Vbias = -999;
			pin2.ADCVal = -999;
			pin2.PulseType = -999;
			pin2.t_year =-999;
			pin2.t_mon = -999;
			pin2.t_day = -999;
			pin2.t_secday = -999;
			
			numpmt++;
			break;
		      }
		    case 11:
		      {
			pin1.NBOF = NBOF;
			pin1.NTimeTrgBOF = NTimeTrgBOF;
			pin1.NtrgBOF = NtrgBOF;
			pin1.BoardAdr = BoardAdr;
			pin1.boardTemp = boardTemp;
			pin1.cspTemp = cspTemp;
			pin1.extTemp = extTemp;
			pin1.Vbias = Vbias;
			pin1.ADCVal = ADCVal;
			pin1.PulseType = PulseType;
			pin1.t_year = t_year;
			pin1.t_mon = t_mon;
			pin1.t_day = t_day;
			pin1.t_secday = t_secday;
			pin1.fired=1;


			pmt.NBOF =-999;
			pmt.NTimeTrgBOF = -999;
			pmt.NtrgBOF =-999;
			pmt.BoardAdr = -999;
			pmt.boardTemp = -999;
			pmt.cspTemp = -999;
			pmt.extTemp = -999;
			pmt.Vbias = -999;
			pmt.ADCVal = -999;
			pmt.PulseType = -999;
			pmt.t_year = -999;
			pmt.t_mon = -999;
			pmt.t_day = -999;
			pmt.t_secday = -999;
			
			pin2.NBOF = -999;
			pin2.NTimeTrgBOF = -999;
			pin2.NtrgBOF = -999;
			pin2.BoardAdr = -999;
			pin2.boardTemp = -999;
			pin2.cspTemp = -999;
			pin2.extTemp = -999;
			pin2.Vbias = -999;
			pin2.ADCVal = -999;
			pin2.PulseType = -999;
			pin2.t_year =-999;
			pin2.t_mon = -999;
			pin2.t_day = -999;
			pin2.t_secday = -999;

			
			numpin1++;		
			break;
		      }
		    case 12:
		      {
			pin2.NBOF = NBOF;
			pin2.NTimeTrgBOF = NTimeTrgBOF;
			pin2.NtrgBOF = NtrgBOF;
			pin2.BoardAdr = BoardAdr;
			pin2.boardTemp = boardTemp;
			pin2.cspTemp = cspTemp;
			pin2.extTemp = extTemp;
			pin2.Vbias = Vbias;
			pin2.ADCVal = ADCVal;
			pin2.PulseType = PulseType;
			pin2.t_year = t_year;
			pin2.t_mon = t_mon;
			pin2.t_day = t_day;
			pin2.t_secday = t_secday;
			pin2.fired=1;

			pmt.NBOF =-999;
			pmt.NTimeTrgBOF = -999;
			pmt.NtrgBOF =-999;
			pmt.BoardAdr = -999;
			pmt.boardTemp = -999;
			pmt.cspTemp = -999;
			pmt.extTemp = -999;
			pmt.Vbias = -999;
			pmt.ADCVal = -999;
			pmt.PulseType = -999;
			pmt.t_year = -999;
			pmt.t_mon = -999;
			pmt.t_day = -999;
			pmt.t_secday = -999;

			pin1.NBOF = -999;
			pin1.NTimeTrgBOF = -999;
			pin1.NtrgBOF = -999;
			pin1.BoardAdr = -999;
			pin1.boardTemp = -999;
			pin1.cspTemp = -999;
			pin1.extTemp = -999;
			pin1.Vbias = -999;
			pin1.ADCVal = -999;
			pin1.PulseType = -999;
			pin1.t_year = -999;
			pin1.t_mon = -999;
			pin1.t_day = -999;
			pin1.t_secday = -999;
			
			numpin2++;
			break;
		      }
		    default:
		      break;
		    }
		    ntMonFrame->Fill();
		   
		    std::cout << numpmt << " " << numpin1 << " " << numpin2 << std::endl;
		  }
		else
		  {
		    printf("ErrorFrame: nloc %d\n",frameloc);
		    for (int ibyte=0;ibyte<frameloc;ibyte++)
		      {
			qq=fscanf(fpIN,"%x ",&byteRD);
			printf("loc %d byteRD %.2x \n",ibyte,byteRD); 
		      }
		    nerr++; if(ChBoard==10)ch10err++; if(ChBoard==11)ch11err++; if(ChBoard==12)ch12err++;
		    // qq=fscanf(fpIN,"\n");
		    qq=1;
		  }
		  
		Nframe++;
	       
		//ErrorCode=0;
		// getchar(); 
	       
	      } // if 
	    
	  }  // end if file
	else {
	  printf("file NULL\n");
	}
	printf("ErrorFrame summary: %d %d %d %d\n",nerr, ch10err, ch11err, ch12err);
	hfile->cd();
	hfile->Write();
	hfile->Close();


	
}

/*

         TFile *f0 = TFile::Open("AnalisiFrame.root");
	 
	 c1=new TCanvas("c1","c1",900,800);
	 TF1 *ft = new TF1("ft","[0]*(pow(2.7172,-[1]*x/64.))",1.,500.);
	 s1->Fit("ft","R");


*/
