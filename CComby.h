#pragma once

#include <array>
#include <cmath>
#include <string>
#include "TFile.h"
#include "TH3.h"
#include "Pythia8/Pythia.h"

class Particles;

class CComby: public Particles {
	
	private:
	
		static const unsigned int ch_number{9}; //number of decay channels
		
		bool check{true}; 
	
		TFile *output;
		
		static unsigned const int centr_num = 5;
		
		std::array <unsigned const int, centr_num> centr_range_low{0, 20, 40, 60, 80};
		std::array <unsigned const int, centr_num> centr_range_high{20, 40, 60, 80, 93};
		
		std::array <const int, 3> pos_part_id{211, 321, 2212};
		std::array <const int, 3> neg_part_id{-211, -321, -2212};
		
		std::array <const std::string, 3> pos_part_name{"Pi", "K", "P"};
		std::array <const std::string, 3> neg_part_name{"Pibar", "Kbar", "Pbar"};
	
		std::vector <TH3F*> fg_hists;
		std::vector <TH3F*> bg_hists;
		
		unsigned int chnum = 0;
		
		void ClearParticles() {
		
			p_id.clear();
			p_iEvent.clear();
			p_e.clear();
			p_px.clear();
			p_py.clear();
			p_pz.clear();
			p_centr.clear();
			
			pbar_id.clear();
			pbar_iEvent.clear();
			pbar_e.clear();
			pbar_px.clear();
			pbar_py.clear();
			pbar_pz.clear();
			pbar_centr.clear();
		
		}
	
	public:
	
		CComby() {	}
	
		int GetNumberOfChannels() {
		
			return ch_number;
	
		}
		
		void SetOutput(std::string output_name, std::string option = "RECREATE") {
		
			if (check) {
		
				output = new TFile(output_name.c_str(), option.c_str());
				check = false;
		
			}
		
			else {
		
				delete output;
				output = new TFile(output_name.c_str(), option.c_str());
			
			}	
			
		
		}
		
		void AddChannel(const unsigned int id1, const unsigned int id2) {
			
			std::string fg_hist_name, bg_hist_name;
			
			chnum++;
			
			for (int count = 0; count < 3; count++) {
			
				if (id1 == pos_part_id[count]) fg_hist_name.append(pos_part_name[count]);
			
			}
			
			for (int count = 0; count < 3; count++) {
			
				if (id2 == neg_part_id[count]) fg_hist_name.append(neg_part_name[count]);
			
			}
			
			bg_hist_name = fg_hist_name;
			
			fg_hist_name.append("_FG");
			bg_hist_name.append("_BG");
			
			for (int count = 0; count < centr_num; count++) {
			
				std::string fg_hist_c_name = fg_hist_name;
				std::string bg_hist_c_name = bg_hist_name;
				
				fg_hist_c_name.append(std::to_string(centr_range_low[count]));
				fg_hist_c_name.append("-");
				fg_hist_c_name.append(std::to_string(centr_range_high[count]));
				
				bg_hist_c_name.append(std::to_string(centr_range_low[count]));
				bg_hist_c_name.append("-");
				bg_hist_c_name.append(std::to_string(centr_range_high[count]));
			
				fg_hists.push_back(new TH3F(fg_hist_c_name.c_str(), fg_hist_c_name.c_str(), 80, 0, 8, 4000, 0, 4, 12, 0, 1.2));
				bg_hists.push_back(new TH3F(bg_hist_c_name.c_str(), bg_hist_c_name.c_str(), 80, 0, 8, 4000, 0, 4, 12, 0, 1.2));
			
			}
			
		}
	
		void ProcessEvents() {
			
			const int p_size = p_id.size();
			const int pbar_size = pbar_id.size();
			
			for (unsigned long int p_num = 0; p_num < p_size; p_num++) {
		
				for (unsigned long int pbar_num = 0; pbar_num < pbar_size; pbar_num++) {
				
					if (p_id[p_num] != pos_part_id[ch_count] || pbar_id[pbar_num] != neg_part_id[ch_count]) continue;
					
					const double e2 = pow(p_e[p_num] + pbar_e[pbar_num], 2);
					const double p2 = pow(p_px[p_num] + pbar_px[pbar_num], 2) + pow(p_py[p_num] + pbar_py[pbar_num], 2) + pow(p_pz[p_num] + pbar_pz[pbar_num], 2);
					const double m = sqrt(e2 - p2);
					const double eta = 1/2*log((sqrt(p2)+p_pz[p_num]+pbar_pz[pbar_num])/(sqrt(p2)-p_pz[p_num]-pbar_pz[pbar_num]));
					
					for (int ch_count = 0; ch_count < chnum; ch_count++) {
						
						for (int count = 0; count < centr_num; count++) {
						
							bg_hists[ch_count*centr_num+count]->Fill(sqrt(p2), m, eta);
							if (p_iEvent[p_num] == pbar_iEvent[pbar_num]) {fg_hists[ch_count*centr_num+count]->Fill(sqrt(p2), m, eta);}
						
						}
						
						
					}
					
				}
		
			}
		
			ClearParticles();
	
		}
		
		void Write() {
			
			for (int num = 0; num < fg_hists.size(); num++) {
			
				fg_hists[num]->Write();
				bg_hists[num]->Write();
			
			}
		}
	
		~CComby() {
			
			for (int num = 0; num < fg_hists.size(); num++) {
			
				delete fg_hists[num], bg_hists[num];
			
			}
			
			output->Close();
			
			delete output;
	
		}
		
};

