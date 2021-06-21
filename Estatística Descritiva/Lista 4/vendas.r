library(ggplot2)

# Lê o conjunto de dados
vendas <- read_excel ("vendas.xls")

# Detalhes do ajuste de regressão linear para o problema
vendas.lm <- lm(vendas ~ mês, data = vendas)
summary(vendas.lm)

# Ajuste do modelo aos dados 
vendas.graph <- ggplot (vendas, aes(x = mês, y = vendas)) + geom_point()
vendas.graph <- vendas.graph + geom_smooth(method="lm", col="black")
vendas.graph

# Ferramentas de diagnóstico
plot (vendas.lm)
plot (lm(vendas$vendas ~ vendas$mês), which = 4)