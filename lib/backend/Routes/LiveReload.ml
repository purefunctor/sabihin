let route () =
  if !Dev.live_reload then
    Dream.get "_livereload" (fun _ ->
        Dream.websocket (fun socket ->
            let%lwt _ = Dream.receive socket in
            Dream.close_websocket socket))
  else Dream.no_route
