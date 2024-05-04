module Register = Register
module Login = Login

let route =
  Dream.scope "/api"
    [ Middleware.verify_csrf_cookie ]
    [
      Dream.post "/register" Register.handler;
      Dream.get "/secrets" Secrets.get;
      Dream.post "/secrets" Secrets.post;
      Dream.post "/login" Login.handler;
      Dream.post "/logout" Logout.handler;
    ]
