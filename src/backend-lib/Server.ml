open Ppx_yojson_conv_lib.Yojson_conv.Primitives

module ErrorResponse = struct
  type 'a t = { code : int; reason : string; content : 'a } [@@deriving yojson]

  let make ~code ~reason ~content = { code; reason; content }

  let make_string ~code ~reason ~content yojson_of_content =
    make ~code ~reason ~content
    |> yojson_of_t yojson_of_content
    |> Yojson.Safe.to_string
end

let error_template _ _ suggested_response =
  let status = Dream.status suggested_response in
  let code = Dream.status_to_int status in
  let reason = Dream.status_to_string status in
  match code with
  | 404 ->
      Dream.set_header suggested_response "Content-Type" Dream.text_html;
      Dream.set_body suggested_response @@ Render.AppPage.render [ "not-found" ];
      Lwt.return suggested_response
  | _ -> (
      match Dream.header suggested_response "Content-Type" with
      | Some "application/json" ->
          let%lwt content =
            Dream.body suggested_response |> Lwt.map Yojson.Safe.from_string
          in
          let body = ErrorResponse.make_string ~code ~reason ~content Fun.id in
          Dream.set_body suggested_response body;
          Lwt.return suggested_response
      | Some _ -> Lwt.return suggested_response
      | None ->
          let%lwt content =
            Dream.body suggested_response
            |> Lwt.map (fun content -> `String content)
          in
          let body = ErrorResponse.make_string ~code ~reason ~content Fun.id in
          Dream.set_header suggested_response "Content-Type"
            Dream.application_json;
          Dream.set_body suggested_response body;
          Lwt.return suggested_response)

let error_handler = Dream.error_template error_template
let interface = "0.0.0.0"

let server_secret =
  match Sys.getenv_opt "SERVER_SECRET" with
  | Some server_secret ->
      Dream.info (fun log -> log "SERVER_SECRET: Production Mode");
      server_secret
  | None ->
      Dream.warning (fun log -> log "SERVER_SECRET: Development Mode");
      Dream.to_base64url (Dream.random 128)

let server_handler =
  Dream.set_secret server_secret
  @@ Dream.logger @@ Dream.cookie_sessions
  @@ Dream.router [ Routes.Static.route; Routes.Pages.route; Routes.Api.route ]

let run () = Dream.run ~error_handler ~interface server_handler
let serve () = Dream.serve ~error_handler ~interface server_handler
