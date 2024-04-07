module Register = Register
module Login = Login

let route =
  Dream.scope "/api"
    [ Middleware.verify_csrf_cookie ]
    [
      Dream.post "/register" Register.handler;
      Dream.post "/secrets" Secrets.handler;
      Dream.post "/login" Login.handler;
      Dream.post "/logout" Logout.handler;
    ]
