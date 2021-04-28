#include "CSVReader.h"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <unordered_map>
#include <algorithm>

#define DEFAULT 1

struct segment {
    double latitude;
    double longitude;
    unsigned int ID;
    unsigned int zip;
    float trashScore;
    int treeScore;
    int cleanStatScore;
    int incomeScore;
    char normalCleanStat;
    char normalTrash;
    char normalTree;
    char normalIncome;
    char needScore;
};

bool LatitudeSort(segment a, segment b) { return a.latitude > b.latitude; }
bool LongitudeSort(segment a, segment b) { return a.longitude > b.longitude; }
bool needScoreSort(segment a, segment b) { return a.needScore > b.needScore; }

struct highLowVals {
    int highTrees;
    int lowTrees;
    int highIncome;
    int lowIncome;
    float highDebris;
    float lowDebris;
};

struct data {
    data() : CleanStatScore(-1), TreeScore(-1), IncomeScore(-1) {}
    char CleanStatScore;
    char TreeScore;
    int IncomeScore;
};

//NOT to be confused with the routeID on the segments! Different things!
struct route {
    float miles;
    float debris[12]; //Avg debris on any day of the month. Cut if memory becomes a problem

    float getAvgDebris() const {
        float avg = 0;
        for (int i = 0; i < 12; i++) {
            avg += debris[i];
        }
        avg /= 12;
        return avg;
    }
};

//For generating the actual routes
class box {
public:
    //Should only be done for the first box. All other boxes should be split
    box(std::vector<segment> segs, unsigned int id) : mSegments(segs), mID(id), mRoute(0), mNeedScore(0)
    {
        int maxLat = 0, minLat = 0, maxLon = 0, minLon = 0;
        for (int i = 1; i < mSegments.size(); i++) {
            if (mSegments[i].latitude < mSegments[minLat].latitude)         minLat = i;
            else if (mSegments[i].latitude > mSegments[maxLat].latitude)    maxLat = i;
            if (mSegments[i].longitude < mSegments[minLon].longitude)       minLon = i;
            else if (mSegments[i].longitude > mSegments[maxLon].longitude)  maxLon = i;
        }

        top = mSegments[maxLat].latitude;
        bottom = mSegments[minLat].latitude;
        right = mSegments[maxLon].longitude;
        left = mSegments[minLon].longitude;
    }

    //Splits the current box into two and returns the new box
    box splitBox(unsigned int id) {
        double latDiff = top - bottom;
        double lonDiff = right - left;

        bool horiSplit = (latDiff > lonDiff); //Determines if we split vertically or horizontally
        if (horiSplit) { std::sort(mSegments.begin(), mSegments.end(), LatitudeSort); }
        else { std::sort(mSegments.begin(), mSegments.end(), LongitudeSort); }
        std::vector<segment> firstHalf(mSegments.begin(), mSegments.begin() + mSegments.size() / 2);
        std::vector<segment> lastHalf(mSegments.begin() + mSegments.size() / 2, mSegments.end());
        mSegments = firstHalf;

        double newCorner = (horiSplit) ? lastHalf[0].latitude : lastHalf[0].longitude;
        if (horiSplit) {
            box newBox(newCorner, right, left, bottom, lastHalf, id);
            bottom = newCorner;
            return newBox;
        }
        else {
            box newBox(top, newCorner, left, bottom, lastHalf, id);
            left = newCorner;
            return newBox;
        }
    }

    void printRouteSegments(std::ofstream& output) {
        for (int i = 0; i < mSegments.size(); i++) {
            output << mSegments[i].ID << "," << mRoute << std::endl;
        }
    }

    void calculateNeed() {
        float totalNeed = 0;
        for (int i = 0; i < mSegments.size(); i++) {
            totalNeed += mSegments[i].needScore;
        }
        mNeedScore = totalNeed / mSegments.size();
    }

    bool isRightNeighbor(const box& other)  const { return (right == other.left && !(top <= other.bottom) && !(bottom >= other.top)); }
    bool isLeftNeighbor(const box& other)   const { return (left == other.right && !(top <= other.bottom) && !(bottom >= other.top)); }
    bool isBottomNeighbor(const box& other) const { return (bottom == other.top && !(right <= other.left) && !(left >= other.right)); }
    bool isTopNeighbor(const box& other)    const { return (top == other.bottom && !(right <= other.left) && !(left >= other.right)); }
    bool isNeighbor(const box& other) const { return isRightNeighbor(other) || isLeftNeighbor(other) || isBottomNeighbor(other) || isTopNeighbor(other); }

    unsigned int getId() const { return mID; }
    unsigned int getRoute() const { return mRoute; }
    size_t getSize() const { return mSegments.size(); }
    float getNeed() const { return mNeedScore; }
    double getTop() const { return top; }
    double getBottom() const { return bottom; }
    double getRight() const { return right; }
    double getLeft() const { return left; }

    void setRoute(unsigned int r) { mRoute = r; }

private:
    //Used for splitting
    box(double t, double r, double l, double b, std::vector<segment> segs, unsigned int id)
        : top(t), right(r), left(l), bottom(b), mSegments(segs), mID(id), mRoute(0), mNeedScore(0) {};

    double top;
    double right;
    double bottom;
    double left;

    unsigned int mID;
    unsigned int mRoute;
    float mNeedScore;
    std::vector<segment> mSegments;
};

bool boxNeedSort(const box& a, const box& b) { return a.getNeed() > b.getNeed(); }

//IMPORTANT: Scaling decimals up by 1000000 so that the difference between segments registers
int64_t euclidianDistance(double x1, double y1, double x2, double y2) {
    x1 *= 100000;
    y1 *= 100000;
    x2 *= 100000;
    y2 *= 100000;
    return static_cast<int16_t>(sqrt(pow((x1 - x2), 2) + pow((y1 - y2), 2)));
}

//generates a distance matrix
//This has the potential to take a looooot of memory.
std::vector<std::vector<int64_t>> generateDistanceMatrix(const std::vector<segment>& segments) {
    std::vector<std::vector<int64_t>> distance_matrix(segments.size());
    for (uint16_t i = 0; i < segments.size(); i++) {
        distance_matrix[i].resize(segments.size());
        for (uint16_t j = 0; j < segments.size(); j++) {
            //Euclidian distance generator
            int64_t distance = euclidianDistance(segments[i].latitude, segments[i].longitude, segments[j].latitude, segments[j].longitude);
            distance_matrix[i][j] = distance;
        }
    }

    return distance_matrix;
}

void printDistanceMatrix(const std::vector<std::vector<int64_t>>& matrix) {
    for (uint16_t i = 0; i < matrix.size(); i++) {
        std::cout << "{";
        for (uint16_t j = 0; j < matrix[i].size(); j++) {
            std::cout << matrix[i][j];
            if (j != matrix[i].size() - 1)
                std::cout << ", ";
        }
        std::cout << "}," << std::endl;
    }
}

//Files must be in CSV Format. First file maps route names to their miles
//Second file maps routes with dates and debris collected that month
std::unordered_map<std::string, route> generateRouteToTrashMap(const std::string& filename)
{
    std::unordered_map<std::string, route> stor;

    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cout << "file failed to open" << std::endl;
        return stor;
    }

    char debrisRow = -1;    //Sum of trash collected that month
    char monthRow = -1;     //Month route was swept
    char routeRow = -1;     //Name of route
    char numRow = -1;       //Number of weeks swept in a month
    char milesRow = -1;     //How long a route is
    auto row = CSVRange(file).begin();
    for (uint8_t i = 0; i < row->size(); i++) {
        if ((*row)[i] == "StrippedRoute") routeRow = i;
        else if ((*row)[i] == "miles")   milesRow = i;
        else if ((*row)[i] == "iMonth")   monthRow = i;
        else if ((*row)[i] == "Sum")    debrisRow = i;
        else if ((*row)[i] == "NumWeeks")    numRow = i;
    }

    std::unordered_map<std::string, int> weeksSwept[12];
    for (auto& r : CSVRange(file))
    {
        std::string routeName = std::string{ r[routeRow] };
        stor[routeName].miles = std::stof(std::string{ r[milesRow] });
        stor[routeName].debris[stoi(std::string{ r[monthRow] }) - 1] += stoi(std::string{ r[debrisRow] });
        weeksSwept[stoi(std::string{ r[monthRow] }) - 1][routeName] += stoi(std::string{ r[numRow] });
    }

    for (int i = 0; i < 12; i++) {
        for (std::pair<std::string, int> element : weeksSwept[i])
        {
            stor[element.first].debris[i] /= element.second;
            stor[element.first].debris[i] *= 10; //This is arbitrary don't judge me
            stor[element.first].debris[i] /= stor[element.first].miles;
            //std::cout << i + 1 << ": " << element.first << " :: " << stor[element.first].debris[i] << std::endl;
            if (stor[element.first].miles == 0 || element.second == 0) {
                std::cout << "ERROR:" << i + 1 << ": " << element.first << " :: " << stor[element.first].debris[i] << "  :: Miles: " << stor[element.first].miles << std::endl;
            }
        }
    }

    return stor;
}

//valToUpdate: 1 = CleanStatScore, 2 = TreeScore, 3 = IncomeScore
void propagateDataMap(std::unordered_map<unsigned int, data>& map, const std::string& filename,
    const std::string IDName, const std::string valName, char valToUpdate)
{
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cout << "File failed to open" << std::endl;
        return;
    }

    char IDRow = -1, valRow = -1;
    auto row = CSVRange(file).begin();
    for (uint8_t i = 0; i < row->size(); i++) {
        if ((*row)[i] == IDName) IDRow = i;
        else if ((*row)[i] == valName) valRow = i;
    }

    for (auto& r : CSVRange(file))
    {
        if (valToUpdate == 1) {
            map[std::stoi(std::string{ r[IDRow] })].CleanStatScore = std::stoi(std::string{ r[valRow] });
        }
        else if (valToUpdate == 2) {
            map[std::stoi(std::string{ r[IDRow] })].TreeScore = std::stoi(std::string{ r[valRow] });
        }
        else if (valToUpdate == 3) {
            map[std::stoi(std::string{ r[IDRow] })].IncomeScore = std::stoi(std::string{ r[valRow] });
        }
        else {
            std::cout << "Invalid Value to Update Provided" << std::endl;
        }
    }
}

std::unordered_map<unsigned int, data> generateDataMap(const std::string& CSFilename, const std::string& TreeFilename, const std::string& incomeFileName) {
    std::unordered_map<unsigned int, data> dataMap(80430); //CS has 42500 entries. Trees has 67597 entries. MHI has 80428
    propagateDataMap(dataMap, CSFilename, "SegmentID", "CS_RoundScore", 1);
    propagateDataMap(dataMap, TreeFilename, "ASSETID", "COUNT__1", 2);
    propagateDataMap(dataMap, incomeFileName, "ASSETID", "AvgMHI", 3);
    return dataMap;
}

highLowVals getDifferentiators(const std::unordered_map<unsigned int, data>& map, const std::unordered_map<std::string, route>& routes, float low, float high) {
    std::vector<int> trees;
    trees.reserve(67590);
    std::vector<int> income;
    income.reserve(80400);
    std::vector<float> debris;
    debris.reserve(37600);

    for (std::pair<unsigned int, data> element : map) {
        if (element.second.TreeScore != -1) {
            trees.push_back(element.second.TreeScore);
        }
        if (element.second.IncomeScore != -1) {
            income.push_back(element.second.IncomeScore);
        }
    }
    for (std::pair<std::string, route> element : routes) {
        debris.push_back(element.second.getAvgDebris());
    }

    std::sort(trees.begin(), trees.end());
    std::sort(income.begin(), income.end(), std::greater());
    std::sort(debris.begin(), debris.end());

    highLowVals diffs;
    diffs.lowTrees = trees[static_cast<int>(trees.size() * low)];
    diffs.highTrees = trees[static_cast<int>(trees.size() * high)];

    diffs.lowIncome = income[static_cast<int>(income.size() * low)];
    diffs.highIncome = income[static_cast<int>(income.size() * high)];

    diffs.lowDebris = debris[static_cast<int>(debris.size() * low)];
    diffs.highDebris = debris[static_cast<int>(debris.size() * high)];

    //std::cout << "Trees: " << diffs.lowTrees << ", " << diffs.highTrees << std::endl;
    //std::cout << "Income: " << diffs.lowIncome << ", " << diffs.highIncome << std::endl;
    //std::cout << "Debris: " << diffs.lowDebris << ", " << diffs.highDebris << std::endl;

    return diffs;
}

//Turns a vector of segments into multiple vectors within a given size range, as necessary
std::vector<std::vector<segment> > splitSegments(std::vector<segment> segments, size_t max)
{
    std::vector<std::vector<segment> > groupings;
    if (segments.size() < max) {
        groupings.push_back(segments);
        return groupings;
    }
    int maxLat = 0, minLat = 0, maxLon = 0, minLon = 0;
    for (int i = 1; i < segments.size(); i++) {
        if (segments[i].latitude < segments[minLat].latitude)        minLat = i;
        else if (segments[i].latitude > segments[maxLat].latitude)   maxLat = i;
        if (segments[i].longitude < segments[minLon].longitude)      minLon = i;
        else if (segments[i].longitude > segments[maxLon].longitude) maxLon = i;
    }
    double latDiff = segments[maxLat].latitude - segments[minLat].latitude;
    double lonDiff = segments[maxLon].longitude - segments[minLon].longitude;

    if (latDiff > lonDiff) {
        std::sort(segments.begin(), segments.end(), LatitudeSort);
    }
    else {
        std::sort(segments.begin(), segments.end(), LongitudeSort);
    }

    std::vector<segment> firstHalf(segments.begin(), segments.begin() + segments.size() / 2);
    std::vector<segment> lastHalf(segments.begin() + segments.size() / 2, segments.end());

    std::vector<std::vector<segment>> firstSubgrouping = splitSegments(firstHalf, max);
    std::vector<std::vector<segment>> secondSubgrouping = splitSegments(lastHalf, max);

    groupings.insert(groupings.begin(), firstSubgrouping.begin(), firstSubgrouping.end());
    groupings.insert(groupings.end(), secondSubgrouping.begin(), secondSubgrouping.end());

    return groupings;
}

void neighborType(box a, box b, std::string aName, std::string bName) {
    if (a.isLeftNeighbor(b))    std::cout << bName << " is left of " << aName << std::endl;
    if (a.isRightNeighbor(b))   std::cout << bName << " is right of " << aName << std::endl;
    if (a.isTopNeighbor(b))     std::cout << bName << " is above " << aName << std::endl;
    if (a.isBottomNeighbor(b))  std::cout << bName << " is below " << aName << std::endl;
}

//Turns a vector of segments into multiple vectors within a given size range, as necessary
std::vector<box> makeBoxes(const std::vector<segment>& segments, size_t max)
{
    int boxID = 1;
    std::vector<box> boxes;
    box start(segments, boxID++);
    boxes.push_back(start);

    bool boxTooBig = (start.getSize() > max);
    while (boxTooBig) {
        boxTooBig = false;
        size_t tempSize = boxes.size();
        for (int i = 0; i < tempSize; i++) {
            if (boxes[i].getSize() > max) {
                box newBox = boxes[i].splitBox(boxID++);
                boxes.push_back(newBox);
                boxTooBig = true;
            }
        }
    }

    return boxes;
}

std::vector<segment> generateSegments(const std::string& filename, const std::unordered_map<std::string, route>& routeToTrash,
    const std::unordered_map<unsigned int, data>& IDtoData, float diffLow, float diffHigh) {

    auto diffs = getDifferentiators(IDtoData, routeToTrash, diffLow, diffHigh);

    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cout << "File failed to open" << std::endl;
        return std::vector<segment>();
    }

    //Determine which rows contain the data we need
    char IDRow = -1, lenRow = -1;
    char zipRow = -1, routeRow = -1;
    char latRow = -1, longRow = -1;
    auto row = CSVRange(file).begin();
    for (uint8_t i = 0; i < row->size(); i++) {
        if ((*row)[i] == "ASSETID")             IDRow = i;
        else if ((*row)[i] == "Shape__Length")  lenRow = i;
        else if ((*row)[i] == "ZIP_R")          zipRow = i;
        else if ((*row)[i] == "StrippedRoute")  routeRow = i;
        else if ((*row)[i] == "Lat")  latRow = i;
        else if ((*row)[i] == "Long")  longRow = i;
    }

    //Populate the streetsegments
    //Make a note of which route maps to which segment
    std::vector<segment> segments;
    segments.reserve(88000);
    unsigned int blankSegments = 0;
    for (auto& r : CSVRange(file))
    {
        segment s;
        s.ID = std::stoi(std::string{ r[IDRow] });
        s.zip = std::stoi(std::string{ r[zipRow] });
        s.latitude = std::stod(std::string{ r[latRow] });
        s.longitude = std::stod(std::string{ r[longRow] });
        s.trashScore = -1;
        s.cleanStatScore = -1;
        s.treeScore = -1;
        s.incomeScore = -1;

        //Set cleanstat scores, tree scores, and income scores:
        if (IDtoData.find(s.ID) != IDtoData.end())
        {
            s.cleanStatScore = IDtoData.at(s.ID).CleanStatScore;
            s.treeScore = IDtoData.at(s.ID).TreeScore;
            s.incomeScore = IDtoData.at(s.ID).IncomeScore;
            //std::cout << "Data found: " << s.ID << ", CleanStat: " << s.cleanStatScore << ", Tree: " << s.treeScore << ", MHI: " << s.incomeScore << std::endl;
        }

        //If the routerow exists, then we add a trashscore
        std::string routeName = std::string{ r[routeRow] };
        if (routeName.compare(std::string()) && routeToTrash.find(routeName) != routeToTrash.end()) {
            s.trashScore = routeToTrash.at(routeName).getAvgDebris();
            //std::cout << "Trashscore found: " << s.ID << ", " << s.trashScore << std::endl;
        }

        //If a segment doesn't have any of these scores, let the programmer know
        if (s.cleanStatScore == -1 && s.trashScore == -1 && s.treeScore == -1 && s.incomeScore == -1)
        {
            //std::cout << "No Scores: " << s.ID << std::endl;
            blankSegments++;
        }

        //Generate normalized scores:
        s.normalCleanStat = (s.cleanStatScore == -1) ? DEFAULT : s.cleanStatScore;

        if (s.treeScore == -1)                      s.normalTree = DEFAULT;
        else if (s.treeScore > diffs.highTrees)     s.normalTree = 3;
        else if (s.treeScore > diffs.lowTrees)      s.normalTree = 2;
        else                                        s.normalTree = 1;

        if (s.incomeScore == -1)                    s.normalIncome = DEFAULT;
        else if (s.incomeScore < diffs.highIncome)  s.normalIncome = 3;
        else if (s.incomeScore < diffs.lowIncome)   s.normalIncome = 2;
        else                                        s.normalIncome = 1;

        if (s.trashScore == -1)                     s.normalTrash = DEFAULT;
        else if (s.trashScore > diffs.highDebris)   s.normalTrash = 3;
        else if (s.trashScore > diffs.lowDebris)    s.normalTrash = 2;
        else                                        s.normalTrash = 1;

        //Create the need score
        s.needScore = s.normalCleanStat + s.normalIncome + s.normalTrash + s.normalTree;
        segments.push_back(s);
    }
    std::cout << "Number of segments with no scores: " << blankSegments << std::endl;
    return segments;
}

//Returns the locations of neigbors to the given box
//ONLY returns neighbors that do not already have a route
std::set<int> getNeighbors(int boxLoc, const std::vector<box>& boxes) {
    std::set<int> neighbors;
    for (int i = 0; i < boxes.size(); i++) {
        if (i == boxLoc) continue; //Do not add our box
        else if (boxes[i].getRoute() != 0) continue; //Do not add boxes with routes
        else if (boxes[boxLoc].isNeighbor(boxes[i])) {
            neighbors.insert(i);
        }
    }
    return neighbors;
}

//For merging islands that consist of a single box
std::set<int> getRoutedNeighbors(int boxLoc, const std::vector<box>& boxes) {
    std::set<int> neighbors;
    for (int i = 0; i < boxes.size(); i++) {
        if (i == boxLoc) continue; //Do not add our box
        else if (boxes[boxLoc].isNeighbor(boxes[i])) {
            neighbors.insert(i);
        }
    }
    return neighbors;
}

//For merging islands greater than one box
std::set<int> getRoutedNeighbors(const box& b, const std::vector<box>& boxes) {
    std::set<int> neighbors;
    for (int i = 0; i < boxes.size(); i++) {
        if (boxes[i].getId() == b.getId()) continue; //Do not add our box
        if (boxes[i].getRoute() == b.getRoute()) continue; //Do not add from same route
        else if (b.isNeighbor(boxes[i])) {
            neighbors.insert(i);
        }
    }
    return neighbors;
}

std::vector<std::vector<box> > createRoutes(const std::vector<segment>& segments) {
    std::vector<box> boxes = makeBoxes(segments, 15); //Create the boxes
    for (int i = 0; i < boxes.size(); i++) { //Calculate need for each box
        boxes[i].calculateNeed();
    }
    //Sort the boxes by need
    std::sort(boxes.begin(), boxes.end(), boxNeedSort);

    std::vector<std::vector<box> > routes(2000); //We should have about 2000 routes

    //Create the routes
    unsigned int routeNum = 1;
    for (int i = 0; i < boxes.size(); i++) {
        if (boxes[i].getRoute() != 0) continue;

        //If we get an island, merge this with closest route
        std::set<int> neighbors = getNeighbors(i, boxes);
        if (neighbors.size() == 0) {
            std::set<int> routedNeighbors = getRoutedNeighbors(i, boxes);
            int closestNeed = *(routedNeighbors.rbegin());

            int mergeRouteNum = boxes[closestNeed].getRoute();
            boxes[i].setRoute(mergeRouteNum);
            routes[mergeRouteNum].push_back(boxes[i]);
            continue;
        }

        boxes[i].setRoute(routeNum);
        routes[routeNum].push_back(boxes[i]); //Add box to route

        //Max route size increases as we get to routes with less need
        int routeSize = 1;
        while (routeSize <= 5 + (i / (boxes.size() / 4)) && (neighbors.size() > 0)) {
            int maxNeed = *(neighbors.begin()); //The nighbor with the greatest need will be the first element

            //Add that neighbor to the route and add this box's neighbors to be considered
            boxes[maxNeed].setRoute(routeNum);
            routes[routeNum].push_back(boxes[maxNeed]); //Add box to route
            neighbors.erase(maxNeed);
            std::set<int> newNeighbors = getNeighbors(maxNeed, boxes);
            neighbors.insert(newNeighbors.begin(), newNeighbors.end());
            routeSize++;
        }

        //Too small for its own route - treat it as an island
        if (routeSize < 4) {
            //Find the route closest in trash score to our current route
            for (int j = 0; j < routes[routeNum].size(); j++) {
                std::set<int> routedNeighbors = getRoutedNeighbors(routes[routeNum][j], boxes);
                neighbors.insert(routedNeighbors.begin(), routedNeighbors.end());
            }
            int closestNeed = *(neighbors.rbegin());
            int mergeRouteNum = boxes[closestNeed].getRoute();

            for (int j = 0; j < routes[routeNum].size(); j++) {
                routes[routeNum][j].setRoute(mergeRouteNum);
                routes[mergeRouteNum].push_back(routes[routeNum][j]);
            }
            while (!routes[routeNum].empty()) routes[routeNum].pop_back();
        }
        else {
            routeNum++; //The route is generated, get value ready for next route
        }
    }

    return routes;
}

//SOURCE: https://stackoverflow.com/questions/1120140/how-can-i-read-and-parse-csv-files-in-c
int main()
{
    auto routeToTrash = generateRouteToTrashMap("C:\\Users\\Owner\\Desktop\\DebrisData.csv");
    auto dataMap = generateDataMap("C:\\Users\\Owner\\Desktop\\CleanStat.csv", "C:\\Users\\Owner\\Desktop\\TreeScore.csv", "C:\\Users\\Owner\\Desktop\\MHI.csv");
    std::vector<segment> segments = generateSegments("C:\\Users\\Owner\\Desktop\\cleanstreetrouteslatlong.csv", routeToTrash, dataMap, .45, .8);

    std::cout << "Segments created" << std::endl;
    //Remove any segments that are unable to be mapped because they lack coordinates:
    std::vector<segment> noCoords;
    for (int i = 0; i < segments.size(); i++) {
        if (segments[i].latitude == -1 || segments[i].longitude == -1) {
            noCoords.push_back(segments[i]);
            segments.erase(segments.begin() + i);
            //std::cout << "Removed an element" << std::endl;
        }
    }

    std::ofstream output("C:\\Users\\Owner\\Desktop\\standardOutput.csv");

    auto routes = createRoutes(segments);
    output << "SegmentID,RouteID" << std::endl;
    for (int i = 0; i < routes.size(); i++) {
        for (int j = 0; j < routes[i].size(); j++) {
            routes[i][j].printRouteSegments(output);
        }
    }

    std::cout << "Finished!" << std::endl;
    //printDistanceMatrix(generateDistanceMatrix(segments[91504]));
}