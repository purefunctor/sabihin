let route =
  Dream.scope "/" []
    [
      Dream.get "/assets/**" @@ Dream.static "assets";
      Dream.get "/static/**" @@ Dream.static "dist";
    ]
