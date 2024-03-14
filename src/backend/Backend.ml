let error_template _ _ suggested_response =
  let status = Dream.status suggested_response in
  let code = Dream.status_to_int status in
  let reason = Dream.status_to_string status in
  match code with
  | 404 ->
      Dream.set_header suggested_response "Content-Type" Dream.text_html;
      Dream.set_body suggested_response @@ Pages.WithApp.render [ "not-found" ];
      Lwt.return suggested_response
  | _ ->
      Dream.set_header suggested_response "Content-Type" Dream.application_json;
      Dream.set_body suggested_response
        (Printf.sprintf {|"code": %i, "reason": "%s"|} code reason);
      Lwt.return suggested_response

let error_handler = Dream.error_template error_template
let interface = "0.0.0.0"

let () =
  Dream.run ~error_handler ~interface
  @@ Dream.logger
  @@ Dream.router
       [
         Dream.get "assets/**" @@ Dream.static "assets";
         Dream.get "static/**" @@ Dream.static "dist";
         Dream.scope "/" []
           [
             Dream.get "/login" (fun _ ->
                 [ "login" ] |> Pages.WithApp.render |> Dream.html);
             Dream.get "/register" (fun _ ->
                 [ "register" ] |> Pages.WithApp.render |> Dream.html);
           ];
       ]
