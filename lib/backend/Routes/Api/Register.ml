open HigherOrderHandlers
open Types_native.Defs_j

let handler request =
  let inner { username; auth_token } =
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
  with_json_body request register_payload_t_of_string inner
