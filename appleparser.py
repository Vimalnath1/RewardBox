import json
import xmltodict
import datetime
import sys

def appleparse():
    file=open(sys.argv[1])
    json_file=open("stuff.json","w")
    data=xmltodict.parse(file.read())
    file.close()
    json_file.write(json.dumps(data))
    json_file.close()
    newjson_file=open("stuff.json","r")
    jsondata=json.load(newjson_file)
    sum=0
    #date=str(datetime.datetime.now())
    date="2017-12-01"
    for i in jsondata["HealthData"]["Record"]:
        if i["@type"]=="HKQuantityTypeIdentifierStepCount":
            if i["@startDate"][0:10]==date[0:10]:
                sum+=int(i["@value"])
            else:
                break
    print(sum)
    return sum
#print(date[0:10])


'''tree=ET.parse(file)
root=tree.getroot()
for i in root.findall(".//*[@type='HKQuantityTypeIdentifierStepCount']"):
    for value in i:
        print(value)'''