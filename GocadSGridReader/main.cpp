/**
 * This file is part of "GocadSGridReader". "GocadSGridReader" is free
 * software: you can redistribute it and/or modify it under the terms of
 * the GNU General Public License as published by the Free Software Foundation,
 * either version 3 of the License, or (at your option) any later version.
 *
 * "GocadSGridReader" is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License with
 * "GocadSGridReader". If not, see <http://www.gnu.org/licenses/>.
 *
 * @date 2013-03-01
 * @author Thomas Fischer
 * @file main.cpp
 */

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "tclap/CmdLine.h"

#include "GocadSGridReader.h"

int main(int argc, char* argv[])
{
	TCLAP::CmdLine cmd("Reads parts of a Gocad structured mesh", ' ', "0.1");

	// Define a value argument and add it to the command line.
	// A value arg defines a flag and a type of value that it expects
	TCLAP::ValueArg<std::string> sg_file_arg("s", "sg", "structured grid file name", true, "",
			"string");

	// Add the argument sg_file_arg to the CmdLine object. The CmdLine object
	// uses this Arg to parse the command line.
	cmd.add(sg_file_arg);

	cmd.parse(argc, argv);

	// read the Gocad SGrid
	FileIO::GocadSGridReader reader(sg_file_arg.getValue());
	std::vector<double> const& pnts(reader.getNodes());
	std::vector<double> const& properties(reader.getCellProperties());

	std::cout << "nodes \t flags \t region flags \t properties" << std::endl;
	const std::size_t n(properties.size());
	for (std::size_t k(n - 50); k < n; k++) {
		std::cout << k << " ";
		for (std::size_t i(0); i < 3; ++i) {
			std::cout << pnts[k * 3 + i] << " ";
		}
		std::cout << "\t";
		std::cout << properties[k] << " ";
		std::cout << std::endl;
	}

	return 0;
}
