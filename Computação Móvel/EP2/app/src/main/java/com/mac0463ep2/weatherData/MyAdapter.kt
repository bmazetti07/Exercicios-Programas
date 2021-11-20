package com.mac0463ep2.weatherData

import android.annotation.SuppressLint
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.TextView
import androidx.recyclerview.widget.RecyclerView
import com.mac0463ep2.R
import java.text.SimpleDateFormat
import java.util.*
import kotlin.collections.ArrayList

class MyAdapter(private val weatherList: ArrayList<CurrentWeatherResponse>): RecyclerView.Adapter<MyAdapter.MyViewHolder>() {
    class MyViewHolder(itemView: View): RecyclerView.ViewHolder(itemView) {
        val cityName: TextView = itemView.findViewById(R.id.historyCityName)
        val date: TextView = itemView.findViewById(R.id.historyDate)
        val temp: TextView = itemView.findViewById(R.id.historyTemp)
    }

    override fun onCreateViewHolder(parent: ViewGroup, viewType: Int): MyViewHolder {
        val itemView = LayoutInflater.from(parent.context).inflate(R.layout.weather_item, parent, false)
        return MyViewHolder(itemView)
    }

    override fun onBindViewHolder(holder: MyViewHolder, position: Int) {
        val currentItem = weatherList[position]

        holder.cityName.text = currentItem.locationName
        holder.date.text = SimpleDateFormat("dd/MM/yyyy hh:mma", Locale.ENGLISH).format(Date(currentItem.date!!.toLong() * 1000))
        holder.temp.text = currentItem.temp + " °C"
    }

    override fun getItemCount(): Int {
        return weatherList.size
    }
}
