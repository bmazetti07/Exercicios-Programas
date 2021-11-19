import React, { Component } from 'react';
import { createNativeStackNavigator } from '@react-navigation/native-stack';
import SongsList from '../screens/SongsList';
import MusicPlayer from '../screens/MusicPlayer';
import LoginWeather from '../screens/LoginWeather';
import WeatherHistory from '../screens/WeatherHistory';
import LoadingScreen from '../screens/LoadingScreen';
import MustLogin from '../screens/MustLogin';

const Stack = createNativeStackNavigator();

const AppNavigator = () => {
  return (
    <Stack.Navigator initialRouteName="PlayingScreen" screenOptions={{ headerShown: true }}>
      <Stack.Screen
        name="PlayingScreen"
        component={MusicPlayer}
        options={{
          title: 'Em execução',
          headerStyle: {
            backgroundColor: '#262831',
          },
          headerTintColor: '#fff',
          headerTitleStyle: {
            fontWeight: 'bold',
          },
        }}
      />

      <Stack.Screen
        name="SongsList"
        component={SongsList}
        options={{
          title: 'Lista de Músicas',
          headerStyle: {
            backgroundColor: '#262831',
          },
          headerTintColor: '#fff',
          headerTitleStyle: {
            fontWeight: 'bold',
          },
        }}
      />

      <Stack.Screen
        name="LoginWeatherScreen"
        component={LoginWeather}
        options={{
          title: 'Clima',
          headerStyle: {
            backgroundColor: '#262831',
          },
          headerTintColor: '#fff',
          headerTitleStyle: {
            fontWeight: 'bold',
          },
        }}
      />

      <Stack.Screen
        name="WeatherHistory"
        component={WeatherHistory}
        options={{
          title: 'Histórico do Clima',
          headerStyle: {
            backgroundColor: '#262831',
          },
          headerTintColor: '#fff',
          headerTitleStyle: {
            fontWeight: 'bold',
          },
        }}
      />

      <Stack.Screen
        name="LoadingScreen"
        component={LoadingScreen}
        options={{
          title: 'Tela de Carregamento',
          headerStyle: {
            backgroundColor: '#262831',
          },
          headerTintColor: '#fff',
          headerTitleStyle: {
            fontWeight: 'bold',
          },
        }}
      />

      <Stack.Screen
        name="LoginScreen"
        component={MustLogin}
        options={{
          headerShown: false,
          headerStyle: {
            backgroundColor: '#262831',
          },
          headerTintColor: '#fff',
          headerTitleStyle: {
            fontWeight: 'bold',
          },
        }}
      />
    </Stack.Navigator>
  );
};

export default AppNavigator;