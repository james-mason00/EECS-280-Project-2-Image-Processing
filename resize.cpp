#include <iostream>
#include <fstream>
#include "processing.hpp"
using namespace std;

int main(int argc, char **argv) {
    //check number of arguments
    if(argc != 4 && argc != 5) {
        cout << "Usage: resize.exe IN_FILENAME OUT_FILENAME WIDTH [HEIGHT]\n"
        << "WIDTH and HEIGHT must be less than or equal to original" << endl;
        return 1;
    }

    string file_name_in = argv[1];
    string file_name_out = argv[2];
    ifstream fin(file_name_in);
    ofstream fout(file_name_out);
    Image img;
    Image_init(&img, fin);
    int newWidth = stoi(argv[3]);
    int newHeight = Image_height(&img);
    
    //if there is a 5th argument, its new height
    if(argc == 5) {
        newHeight = stoi(argv[4]);
    }

    //check files can be opened
    if(!fout.is_open()) {
        cout << "Error opening file: " << file_name_out << endl;
        return 1;
    }
    if(!fin.is_open()) {
        cout << "Error opening file: " << file_name_in << endl;
        return 1;
    }
    
    if(newWidth <= 0 || newWidth > Image_width(&img)) {
        cout << "Usage: resize.exe IN_FILENAME OUT_FILENAME WIDTH [HEIGHT]\n"
        << "WIDTH and HEIGHT must be less than or equal to original" << endl;
        return 1;
    }

    if(newHeight <= 0 || newHeight > Image_height(&img)) {
        cout << "Usage: resize.exe IN_FILENAME OUT_FILENAME WIDTH [HEIGHT]\n"
        << "WIDTH and HEIGHT must be less than or equal to original" << endl;
        return 1;
    }

    seam_carve(&img, newWidth, newHeight);
    Image_print(&img, fout);
    
    return 0;

}
