let get route path =
  Dream.get route (fun _ -> path |> Page.toString |> Dream.html)

let route =
  Dream.scope "/"
    [ Middleware.issue_csrf_cookie ]
    [
      get "/" [];
      get "/login" [ "login" ];
      get "/register" [ "register" ];
      get "/profile" [ "profile" ];
      get "/get-started" [ "get-started" ];
      get "/inbox" [ "inbox" ];
      get "/write" [ "write" ];
    ]
