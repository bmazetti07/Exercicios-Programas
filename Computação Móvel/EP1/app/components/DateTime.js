import React, { useEffect, useState } from 'react'
import { View, Text, StyleSheet, Image } from 'react-native'
import WeatherScroll from '../components/WeatherScroll';

const WeatherItem = ({ title, value, unit }) => {
  return (
    <View style={styles.weatherItem}>
      <Text style={styles.weatherItemTopic}>{title}</Text>
      <Text style={styles.weatherItemTopic}>{value}{unit}</Text>
    </View>
  )
}

const semana = ['Domingo', 'Segunda', 'Terça', 'Quarta', 'Quinta', 'Sexta', 'Sabado'];
const ano = ['Jan', 'Fev', 'Mar', 'Abr', 'Mai', 'Jun', 'Jul', 'Ago', 'Set', 'Out', 'Nov', 'Dez'];

const DateTime = ({ currentWeatherDetails }) => {
  const [dia, setDia] = useState('')
  const [horario, setHorario] = useState('')

  useEffect(() => {
    setInterval(() => {
      const time = new Date();
      const mes = time.getMonth();
      const data = time.getDate();
      const dia = time.getDay();
      const hora = time.getHours();
      const minutos = time.getMinutes();

      setDia(semana[dia] + ', ' + data + ', ' + ano[mes])

      setHorario((hora < 10 ? '0' + hora : hora) + ':' + (minutos < 10 ? '0' + minutos : minutos))
    }, 1000)
  }, [])

  const umidade = currentWeatherDetails.main.humidity
  const pressao = currentWeatherDetails.main.pressure
  const vento = currentWeatherDetails.wind.speed
  const nebulosidade = currentWeatherDetails.clouds.all
  let lat = currentWeatherDetails.coord.lat
  lat = Math.floor(lat * 100) / 100;
  let lon = currentWeatherDetails.coord.lon
  lon = Math.floor(lon * 100) / 100;
  const cidade = currentWeatherDetails.name

  return (
    <View style={styles.weatherContainer}>
      <View style={{ flex: 2 }}>
        <View>
          <Text style={styles.weatherHeading}>{horario}</Text>
        </View>
        <View>
          <Text style={styles.weatherSubHeading}>{dia}</Text>
        </View>
        <View style={styles.weatherItensContainer}>
          <WeatherItem title="Umidade" value={umidade} unit="%" />
          <WeatherItem title="Pressão" value={pressao} unit="hPa" />
          <WeatherItem title="Vel. Vento" value={vento} unit="m/s" />
          <WeatherItem title="Nebulosidade" value={nebulosidade} unit="%" />
        </View>
      </View>

      <View style={styles.rightAlign}>
        <Text style={styles.weatherPlace}>{cidade}</Text>
        <Text style={styles.weatherCoords}>{lat}N {lon}L</Text>
      </View>
    </View>
  )
}

export default DateTime

const styles = StyleSheet.create({
  weatherContainer: {
    flex: 1,
    flexDirection: "row",
    justifyContent: "space-between",
    padding: 15,
    marginBottom: 40,
    paddingLeft: 15,
  },

  weatherHeading: {
    fontSize: 45,
    color: 'white',
    fontWeight: '100',
  },

  weatherSubHeading: {
    fontSize: 25,
    color: '#eee',
    fontWeight: '300',
  },

  weatherItensContainer: {
    backgroundColor: "#18181b99",
    borderRadius: 10,
    padding: 10,
    marginTop: 10,
    borderWidth: 1,
    borderColor: 'gray',
    elevation: 100,
  },

  weatherItem: {
    flexDirection: "row",
    justifyContent: 'space-between'
  },

  weatherItemTopic: {
    color: 'white',
    fontSize: 14,
    fontWeight: '100',
    justifyContent: 'space-between'
  },

  weatherPlace: {
    fontSize: 20,
    color: 'white',
    textAlign: 'center',
  },

  weatherCoords: {
    fontSize: 16,
    color: 'white',
    fontWeight: '700',
    textAlign: 'center',
  },

  rightAlign: {
    textAlign: 'right',
    marginTop: 20,
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

  temperatura: {
    fontSize: 15,
    color: 'white',
    fontWeight: '100',
    textAlign: 'center',
  },

  innerContainer: {
    paddingRight: 40,
  },
})