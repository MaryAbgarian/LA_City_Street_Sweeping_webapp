from bs4 import BeautifulSoup
from shapely.geometry import LineString
from shapely_geojson import dumps, Feature, FeatureCollection
import csv
import json
import os

with open("dict.txt", "r") as read_file:
    dictionary = json.load(read_file)
print("dictionary loaded")

#with open("soup.txt", "rb") as f:
#    soup = BeautifulSoup(f.read(), "lxml")

location = os.getcwd()
features = []

for file in os.listdir(location):
    if file.endswith(".csv"):
        with open(file) as csv_file:
            print("Opened",file)
            csv_reader = csv.reader(csv_file, delimiter=',')
            for row in csv_reader:
                assetID = row[0]
                if assetID.isnumeric():
                    coordList = []
                    for coordpair in dictionary.get(assetID).split():
                        coordList.append(list(map(float, coordpair.split(","))))
                    features.append(Feature(LineString(coordList), {'assetID': assetID,'route':row[1],'need':row[2]}))

        with open(file.replace(".csv",".geojson"), "w") as f:
            feature_collection = FeatureCollection(features)
            print(dumps(FeatureCollection(features), separators=(',', ':')), file=f)
        print("Fiished",file)
