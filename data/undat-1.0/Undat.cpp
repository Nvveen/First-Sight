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
//       Filename:  Undat.cpp
// 
//    Description:  A simple program that compresses and creates .dat files
//                  which are containers for .obj, .mtl and .png files. It uses
//                  the special header file for this.
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
#include    <cstdlib>
#include    <vector>
#include    <string>
#include    "Undat.h"

// Extension types
enum EXT_t {
    OBJ,
    MTL,
    PNG,
    UNKNOWN
};

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
                 "       --extract, -x      Extract a .dat file\n";
}		// -----  end of function printHelp  -----

// ===  FUNCTION  =============================================================
//         Name:  createFile
//  Description:  Creates a file by making a ModelData object, and using that
//                to write to a .dat file.
// ============================================================================
    void
createFile ( std::vector<std::string> sArgv )
{
    std::cout << "Creating data file...\n";
    // Parse arguments, and set which filename has which extension.
    std::string obj, mtl, png;
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
    // Create modeldata
    ModelData data(obj, mtl, png);
    if ( !data.create() ) {
        std::cerr << "Creating the data file failed horribly.\n";
        exit(1);
    }
    // Write data contained in the ModelData object to their respective files.
    if ( !data.writeDat() ) {
        std::cerr << "Failed to create the .dat file.\n";
        exit(1);
    }
    else {
        std::cout << "Successfully created the .dat file.\n";
    }
}		// -----  end of function createFile  -----

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
        else {
            std::cout << "Did not recognize the modes.\n";
            printHelp();
            exit(1);
        }
    }
}				// ----------  end of function main  ----------
