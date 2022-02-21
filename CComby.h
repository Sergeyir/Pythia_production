#pragma once

#include <array>
#include <cmath>
#include <string>
#include "TFile.h"
#include "TH2.h"
#include "Pythia8/Pythia.h"
#include <memory>

class Particles;

class CComby: public Particles {
	
	private:
	
		bool check{true}; 
	
		TFile *output;
		
		static unsigned const int centr_num = 5;
		
		std::array <unsigned const int, centr_num> centr_range_low{0, 10, 20, 30, 80};
		std::array <unsigned const int, centr_num> centr_range_high{20, 40, 60, 80, 93};
		
		std::array <std::string, 5> output_dir_names;
		
		std::array <const int, 3> pos_part_id{211, 321, 2212};
		std::array <const int, 3> neg_part_id{-211, -321, -2212};
		
		std::array <const std::string, 3> pos_part_name{"Pi", "K", "P"};
		std::array <const std::string, 3> neg_part_name{"Pibar", "Kbar", "Pbar"};
	
		std::vector <std::unique_ptr<TH2F>> fg_hists;
		std::vector <std::unique_ptr<TH2F>> bg_hists;
		
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
	
		CComby() {
			
			for (int count = 0; count < 5; count++) {
			
				output_dir_names[count] = "c0";
				output_dir_names[count].append(std::to_string(count));
				output_dir_names[count].append("_z00_r00");
			
			}
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
			
			
			
			for (int count = 0; count < 5; count++) {output->mkdir(output_dir_names[count].c_str());}
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
			
			fg_hist_name.append("_V1");
			bg_hist_name.append("_V1");
			
			fg_hist_name.append("_FG");
			bg_hist_name.append("_BG");
			
			const unsigned int hists_size = fg_hists.size();
			
			fg_hists.resize(hists_size + centr_num);
			bg_hists.resize(hists_size + centr_num);
			
			for (int count = 0; count < centr_num; count++) {
			
				std::string fg_hist_name_c = fg_hist_name;
				std::string bg_hist_name_c = bg_hist_name;
				
				fg_hist_name_c.append(std::to_string(count));
				bg_hist_name_c.append(std::to_string(count));
				
				fg_hists[hists_size+count].reset(new TH2F(fg_hist_name_c.c_str(), fg_hist_name_c.c_str(), 80, 0, 8, 4000, 0, 4));
				bg_hists[hists_size+count].reset(new TH2F(bg_hist_name_c.c_str(), fg_hist_name_c.c_str(), 80, 0, 8, 4000, 0, 4));
				
			}
			
		}
	
		void ProcessEvents() {
			
			const int p_size = p_id.size();
			const int pbar_size = pbar_id.size();
			
			for (unsigned long int p_num = 0; p_num < p_size; p_num++) {
		
				for (unsigned long int pbar_num = 0; pbar_num < pbar_size; pbar_num++) {
				
					const double e2 = pow(p_e[p_num] + pbar_e[pbar_num], 2);
					const double p2 = pow(p_px[p_num] + pbar_px[pbar_num], 2) + pow(p_py[p_num] + pbar_py[pbar_num], 2) + pow(p_pz[p_num] + pbar_pz[pbar_num], 2);
					const double m = sqrt(e2 - p2);
					//const double eta = 1/2*log((sqrt(p2)+p_pz[p_num]+pbar_pz[pbar_num])/(sqrt(p2)-p_pz[p_num]-pbar_pz[pbar_num]));
					
					for (int ch_count = 0; ch_count < chnum; ch_count++) {
					
					if (p_id[p_num] != pos_part_id[ch_count] || pbar_id[pbar_num] != neg_part_id[ch_count]) continue;
						
						for (int count = 0; count < centr_num; count++) {
						
							bg_hists[ch_count*centr_num+count]->Fill(sqrt(p2), m);
							if (p_iEvent[p_num] == pbar_iEvent[pbar_num]) {fg_hists[ch_count*centr_num+count]->Fill(sqrt(p2), m);}
						
						}
						
						
					}
					
				}
		
			}
		
			ClearParticles();
	
		}
		
		void Write() {
			
			for (int ch_count = 0; ch_count < chnum; ch_count++) {
			
				for (int count = 0; count < centr_num; count++) {
					
					output->cd(output_dir_names[count].c_str());
					
					fg_hists[ch_count*centr_num+count]->Write();
					bg_hists[ch_count*centr_num+count]->Write();
				
				}
			}
			
			output->Close();
		}
	
		~CComby() {
		
			/*
		
			while(!fg_hists.empty()) {
				
				delete fg_hists.back(), bg_hists.back();
				
				fg_hists.pop_back();
				bg_hists.pop_back();
				
				std::cout << "Absolutely useless memory deleting" << std::endl;
			
			}
			
			*/
			
			output->Close();
			
			delete output;
	
		}
		
};

