open HigherOrderHandlers
open Types_native.Defs_j

let handler request =
  let inner { username; auth_token } =
    let%lwt insert_result =
      Dream.sql request (fun connection ->
          Models.User.insert ~username ~auth_token connection)
    in
    match insert_result with
    | Ok (_, public_id) ->
        Dream.info (fun log -> log "Created User: %s" public_id);
        Dream.json @@ string_of_register_response_t { public_id }
    | Error e ->
        Dream.error (fun log -> log "Failed with %s" @@ Caqti_error.show e);
        Dream.json ~code:422
        @@ string_of_register_error_content_t `CouldNotRegister
  in
  with_json_body request register_payload_t_of_string inner
