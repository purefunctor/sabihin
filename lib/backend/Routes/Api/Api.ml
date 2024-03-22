let route =
  Dream.scope "/api"
    [ Middleware.verify_csrf_cookie ]
    [
      Dream.post "/register" Register.handler; Dream.post "/login" Login.handler;
    ]
