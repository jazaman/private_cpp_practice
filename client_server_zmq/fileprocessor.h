/*
 * fileprocessor.h
 *
 *  Created on: Apr 11, 2017
 *      Author: jamil
 */

#ifndef FILEPROCESSOR_H_
#define FILEPROCESSOR_H_

#include <string>

class FileProcessor {
public:
	FileProcessor();
	virtual ~FileProcessor();
	void read_file(const std::string& filename);
};

#endif /* FILEPROCESSOR_H_ */
