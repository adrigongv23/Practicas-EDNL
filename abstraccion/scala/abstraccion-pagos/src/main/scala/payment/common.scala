package payment

type Money = Double
sealed trait TransactionStatus

case object Success extends TransactionStatus
case object Failure extends TransactionStatus
