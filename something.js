import { spawn } from "react-native-childprocess/lib/commonjs";
export {steps};

var steps; 
const pythonstuff=await spawn("python", ["androidparser.py","derived_com.google.step_count.delta_com.google.json"]);
//const pythonstuff=spawn("C:/Users/Vimalnath Selvam/AppData/Local/Microsoft/WindowsApps/python3.9.exe", ["appleparser.py","export.xml"]);
pythonstuff.stdout.on("data", function (data) {
    console.log(data.toString());
    steps=data.toString()
  });
/*const jsonData=require("./derived_com.google.step_count.delta_com.google.json");
console.log(jsonData);*/
