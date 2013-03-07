/**
 * @file GocadSGridReader.cpp
 * @author Thomas Fischer
 * @date Mar 7, 2013
 * @brief 
 *
 * @copyright
 * Copyright (c) 2013, OpenGeoSys Community (http://www.opengeosys.org)
 *            Distributed under a Modified BSD License.
 *              See accompanying file LICENSE.txt or
 *              http://www.opengeosys.org/LICENSE.txt
 */

#include "GocadSGridReader.h"

#include <fstream>
#include <sstream>
#include <iostream>

#include <boost/tokenizer.hpp>

namespace FileIO
{

GocadSGridReader::GocadSGridReader(std::string const& fname) :
		_fname(fname), _path(_fname.substr(0, _fname.find_last_of("/\\") + 1))
{
	// check if file exists
	std::ifstream in(_fname.c_str());
	if (!in) {
		std::cout << "Could not open " << _fname << "." << std::endl;
		in.close();
		return;
	}

	// read information in the stratigraphic grid file
	std::string line;
	while (std::getline(in, line)) {
		if (line.find("AXIS_N ") != std::string::npos) {
			parseDims(line);
		}
		if (line.find("POINTS_FILE ") != std::string::npos) {
			parsePointsFileName(line);
		}
		if (line.find("PROP_FILE ") != std::string::npos) {
			parsePropertiesFileName(line);
		}
//		if (line.find("FLAGS_FILE ") != std::string::npos
//				&& line.find("REGION_FLAGS_FILE ") == std::string::npos) {
//			parseFlagsFileName(line);
//		}
//		if (line.find("REGION_FLAGS_FILE ") != std::string::npos) {
//			parseRegionFlagsFileName(line);
//		}
	}

	readNodesBinary();
	readPropertiesBinary();
}

GocadSGridReader::~GocadSGridReader()
{
}

void GocadSGridReader::parseDims(std::string const& line)
{
	std::size_t x_dim(0), y_dim(0), z_dim(0);
	boost::tokenizer<> tok(line);
	auto it(tok.begin());
	it++; // overread token "AXIS"
	it++; // overread "N"
	std::stringstream ssx(*(it), std::stringstream::in | std::stringstream::out);
	ssx >> x_dim;
	it++;
	std::stringstream ssy(*it, std::stringstream::in | std::stringstream::out);
	ssy >> y_dim;
	it++;
	std::stringstream ssz(*it, std::stringstream::in | std::stringstream::out);
	ssz >> z_dim;
	_index_calculator = IndexCalculator(x_dim, y_dim, z_dim);
	_nodes.resize(_index_calculator._n_nodes * 3);
	_properties.resize(_index_calculator._n_cells);
}

void GocadSGridReader::parsePointsFileName(std::string const& line)
{
	std::size_t beg_pos(line.find_first_of(" ") + 1);
	_pnts_fname = _path + line.substr(beg_pos, line.length() - beg_pos);
}

void GocadSGridReader::parsePropertiesFileName(std::string const& line)
{
	std::size_t beg_pos(line.find_first_of(" ") + 3);
	_properties_fname = _path + line.substr(beg_pos, line.length() - beg_pos);
}

void GocadSGridReader::parseFlagsFileName(std::string const& line)
{
	std::size_t beg_pos(line.find_first_of(" ") + 1);
	_flags_fname = _path + line.substr(beg_pos, line.length() - beg_pos);
}

void GocadSGridReader::parseRegionFlagsFileName(std::string const& line)
{
	std::size_t beg_pos(line.find_first_of(" ") + 1);
	_region_flags_fname = _path + line.substr(beg_pos, line.length() - beg_pos);
}

void GocadSGridReader::readNodesBinary()
{
	std::ifstream in(_pnts_fname.c_str(), std::ios::in | std::ios::binary);
	if (!in) {
		std::cout << "could not open file " << _pnts_fname << std::endl;
		return;
	}

	char inbuff[12], reword[4];

	for (std::size_t k(0); k < _index_calculator._n_nodes; k++) {
		in.read((char*) inbuff, 12 * sizeof(char));
		for (std::size_t i = 0; i < 12; i += 4) {
			for (std::size_t j = 0; j < 4; j++) {
				reword[j] = inbuff[i + 3 - j];
			}
			_nodes[3 * k + i / 4] = static_cast<double>(*reinterpret_cast<float*>(&reword[0]));
		}
	}
	in.close();
}

void GocadSGridReader::readPropertiesBinary()
{
	std::ifstream in(_properties_fname.c_str());
	if (!in) {
		std::cout << "Could not open " << _properties_fname << "." << std::endl;
		in.close();
		return;
	}

	char inbuff[4], reword[4];

	const std::size_t n_properties(_properties.size());
	for (std::size_t k(0); k < n_properties; k++) {
		in.read((char*) inbuff, 4 * sizeof(char));
		if (in) {
			for (std::size_t j = 0; j < 4; j++) {
				reword[j] = inbuff[3 - j];
			}
			_properties[k] = static_cast<double>(*reinterpret_cast<float*>(&reword[0]));
		} else {
			k = n_properties;
		}
	}
}


//void GocadSGridReader::readFlagsBinary(std::size_t n_flags,
//		std::vector<double> &flags)
//{
//	std::ifstream in(_flags_fname.c_str());
//	if (!in) {
//		std::cout << "Could not open " << _flags_fname << "." << std::endl;
//		in.close();
//		return;
//	}
//
//	char inbuff[4], reword[4];
//
//	for (std::size_t k(0); k < n_flags; k++) {
//		in.read((char*) inbuff, 4 * sizeof(char));
//		for (std::size_t i = 0; i < 4; i += 4) {
//			for (std::size_t j = 0; j < 4; j++) {
//				reword[j] = inbuff[i + 3 - j];
//			}
//			if (k > n_flags - 20)
//				std::cout << (*reinterpret_cast<int*>(&reword[0])) << std::endl;
//			flags[4 * k + i / 4] = static_cast<double>(*reinterpret_cast<float*>(&reword[0]));
//		}
//	}
//}
//
//void readBinarySGridRegionFlags(std::string const& fname, std::size_t n_region_flags,
//		std::vector<double> &region_flags)
//{
//	std::ifstream in(fname.c_str());
//	if (!in) {
//		std::cout << "Could not open " << fname << "." << std::endl;
//		in.close();
//		return;
//	}
//
//	char inbuff[16], reword[4];
//
//	for (std::size_t k(0); k < n_region_flags; k++) {
//		in.read((char*) inbuff, 16 * sizeof(char));
//		for (std::size_t i = 0; i < 16; i += 4) {
//			for (std::size_t j = 0; j < 4; j++) {
//				reword[j] = inbuff[i + 3 - j];
//			}
//			region_flags[4 * k + i / 4] =
//					static_cast<double>(*reinterpret_cast<float*>(&reword[0]));
//		}
//	}
//}

} // end namespace FileIO
