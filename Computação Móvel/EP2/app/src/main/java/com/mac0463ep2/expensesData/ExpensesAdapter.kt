package com.mac0463ep2.expensesData

import android.graphics.Color
import android.view.LayoutInflater
import android.widget.TextView
import android.view.View
import android.view.ViewGroup
import android.widget.ImageView
import androidx.recyclerview.widget.RecyclerView
import com.mac0463ep2.R

class ExpensesAdapter(private val expensesList: ArrayList<ExpensesModel>): RecyclerView.Adapter<ExpensesAdapter.ExpensesViewHolder>() {
    class ExpensesViewHolder(itemView: View) : RecyclerView.ViewHolder(itemView) {
        val title: TextView = itemView.findViewById(R.id.expenses_title)
        val value: TextView = itemView.findViewById(R.id.expenses_value)
        val type_string: TextView = itemView.findViewById(R.id.expenses_type_string)
        val date: TextView = itemView.findViewById(R.id.expenses_date)
        val icon: ImageView = itemView.findViewById(R.id.expenses_icon)
    }

    override fun onCreateViewHolder(parent: ViewGroup, viewType: Int): ExpensesViewHolder {
        val itemView = LayoutInflater.from(parent.context).inflate(R.layout.card_expenses_item, parent, false)
        return ExpensesViewHolder(itemView)
    }

    override fun onBindViewHolder(holder: ExpensesViewHolder, position: Int) {
        val currentItem = expensesList[position]

        holder.title.text = currentItem.expenses_title
        holder.type_string.text = currentItem.expenses_type_string
        holder.date.text = currentItem.expenses_date
        holder.icon.setImageResource(currentItem.expenses_icon)

        if (currentItem.expenses_type == true) {
            holder.value.setTextColor(Color.parseColor("#32CD32"))
            holder.value.text = "+ R$ " + currentItem.expenses_value
        }
        else {
            holder.value.setTextColor(Color.parseColor("#FF6347"))
            holder.value.text = "- R$ " + currentItem.expenses_value
        }
    }

    override fun getItemCount(): Int {
        return expensesList.size
    }
}