package com.mac0463ep2

import android.content.Intent
import android.content.pm.PackageManager
import android.location.Location
import android.os.Bundle
import android.util.DisplayMetrics
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.Toast
import androidx.core.app.ActivityCompat
import androidx.fragment.app.Fragment
import androidx.recyclerview.widget.LinearLayoutManager
import androidx.recyclerview.widget.RecyclerView
import com.google.android.gms.location.FusedLocationProviderClient
import com.google.android.gms.location.LocationServices
import com.google.firebase.auth.FirebaseAuth
import com.google.firebase.database.*
import com.mac0463ep2.weatherData.CurrentWeatherResponse
import com.mac0463ep2.weatherData.MyAdapter
import com.mac0463ep2.databinding.FragmentWeatherBinding
import com.squareup.picasso.Picasso
import kotlinx.coroutines.CoroutineScope
import kotlinx.coroutines.Dispatchers.IO
import kotlinx.coroutines.Dispatchers.Main
import kotlinx.coroutines.launch
import kotlinx.coroutines.withContext
import org.json.JSONObject
import java.net.URL
import java.text.SimpleDateFormat
import java.util.*
import kotlin.math.roundToInt

class WeatherFragment : Fragment() {
    private lateinit var binding: FragmentWeatherBinding
    private lateinit var firebaseAuth: FirebaseAuth

    private var currentLocation: Location? = null
    private var fusedLocationProviderClient: FusedLocationProviderClient? = null

    private lateinit var database: DatabaseReference
    private lateinit var weatherRecyclerView: RecyclerView
    private lateinit var weatherArrayList: ArrayList<CurrentWeatherResponse>
    private lateinit var weatherJSON: JSONObject

    private companion object {
        private const val BASE_WEATHER_URL = "https://api.openweathermap.org/data/2.5/weather?"
        private const val WEATHER_API_KEY = "2655473f065b2f9524803e99f782218f"
        private const val REQUEST_CODE = 101
    }

    override fun onCreateView(
        inflater: LayoutInflater,
        container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View {

        binding = FragmentWeatherBinding.inflate(layoutInflater)
        return binding.root
    }

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)

        val displayMetrics = DisplayMetrics()
        activity?.windowManager?.defaultDisplay?.getMetrics(displayMetrics)

        val width = displayMetrics.widthPixels
        val height = displayMetrics.heightPixels

        binding.weatherBGImage.layoutParams.height = (height * 0.8).toInt()
        binding.weatherBGImage.layoutParams.width = width

        binding.weatherLoading.visibility = View.VISIBLE
        binding.weatherContainer.visibility = View.GONE
        binding.weatherErrorText.visibility = View.GONE

        firebaseAuth = FirebaseAuth.getInstance()
        checkUser()

        fusedLocationProviderClient = LocationServices.getFusedLocationProviderClient(requireContext())
        fetchLocation()

        weatherRecyclerView = binding.historyList
        weatherRecyclerView.layoutManager = LinearLayoutManager(requireContext())
        weatherRecyclerView.setHasFixedSize(true)

        weatherArrayList = arrayListOf()
        getUserData()

        binding.saveWeather.setOnClickListener {
            saveWeatherToFirebase()
        }
    }

    override fun onRequestPermissionsResult(
        requestCode: Int,
        permissions: Array<out String>,
        grantResults: IntArray
    ) {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults)
        when (requestCode) {
            REQUEST_CODE -> {
                if (grantResults.isNotEmpty() && grantResults[0] == PackageManager.PERMISSION_GRANTED) {
                    fetchLocation()
                }

                else {
                    binding.weatherLoading.visibility = View.GONE
                    binding.permissionErrorText.visibility = View.VISIBLE
                }
            }
        }
    }

    private fun fetchLocation() {
        if (ActivityCompat.checkSelfPermission(requireContext(), android.Manifest.permission.ACCESS_FINE_LOCATION) != PackageManager.PERMISSION_GRANTED
            && ActivityCompat.checkSelfPermission(requireContext(), android.Manifest.permission.ACCESS_COARSE_LOCATION) != PackageManager.PERMISSION_GRANTED
        ) {
            ActivityCompat.requestPermissions(requireActivity(), arrayOf(android.Manifest.permission.ACCESS_FINE_LOCATION), REQUEST_CODE)
            return
        }

        val task = fusedLocationProviderClient!!.lastLocation
        task.addOnSuccessListener { location ->
            if (location != null) {
                currentLocation = location

                val latitude = currentLocation!!.latitude
                val longitude = currentLocation!!.longitude

                showWeather(latitude, longitude)
            }
        }

        task.addOnFailureListener { error ->
            println(error)
        }
    }

    private fun checkUser() {
        val firebaseUser = firebaseAuth.currentUser
        if (firebaseUser == null) {
            startActivity(Intent(requireContext(), LoginActivity::class.java))
        }
    }

    private fun showWeather(latitude: Double, longitude: Double) {
        CoroutineScope(IO).launch {
            val weatherUrl = "${BASE_WEATHER_URL}lat=${latitude}&lon=${longitude}&units=metric&appid=${WEATHER_API_KEY}&lang=pt"
            val response: String? = try {
                URL(weatherUrl).readText(Charsets.UTF_8)
            } catch(e: Exception) {
                null
            }

            withContext(Main) {
                try {
                    val jsonObj = JSONObject(response!!)
                    weatherJSON = JSONObject(response)

                    val main = jsonObj.getJSONObject("main")
                    val weather = jsonObj.getJSONArray("weather").getJSONObject(0)

                    val temp = main.getDouble("temp").roundToInt().toString()
                    val description = weather.getString("description")
                    val locationName = jsonObj.getString("name")
                    val updateAt = jsonObj.getLong("dt")
                    val updateText = SimpleDateFormat("dd/MM/yyyy hh:mma", Locale.ENGLISH).format(Date(updateAt * 1000))

                    val words = description.split(' ');
                    binding.conditionCurrent.text = words.joinToString(separator = " ") { word -> word.replaceFirstChar { it.uppercase() } }
                    binding.tempCurrent.text = temp
                    binding.location.text = locationName
                    binding.dateCurrentWeather.text = updateText

                    val firebaseUser = firebaseAuth.currentUser
                    val pic = firebaseUser?.photoUrl
                    Picasso
                        .get()
                        .load(pic)
                        .resize(32, 32)
                        .into(binding.profilePic)

                    binding.weatherLoading.visibility = View.GONE
                    binding.weatherContainer.visibility = View.VISIBLE
                }

                catch(e: Exception) {
                    binding.weatherLoading.visibility = View.GONE
                    binding.weatherErrorText.visibility = View.VISIBLE
                }
            }
        }
    }

    private fun saveWeatherToFirebase() {
        val locationName = weatherJSON.getString("name")

        val date = weatherJSON.getLong("dt").toString()

        val main = weatherJSON.getJSONObject("main")
        val temp = main.getDouble("temp").roundToInt().toString()

        val weather = weatherJSON.getJSONArray("weather").getJSONObject(0)
        val weatherIcon = weather.getString("icon")

        val firebaseUser = firebaseAuth.currentUser
        database = FirebaseDatabase.getInstance().getReference(firebaseUser!!.uid)
        val currentWeather = CurrentWeatherResponse(locationName, date, temp, weatherIcon)
        database.child(date).setValue(currentWeather)
            .addOnSuccessListener {
                Toast.makeText(requireContext(), "Salvo com sucesso!", Toast.LENGTH_SHORT).show()
            }

            .addOnFailureListener {
                Toast.makeText(requireContext(), "Ocorreu algum erro!", Toast.LENGTH_SHORT).show()
            }
    }

    private fun getUserData() {
        val firebaseUser = firebaseAuth.currentUser
        database = FirebaseDatabase.getInstance().getReference(firebaseUser!!.uid)

        database.addValueEventListener(object: ValueEventListener {
            override fun onDataChange(snapshot: DataSnapshot) {
                if (snapshot.exists()) {
                    for (weatherSnapshot in snapshot.children) {
                        val weatherItem = weatherSnapshot.getValue(CurrentWeatherResponse::class.java)
                        if (!weatherArrayList.contains(weatherItem))
                            weatherArrayList.add(weatherItem!!)
                    }

                    weatherRecyclerView.adapter = MyAdapter(weatherArrayList)
                }
            }

            override fun onCancelled(error: DatabaseError) {
                println("Cancelado!")
            }
        })
    }

    private fun String.capitalizeFirstLetter() = this.split(" ").joinToString(" ") { it.capitalize() }.trimEnd()
}