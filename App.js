import { StatusBar } from 'expo-status-bar';
import { Button, StyleSheet, Text, View ,Platform} from 'react-native';
import MainScreen from './app/screens/MainScreen.js';
import SetGoalScreen from './app/screens/SetGoalScreen.js';
import { NavigationContainer } from '@react-navigation/native';
import { createNativeStackNavigator } from '@react-navigation/native-stack';

const Stack=createNativeStackNavigator();
export default function App() {
  return (
    <NavigationContainer>
      <Stack.Navigator initalRouteName="Home">
        <Stack.Screen name="Home" component={MainScreen}>
        </Stack.Screen>
        <Stack.Screen name="Set Goal" component={SetGoalScreen}>
        </Stack.Screen>
      </Stack.Navigator>
    </NavigationContainer>
    );
}
