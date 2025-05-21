package payment

import org.scalatest.funsuite.AnyFunSuite

class PaymentMethodSpec extends AnyFunSuite {

  test("CreditCard permite pago si hay suficiente crédito") {
    val card = new CreditCard("1234", 100.0)
    val result = card.pay(50.0)
    assert(result == Success)
  }

  test("CreditCard rechaza pago si no hay suficiente crédito") {
    val card = new CreditCard("1234", 30.0)
    val result = card.pay(50.0)
    assert(result == Failure)
  }

  test("PayPal permite pago si hay saldo suficiente") {
    val paypal = new PayPal("user@example.com", 80.0)
    val result = paypal.pay(60.0)
    assert(result == Success)
  }

  test("PayPal rechaza pago si el saldo es insuficiente") {
    val paypal = new PayPal("user@example.com", 20.0)
    val result = paypal.pay(30.0)
    assert(result == Failure)
  }

  test("PayPal recarga saldo correctamente") {
    val paypal = new PayPal("user@example.com", 0.0)
    paypal.recharge(100.0)
    assert(paypal.getBalance == 100.0)
  }
}
