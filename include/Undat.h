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
//       Filename:  Undat.h
// 
//    Description:  Header file for the program that compresses and creates
//                  .dat files, which are containers for .obj, .mtl and .png
//                  files.
// 
//        Version:  1.0
//        Created:  04/19/2011 06:30:33 PM
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Neal van Veen (), nealvanveen@gmail.com
// 
//                  Copyright 2011, Neal van Veen.
//
// ============================================================================

#include    <iostream>
#include    <fstream>
#include    <sstream>
#include    <SFML/Graphics.hpp>
#include    <snappy.h>

enum MD_TYPE {
    MD_OBJ,
    MD_MTL,
    MD_PNG,
    MD_DAT
};

// ============================================================================
//        Class:  Modeldata
//  Description:  
// ============================================================================
class Modeldata
{
    public:
        // ====================  LIFECYCLE     ================================
        Modeldata ( std::string datFileName );// constructor
        Modeldata ( std::string objFileName, std::string mtlFileName,
                    std::string pngFileName );
        void readDat ();
        void readModelData ();
        bool writeDat ();
        bool writeModelData ();
        bool extract ();
        bool create ();
        void splitFileName ( std::string fileName );

        // ====================  ACCESSORS     ================================
        std::string getData ( MD_TYPE dataType );
        sf::Image getImage ();

        // ====================  MUTATORS      ================================

        // ====================  OPERATORS     ================================

    protected:
        // ====================  DATA MEMBERS  ================================

    private:
        // ====================  DATA MEMBERS  ================================
        std::string dataName_;
        // The next struct defines all filenames each object gets.
        struct {
            std::string objFileName;
            std::string mtlFileName;
            std::string pngFileName;
            std::string datFileName;
        } fileNames_; 

        // This struct stores all the raw data in the .dat file and the
        // modeldata files in strings and an sf::Image format for the png.
        struct undatData {
            std::string objData;
            std::string mtlData;
            sf::Image pngData;
            std::string datData;
        } data_;

}; // -----  end of class Modeldata  -----

//-----------------------------------------------------------------------------
//       Class:  Modeldata
//      Method:  Modeldata
// Description:  constructor
//-----------------------------------------------------------------------------
    inline 
Modeldata::Modeldata ( std::string datFileName )
{
    fileNames_.datFileName = datFileName;
    splitFileName(fileNames_.datFileName);
    fileNames_.objFileName = dataName_ + ".obj";
    fileNames_.mtlFileName = dataName_ + ".mtl";
    fileNames_.pngFileName = dataName_ + ".png";
    readDat();
}  // -----  end of method Modeldata::Modeldata  (constructor)  -----

//-----------------------------------------------------------------------------
//       Class:  Modeldata
//      Method:  Modeldata
// Description:  constructor
//-----------------------------------------------------------------------------
    inline
Modeldata::Modeldata ( std::string objFileName, std::string mtlFileName,
                       std::string pngFileName )
{
    fileNames_.objFileName = objFileName;
    fileNames_.mtlFileName = mtlFileName;
    fileNames_.pngFileName = pngFileName;
    splitFileName(fileNames_.objFileName);
    fileNames_.datFileName = dataName_ + ".dat";
    readModelData();
}  // -----  end of method Modeldata::Modeldata  (constructor)  -----

//-----------------------------------------------------------------------------
//       Class:  Modeldata
//      Method:  readDat
// Description:  Reads a .dat file into memory.
//-----------------------------------------------------------------------------
    inline void
Modeldata::readDat ()
{
    std::ifstream datStream(fileNames_.datFileName.c_str());
    if ( !datStream.good() ) {
        std::cerr << "Could not open the .dat file\n";
        exit(1);
    }
    // We need the file as a string, so use the rdbuf method for conversion
    std::ostringstream oss;
    oss << datStream.rdbuf();
    data_.datData = oss.str();
}		// -----  end of method Modeldata::readDat  -----

//-----------------------------------------------------------------------------
//       Class:  Modeldata
//      Method:  readModelData
// Description:  This method reads all 3 files belonging to a .dat file into
//               memory.
//-----------------------------------------------------------------------------
    inline void
Modeldata::readModelData ()
{
    std::ifstream objStream(fileNames_.objFileName.c_str());
    std::ifstream mtlStream(fileNames_.mtlFileName.c_str());
    if ( !objStream.good() ) {
        std::cerr << "Could not open the .obj file.\n";
        exit(1);
    }
    if ( !mtlStream.good() ) {
        std::cerr << "Could not open the .mtl file.\n";
        exit(1);
    }
    // We need to convert the filestreams into strings via stringstreams.
    std::ostringstream objOSS;
    objOSS << objStream.rdbuf();
    std::ostringstream mtlOSS;
    mtlOSS << mtlStream.rdbuf();
    // Set into memory.
    data_.objData = objOSS.str();
    data_.mtlData = mtlOSS.str();

    // Load the image.
    sf::Image image;
    if ( !image.LoadFromFile(fileNames_.pngFileName.c_str()) ) {
        std::cerr << "Could not load the PNG file.\n";
        exit(1);
    }
    // Set image into memory.
    data_.pngData = image;
}		// -----  end of method Modeldata::readModelData  -----

//-----------------------------------------------------------------------------
//       Class:  Modeldata
//      Method:  writeDat
// Description:  Writes the .dat contents from memory into a .dat file.
//-----------------------------------------------------------------------------
    inline bool
Modeldata::writeDat ()
{
    // Writes the contents of the .dat file from memory into an actual .dat
    // file.
    std::ofstream ofs(fileNames_.datFileName.c_str());
    if ( ofs.good () ) {
        ofs << data_.datData;
        return true;
    }
    else {
        return false;
    }
}		// -----  end of method Modeldata::writeDat  -----

//-----------------------------------------------------------------------------
//       Class:  Modeldata
//      Method:  writeModelData
// Description:  Writes the contents of the modeldata objects from memory
//               into files.
//-----------------------------------------------------------------------------
    inline bool
Modeldata::writeModelData ()
{
    std::ofstream objStream(fileNames_.objFileName.c_str());
    std::ofstream mtlStream(fileNames_.mtlFileName.c_str());
    if ( objStream.good() ) {
        objStream << data_.objData;
    }
    else {
        return false;
    }
    if ( mtlStream.good() ) {
        mtlStream << data_.mtlData;
    }
    else {
        return false;
    }
    // Image saving isn't done via streams, but via the SFML method.
    data_.pngData.SaveToFile(fileNames_.pngFileName);
    return true;
}		// -----  end of method Modeldata::writeModelData  -----

//-----------------------------------------------------------------------------
//       Class:  Modeldata
//      Method:  extract
// Description:  Extract doesn't actually extract the files, but rather uses
//               the data stored in memory about the .dat file by decompressing
//               it, and seperately reading the contents of the objects into 
//               memory.
//-----------------------------------------------------------------------------
    inline bool
Modeldata::extract ()
{
    if ( !fileNames_.datFileName.empty() ) {
        // Define size and a location to store the decompressed data.
        size_t fileStringSize = data_.datData.size();
        std::string decompressedString;
        // Use snappy to decompress.
        snappy::Uncompress(data_.datData.c_str(), fileStringSize, 
                           &decompressedString);

        // Convert the string into a stringstream.
        std::istringstream iss(decompressedString);
        std::ostringstream objStream;
        std::ostringstream mtlStream;

        std::string line;
        // Walk through the stringstream, while adding each line to the 
        // objects' stringstreams. If an EOF is encountered, the next stream is
        // used.
        while ( iss.good() ) {
            getline(iss, line);
            if ( line == "EOF" ) break;
            objStream << line << "\n";
        }
        while ( iss.good() ) {
            getline(iss, line);
            if ( line == "EOF" ) break;
            mtlStream << line << "\n";
        }
        // Set the data  into memory.
        data_.objData = objStream.str();
        data_.mtlData = mtlStream.str();

        // Height and width is stored in memory to build the array.
        unsigned int width, height;
        // Read it in.
        iss >> width;
        iss >> height;
        size_t size = width * height * 4;
        sf::Uint8* pixels = new sf::Uint8[size];
        // Iterate through the remaining contents of the .dat file to build
        // up the array of pixels.
        for ( unsigned int i = 0; i < size; i += 1 ) {
            short val;
            iss >> val;
            pixels[i] = (sf::Uint8)val;
        }
        // The pixels can be used to reconstruct another sf::Image.
        sf::Image image;
        if ( !image.LoadFromPixels(width, height, pixels) ) {
            return false;
        }
        // The image can now be set in memory.
        data_.pngData = image;

        return true;
    }
    else {
        return false;
    }
}		// -----  end of method Modeldata::extract  -----

//-----------------------------------------------------------------------------
//       Class:  Modeldata
//      Method:  create
// Description:  Creates the memory representation of a .dat file from the 
//               data read into the modeldata.
//-----------------------------------------------------------------------------
    inline bool
Modeldata::create ()
{
    // Set a stringstream to serialize the plaintext data.
    std::ostringstream oss;
    oss << data_.objData;
    oss << "EOF\n";
    oss << data_.mtlData;
    oss << "EOF\n";

    // Create a pixel array out of the image set in memory.
    sf::Image image = data_.pngData;
    const sf::Uint8* pixels = image.GetPixelsPtr();
    // Get its width and height
    size_t size = image.GetWidth() * image.GetHeight() * 4;
    // Serialize width and height
    oss << image.GetWidth() << "\n";
    oss << image.GetHeight() << "\n";
    // Serialize the array of pixels
    for ( unsigned int i = 0; i < size; i += 1 ) {
        oss << (short)pixels[i] << " ";
    }

    // Create a string out of the stringstream to compress it.
    std::string fileString = oss.str();
    std::string compressedString;
    // Set the size
    size_t fileSize = fileString.size();
    // Compress
    snappy::Compress(fileString.c_str(), fileSize, &compressedString);

    // Set the data in memory.
    data_.datData = compressedString;
    return true;
}		// -----  end of method Modeldata::create  -----

//-----------------------------------------------------------------------------
//       Class:  Modeldata
//      Method:  splitFileName
// Description:  Splits a file name from its suffix and sets it in memory.
//-----------------------------------------------------------------------------
    inline void
Modeldata::splitFileName ( std::string fileName )
{
    std::stringstream ss(fileName);
    getline(ss, dataName_, '.');
}		// -----  end of method Modeldata::splitFileName  -----

//-----------------------------------------------------------------------------
//       Class:  Modeldata
//      Method:  getData
// Description:  Returns, depending on the datatype given, the data set in
//               memory.
//-----------------------------------------------------------------------------
    inline std::string
Modeldata::getData ( MD_TYPE dataType )
{
    if ( dataType == MD_OBJ ) return data_.objData;
    if ( dataType == MD_MTL ) return data_.mtlData;
    if ( dataType == MD_DAT ) return data_.datData;
    std::cerr << "Invalid datatype given.\n";
    exit(1);
}		// -----  end of method Modeldata::getData  -----

//-----------------------------------------------------------------------------
//       Class:  Modeldata
//      Method:  getImage
// Description:  Returns the PNG image.
//-----------------------------------------------------------------------------
    inline sf::Image
Modeldata::getImage ()
{
    return data_.pngData;
}		// -----  end of method Modeldata::getImage  -----

