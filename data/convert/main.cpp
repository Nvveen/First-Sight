#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>

int main ( int argc, char *argv[] ) {
    std::string arg1;
    if ( argc > 1 )
        arg1 = argv[1];
    std::string fileName;
    std::string fileOut;
    std::string helpText = "Usage: program [option] [file in] [file out]\n"
                           "Options:\n"
                           "    -h, --help     Display this help text. \n";
    if ( argc == 3 ) {
        fileName = argv[1];
        fileOut = argv[2];
    }
    else if ( argc == 2 && (arg1 == "-h" || arg1 == "--help" ) ) {
        std::cout << helpText;
    }
    else {
        std::cerr << "Invalid number of arguments.\n";
        std::cout << helpText;
        exit(1);
    }
    if ( fileName == fileOut ) {
        std::cerr << "Can't output to the same file.\n";
        exit(1);
    }
    std::ifstream file(fileName.c_str());
    if ( !file.is_open() ) {
        std::cerr << "Error opening " << fileName << ".\n";
        exit(1);
    }
    std::ofstream out(fileOut.c_str(), std::ios::binary);
    std::string line;
    getline(file, line);
    size_t commaPos = line.find(',');
    line.erase(commaPos, line.size()-commaPos);
    std::istringstream iss(line);
    int size;
    iss >> size;
    out << size << " ";
    unsigned long tempr(0), tempg(0), tempb(0), tempa(0);
    unsigned long maskr(255 << 24), maskg(255 << 16), maskb(255 << 8), 
                  maska(255);
    while ( file.good() ) {
        getline(file, line);
        std::istringstream iss2(line);
        while ( iss2.good() && line.size() > 0 ) {
            getline(iss2, line, ',');
            line.erase(0, 1);
            std::istringstream hex(line);
            hex.setf(std::ios::hex, std::ios::basefield);
            hex >> tempr;
            tempg = tempb = tempa = tempr;
            tempr &= maskr; tempg &= maskg; tempb &= maskb;
            tempa &= maska;
            tempr = tempr >> 24; tempg = tempg >> 16; tempb = tempb >> 8;
            out << (unsigned char)tempr << " ";
            out << (unsigned char)tempg << " ";
            out << (unsigned char)tempb << " ";
            out << (unsigned char)tempa << " ";
        }
    }
    file.close();
    out.close();
}
