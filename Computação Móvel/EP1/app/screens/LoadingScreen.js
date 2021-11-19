import React, { useEffect } from 'react';
import firebase from '../config/firebaseconfig'
import {
  StyleSheet,
  SafeAreaView,
  ActivityIndicator,
} from 'react-native'
import { useIsFocused } from "@react-navigation/native";

const LoadingScreen = ({ navigation }) => {
  const isFocused = useIsFocused();

  useEffect(() => {
    if (isFocused)
      checkIfLoggedIn()
  }, [isFocused])

  const checkIfLoggedIn = (user) => {
    firebase.auth().onAuthStateChanged((user) => {
      if (user)
        navigation.navigate('LoginWeatherScreen')
      else
        navigation.navigate('LoginScreen')
    })
  };

  return (
    <SafeAreaView style={styles.container}>
      <ActivityIndicator size="large" color={'#fff'} />
    </SafeAreaView>
  );
};

const styles = StyleSheet.create({
  container: {
    flex: 1,
    alignItems: 'center',
    justifyContent: 'center',
    backgroundColor: '#222831'
  }
});

export default LoadingScreen;