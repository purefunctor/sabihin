open Types_native.Dynamic

let error_template _ _ suggested_response =
  let status = Dream.status suggested_response in
  let code = Dream.status_to_int status in
  match code with
  | 404 ->
      Dream.set_header suggested_response "Content-Type" Dream.text_html;
      Dream.set_body suggested_response @@ Render.AppPage.render [ "not-found" ];
      Lwt.return suggested_response
  | _ ->
      let content_type = Dream.header suggested_response "Content-Type" in
      let%lwt content = Dream.body suggested_response in

      let body, content_type =
        match content_type with
        | Some "application/json" ->
            ( string_of_unparsed_error_response { content = `JSON content },
              "application/json" )
        | Some content_type -> (content, content_type)
        | None ->
            ( string_of_unparsed_error_response { content = `Raw content },
              "application/json" )
      in

      Dream.set_body suggested_response body;
      Dream.set_header suggested_response "Content-Type" content_type;

      Lwt.return suggested_response

let error_handler = Dream.error_template error_template
let interface = "0.0.0.0"

let get_server_handler database_url server_secret () =
  Dream.set_secret server_secret
  @@ Dream.logger @@ Dream.cookie_sessions
  @@ Dream.sql_pool database_url
  @@ Dream.router
       [
         Routes.Static.route;
         Routes.Pages.route;
         Routes.Api.route;
         Routes.LiveReload.route ();
       ]

let run ~database_url ~server_secret () =
  Dream.run ~error_handler ~interface
  @@ get_server_handler database_url server_secret ()

let serve ?stop ~database_url ~server_secret () =
  Dream.serve ?stop ~error_handler ~interface
  @@ get_server_handler database_url server_secret ()
