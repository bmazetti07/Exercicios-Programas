package com.mac0463ep2.weatherData

data class CurrentWeatherResponse(
    val locationName: String ?= null,
    val date: String ?= null,
    val temp: String ?= null,
    val weatherIcon: String ?= null
){}