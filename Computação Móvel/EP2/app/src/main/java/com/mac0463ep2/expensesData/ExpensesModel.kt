package com.mac0463ep2.expensesData

data class ExpensesModel (
    val expenses_title: String?,
    val expenses_value: String?,
    val expenses_type_string: String?,
    val expenses_type: Boolean?,
    val expenses_date: String?,
    val expenses_icon: Int
)