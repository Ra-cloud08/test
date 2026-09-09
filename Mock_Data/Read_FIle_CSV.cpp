#include <fstream>
#include <iostream>
#include <sstream>// used to split data to piece by piece tha it easier to read
#include <string>

int main() {
    // in .csv we have 170 rows and 5 columns
    const int ROWS = 170, COLS = 5;
    std::string data[ROWS][COLS];// 1 cell can stroe a string
    std::string line; // store 1 line that we read from file just like row
    std::ifstream file("Calories.csv"); // open and read data from file
    int row = 0;

    // reading each line
    while (row < ROWS && std::getline(file, line)) {// read 1 line of file and store in line, repeat 170 times
        // stringstream is a class in sstream library to read or split data in a string
        std::stringstream stream(line);// A stream is like a flow of data that you can read 1 column at a time
        for (int col = 0; col < COLS; col++) {// lopp each column, repeat 5 times
            std::getline(stream, data[row][col], ',');// read data from stream until it meet comma and store it in 2d array
        }
        row++;
    }
    // ouput like table 
    for (int i = 0; i < row; i++) {
        std::cout << i << "\t";
        for (int j = 0; j < COLS; j++) {
            std::cout << data[i][j] << "\t";
        }
        std::cout << '\n';
    }
    return 0;
}
