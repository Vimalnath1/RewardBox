import React from 'react';
import { StyleSheet,View,Button,Text, StatusBar, Platform ,backgroundColor,Linking} from 'react-native';
import SetGoalScreen from './SetGoalScreen';
import App from '../../App';
import { NavigationContainer} from '@react-navigation/native';
import {createNativeStackNavigator} from '@react-navigation/native-stack';

/*const pythonstuff=spawn("python", ["androidparser.py","derived_com.google.step_count.delta_com.google.json"]);
pythonstuff.stdout.on("data", function (data) {
    console.log(data.toString());
  });
pythonstuff.stdout.on("d")*/
function importData(){
  if (Platform.OS=="android"){
    Linking.openURL("https://takeout.google.com/");
  }
  else if (Platform.OS=="ios"){
    console.log("Imagine using Apple");
  }
}
function MainScreen({ navigation }) {
  return (
    <View style={styles.container}>
        <Text>  Hello Reward Box Users! We are so excited for you to use our product. On this app, you must connect to your Reward Box via Bluetooth and then choose your exercise goal. Then export your health data from Apple Health or Google Fit. If you've reached your fitness goal, enjoy your treats!</Text>
        <Button style={styles.button}  title="Set Exercise Goal" onPress={()=>navigation.navigate("Set Goal")}/>
        <Button style={styles.button} title="Import Health Data" onPress={()=>importData()}/>
    </View>
);
}

const styles = StyleSheet.create({
    container: {
      flex: 1,
      backgroundColor: '#d9d9d9',
      alignItems: 'center',
      paddingTop: Platform.OS==="android" ? StatusBar.currentHeight:0
    },
    button: {
        paddingTop:"25%",
    }
  });

  

export default MainScreen;