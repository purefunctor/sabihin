open HigherOrderHandlers
open Types_native.Definitions_j
open Vault_native

let get_user request username =
  Dream.sql request @@ Models.User.get_by_username ~username

let login_salt ?client_random () =
  let digest =
    match client_random with
    | Some client_random ->
        Dream.info (fun log -> log "Using client salt.");
        let client_random = Base64.decode_exn client_random in
        Salt.compute_digest (`String client_random)
    | None ->
        Dream.info (fun log -> log "Using server salt.");
        Cipher.server_salt ()
  in
  let salt = digest |> Cstruct.to_string |> Base64.encode_exn in
  Dream.json @@ string_of_login_salt_response { salt }

let login_422 error content =
  (match error with
  | `InvalidCredentials ->
      Dream.error (fun log -> log "Invalid credentials, rejecting.")
  | `BypassedClientSide username ->
      Dream.error (fun log -> log "Bypassed validation: %s." username)
  | #Caqti_error.t as e ->
      Dream.error (fun log -> log "Failed with %s." (Caqti_error.show e)));
  Dream.json ~code:422 @@ string_of_login_error_content content

let handle_salt_mode request username =
  match%lwt get_user request username with
  | Ok (Some { client_random; _ }) -> login_salt ~client_random ()
  | Ok None -> login_salt ()
  | Error e -> login_422 e `CouldNotLogIn

let handle_auth_mode request submitted_username submitted_auth_token =
  match%lwt get_user request submitted_username with
  | Ok (Some { id; public_id; auth_token; _ })
    when String.equal submitted_auth_token auth_token ->
      Session.create_session request id public_id;%lwt
      Dream.json @@ string_of_login_auth_response { public_id }
  | Ok (Some _) | Ok None -> login_422 `InvalidCredentials `CouldNotLogIn
  | Error e -> login_422 e `CouldNotLogIn

let handler request =
  let inner ({ username; auth_token } : login_payload) =
    match ValidationUsername.validate username with
    | Validated Success -> (
        match auth_token with
        | None -> handle_salt_mode request username
        | Some auth_token ->
            let auth_token = Cipher.double_hmac auth_token in
            handle_auth_mode request username auth_token)
    | _ -> login_422 (`BypassedClientSide username) `CouldNotLogIn
  in
  with_json_body request login_payload_of_string inner
