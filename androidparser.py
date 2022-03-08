import json
import sys



file=open(sys.argv[1])
#file=open(derived_com.google.step_count.delta_com.google.json)
data=json.load(file)
numberofmovements=0
steps=0
for i in data["Data Points"]:
    numberofmovements+=1
for j in range(numberofmovements):
    steps+=data["Data Points"][j]["fitValue"][0]["value"]["intVal"]
    
print(steps)
file.close()


