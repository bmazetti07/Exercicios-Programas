package com.mac0463ep2

import android.os.Bundle
import androidx.fragment.app.Fragment
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import androidx.recyclerview.widget.DividerItemDecoration
import androidx.recyclerview.widget.LinearLayoutManager
import androidx.recyclerview.widget.RecyclerView
import androidx.viewpager.widget.ViewPager
import com.google.firebase.auth.FirebaseAuth
import com.google.firebase.auth.FirebaseUser
import com.mac0463ep2.cardData.CardAdapter
import com.mac0463ep2.cardData.CardModel
import com.mac0463ep2.databinding.FragmentCardBinding
import com.mac0463ep2.expensesData.ExpensesAdapter
import com.mac0463ep2.expensesData.ExpensesModel
import kotlinx.android.synthetic.main.fragment_card.*

class CardFragment : Fragment() {
    private lateinit var binding : FragmentCardBinding

    private lateinit var expensesModelList : ArrayList<ExpensesModel>
    private lateinit var expensesRecyclerView : RecyclerView

    private lateinit var cardModelList : ArrayList<CardModel>
    private lateinit var cardAdapter : CardAdapter

    private var firebaseUser: FirebaseUser? = FirebaseAuth.getInstance().currentUser
    private var displayName: String? = firebaseUser?.displayName?.uppercase()

    override fun onCreateView(
        inflater: LayoutInflater, container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View {
        binding = FragmentCardBinding.inflate(layoutInflater)
        return binding.root
    }

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)

        binding.username.text = displayName

        loadCards()

        loadExpenses(0)
        cards_view_pager.addOnPageChangeListener(object: ViewPager.OnPageChangeListener {
            override fun onPageScrollStateChanged(state: Int) {}
            override fun onPageScrolled(position: Int, positionOffset: Float, positionOffsetPixels: Int) {}

            override fun onPageSelected(position: Int) {
                loadExpenses(position)
            }
        })
    }

    private fun loadCards() {
        cardModelList = ArrayList()

        cardModelList.add(
            CardModel(
                R.drawable.card_background1,
                "NUBANK",
                "0123 4567 8910 1112",
                "12/27",
                displayName,
                "R$ 15.000,00",
                R.drawable.mastercard
            )
        )

        cardModelList.add(
            CardModel(
                R.drawable.card_background2,
                "INTER",
                "1211 1098 7654 3210",
                "07/28",
                displayName,
                "R$ 1.250,00",
                R.drawable.elo
            )
        )

        cardAdapter = CardAdapter(requireContext(), cardModelList)
        cards_view_pager.adapter = cardAdapter
        cards_view_pager.setPadding(100, 0, 100, 0)
    }

    private fun loadExpenses(position: Int) {
        expensesModelList = ArrayList()

        when(position) {
            0 -> {
                expensesModelList.addAll(
                    listOf(
                        ExpensesModel(
                            "Padaria",
                            "3.000,00",
                            "Compra",
                            false,
                            "16 Dezembro 2015",
                            R.drawable.ic_coffee
                        ),
                        ExpensesModel(
                            "PSN Plus",
                            "199,99",
                            "Assinatura",
                            false,
                            "01 Janeiro 2021",
                            R.drawable.ic_games
                        ),
                        ExpensesModel(
                            "Trabalho",
                            "15.054,01",
                            "Renda",
                            true,
                            "31 Fevereiro 2021",
                            R.drawable.ic_work
                        )
                    )
                )
            }

            1 -> {
                expensesModelList.add(
                        ExpensesModel(
                            "Trabalho",
                            "20,00",
                            "Renda",
                            true,
                            "05 Dezembro 2021",
                            R.drawable.ic_work
                        )
                )
            }

            else -> {

            }
        }

        expensesRecyclerView = binding.expensesRecyclerView
        expensesRecyclerView.layoutManager = LinearLayoutManager(context)
        expensesRecyclerView.setHasFixedSize(true)

        expensesRecyclerView.adapter = ExpensesAdapter(expensesModelList)
    }
}