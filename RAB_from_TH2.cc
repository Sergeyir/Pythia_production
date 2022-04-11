#include "iostream"
#include "TFile.h"
#include "TH2.h"
#include "TH1.h"
#include "TGraphErrors.h"
#include "TCanvas.h"
#include <string>
#include <array>
#include "TColor.h"
#include "TCanvas.h"
#include "TLatex.h"
#include "TVirtualPad.h"

using namespace std;

int RAB_from_TH2() {

	string hist_name = "nkstars";

	TH1D *pp_part_proj, *NN_part_proj;

	const int centr_num = 5;

	array<const int, centr_num> centr_min = {0, 20, 40, 60, 80};
	array<const int, centr_num> centr_max = {20, 40, 60, 80, 93};

	array <const float, centr_num> ncolls = {313.8, 129.3, 41.84, 10.1, 2.28};

	array<Color_t, centr_num> marker_color = {kRed, kOrange, kGreen, kCyan, kBlue};
	array<const int, centr_num> marker_style = {53, 54, 55, 56, 59};
	
	array <const float, 14> pt_min = {0.0, 0.0, 1.4, 1.7, 1.9, 2.1, 2.3, 2.6, 2.9, 3.4, 4.0, 4.5, 5.0, 6.5};
	array <const float, 14> pt_max = {8.0, 1.4, 1.7, 1.9, 2.1, 2.3, 2.6, 2.9, 3.4, 4.0, 4.5, 5.0, 6.5, 8.0};
	
	TFile *input_pp = new TFile("~/Root/Projects/data/RAB/pythiaPP200_centr_1_.root");
	TFile *input_NN = new TFile("~/Root/Projects/data/RAB/pythiaCuAu200_centr_1_.root");

	TH2D *pp_particles = (TH2D*) input_pp->Get(hist_name.c_str());
	TH2D *NN_particles = (TH2D*) input_NN->Get(hist_name.c_str());

	TCanvas *RAB_canv = new TCanvas("R_{AB}", "RAB", 900, 900);
	
	TH1F *hr = new TH1F("hr", "hr", 100, 0, 8);;
	hr->GetXaxis()->SetTitle("p_{T}, GeV");
	hr->GetYaxis()->SetTitle("R_{AB}");

	hr->GetXaxis()->SetTitleSize(0.028);
	hr->GetYaxis()->SetTitleSize(0.028);

	gStyle->SetOptStat(0);

	gPad->SetLogy();

	hr->Draw("AXIS");

	TLegend *legend = new TLegend(0.7, 0.75, 0.95, 0.95);

	for (int centr_count = 0; centr_count <	centr_num; centr_count++) {

		string pp_name = "pp_proj";
		pp_name.append(to_string(centr_count));

		string NN_name = "NN_proj";
		NN_name.append(to_string(centr_count));

		pp_part_proj = pp_particles->ProjectionX(pp_name.c_str(), 1, 1);
		NN_part_proj = NN_particles->ProjectionX(NN_name.c_str(), centr_count+1, centr_count+1);
		
		NN_part_proj->Divide(pp_part_proj);

		TGraphErrors *RAB = new TGraphErrors(pt_min.size() - 1);
		
		RAB->SetMarkerStyle(marker_style[centr_count]);
		RAB->SetMarkerColor(marker_color[centr_count]);
		RAB->SetMarkerSize(2);

		RAB->SetLineColor(marker_color[centr_count]);
		
		for (int pt_count = 1; pt_count < pt_min.size(); pt_count++) {

			RAB->SetPoint(pt_count - 1, (pt_max[pt_count]+pt_min[pt_count])/2, NN_part_proj->GetBinContent(pt_count)/ncolls[centr_count]);
			RAB->SetPointError(pt_count - 1, 0, NN_part_proj->GetBinError(pt_count)/sqrt(ncolls[centr_count]));

		}

		RAB->Draw("P");

		string legend_entry = to_string(centr_min[centr_count]);
		legend_entry.append("-");
		legend_entry.append(to_string(centr_max[centr_count]));

		legend->AddEntry(RAB, legend_entry.c_str(), "p");

	}

	legend->Draw();	

	string output_name = "output/";
	output_name.append(hist_name);
	output_name.append(".png");
	RAB_canv->SaveAs(output_name.c_str());

	return 1;

}	
