#include "CSVReader.h"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

struct segment {
    float x, y;
    unsigned int sectID;
    unsigned int length;
    //    unsigned int gridID;
    std::string to, from, name;
};

//IMPORTANT: Scaling floats up by 100 so that the difference between segments registers
int64_t euclidianDistance(float x1, float y1, float x2, float y2) {
    x1 *= 100;
    y1 *= 100;
    x2 *= 100;
    y2 *= 100;
    return sqrt(pow((x1 - x2), 2) + pow((y1 - y2), 2));
}

//SOURCE: https://stackoverflow.com/questions/1120140/how-can-i-read-and-parse-csv-files-in-c
int main()
{
    std::ifstream file("C:\\Users\\Owner\\Desktop\\samplesmalldata.csv");
    if (!file.is_open()) {
        std::cout << "File failed to open" << std::endl;
        return -1;
    }

    //Determine which rows contain the data we need
    char xRow = -1, yRow = -1, IDRow = -1, lenRow = -1, toRow = -1, fromRow = -1, nameRow = -1; //, gridRow = -1;
    auto row = CSVRange(file).begin();
    for (int i = 0; i < row->size(); i++) {
        //std::cout << (*row)[i] << std::endl;

        if ((*row)[i] == "LA NE  MILES")    xRow = i;
        else if ((*row)[i] == "CL  MILES")  yRow = i;
        else if ((*row)[i] == "SECT  ID")   IDRow = i;
        else if ((*row)[i] == "ST  LENGTH") lenRow = i;
        else if ((*row)[i] == "ST  TO")     toRow = i;
        else if ((*row)[i] == "ST  FROM")   fromRow = i;
        else if ((*row)[i] == "ST  NAME")   nameRow = i;
        //else if ((*row)[i] == "Grid  ID")   gridRow = i;
    }

    //Populate the streetsegments
    std::vector<segment> segments;
    for (auto& r : CSVRange(file))
    {
        segment* s = new segment();
        s->x = std::stof(std::string{ r[xRow] });
        s->y = std::stof(std::string{ r[yRow] });
        s->sectID = std::stoi(std::string{ r[IDRow] });
        s->length = std::stoi(std::string{ r[lenRow] });
        //s->gridID = std::stoi( std::string { r[gridRow] }); //Crashes because the cell can be null
        s->name = std::string{ r[nameRow] };
        s->to = std::string{ r[toRow] };
        s->from = std::string{ r[fromRow] };

        segments.push_back(*s);
    }

    //create the distance matrix
    //This has the potential to take a looooot of memory.
    std::vector<std::vector<int64_t>> distance_matrix(segments.size());
    for (int i = 0; i < segments.size(); i++) {
        distance_matrix[i].resize(segments.size());
        for (int j = 0; j < segments.size(); j++) {
            //Euclidian distance generator
            int64_t distance = euclidianDistance(segments[i].x, segments[i].y, segments[j].x, segments[j].y);
            distance_matrix[i][j] = distance;
        }
    }

    for (int i = 0; i < distance_matrix.size(); i++) {
        std::cout << "{";
        for (int j = 0; j < distance_matrix[i].size(); j++) {
            std::cout << distance_matrix[i][j];
            if (j != distance_matrix[i].size() - 1)
                std::cout << ", ";
        }
        std::cout << "}," << std::endl;
    }

    std::cout << "Distance matrix generated!" << std::endl;
}