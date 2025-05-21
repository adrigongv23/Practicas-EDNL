package payment

class CreditCard(cardNumber: String, var creditLimit: Money) extends PaymentMethod {

  val name: String = "Tarjeta de Crédito"
  def pay(amount: Money): TransactionStatus = {
    if (amount <= creditLimit) {
      creditLimit -= amount
      logTransaction(amount, Success)
      Success
    } else {
      logTransaction(amount, Failure)
      Failure
    }
  }
}