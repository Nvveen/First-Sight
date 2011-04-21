// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// ============================================================================
// 
//       Filename:  undat.cpp
// 
//    Description:  A simple program that compresses and creates .dat files
//                  which are containers for .obj, .mtl and .png files.
// 
//        Version:  1.0
//        Created:  04/15/2011 04:50:33 PM
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Neal van Veen (), nealvanveen@gmail.com
//                  
//                  Copyright 2011, Neal van Veen.
// 
// ============================================================================

#include    <iostream>
#include    <sstream>
#include    <fstream>
#include    <cstdlib>
#include    <vector>
#include    <string>
#include    <FreeImage.h>
#include    <snappy.h>

// Extension types
enum EXT_t {
    OBJ,
    MTL,
    PNG,
    UNKNOWN
};

// ===  FUNCTION  =============================================================
//         Name:  getFileName
//  Description:  Uses a stringstream to capture the filename.
// ============================================================================
    std::string
getFileName ( std::string file )
{
    std::stringstream ss(file);
    std::string fileName;
    getline(ss, fileName, '.');
    return fileName;
}		// -----  end of function getFileName  -----

// ===  FUNCTION  =============================================================
//         Name:  parseExtension
//  Description:  Looks at the identifying extension, and checks which one it
//                corresponds to in our enum of extensions.
// ============================================================================
    EXT_t
parseExtension ( std::string fileName )
{
    std::stringstream ss(fileName);
    std::string ext;
    while ( ss.good() ) getline(ss, ext, '.');
    if ( ext == "obj" ) return OBJ;
    else if ( ext == "mtl") return MTL;
    else if ( ext == "png") return PNG;
    else return UNKNOWN;
}		// -----  end of function parseExtension  -----

// ===  FUNCTION  =============================================================
//         Name:  printHelp
//  Description:  Prints help.
// ============================================================================
    void
printHelp ()
{
    std::cout << "Usage: undat [mode] [file1] {[file 2] [file 3]}\n"
                 "  Modes: \n"
                 "       --extract, -x      Extract a .dat file\n"
                 "       --create,  -c      Create a .dat file out of a .obj, "
                                            ".mtl and .png file. \n";
}		// -----  end of function printHelp  -----

// ===  FUNCTION  =============================================================
//         Name:  createFile
//  Description:  Creates a file by reading the 2 plaintext files, converting
//                the png file with FreeImage, extracting the byte array,
//                and appending everything to one string. This is then
//                compressed with snappy, and written to a .dat file.
// ============================================================================
    void
createFile ( std::vector<std::string> sArgv )
{
    std::cout << "Creating data file...\n";
    // Parse arguments, and set which filename has which extension.
    std::string obj, mtl, png, datName;
    datName = sArgv[1];
    for ( int i = 2; i <= 4; i += 1 ) {
        if ( parseExtension(sArgv[i]) == OBJ )
            obj = sArgv[i];
        else if ( parseExtension(sArgv[i]) == MTL )
            mtl = sArgv[i];
        else if ( parseExtension(sArgv[i]) == PNG )
            png = sArgv[i];
        else if ( parseExtension(sArgv[i]) == UNKNOWN ) {
            std::cerr << "Unknown file type(s): " << sArgv[i];
            std::cerr << "\n";
            exit(1);
        }
    }
    // Create the filename for the end result
    std::string fileName = getFileName(obj);
    fileName += ".dat";
    // Read in the 2 plaintext model files
    std::ifstream objStream(obj.c_str());
    std::ifstream mtlStream(mtl.c_str());
    // Create an output stringstream to capture the data from the model files
    // and delimit them with an EOF delimiter.
    std::ostringstream oss;
    oss << objStream.rdbuf();
    oss << "EOF\n";
    oss << mtlStream.rdbuf();
    oss << "EOF\n";
    // Close the file, since we don't need them anymore
    objStream.close();
    mtlStream.close();

    // Start FreeImage by loading the PNG file and converting it to a 32 bit
    // image format.
    FreeImage_Initialise();
    FIBITMAP* dib = FreeImage_Load(FIF_PNG, png.c_str());
    FIBITMAP* src = FreeImage_ConvertTo32Bits(dib);
    // Get required data information to build a new array with when extracting
    int width = FreeImage_GetWidth(src);
    int height = FreeImage_GetHeight(src);
    int scan_width = FreeImage_GetPitch(src);
    // Insert the data from the PNG after it is converted into a byte array.
    BYTE* bits = new BYTE[height*scan_width];
    FreeImage_ConvertToRawBits(bits, src, scan_width, 32, FI_RGBA_RED_MASK,
                               FI_RGBA_GREEN_MASK, FI_RGBA_BLUE_MASK, TRUE);
    // We don't need FreeImage anymore
    FreeImage_Unload(src);
    FreeImage_DeInitialise();

    // Append everything to the output stringstream
    oss << width << "\n";
    oss << height << "\n";
    oss << scan_width << "\n";
    // Including the bytes (yes, I know it says bits, I just like to mindfuck
    // myself). Also, we need to cast the BYTE class to a short, so the image
    // doesn't get fucked up.
    for ( long i = 0; i < height*scan_width; i += 1 ) {
        oss << (short)bits[i] << " ";
    }
    // Finally, we need to write the stringstream to an actual file.
    std::ofstream outStream(fileName.c_str());
    // For that, we need the stringstream as a string.
    std::string fileString = oss.str();
    // Snappy needs the string size as a parameter.
    size_t fileSize = fileString.size();
    // The string that the compressed data will be put into.
    std::string compressedString;
    // Compress
    snappy::Compress(fileString.c_str(), fileSize, &compressedString);
    // Write the compressed data to the file.
    outStream << compressedString;
    outStream.close();
    std::cout << "Created " << fileName << ".\n";
}		// -----  end of function createFile  -----

// ===  FUNCTION  =============================================================
//         Name:  extractFile
//  Description:  Extract the file by reading the .dat object, decompressing
//                it, seperating the different parts into different files,
//                and specifying how to build the PNG file. It then reads
//                the raw bytes from the file, makes a PNG from it, and
//                saves it.
// ============================================================================
    void
extractFile ( std::string fileName )
{
    std::cout << "Unpacking " << fileName << "...\n";
    // Open the input file
    std::ifstream datStream(fileName.c_str());
    // Stuff it into a string stream
    std::ostringstream oss;
    oss << datStream.rdbuf();
    // Make a string out of the stringstream
    std::string fileString = oss.str();
    // Get its size
    size_t fileSize = fileString.size();
    // Decompress the string
    std::string decompressedString;
    snappy::Uncompress(fileString.c_str(), fileSize, &decompressedString);
    // Make an input stringstream from the result. All data is now read from
    // this input stringstream.
    std::istringstream iss(decompressedString);

    // Get the filename, split it at the . to get its name without the
    // extension, so we can name our objects in the .dat file the same.
    std::stringstream ss(fileName);
    getline(ss, fileName, '.');
    // Define the seperate names.
    std::string obj(fileName+".obj"), mtl(fileName+".mtl");
    std::string png(fileName+".png");
    // Open an output stream for the plaintext files.
    std::ofstream objStream(obj.c_str());
    std::ofstream mtlStream(mtl.c_str());
    std::string line;
    // Get one line at a time until an EOF is encountered, at which point, the
    // reading stops. Every read line is immediately inserted into the
    // filestream.
    while ( iss.good() ) {
        getline(iss, line);
        if ( line == "EOF" ) break;
        objStream << line << "\n";
    }
    // If the break has been encountered, continue with the next file.
    while ( iss.good() ) {
        getline(iss, line);
        if ( line == "EOF" ) break;
        mtlStream << line << "\n";
    }

    // We don't need the plaintext files anymore.
    objStream.close();
    mtlStream.close();
    // Get the required image data.
    int width, height, scan_width;
    iss >> width;
    iss >> height;
    iss >> scan_width;
    // Begin FreeImage, and read in the bytes, while casting them back to
    // BYTE values.
    FreeImage_Initialise();
    BYTE* bits = new BYTE[height*scan_width];
    for ( long i = 0; i < height*scan_width; i += 1 ) {
        short val;
        iss >> val;
        bits[i] = (BYTE)val;
    }
    // Convert the BYTE array to a png.
    FIBITMAP* dib = FreeImage_ConvertFromRawBits(bits, width, height, 
                                                scan_width, 32, 
                                                FI_RGBA_RED_MASK,
                                                FI_RGBA_GREEN_MASK,
                                                FI_RGBA_BLUE_MASK,
                                                FALSE);
    // Save the PNG
    FreeImage_Save(FIF_PNG, dib, png.c_str());
    FreeImage_DeInitialise();
    // Close the datastream.
    datStream.close();
    std::cout << "Unpacked " << obj << ".\n";
    std::cout << "Unpacked " << mtl << ".\n";
    std::cout << "Unpacked " << png << ".\n";
}		// -----  end of function extractFile  -----

    int
main ( int argc, char *argv[] )
{
    // Convert arg list to a vector
    std::vector<std::string> sArgv;
    for ( int i = 0; i < argc; i += 1 )
        sArgv.push_back(std::string(argv[i]));
    if ( argc == 1 || (argc >= 2 && sArgv[1] == "--help") ) {
        printHelp();
    }
    else if ( argc > 1 ) {
        if ( sArgv[1] == "--create" || sArgv[1] == "-c" ) {
            if ( argc != 5 ) {
                std::cout << "Invalid number of parameters.\n";
                printHelp();
                exit(1);
            }
            else {
                createFile(sArgv);
            }
        }
        else if ( sArgv[1] == "--extract" || sArgv[1] == "-x" ) {
            if ( argc != 3 ) {
                std::cerr << "Invalid number of parameters.\n";
                printHelp();
                exit(1);
            }
            else {
                std::string fileName = sArgv[2];
                extractFile(fileName);
            }
        }
    }
}				// ----------  end of function main  ----------
