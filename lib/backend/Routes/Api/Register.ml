open HigherOrderHandlers
open Types_native.Definitions_j

let insert_user request username auth_token client_random =
  Dream.sql request @@ Models.User.insert ~username ~auth_token ~client_random

let register_422 message content =
  (match message with
  | `BypassedClientSide username ->
      Dream.error (fun log -> log "Bypassed validation: %s." username)
  | #Caqti_error.t as e ->
      Dream.error (fun log -> log "Failed with %s." (Caqti_error.show e)));
  Dream.json ~code:422 @@ string_of_register_error_content content

let handle_register request username auth_token client_random =
  match%lwt insert_user request username auth_token client_random with
  | Ok (id, public_id) ->
      Session.create_session request id public_id;%lwt
      Dream.json @@ string_of_register_response { public_id }
  | Error e -> register_422 e `CouldNotRegister

let handler request =
  let inner ({ username; auth_token; client_random } : register_user_payload) =
    match ValidationUsername.validate username with
    | Validated Success ->
        let auth_token = Cipher.double_hmac auth_token in
        handle_register request username auth_token client_random
    | _ -> register_422 (`BypassedClientSide username) `CouldNotRegister
  in
  with_json_body request register_user_payload_of_string inner
