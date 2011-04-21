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
#include    <snappy.h>
#include    <png++/png.hpp>

static const int PNGSIGSIZE = 8;

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
        Modeldata ( std::string objFileName, std::string mtlFileName,
                    std::string pngFileName );
        Modeldata ( std::string datFileName );
        bool writeDat ();
        bool create ();

        // ====================  ACCESSORS     ================================
        std::string getData ( MD_TYPE dataType );
        png::image<png::rgba_pixel> getImage ();

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
            std::string datData;
            png::image<png::rgba_pixel> pngData;
        } data_;

        void splitFileName ( std::string fileName );
        void readDat ();
        void readModelData ();

}; // -----  end of class Modeldata  -----

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
//      Method:  readDat
// Description:  Reads the .dat file into memory, and then decyphers its
//               content.
//-----------------------------------------------------------------------------
    inline void
Modeldata::readDat ()
{
    // Check if the .dat file has been read.
    std::ifstream datStream(fileNames_.datFileName.c_str());
    if ( !datStream.good() ) {
        std::cout << "Could not read the .dat file.\n";
        exit(1);
    }
    // Convert it to a stream for decompression
    std::ostringstream oss;
    oss << datStream.rdbuf();
    data_.datData = oss.str();
    datStream.close();

    // Get the filesize and an output string.
    size_t fileStringSize = data_.datData.size();
    std::string decompressedString;
    // Decompress
    snappy::Uncompress(data_.datData.c_str(), fileStringSize, 
                       &decompressedString);

    // Make a stream out of the decompressed string
    std::istringstream iss(decompressedString);
    std::ostringstream objStream;
    std::ostringstream mtlStream;
    std::string line;

    // Read line by line the obj and mtl data, and put them in a string.
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
    // Set the strings in memory.
    data_.objData = objStream.str();
    data_.mtlData = mtlStream.str();

    // Get the image dimensions
    size_t width, height;
    iss >> width;
    iss >> height;
    // Create an empty image
    png::image<png::rgba_pixel> image(width, height);
    // For every 4 values encountered in the .dat file, create a pixel, and
    // set the pixel in the image.
    for ( size_t i = 0; i < height; i += 1 ) {
        for ( size_t j = 0; j < width; j += 1 ) {
            png::basic_rgba_pixel<unsigned char> pixel;
            short val;
            iss >> val;
            pixel.red = (unsigned char)val;
            iss >> val;
            pixel.green = (unsigned char)val;
            iss >> val;
            pixel.blue = (unsigned char)val;
            iss >> val;
            pixel.alpha = (unsigned char)val;
            image.set_pixel(j, i, pixel);
        }
    }
    // Set the image in data.
    data_.pngData = image;
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
    // We need to convert the plaintext filestreams into strings via 
    // stringstreams.
    std::ostringstream objOSS;
    objOSS << objStream.rdbuf();
    std::ostringstream mtlOSS;
    mtlOSS << mtlStream.rdbuf();
    // Set into memory.
    data_.objData = objOSS.str();
    data_.mtlData = mtlOSS.str();

    data_.pngData = png::image<png::rgba_pixel>(fileNames_.pngFileName.c_str());

    objStream.close();
    mtlStream.close();
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
        ofs.close();
        return true;
    }
    ofs.close();
    return false;
}		// -----  end of method Modeldata::writeDat  -----

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

    // Get the dimensions of the image.
    size_t width = data_.pngData.get_width();
    size_t height = data_.pngData.get_height();

    oss << width << "\n";
    oss << height << "\n";
    for ( unsigned int i = 0; i < height; i += 1 ) {
        for ( unsigned int j = 0; j < width; j += 1 ) {
            // Each pixel contains 4 values, rgba. We need to insert these
            // separately.
            png::basic_rgba_pixel<unsigned char> pixel = data_.pngData[i][j];
            short val = (unsigned char)pixel.red;
            oss << val << " ";
            val = (unsigned char)pixel.green;
            oss << val << " ";
            val = (unsigned char)pixel.blue;
            oss << val << " ";
            val = (unsigned char)pixel.alpha;
            oss << val << " ";
        }
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
// Description:  Returns the image data.
//-----------------------------------------------------------------------------
    inline png::image<png::rgba_pixel>
Modeldata::getImage ()
{
    return data_.pngData;
}		// -----  end of method Modeldata::getImage  -----

