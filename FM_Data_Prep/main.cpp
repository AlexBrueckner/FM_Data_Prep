#include <iostream>	//cout for everybody's enjoyment
#include <fstream>  //ifstream and ofstream to write stuff and read it
#include <sstream>  //stringstream to build strings
#include <vector>   //vector to hold our stuff
#include <string>   //and a string to hold our text
#include <chrono>   //time measurement

#include "BruckiesUtils.hpp" //to allow splitting strings (thanks for nothing std)


//I'd usually say use namespace std; but somebody at work threw a tantrum at me for doing this so..
//happy std::'ing everything, suckers

//The heart of the dungeon. And yes, I have the pointer-semantic swag.
int main(int argc, char** argv) {

	//Standard InputFile Stream to read 
	std::ifstream filereader;
	//Standard string stream to build the file names
	std::stringstream namebuilder;
	//Standard outputfile stream to write
	std::ofstream filewriter;
	//Line holder
	std::string lineholder;
	//now this is debatable but it seems to be rather quick
	std::vector<std::string> lines;

	std::cout << "Memory prealloc underway.." << std::endl;
	std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
	//Max. number of entries we want to process at once
	unsigned const int maxentries = 750000;
	std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
	std::cout << "Completed. Took " << duration << "ms!" << std::endl;

	//Current number of entries we looked at
	unsigned int numentries = 0;
	//Current number of chunks we produced
	unsigned int numfiles = 0;

	//Reserve memory, this will make things MUCH quicker
	lines.reserve(maxentries);

	//Right so we gotta read this big boy line by line, then split it up into several
	//smaller chunks. All we need is the system name, which is the 3rd Column. Using this,
	//we can later call an online REST API and get some data. Way less than optimal but
	//it ain't exactly my fault when people can't create proper data dumps

	filereader.open("systems.csv");

	//first line is junk, so just toss it over
	std::getline(filereader, lineholder);

	//and carry on as if nothing happened:
	std::cout << "Starting filter process" << std::endl;
	//Read everything. Yes, everything. EVERY. THING.
	std::chrono::high_resolution_clock::time_point filterstart = std::chrono::high_resolution_clock::now();
	while (std::getline(filereader, lineholder)) {

		//Hold on a second, did we already reach the maximum?
		//DUMP THEM ALL.
		if (numentries > maxentries) {
			std::chrono::high_resolution_clock::time_point dumpstart = std::chrono::high_resolution_clock::now();
			std::cout << "Got " << numentries << " entries piled up, I gotta get rid of em." << std::endl;

			

			//Remember the stringstream? We want a bunch of files so we gotta fabricate the names.
			//On that note some string operators would be sexy, y'know what I mean?

			//Now this is a little odd but each time we assign a string we have to reset the internal pointer
			namebuilder.str("\\SegmentedFiles\\");
			namebuilder.str("chunkated_systemnames_");
			namebuilder.seekp(0, std::ios_base::end); //That's better!
			namebuilder << numfiles++ << ".csv";

			//I probably have to run this a few times so we make sure to truncate everything.
			filewriter.open(namebuilder.str().c_str(), std::ofstream::trunc);
			//Now dump everything in there. 
			for (auto s : lines) {
				//Bonus: std::endl flushes the stream for us. Awesome.
				filewriter << s << std::endl;
			}
			//works done, close it all up
			filewriter.close();
			//and reset the namebuilder of course
			namebuilder.str("\\SegmentedFiles\\");
			namebuilder.seekp(0, std::ios_base::end);
			std::chrono::high_resolution_clock::time_point dumpend = std::chrono::high_resolution_clock::now();
			auto dumpduration = std::chrono::duration_cast<std::chrono::milliseconds>(dumpend - dumpstart).count();
			std::cout << "Dumped " << numentries << " chunks for you. Took me " << dumpduration << " ms to do so." << std::endl;
			numentries = 0;

			std::chrono::high_resolution_clock::time_point allocstart = std::chrono::high_resolution_clock::now();
			//This is important: We have to get rid of all the cached data else we're building a data pyramid of doom.
			lines.clear();
			//Just to be safe, realloc
			lines.reserve(maxentries);
			std::chrono::high_resolution_clock::time_point allocend = std::chrono::high_resolution_clock::now();
			auto allocduration = std::chrono::duration_cast<std::chrono::milliseconds>(allocend - allocstart).count();
			std::cout << "I also just purged all the trash for you and got a new container. It took me " << allocduration << " millis to do so." << std::endl;
		}


		//Now behold, remember we only need the third column?
		lines.push_back(split(lineholder, ",")[2]);
		numentries++;
	}
	std::chrono::high_resolution_clock::time_point filterend = std::chrono::high_resolution_clock::now();
	auto totalduration = std::chrono::duration_cast<std::chrono::seconds>(filterend - filterstart).count();
	std::cout << "Job's done. It took me " << totalduration << "seconds to produce " << numfiles<< " chunks." << std::endl;
	system("PAUSE");
	filereader.close();
}