open HigherOrderHandlers
open Types_native.Definitions_j

let handler request =
  let inner { username; auth_token } =
    match ValidationUsername.validate username with
    | Validated Success -> (
        let auth_token = Cipher.double_hmac auth_token in
        let%lwt insert_result =
          Dream.sql request (fun connection ->
              Models.User.insert ~username ~auth_token connection)
        in
        match insert_result with
        | Ok (id, public_id) ->
            let id = Printf.sprintf "%li" id in
            Dream.info (fun log -> log "Created User: %s" public_id);
            Dream.set_session_field request "id" id;%lwt
            Dream.set_session_field request "public_id" public_id;%lwt
            Dream.json @@ string_of_register_response { public_id }
        | Error e ->
            Dream.error (fun log -> log "Failed with %s" @@ Caqti_error.show e);
            Dream.json ~code:422
            @@ string_of_register_error_content `CouldNotRegister)
    | _ ->
        Dream.error (fun log -> log "Rejecting invalid username: %s" username);
        Dream.json ~code:422
        @@ string_of_register_error_content `CouldNotRegister
  in
  with_json_body request register_user_payload_of_string inner
