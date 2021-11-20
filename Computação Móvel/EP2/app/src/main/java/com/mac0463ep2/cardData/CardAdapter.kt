package com.mac0463ep2.cardData

import android.content.Context
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.Toast
import androidx.viewpager.widget.PagerAdapter
import com.mac0463ep2.R
import kotlinx.android.synthetic.main.card_item.view.*

class CardAdapter (private val context: Context, private val cardModelArrayList: ArrayList<CardModel>) : PagerAdapter() {
    override fun getCount(): Int {
        return cardModelArrayList.size
    }

    override fun isViewFromObject(view: View, `object`: Any): Boolean {
        return view == `object`
    }

    override fun instantiateItem(container: ViewGroup, position: Int): Any {
        val view = LayoutInflater.from(context).inflate(R.layout.card_item, container, false)

        val card = cardModelArrayList[position]
        val card_background = card.card_background
        val bank_name = card.bank_name
        val card_number = card.card_number
        var exp_date = card.exp_date
        var client_name = card.client_name
        var balance = card.balance
        var logo_image = card.logo_image

        view.card_background.setImageResource(card_background)
        view.bank_name.text = bank_name
        view.card_number.text = card_number
        view.expiration_date.text = exp_date
        view.client_name.text = client_name
        view.balance.text = balance
        view.bank_logo.setImageResource(logo_image)

        view.setOnClickListener {
            Toast.makeText(context, "$bank_name", Toast.LENGTH_SHORT).show()
        }

        container.addView(view, position)

        return view
    }

    override fun destroyItem(container: ViewGroup, position: Int, `object`: Any) {
        container.removeView(`object` as View)
    }
}