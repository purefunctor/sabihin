open HigherOrderHandlers

module Fields = struct
  open Ppx_yojson_conv_lib.Yojson_conv.Primitives

  type t = { username : string; auth_token : string } [@@deriving yojson]

  let from_string_opt value =
    try Some (value |> Yojson.Safe.from_string |> t_of_yojson) with _ -> None
end

let handler request =
  let inner Fields.{ username; auth_token } =
    let%lwt insert_result =
      Dream.sql request (fun connection ->
          Models.User.insert ~username ~auth_token connection)
    in
    match insert_result with
    | Ok (id, public_id) ->
        Dream.respond @@ Printf.sprintf "%li;%s" id public_id
    | Error e ->
        Dream.error (fun log -> log "Failed with %s" @@ Caqti_error.show e);
        Dream.respond ~code:422 "Could not register user."
  in
  with_json_body request Fields.from_string_opt inner
