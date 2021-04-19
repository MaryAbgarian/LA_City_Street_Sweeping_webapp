#include "CSVReader.h"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

struct segment {
    unsigned int ID;
    unsigned int zip;
    float length;
    std::string route;
//    char avgDebris; //-1 if unknown
};


//IMPORTANT: Scaling floats up by 100 so that the difference between segments registers
int64_t euclidianDistance(float x1, float y1, float x2, float y2) {
    x1 *= 100;
    y1 *= 100;
    x2 *= 100;
    y2 *= 100;
    return sqrt(pow((x1 - x2), 2) + pow((y1 - y2), 2));
}

//generates a distance matrix
//This has the potential to take a looooot of memory.
std::vector<std::vector<int64_t>> generateDistanceMatrix(const std::vector<segment>& segments) {
    std::vector<std::vector<int64_t>> distance_matrix(segments.size());
    for (int i = 0; i < segments.size(); i++) {
        distance_matrix[i].resize(segments.size());
        for (int j = 0; j < segments.size(); j++) {
            //Euclidian distance generator
            int64_t distance = 0;
//            int64_t distance = euclidianDistance(segments[i].x, segments[i].y, segments[j].x, segments[j].y);
            distance_matrix[i][j] = distance;
        }
    }

    return distance_matrix;
}

void printDistanceMatrix(const std::vector<std::vector<int64_t>>& matrix) {
    for (int i = 0; i < matrix.size(); i++) {
        std::cout << "{";
        for (int j = 0; j < matrix[i].size(); j++) {
            std::cout << matrix[i][j];
            if (j != matrix[i].size() - 1)
                std::cout << ", ";
        }
        std::cout << "}," << std::endl;
    }
}

//SOURCE: https://stackoverflow.com/questions/1120140/how-can-i-read-and-parse-csv-files-in-c
int main()
{
    std::ifstream file("C:\\Users\\Owner\\Desktop\\Clean_Street_Routes.csv");
    if (!file.is_open()) {
        std::cout << "File failed to open" << std::endl;
        return -1;
    }

    //Determine which rows contain the data we need
    char xRow = -1, yRow = -1, IDRow = -1, lenRow = -1, toRow = -1, fromRow = -1, nameRow = -1; //, gridRow = -1;
    char zipRow = 0, routeRow = 0;
    auto row = CSVRange(file).begin();
    for (int i = 0; i < row->size(); i++) {
        //std::cout << (*row)[i] << std::endl;
  
//        if ((*row)[i] == "LA NE  MILES")    xRow = i;
//        else if ((*row)[i] == "CL  MILES")  yRow = i;
          if ((*row)[i] == "ID")   IDRow = i;
          else if ((*row)[i] == "Shape__Length") lenRow = i;
          else if ((*row)[i] == "ZIP_R") zipRow = i;
          else if ((*row)[i] == "Route") routeRow = i;
//        else if ((*row)[i] == "ST  TO")     toRow = i;
//        else if ((*row)[i] == "ST  FROM")   fromRow = i;
//        else if ((*row)[i] == "ST  NAME")   nameRow = i;
    }
    
    //Populate the streetsegments
    //Make a note of which route maps to which segment
    std::vector<segment> segments;
    std::unordered_map<std::string, uint16_t> routeToSegment;
    segments.reserve(120000);
    for (auto& r : CSVRange(file))
    {
        segment s;
        s.ID = std::stoi(std::string{ r[IDRow] });
        s.length = 0;
        //s.length = std::stof(std::string{ r[lenRow] }); //Unable to convert to float after segment 1078
        s.zip = std::stoi(std::string{ r[zipRow] });
        s.route = std::string{ r[routeRow] };
        //s.avgDebris = -1;
        //s.x = std::stof( std::string { r[xRow] });
        //s.y = std::stof( std::string { r[yRow] });
        //s->gridID = std::stoi( std::string { r[gridRow] }); //Crashes because the cell can be null
        //s.name = std::string{ r[nameRow] };
        //s.to   = std::string{ r[toRow] };
        //s.from = std::string{ r[fromRow] };

        segments.push_back(s);

        //If the routerow exists, then we have some fun stuff to do
    }
    
    std::cout << "Finished!" << std::endl;

//    printDistanceMatrix(generateDistanceMatrix(segments));
}