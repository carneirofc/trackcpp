// The MIT License (MIT)
//
// Copyright (c) 2015 LNLS Accelerator Division
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include <trackcpp/trackcpp.h>
#include <vector>
#include <cstdlib>

void print_header (FILE* fp) {
	fprintf(fp, "# %s\n", string_version.c_str());
	fprintf(fp, "# Accelerator Physics Group - LNLS\n");
	fprintf(fp, "# Campinas BRAZIL\n");
	fprintf(fp, "# contact: xresende@gmail.com\n");
}

Status::type print_closed_orbit(const Accelerator& accelerator, const std::vector<Pos<double>>& cod, const std::string& filename) {

	FILE* fp;
	fp = fopen(filename.c_str(), "w");
	if (fp == nullptr) return Status::file_not_opened;

	const std::vector<Element>& the_ring = accelerator.lattice;

	const char str[] = "------------------------";

	print_header(fp);
	fprintf(fp, "# [closed-orbit]\n");
	fprintf(fp, "# ebeam_energy[eV] : %f\n", accelerator.energy);
	fprintf(fp, "# harmonic_number  : %i\n", accelerator.harmonic_number);
	fprintf(fp, "# cavity_state     : %s\n", accelerator.cavity_on ? "on" : "off");
	fprintf(fp, "# radiation_state  : %s\n", accelerator.radiation_on ? "on" : "off");
	fprintf(fp, "# chamber_state    : %s\n", accelerator.vchamber_on ? "on" : "off");
	fprintf(fp, "\n");
	fprintf(fp, "%-5s %-15s %-24s %-24s %-24s %-24s %-24s %-24s %-24s\n", "# idx", "fam_name", "s[m]", "rx[m]", "px[rad]", "ry[m]", "py[rad]", "de", "dl[m]");
	fprintf(fp, "%-5s %-15s %-24s %-24s %-24s %-24s %-24s %-24s %-24s\n", "# ---", "---------------", str, str, str, str, str, str, str);

	double s = 0;
	for(unsigned int i=0; i<the_ring.size(); ++i) {
		fprintf(fp, "%05i %-15s %+24.17E %+24.17E %+24.17E %+24.17E %+24.17E %+24.17E %+24.17E\n", i+1, the_ring[i].fam_name.c_str(), s, cod[i].rx, cod[i].px, cod[i].ry, cod[i].py, cod[i].de, cod[i].dl);
		s += the_ring[i].length;
	}
	fclose(fp);
	return Status::success;
}

Status::type print_tracking_linepass(const Accelerator& accelerator, const std::vector<Pos<double>>& points, const unsigned int start_element, const std::string& filename) {

	FILE* fp;
	fp = fopen(filename.c_str(), "w");
	if (fp == nullptr) return Status::file_not_opened;

	const std::vector<Element>& the_ring = accelerator.lattice;

	const char str[] = "------------------------";

	print_header(fp);
	fprintf(fp, "# [track_linepass]\n");
	fprintf(fp, "# ebeam_energy[eV]  : %f\n", accelerator.energy);
	fprintf(fp, "# harmonic_number   : %i\n", accelerator.harmonic_number);
	fprintf(fp, "# cavity_state      : %s\n", accelerator.cavity_on ? "on" : "off");
	fprintf(fp, "# radiation_state   : %s\n", accelerator.radiation_on ? "on" : "off");
	fprintf(fp, "# chamber_state     : %s\n", accelerator.vchamber_on ? "on" : "off");
	fprintf(fp, "\n");

	fprintf(fp, "%-5s %-15s %-24s %-24s %-24s %-24s %-24s %-24s %-24s\n", "# idx", "fam_name", "s[m]", "rx[m]", "px[rad]", "ry[m]", "py[rad]", "de", "dl[m]");
	fprintf(fp, "%-5s %-15s %-24s %-24s %-24s %-24s %-24s %-24s %-24s\n", "# ---", "---------------", str, str, str, str, str, str, str);

	unsigned int nr_elements = accelerator.lattice.size();
	double s = 0; for(unsigned int i=0; i<start_element; ++i) s += the_ring[i].length;
	for(unsigned int i=0; i<nr_elements; ++i) {
		unsigned int el_idx = (start_element + i) % nr_elements;
		if (el_idx == 0) s = 0;
		if (points.size()<=i) {
			fprintf(fp, "%05i %-15s %+24.17E %+24.17E %+24.17E %+24.17E %+24.17E %+24.17E %+24.17E\n", el_idx, accelerator.lattice[el_idx].fam_name.c_str(), s, nan(""), nan(""), nan(""), nan(""), nan(""), nan(""));
		} else {
			fprintf(fp, "%05i %-15s %+24.17E %+24.17E %+24.17E %+24.17E %+24.17E %+24.17E %+24.17E\n", el_idx, accelerator.lattice[el_idx].fam_name.c_str(), s, points[i].rx, points[i].px, points[i].ry, points[i].py, points[i].de, points[i].dl);
		}
		s += the_ring[el_idx].length;
	}

	fclose(fp);
	return Status::success;
}

Status::type print_tracking_ringpass(const Accelerator& accelerator, const std::vector<Pos<double>>& points, const unsigned int start_element, const std::string& filename) {

	FILE* fp;
	fp = fopen(filename.c_str(), "w");
	if (fp == nullptr) return Status::file_not_opened;

	print_header(fp);
	fprintf(fp, "# ebeam_energy[eV]  : %f\n", accelerator.energy);
	fprintf(fp, "# harmonic_number   : %i\n", accelerator.harmonic_number);
	fprintf(fp, "# cavity_state      : %s\n", accelerator.cavity_on ? "on" : "off");
	fprintf(fp, "# radiation_state   : %s\n", accelerator.radiation_on ? "on" : "off");
	fprintf(fp, "# chamber_state     : %s\n", accelerator.vchamber_on ? "on" : "off");
	fprintf(fp, "\n");

	unsigned int nr_elements = accelerator.lattice.size();
	for(unsigned int i=0; i<nr_elements; ++i) {
		unsigned int el_idx = (start_element + i) % nr_elements;
		if (points.size()<=i) {
			fprintf(fp, "%05i %-15s %+24.17E %+24.17E %+24.17E %+24.17E %+24.17E %+24.17E\n", el_idx, accelerator.lattice[el_idx].fam_name.c_str(), nan(""), nan(""), nan(""), nan(""), nan(""), nan(""));
		} else {
			fprintf(fp, "%05i %-15s %+24.17E %+24.17E %+24.17E %+24.17E %+24.17E %+24.17E\n", el_idx, accelerator.lattice[el_idx].fam_name.c_str(), points[i].rx, points[i].ry, points[i].de, points[i].px, points[i].py, points[i].dl);
		}
	}

	fclose(fp);
	return Status::success;
}

Status::type print_dynapgrid(const Accelerator& accelerator, const std::vector<DynApGridPoint>& grid, const std::string& label, const std::string& filename, bool print_tunes) {

	FILE* fp;
	fp = fopen(filename.c_str(), "w");
	if (fp == nullptr) return Status::file_not_opened;

	const char str[] = "------------------------";

	print_header(fp);
	fprintf(fp, "# %s\n", label.c_str());
	fprintf(fp, "# ebeam_energy[eV]  : %f\n", accelerator.energy);
	fprintf(fp, "# harmonic_number   : %i\n", accelerator.harmonic_number);
	fprintf(fp, "# cavity_state      : %s\n", accelerator.cavity_on ? "on" : "off");
	fprintf(fp, "# radiation_state   : %s\n", accelerator.radiation_on ? "on" : "off");
	fprintf(fp, "# chamber_state     : %s\n", accelerator.vchamber_on ? "on" : "off");
	fprintf(fp, "\n");
	if (print_tunes) {
		fprintf(fp, "%-5s %-5s %-5s %-5s %-24s %-24s %-24s %-24s %-24s %-24s %-24s %-24s %-24s %-24s %-24s\n",  "# s_e", "l_t", "l_e", "l_p", "start_s[m]", "rx[m]", "ry[m]", "de", "px[rad]", "py[rad]", "dl[m]", "nux1", "nuy1", "nux2", "nuy2");
		fprintf(fp, "%-5s %-5s %-5s %-5s %-24s %-24s %-24s %-24s %-24s %-24s %-24s %-24s %-24s %-24s %-24s\n",  "# ---", "-----", "-----", "-----", str, str, str, str, str, str, str, str, str, str, str);
	} else {
		fprintf(fp, "%-5s %-5s %-5s %-5s %-24s %-24s %-24s %-24s %-24s %-24s %-24s\n",  "# s_e", "l_t", "l_e", "l_p", "start_s[m]", "rx[m]", "ry[m]", "de", "px[rad]", "py[rad]", "dl[m]");
		fprintf(fp, "%-5s %-5s %-5s %-5s %-24s %-24s %-24s %-24s %-24s %-24s %-24s\n",  "# ---", "-----", "-----", "-----", str, str, str, str, str, str, str);
	}

	std::vector<double> s = latt_findspos(accelerator.lattice, latt_range(accelerator.lattice));
	if (print_tunes) {
		for(unsigned int i=0; i<grid.size(); ++i) {
			const Pos<double>& p = grid[i].p;
			fprintf(fp, "%-5i %-5i %-5i %-5i %+24.17E %+24.17E %+24.17E %+24.17E %+24.17E %+24.17E %+24.17E %+24.17E %+24.17E %+24.17E %+24.17E\n",  grid[i].start_element, grid[i].lost_turn, grid[i].lost_element, grid[i].lost_plane, s[grid[i].start_element], p.rx, p.ry, p.de, p.px, p.py, p.dl, grid[i].nux1, grid[i].nuy1, grid[i].nux2, grid[i].nuy2);
		}
	} else {
		for(unsigned int i=0; i<grid.size(); ++i) {
			const Pos<double>& p = grid[i].p;
			fprintf(fp, "%-5i %-5i %-5i %-5i %+24.17E %+24.17E %+24.17E %+24.17E %+24.17E %+24.17E %+24.17E\n",  grid[i].start_element, grid[i].lost_turn, grid[i].lost_element, grid[i].lost_plane, s[grid[i].start_element], p.rx, p.ry, p.de, p.px, p.py, p.dl);
		}
	}

	fclose(fp);
	return Status::success;
}