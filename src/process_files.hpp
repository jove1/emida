#pragma once
#include <filesystem>
#include <fstream>
#include "get_offset.hpp"
#include "load_tiff.hpp"
#include "slice_picture.hpp"
#include "draw_tiff.hpp"
#include "args_parser.hpp"

namespace emida
{

template<typename T>
void write_offsets(const params& a, std::vector<vec2<T>>& offsets, const std::vector<std::array<T, 6>>& coefs)
{
	for (size_t i = 0; i < offsets.size(); ++i)
	{
		//sometimes the resulting float is outputted as nan and sometimes as nan(ind). Normalize that here.
		if (std::isnan(offsets[i].x))
			offsets[i].x = std::numeric_limits<T>::quiet_NaN();
		if (std::isnan(offsets[i].y))
			offsets[i].y = std::numeric_limits<T>::quiet_NaN();
		if(a.write_coefs)
			printf("%llu %llu %f %f %f %f %f %f %f %f\n", a.slice_mids[i].x, a.slice_mids[i].y, offsets[i].x, offsets[i].y, coefs[i][0], coefs[i][1], coefs[i][2], coefs[i][3], coefs[i][4], coefs[i][5]);
		else
			printf("%llu %llu %f %f\n", a.slice_mids[i].x, a.slice_mids[i].y, offsets[i].x, offsets[i].y);
	}
}

template<typename T>
inline void process_files(const params& a)
{
	stopwatch sw(true, 3);

	auto slice_begins = a.slice_mids;
	for (auto& o : slice_begins)
		o = o - (a.slice_size / 2);

	size2_t pic_size;
	if(!get_size(a.initial_file_name, pic_size))
		return;

	std::vector<uint16_t> initial_raster(pic_size.area());
	if (!load_tiff(a.initial_file_name, initial_raster.data(), pic_size))
		return;

	gpu_offset<T, uint16_t> offs(pic_size, &slice_begins, a.slice_size, a.cross_size, a.cross_pol, a.fitting_size);
	offs.allocate_memory(initial_raster.data());

	

	std::ifstream infile(a.deformed_list_file_name);
	std::string line;
	while (std::getline(infile, line))
	{
		std::stringstream iss(line);
		double x, y;
		std::string fname;
		iss >> x;
		iss >> y;
		iss.ignore();
		std::getline(iss, fname);


		//TODO: allocate cuda host memory to avoid copying the data twice
		std::vector<uint16_t> deformed_raster(pic_size.area());
		bool OK = true;
		OK &= load_tiff(fname, deformed_raster.data(), pic_size); sw.tick("Load tiff: ", 2);
		if (!OK)
			continue;

		auto [offsets, coefs] = offs.get_offset(deformed_raster.data());
		sw.tick("Get offset: ", 2);

		if (a.analysis)
			stopwatch::global_stats.inc_histogram(offsets);
	
		printf("%f %f %llu %s\n", x, y, a.slice_mids.size(), fname.c_str());
		write_offsets(a, offsets, coefs);
		fprintf(stderr,".");
		fflush(stderr);
		sw.tick("Write offsets: ", 2);

		sw.tick("ONE: ", 1);
	}
	fprintf(stderr,"\n");
	sw.total();
	if (a.analysis)
	{
		std::cerr << "Border X: " << stopwatch::global_stats.border.x << "\n";
		std::cerr << "Border Y: " << stopwatch::global_stats.border.y << "\n";
		std::cerr << "Total pics: " << stopwatch::global_stats.total_pics << "\n";
		stopwatch::global_stats.write_histogram();
	}
}

}
