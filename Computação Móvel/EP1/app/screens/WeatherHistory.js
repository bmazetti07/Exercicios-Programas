import React, { useEffect, useState, Component } from 'react'
import {
  Text,
  StyleSheet,
  View,
  Dimensions,
  SafeAreaView,
  ScrollView,
} from 'react-native'
import { TouchableOpacity } from 'react-native-gesture-handler';
import Entypo from 'react-native-vector-icons/Entypo';
import MaterialCommunityIcons from 'react-native-vector-icons/MaterialCommunityIcons';
import WeatherScroll from '../components/WeatherScroll';
import AsyncStorage from '@react-native-async-storage/async-storage';

const { width } = Dimensions.get('window')

const WeatherHistory = ({ navigation }) => {
  const [weatherData, setWeatherData] = useState([])

  const getData = async () => {
    try {
      const keys = await AsyncStorage.getAllKeys();
      const result = await AsyncStorage.multiGet(keys);

      setWeatherData(result)
    } catch (error) {
      console.error(error)
    }
  }

  const removeData = async (key) => {
    try {
      await AsyncStorage.removeItem(key)
    } catch (error) {
      alert(error)
    } finally {
      await getData()
    }
  }

  useEffect(() => {
    getData()
  }, []);

  return (
    <SafeAreaView style={styles.container}>
      <ScrollView>
        {weatherData.map(item => {
          if (item[0].length <= 16) {
            return (
              <View key={item[0]} style={{ flex: 1 }}>
                <TouchableOpacity onPress={() => removeData(item[0])}>
                  <Text style={styles.dateHistory}> {item[0]} </Text>
                  <WeatherScroll currentWeatherDetails={JSON.parse(item[1])} />
                </TouchableOpacity>
              </View>
            )
          }
        }
        )}
      </ScrollView>

      <View style={styles.bottomContainer}>
        <View style={styles.bottomControls}>
          <TouchableOpacity onPress={() => navigation.navigate('PlayingScreen')}>
            <MaterialCommunityIcons color='#fff' name="headphones" size={30} />
          </TouchableOpacity>

          <TouchableOpacity onPress={() => navigation.navigate('SongsList')}>
            <Entypo color='#fff' name="list" size={30} />
          </TouchableOpacity>

          <TouchableOpacity onPress={() => navigation.navigate('LoadingScreen')}>
            <MaterialCommunityIcons color='#fff' name="cloud" size={30} />
          </TouchableOpacity>
        </View>
      </View>
    </SafeAreaView>
  )
}

export default WeatherHistory

const styles = StyleSheet.create({
  container: {
    flex: 1.5,
    backgroundColor: '#222831'
  },

  mainContainer: {
    flexDirection: 'row',
    flex: 1,
    width: '100%',
    alignItems: 'center',
    justifyContent: 'center'
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
    justifyContent: 'space-between',
    width: '80%'
  },

  dateHistory: {
    fontWeight: '200',
    fontSize: 20,
    paddingBottom: 0,
    color: 'white',
    alignSelf: 'center'
  }
})
