package com.mac0463ep2

import android.content.Intent
import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.util.Log
import android.widget.Toast
import androidx.activity.result.ActivityResult
import androidx.activity.result.contract.ActivityResultContracts
import com.google.android.gms.auth.api.signin.GoogleSignIn
import com.google.android.gms.auth.api.signin.GoogleSignInAccount
import com.google.android.gms.auth.api.signin.GoogleSignInClient
import com.google.android.gms.auth.api.signin.GoogleSignInOptions
import com.google.android.gms.common.api.ApiException
import com.google.firebase.auth.FirebaseAuth
import com.google.firebase.auth.GoogleAuthProvider
import com.mac0463ep2.databinding.ActivityLoginBinding
import java.lang.Exception

class LoginActivity : AppCompatActivity() {
    private lateinit var binding: ActivityLoginBinding

    private lateinit var googleSignInClient: GoogleSignInClient
    private lateinit var firebaseAuth: FirebaseAuth

    private companion object {
        private const val RC_SIGN_IN = 100
        private const val TAG = "GOOGLE_SIGN_IN_TAG"
    }

    private val registerGoogleBtn = registerForActivityResult(
        ActivityResultContracts.StartActivityForResult()
    ) { result: ActivityResult ->
        Log.d(TAG, "onActivityResult: Google SignIn intent result")
        if (result.resultCode == RESULT_OK) {
            val accountTask = GoogleSignIn.getSignedInAccountFromIntent(result.data)
            try {
                val account = accountTask.getResult(ApiException::class.java)
                firebaseAuthWithGoogleAccount(account)
            } catch (e: Exception) {
                Log.d(TAG, "onActivityResult: ${e.message}")
            }
        }
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        binding = ActivityLoginBinding.inflate(layoutInflater)
        setContentView(binding.root)

        firebaseAuth = FirebaseAuth.getInstance()
        checkUser()

        val googleSignInOptions = GoogleSignInOptions.Builder(GoogleSignInOptions.DEFAULT_SIGN_IN)
            .requestIdToken("1098455984904-s07tg9ao9duc5l1a760s7i84pqk5pqvo.apps.googleusercontent.com")
            .requestEmail()
            .build()
        googleSignInClient = GoogleSignIn.getClient(this, googleSignInOptions)

        binding.googleSignInBtn.setOnClickListener {
            Log.d(TAG, "onCreate: begin Google SignIn")
            val intent = googleSignInClient.signInIntent
            registerGoogleBtn.launch(intent)
        }

        binding.googleSignInBtn.translationY = 300f
        binding.email.translationX = 800f
        binding.pass.translationX = 800f
        binding.forgetPass.translationX = 800f
        binding.button.translationX = 800f

        binding.googleSignInBtn.alpha = 0f
        binding.email.alpha = 0f
        binding.pass.alpha = 0f
        binding.forgetPass.alpha = 0f
        binding.button.alpha = 0f

        binding.googleSignInBtn.animate().translationY(0f).alpha(1f).setDuration(1000).setStartDelay(400).start()
        binding.email.animate().translationX(0f).alpha(1f).setDuration(800).setStartDelay(300).start()
        binding.pass.animate().translationX(0f).alpha(1f).setDuration(800).setStartDelay(500).start()
        binding.forgetPass.animate().translationX(0f).alpha(1f).setDuration(800).setStartDelay(500).start()
        binding.button.animate().translationX(0f).alpha(1f).setDuration(800).setStartDelay(700).start()
    }

    private fun checkUser() {
        val firebaseUser = firebaseAuth.currentUser
        if (firebaseUser != null) {
            //startActivity(Intent(this@LoginActivity, WeatherActivity::class.java))
            //startActivity(Intent(this@LoginActivity, MapsActivity::class.java))
            startActivity(Intent(this@LoginActivity, MainActivity::class.java))
            finish()
        }
    }

    private fun firebaseAuthWithGoogleAccount(account: GoogleSignInAccount?) {
        Log.d(TAG, "firebaseAuthWithGoogleAccount: begin firebase auth with google account")

        val credential = GoogleAuthProvider.getCredential(account!!.idToken, null)
        firebaseAuth.signInWithCredential(credential)
            .addOnSuccessListener { authResult ->
                Log.d(TAG, "firebaseAuthWithGoogleAccount: LoggedIn")

                val firebaseUser = firebaseAuth.currentUser

                val uid = firebaseUser!!.uid
                val email = firebaseUser.email

                Log.d(TAG, "firebaseAuthWithGoogleAccount: UID: $uid")
                Log.d(TAG, "firebaseAuthWithGoogleAccount: Email: $email")

                if(authResult.additionalUserInfo!!.isNewUser) {
                    Log.d(TAG, "firebaseAuthWithGoogleAccount: Conta criada... \n$email")
                    Toast.makeText(this@LoginActivity, "Conta criada... $email", Toast.LENGTH_SHORT).show()
                }
                else {
                    Log.d(TAG, "firebaseAuthWithGoogleAccount: Usuário já existente... \n$email")
                    Toast.makeText(this@LoginActivity, "Logado com $email", Toast.LENGTH_SHORT).show()
                }

                //startActivity(Intent(this@LoginActivity, WeatherActivity::class.java))
                //startActivity(Intent(this@LoginActivity, MapsActivity::class.java))
                startActivity(Intent(this@LoginActivity, MainActivity::class.java))
                finish()
            }

            .addOnFailureListener { e ->
                Log.d(TAG, "firebaseAuthWithGoogleAccount: Login Falhou por conta de ${e.message}")
                Toast.makeText(this@LoginActivity, "Login Falhou por conta de ${e.message}", Toast.LENGTH_SHORT).show()
            }
    }
}