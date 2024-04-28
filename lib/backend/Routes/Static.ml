let route () =
  (* The assets route is where Vite bundles the generated assets. *)
  let assets_route =
    if Vite.is_dev () then Dream.no_route
    else Dream.get "/assets/**" @@ Dream.static "dist/assets"
  in
  (* The static route is where non-bundled assets are located. *)
  let static_route =
    Dream.get "/static/**" @@ Dream.static @@ Vite.get_static_path ()
  in
  Dream.scope "/" [] [ assets_route; static_route ]
