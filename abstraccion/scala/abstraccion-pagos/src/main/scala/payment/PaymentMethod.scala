package payment

// Clase abstracta base
abstract class PaymentMethod {

  val name: String

  def pay(amount: Money): TransactionStatus

  protected def logTransaction(amount: Money, status: TransactionStatus): Unit = {
    println(s"[$name] Transacción de $amount€ => Estado: $status")
  }
}
