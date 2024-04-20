let route =
  Dream.scope "/" [] [ Dream.get "/assets/**" @@ Dream.static "public" ]
