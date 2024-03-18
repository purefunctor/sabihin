let get route path =
  Dream.get route (fun _ -> path |> Render.AppPage.render |> Dream.html)

let route =
  Dream.scope "/"
    [ Middleware.issue_csrf_cookie ]
    [ get "/" []; get "/login" [ "login" ]; get "/register" [ "register" ] ]
