package payment

class PayPal(private val email: String, private var balance: Money) extends PaymentMethod {

  val name: String = "PayPal"

  def pay(amount: Money): TransactionStatus = {
    if (amount <= balance) {
      balance -= amount
      logTransaction(amount, Success)
      Success
    } else {
      logTransaction(amount, Failure)
      Failure
    }
  }

  def recharge(amount: Money): Unit = {
    balance += amount
  }
  
  def getBalance: Money = balance
}
