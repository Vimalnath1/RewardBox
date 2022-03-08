import React from 'react';
import { StyleSheet, Text, View, TouchableOpacity,Button,StatusBar } from 'react-native';
import DialogInput from 'react-native-dialog-input';
import { steps } from '../../something';


var stepsgoal;
//var steps=4;
export default class App extends React.Component {
  constructor(props){ 
    super(props);
    this.state = {
      isDialogVisible: false,
    }
  }
  showDialog(isShow){
    this.setState({isDialogVisible: isShow});
  }
  sendInput(inputText){
    console.log("sendInput (DialogInput#1): "+inputText);
    stepsgoal=parseInt(inputText);
    if (stepsgoal>=5000){
        this.setState({isDialogVisible:false});   
    }
    else{
        stepsgoal="Too low, try a different goal"
    }
    console.log(stepsgoal);
  }
  render() {
    return (
      <View style={styles.container}>
          <Text>Choose the goal you want to achieve for the reward box to open. You can have a step goal, calorie goal, or heart points goal.</Text>
        <DialogInput isDialogVisible={this.state.isDialogVisible}
                    title={"Type in your step goal. It has to be over 5000."}
                    hintInput={"Type here"}
                    submitInput={ (inputText) => {this.sendInput(inputText)}}
                    closeDialog={ () => {this.showDialog(false)}}>
        </DialogInput>
        <Button title="Set Step Goal"onPress={()=>{this.showDialog(true)}} style={{padding:10}}/>
        <Text>Step Goal: {stepsgoal}</Text>
        <Text>Steps Left: {stepsgoal-steps}</Text> 
      </View>
    
    );
  }
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
export {stepsgoal};