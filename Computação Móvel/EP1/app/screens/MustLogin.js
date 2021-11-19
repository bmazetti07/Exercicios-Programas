import React, { useState } from 'react';
import {
  StyleSheet,
  SafeAreaView,
  Text,
  Dimensions,
  View
} from 'react-native'
import * as Google from 'expo-google-app-auth'
import AntDesign from 'react-native-vector-icons/AntDesign';
import firebase from '../config/firebaseconfig'
import { TouchableOpacity } from 'react-native-gesture-handler';
import { LinearGradient } from 'expo-linear-gradient';
import MaterialCommunityIcons from 'react-native-vector-icons/MaterialCommunityIcons';
import Entypo from 'react-native-vector-icons/Entypo';

const { width } = Dimensions.get('window');

const MustLogin = ({ navigation }) => {
  const config = {
    androidClientId: '1038132406903-n33n0vf72djk5jf1hnclq0o4m4n2fpt5.apps.googleusercontent.com',
  }

  const signInWithGoogleAsync = async () => {
    try {
      const result = await Google.logInAsync({ ...config, scopes: ['profile', 'email'] })

      if (result.type === 'success') {
        const credential = firebase.auth.GoogleAuthProvider.credential(
          result.idToken,
          result.accessToken,
        );

        await firebase.auth().signInWithCredential(credential);
      }
    }

    catch (e) {
      return { error: true };
    }
  }

  return (
    <SafeAreaView style={styles.container}>
      <View style={styles.mainContainer}>
        <TouchableOpacity onPress={() => signInWithGoogleAsync()}>
          <LinearGradient
            colors={['red', 'red']}
            style={styles.signIn}
          >
            <AntDesign color='#fff' name="google" size={30} />
            <Text style={styles.textSign}> LOGAR COM O GOOGLE </Text>
          </LinearGradient>
        </TouchableOpacity>
      </View>

      <View style={styles.bottomContainer}>
        <View style={styles.bottomControls}>
          <TouchableOpacity onPress={() => navigation.navigate('PlayingScreen')}>
            <MaterialCommunityIcons color='#fff' name="headphones" size={30} />
          </TouchableOpacity>

          <TouchableOpacity onPress={() => navigation.navigate('SongsList')}>
            <Entypo color='#fff' name="list" size={30} />
          </TouchableOpacity>
        </View>
      </View>
    </SafeAreaView>
  );
};

const styles = StyleSheet.create({
  container: {
    flex: 1,
    backgroundColor: '#222831'
  },

  mainContainer: {
    flex: 1,
    alignItems: 'center',
    justifyContent: 'center'
  },

  signIn: {
    width: 220,
    height: 40,
    justifyContent: 'center',
    alignItems: 'center',
    borderRadius: 20,
    flexDirection: 'row',
    margin: 20,
  },

  textSign: {
    color: 'white',
    fontWeight: 'bold'
  },

  bottomContainer: {
    borderTopColor: '#393E46',
    borderTopWidth: 1,
    width: width,
    alignItems: 'center',
    paddingVertical: 15
  },

  bottomControls: {
    flexDirection: 'row',
    justifyContent: 'space-around',
    width: '80%'
  },
});

export default MustLogin;