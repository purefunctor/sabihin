open HigherOrderHandlers
open Types_native.Definitions_j

let report_error public_id e =
  Dream.error (fun log ->
      let message =
        match e with
        | `Msg message -> message
        | #Caqti_error.t as caqti_error -> Caqti_error.show caqti_error
      in
      log "%s: failed with %s" public_id message)

let get_secrets request (user_id : int32) =
  let open Lwt_result.Syntax in
  let* secrets =
    Dream.sql request (fun db -> Models.Secrets.get_by_user_id ~user_id db)
  in
  match secrets with
  | Some
      {
        user_id = _;
        encrypted_master_key;
        master_key_iv;
        encrypted_protection_key;
        protection_key_iv;
        exported_protection_key;
        encrypted_verification_key;
        verification_key_iv;
        exported_verification_key;
      } ->
      Lwt.return_ok
      @@ Some
           {
             encrypted_master_key;
             master_key_iv;
             encrypted_protection_key;
             protection_key_iv;
             exported_protection_key;
             encrypted_verification_key;
             verification_key_iv;
             exported_verification_key;
           }
  | None -> Lwt.return_ok None

let insert_secrets request (user_id : int32)
    ({
       encrypted_master_key;
       master_key_iv;
       encrypted_protection_key;
       exported_protection_key;
       protection_key_iv;
       encrypted_verification_key;
       exported_verification_key;
       verification_key_iv;
     } :
      register_keys_payload) =
  let open Lwt_result.Syntax in
  Dream.sql request @@ fun connection ->
  let* has_keys =
    Models.Secrets.get_by_user_id ~user_id connection
    |> Lwt_result.map Option.is_some
  in
  if has_keys then Lwt.return_ok false
  else
    let* () =
      Models.Secrets.insert ~user_id ~encrypted_master_key ~master_key_iv
        ~encrypted_protection_key ~protection_key_iv ~exported_verification_key
        ~encrypted_verification_key ~verification_key_iv
        ~exported_protection_key connection
    in
    Lwt.return_ok true

let get request =
  let inner (session : Session.t) =
    match%lwt get_secrets request session.id with
    | Ok (Some secrets) ->
        Dream.info (fun log -> log "%s: secrets found" session.public_id);
        Dream.json @@ string_of_register_keys_payload secrets
    | Ok None ->
        Dream.warning (fun log -> log "%s: secrets not found" session.public_id);
        Dream.respond ~status:`Not_Found "Secrets not found."
    | Error e ->
        report_error session.public_id e;
        Dream.respond ~status:`Not_Found "Secrets not found."
  in
  with_session request inner

let post request =
  let inner (session : Session.t) =
    let inner (payload : register_keys_payload) =
      match%lwt insert_secrets request session.id payload with
      | Ok true ->
          Dream.info (fun log -> log "Created Secrets: %s" session.public_id);
          Dream.empty `No_Content
      | Ok false ->
          Dream.info (fun log -> log "Ignoring Secrets: %s" session.public_id);
          Dream.respond ~status:`Bad_Request "Already registered."
      | Error e ->
          report_error session.public_id e;
          Dream.respond ~status:`Bad_Request "Invalid payload."
    in
    with_json_body request register_keys_payload_of_string inner
  in
  with_session request inner
