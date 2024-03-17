module Register = struct
  let handler request =
    match%lwt Dream.multipart ~csrf:false request with
    | `Ok [ ("username", [ (_, username) ]) ] ->
        Dream.respond ~status:`OK username
    | _ -> Dream.respond ~status:`Bad_Request "Bad Request."
end

let route =
  Dream.scope "/api"
    [ Middleware.verify_csrf_cookie ]
    [
      Dream.post "/register" Register.handler;
    ]
