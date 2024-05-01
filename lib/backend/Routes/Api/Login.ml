open HigherOrderHandlers
open Types_native.Definitions_j
open Vault_native

let get_user request username =
  Dream.sql request @@ fun db ->
  match%lwt Models.User.get_by_username ~username db with
  | Ok user -> Lwt.return_ok user
  | Error e -> Lwt.return_error e

let handler request =
  let handle_salt username =
    match%lwt get_user request username with
    | Ok (Some { client_random; _ }) ->
        Dream.info (fun log -> log "User exists, serving client salt.");
        let salt =
          client_random |> Base64.decode_exn |> Bytes.of_string
          |> Salt.compute_digest
        in
        Dream.json @@ string_of_login_salt_response { salt }
    | Ok None ->
        Dream.info (fun log -> log "User does not exist, serving server salt.");
        let salt = Cipher.server_salt () in
        Dream.json @@ string_of_login_salt_response { salt }
    | Error (#Caqti_error.t as e) ->
        Dream.error (fun log -> log "Failed with %s" @@ Caqti_error.show e);
        Dream.respond ~code:422 @@ string_of_login_error_content `CouldNotLogIn
  in
  let handle_auth submitted_username submitted_auth_token =
    let submitted_auth_token = Cipher.double_hmac submitted_auth_token in
    match%lwt get_user request submitted_username with
    | Ok (Some user)
      when String.equal submitted_username user.username
           && String.equal submitted_auth_token user.auth_token ->
        let id = Printf.sprintf "%li" user.id in
        let public_id = user.public_id in
        Dream.info (fun log -> log "User exists, creating session.");
        Dream.set_session_field request "id" id;%lwt
        Dream.set_session_field request "public_id" public_id;%lwt
        Dream.empty `No_Content
    | Ok (Some _) | Ok None ->
        Dream.error (fun log -> log "Invalid credentials, serving error.");
        Dream.respond ~code:422 @@ string_of_login_error_content `CouldNotLogIn
    | Error (#Caqti_error.t as e) ->
        Dream.error (fun log -> log "Failed with %s" @@ Caqti_error.show e);
        Dream.respond ~code:422 @@ string_of_login_error_content `CouldNotLogIn
  in
  let inner ({ username; auth_token } : login_payload) =
    match ValidationUsername.validate username with
    | Validated Success -> (
        match auth_token with
        | None -> handle_salt username
        | Some auth_token -> handle_auth username auth_token)
    | _ ->
        Dream.error (fun log -> log "Rejecting invalid username: %s" username);
        Dream.respond ~code:422 @@ string_of_login_error_content `CouldNotLogIn
  in
  with_json_body request login_payload_of_string inner
