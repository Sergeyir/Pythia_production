#include <iostream>
#include <array>
#include "TH1.h"
#include "TFile.h"
#include "TCanvas.h"

int centr_range_ident() {

	system("echo 'bruh'");

	TH1D *ncharged_per_event_hist, *zero_ev_hist;
	std::array<TH1D*, 5> centr_ncharged;
	
	std::array<int, 5> centr_l_range = {0, 20, 40, 60, 80};
	std::array<int, 5> centr_h_range = {20, 40, 60, 80, 93};
	
	TFile *input, *output;
	
	std::string name = "/home/sergey/pythia/pythia8306/pythia_production/output/pythiaCuAu200_centr_1_.root";
	
	input = new TFile(name.c_str());
	ncharged_per_event_hist = (TH1D*) input->Get("ncharged");
	
	unsigned int nbins = ncharged_per_event_hist->GetNbinsX();
	
	centr_ncharged[0] = new TH1D("centr", "centr", nbins, 0, nbins);
	centr_ncharged[1] = new TH1D("c20_40", "centr", nbins, 0, nbins);
	centr_ncharged[2] = new TH1D("c40_60", "centr", nbins, 0, nbins);
	centr_ncharged[3] = new TH1D("c60_80", "centr", nbins, 0, nbins);
	centr_ncharged[4] = new TH1D("c80_93", "centr", nbins, 0, nbins);
	zero_ev_hist = new TH1D("0", "0", nbins, 0, nbins); 
	
	for (int count = 0; count < 5; count++) centr_ncharged[count]->SetLineColor(kBlack);
	
	centr_ncharged[0]->SetFillColor(kBlue-10);
	centr_ncharged[1]->SetFillColor(kBlue-8);
	centr_ncharged[2]->SetFillColor(kBlue-6);
	centr_ncharged[3]->SetFillColor(kBlue-2);
	centr_ncharged[4]->SetFillColor(kBlue+4);
	zero_ev_hist->SetFillColor(kBlack);
	
	const long long unsigned int ncharged_total = ncharged_per_event_hist->Integral(1, nbins);
	
	int cnum = 0;
	
	std::array<int, 5> ncharged_centr_range;
	
	unsigned long long int prev_sum = 0, sum = 0;
	
	TCanvas *c = new TCanvas("bruh");
	gPad->SetLogy();
	
	ncharged_per_event_hist->Draw("AXIS");
	
	for (int count = 1; count < nbins; count++) {
		
		prev_sum = sum;
		sum = ncharged_per_event_hist->Integral(nbins-count, nbins);
		
		centr_ncharged[cnum]->SetBinContent(nbins-count, ncharged_per_event_hist->GetBinContent(nbins-count));
		
		if (sum > centr_h_range[cnum]*ncharged_total/100) {
		
			if (sum - centr_h_range[cnum]*ncharged_total/100 < centr_h_range[cnum]*ncharged_total/100 - prev_sum) ncharged_centr_range[cnum] = nbins-count;
			else ncharged_centr_range[cnum] = nbins-count+1;
			
			centr_ncharged[cnum]->Draw("SAME");
			
			if (static_cast<int>(sum/ncharged_total*100) != 100) cout << cnum << " " << nbins - count << " " << endl;
			else cout << cnum << " " << nbins - count << " " << (sum-ncharged_per_event_hist->GetBinContent(1))/ncharged_total*100 << endl;
			
			cnum++;
		
		}
		
	}
	
	zero_ev_hist->SetBinContent(1, ncharged_per_event_hist->GetBinContent(1));
	zero_ev_hist->Draw("SAME");
	
	return 0;
	
}
