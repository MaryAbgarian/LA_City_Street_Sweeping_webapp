from bs4 import BeautifulSoup
import json
f = open('data.kml')
data = "\n".join(f.readlines())
print("After Data")
soup = BeautifulSoup(data, 'html.parser')
print("Progress!")
#new_soup = str(soup)
#with open("soup.txt", "w+") as f:
#    f.write(new_soup)
#print("Soup saved!")

dict = {}

with open('output.csv', "w") as f:
    for c in soup.find_all('placemark'):
        simpledatas = c.extendeddata.find_all('simpledata')
        asset = '-1'
        for s in simpledatas:
            if s['name'] == "ASSETID":
                asset = s.string
        #print(asset +',"'+c.coordinates.string+'"', file=f)
        dict[asset] = c.coordinates.string

with open('dict.txt', 'w', encoding="utf8") as handle:
  json.dump(dict, handle)