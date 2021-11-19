import React, { useEffect, useState } from 'react'
import {
  Text,
  StyleSheet,
  View,
  Dimensions,
  SafeAreaView,
  StatusBar,
  ActivityIndicator,
  Button,
} from 'react-native'
import { TouchableOpacity } from 'react-native-gesture-handler';
import { LinearGradient } from 'expo-linear-gradient';
import Entypo from 'react-native-vector-icons/Entypo';
import MaterialCommunityIcons from 'react-native-vector-icons/MaterialCommunityIcons';
import AntDesign from 'react-native-vector-icons/AntDesign';
import DateTime from '../components/DateTime';
import WeatherScroll from '../components/WeatherScroll';
import * as Google from 'expo-google-app-auth'
import * as Location from 'expo-location';
import AsyncStorage from '@react-native-async-storage/async-storage';
import firebase from '../config/firebaseconfig';

const { width } = Dimensions.get('window')
const WEATHER_API_KEY = "2655473f065b2f9524803e99f782218f";
const BASE_WEATHER_URL = "https://api.openweathermap.org/data/2.5/weather?";
const semana = ['Domingo', 'Segunda', 'Terça', 'Quarta', 'Quinta', 'Sexta', 'Sabado'];

const LoginWeather = ({ navigation }) => {
  const [errorMessage, setErrorMessage] = useState(null)
  const [currentWeatherDetails, setCurrentWeatherDetails] = useState(null)
  const [currentDate, setCurrentDate] = useState(null)
  const [weekDay, setWeekDay] = useState(null)

  const getCurrentDate = () => {
    const time = new Date();
    const mes = time.getMonth() + 1;
    const date = time.getDate();
    const ano = time.getFullYear();
    const hora = time.getHours();
    const minutos = time.getMinutes();

    const day = semana[time.getDay()]
    setWeekDay(day)

    return (date + '/' + mes + '/' + ano + ' ' + (hora < 10 ? '0' + hora : hora) + ':' + (minutos < 10 ? '0' + minutos : minutos))
  }

  useEffect(() => {
    load();
  }, [weekDay, currentDate]);

  const load = async () => {
    setCurrentWeatherDetails(null)
    setErrorMessage(null)
    try {
      let { status } = await Location.requestForegroundPermissionsAsync()

      if (status !== "granted") {
        setErrorMessage("É necessário permissão para rodar o aplicativo!")
        return
      }
      const location = await Location.getCurrentPositionAsync()

      const { latitude, longitude } = location.coords
      const weatherUrl = `${BASE_WEATHER_URL}lat=${latitude}&lon=${longitude}&units=metric&appid=${WEATHER_API_KEY}&lang=pt`
      const response = await fetch(weatherUrl)
      const result = await response.json()

      if (response.ok) {
        setCurrentDate(getCurrentDate())

        result.weekDay = weekDay
        setCurrentWeatherDetails(result)
      }
      else {
        setErrorMessage(result.message)
      }
    }

    catch (error) {
      load()
      setErrorMessage(error.message)
    }
  }

  const saveWeather = async () => {
    try {
      await AsyncStorage.setItem(currentDate, JSON.stringify(currentWeatherDetails))
    } catch (error) {
      alert(error)
    }
  }

  if (currentWeatherDetails) {
    return (
      <SafeAreaView style={styles.container}>
        <StatusBar barStyle='default' />

        <DateTime currentWeatherDetails={currentWeatherDetails} />

        <WeatherScroll currentWeatherDetails={currentWeatherDetails} />

        <View style={styles.mainContainer}>
          <View style={styles.infoContainer}>
            <TouchableOpacity onPress={() => saveWeather()}>
              <Button
                title="SALVAR INFO CLIMA ATUAL"
              />
            </TouchableOpacity>

            <TouchableOpacity onPress={() => navigation.navigate('WeatherHistory')}>
              <MaterialCommunityIcons color='#fff' name="history" size={35} />
            </TouchableOpacity>
          </View>

          <View style={styles.secondInfoContainer}>
            <TouchableOpacity onPress={() => firebase.auth().signOut()}>
              <LinearGradient
                colors={['red', 'red']}
                style={styles.signIn}
              >
                <AntDesign color='#fff' name="google" size={30} />
                <Text style={styles.textSign}> DESLOGAR DO GOOGLE </Text>
              </LinearGradient>
            </TouchableOpacity>


            <TouchableOpacity onPress={() => load()}>
              <AntDesign color='#fff' name="reload1" size={30} />
            </TouchableOpacity>
          </View>
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
      </SafeAreaView >
    )
  }

  else {
    return (
      <View style={styles.container}>
        <View style={styles.mainContainer}>
          <ActivityIndicator size="large" color={'#fff'} />
        </View>
      </View>
    )
  }
}

export default LoginWeather

const styles = StyleSheet.create({
  container: {
    flex: 1.5,
    backgroundColor: '#222831'
  },

  mainContainer: {
    flex: 1,
    width: '100%',
    justifyContent: 'center',
    alignItems: 'center'
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

  infoContainer: {
    flexDirection: 'row',
    alignContent: 'center',
    justifyContent: 'space-between',
    alignContent: 'space-around',
    marginRight: 5,
    paddingTop: 7,
    height: 50,
    width: '65%',
  },

  secondInfoContainer: {
    flexDirection: 'row',
    height: 60,
    alignItems: 'center',
    marginRight: 35
  }
})
