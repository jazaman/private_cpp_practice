/*
 * fileprocessor.cpp
 *
 *  Created on: Apr 11, 2017
 *      Author: jamil
 */
#include "fileprocessor.h"
#include <vector>
#include <chrono>
#include <fstream>
#include <iostream>


FileProcessor::FileProcessor() {
	// TODO Auto-generated constructor stub

}

FileProcessor::~FileProcessor() {
	// TODO Auto-generated destructor stub
}

void FileProcessor :: read_file (const std::string& file_name) {
    std::ifstream input( file_name, std::ios::binary );
	//std::ofstream output( "C:\\myfile.gif", std::ios::binary );

	/*
	std::copy(
	    std::istreambuf_iterator<char>(input),
	    std::istreambuf_iterator<char>( ),
	    std::ostreambuf_iterator<char>(output));
	    */
	std::vector<char> buffer((
	            std::istreambuf_iterator<char>(input)),
	            (std::istreambuf_iterator<char>()));

	std::cout << "The file size is " << buffer.size()/1000000 << "MB" << std::endl;
}

