package com.mac0463ep2

import android.content.Intent
import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import androidx.databinding.DataBindingUtil.setContentView
import androidx.fragment.app.Fragment
import com.google.android.gms.auth.api.signin.GoogleSignIn
import com.google.android.gms.auth.api.signin.GoogleSignInOptions
import com.google.firebase.auth.FirebaseAuth
import com.mac0463ep2.databinding.FragmentProfileBinding
import com.squareup.picasso.Picasso

class ProfileFragment : Fragment() {
    private lateinit var binding: FragmentProfileBinding
    private lateinit var firebaseAuth: FirebaseAuth

    override fun onCreateView(
        inflater: LayoutInflater,
        container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View {

        binding = FragmentProfileBinding.inflate(layoutInflater)
        return binding.root
    }

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)

        firebaseAuth = FirebaseAuth.getInstance()
        checkUser()

        binding.logoutBtn.setOnClickListener {
            val googleSignInOptions = GoogleSignInOptions.Builder(GoogleSignInOptions.DEFAULT_SIGN_IN)
                .requestIdToken("1098455984904-s07tg9ao9duc5l1a760s7i84pqk5pqvo.apps.googleusercontent.com")
                .requestEmail()
                .build()
            val googleSignInClient = GoogleSignIn.getClient(requireContext(), googleSignInOptions)
            googleSignInClient.signOut()

            firebaseAuth.signOut()
            checkUser()
        }
    }

    private fun checkUser() {
        val firebaseUser = firebaseAuth.currentUser
        if (firebaseUser == null) {
            startActivity(Intent(requireContext(), LoginActivity::class.java))
        }
        else {
            val displayName = firebaseUser.displayName
            val email = firebaseUser.email
            val pic = firebaseUser.photoUrl

            Picasso
                .get()
                .load(pic)
                .resize(500, 500)
                .into(binding.profilePic)
            binding.emailTv.text = email
            binding.tvTitle.text = displayName?.uppercase()
        }
    }
}