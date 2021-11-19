import React, { useState, useEffect } from 'react'
import {
  View,
  ScrollView,
  Image,
  Text,
  StyleSheet
} from 'react-native'

function toTitleCase(str) {
  return str.replace(
    /\w\S*/g,
    function (txt) {
      return txt.charAt(0).toUpperCase() + txt.substr(1).toLowerCase();
    }
  );
}

const WeatherScroll = ({ currentWeatherDetails }) => {
  const imgID = currentWeatherDetails.weather[0].icon
  const img = { uri: `https://openweathermap.org/img/wn/${imgID}@4x.png` }
  const descricao = toTitleCase(currentWeatherDetails.weather[0].description)
  const minima = Math.round(currentWeatherDetails.main.temp_min)
  const maxima = Math.round(currentWeatherDetails.main.temp_max)
  const atual = Math.round(currentWeatherDetails.main.temp)
  const diaSemana = currentWeatherDetails.weekDay

  return (
    <ScrollView style={styles.scrollView}>
      <View style={styles.CurrentTempContainer}>
        <View style={styles.leftContainer}>
          <Image source={img} style={styles.image} />
          <Text style={styles.descricao}> {descricao} </Text>
        </View>
        <View style={styles.rightContainer}>
          <Text style={styles.dia}>{diaSemana}</Text>
          <Text style={styles.temperaturaAtual}>{atual}&#176;C</Text>
          <Text style={styles.temperaturaMinima}>Mínima: {minima}&#176;C</Text>
          <Text style={styles.temperaturaMaxima}>Máxima: {maxima}&#176;C</Text>
        </View>
      </View>
    </ScrollView>
  )
}

export default WeatherScroll

const styles = StyleSheet.create({
  scrollView: {
    flex: 0.4,
    backgroundColor: '#222831',
    paddingLeft: 20,
    paddingRight: 20,
    paddingBottom: 30,
  },

  image: {
    width: 150,
    height: 150,
  },

  CurrentTempContainer: {
    flexDirection: 'row',
    backgroundColor: '#18181b99',
    justifyContent: 'center',
    alignItems: 'center',
    borderWidth: 1,
    borderRadius: 15,
    borderColor: 'gray',
    padding: 15,
    marginTop: 10,
  },

  dia: {
    fontSize: 20,
    color: 'white',
    backgroundColor: 'gray',
    padding: 10,
    textAlign: 'center',
    borderRadius: 50,
    fontWeight: '200',
    marginBottom: 15,
  },

  temperaturaMinima: {
    fontSize: 15,
    color: '#ADD8E6',
    fontWeight: '100',
    textAlign: 'center',
  },

  temperaturaMaxima: {
    fontSize: 15,
    color: '#FF6347',
    fontWeight: '100',
    textAlign: 'center',
  },

  temperaturaAtual: {
    fontSize: 25,
    color: 'white',
    fontWeight: '100',
    textAlign: 'center',
    marginBottom: 5,
  },

  rightContainer: {
    paddingRight: 40,
  },

  leftContainer: {
    flexDirection: 'column',
    justifyContent: 'space-between'
  },

  descricao: {
    color: 'white',
    textAlign: 'center',
    fontSize: 20,
    marginTop: -20,
    justifyContent: 'center'
  }
})