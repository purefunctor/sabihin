open HigherOrderHandlers
open Types_native.Definitions_j

let secrets_to_register_keys
    ({
       user_id = _;
       encrypted_master_key;
       encrypted_protection_key;
       exported_protection_key;
       encrypted_verification_key;
       exported_verification_key;
     } :
      Models.Secrets.t) =
  {
    encrypted_master_key;
    encrypted_protection_key;
    exported_protection_key;
    encrypted_verification_key;
    exported_verification_key;
  }

let get_secrets request (user_id : int32) =
  let open Lwt_result.Syntax in
  let* secrets =
    Dream.sql request (fun db -> Models.Secrets.get_by_user_id ~user_id db)
  in
  Lwt.return_ok @@ Option.map secrets_to_register_keys secrets

let insert_secrets request (user_id : int32) (secrets : register_keys_payload) =
  let open Lwt_result.Syntax in
  Dream.sql request @@ fun connection ->
  match%lwt Models.Secrets.get_by_user_id ~user_id connection with
  | Error _ -> Lwt.return_ok false
  | Ok _ ->
      let {
        encrypted_master_key;
        encrypted_protection_key;
        exported_protection_key;
        encrypted_verification_key;
        exported_verification_key;
      } =
        secrets
      in
      let* () =
        Models.Secrets.insert ~user_id ~encrypted_master_key
          ~encrypted_protection_key ~exported_verification_key
          ~encrypted_verification_key ~exported_protection_key connection
      in
      Lwt.return_ok true

let secrets_log message =
  match message with
  | `AlreadyRegistered -> Dream.error (fun log -> log "Already registered.")
  | `CreatedSecrets -> Dream.info (fun log -> log "Secrets created.")
  | `HasSecrets -> Dream.info (fun log -> log "Secrets found.")
  | `NoSecrets -> Dream.error (fun log -> log "No secrets found.")
  | #Caqti_error.t as e ->
      Dream.error (fun log -> log "Failed with: %s." (Caqti_error.show e))

let secrets_400 = Dream.empty `Bad_Request
let secrets_404 = Dream.empty `Not_Found

let get request =
  let inner ({ id; _ } : Session.t) =
    match%lwt get_secrets request id with
    | Ok (Some secrets) ->
        secrets_log `HasSecrets;
        Dream.json @@ string_of_register_keys_payload secrets
    | Ok None ->
        secrets_log `NoSecrets;
        secrets_404
    | Error e ->
        secrets_log e;
        secrets_404
  in
  with_session request inner

let post request =
  let inner (session : Session.t) =
    let inner (payload : register_keys_payload) =
      match%lwt insert_secrets request session.id payload with
      | Ok true ->
          secrets_log `CreatedSecrets;
          Dream.empty `No_Content
      | Ok false ->
          secrets_log `AlreadyRegistered;
          secrets_400
      | Error e ->
          secrets_log e;
          secrets_400
    in
    with_json_body request register_keys_payload_of_string inner
  in
  with_session request inner
