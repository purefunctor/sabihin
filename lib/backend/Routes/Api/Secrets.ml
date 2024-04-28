open HigherOrderHandlers
open Types_native.Definitions_j

let insert request (user_id : int32) (keys : register_keys_payload) =
  let open Lwt_result.Syntax in
  let base64_decode field =
    field |> Base64.decode |> Result.map Bytes.of_string |> Lwt_result.lift
  in

  let* client_random_value = base64_decode keys.client_random_value in
  let* encrypted_master_key = base64_decode keys.encrypted_master_key in
  let* master_key_iv = base64_decode keys.master_key_iv in
  let* encrypted_protection_key = base64_decode keys.encrypted_protection_key in
  let* protection_key_iv = base64_decode keys.protection_key_iv in
  let* exported_protection_key = base64_decode keys.exported_protection_key in
  let* encrypted_verification_key =
    base64_decode keys.encrypted_verification_key
  in
  let* verification_key_iv = base64_decode keys.verification_key_iv in
  let* exported_verification_key =
    base64_decode keys.exported_verification_key
  in

  Dream.sql request (fun db ->
      let (module Db) = db in
      Models.Secrets.insert ~user_id ~client_random_value ~encrypted_master_key
        ~master_key_iv ~encrypted_protection_key ~protection_key_iv
        ~exported_verification_key ~encrypted_verification_key
        ~verification_key_iv ~exported_protection_key db)

let handler request =
  let inner (session : Session.t) =
    let inner (payload : register_keys_payload) =
      match%lwt insert request session.id payload with
      | Ok () ->
          Dream.info (fun log -> log "Created Secrets: %s" session.public_id);
          Dream.empty `No_Content
      | Error e ->
          Dream.error (fun log ->
              let message =
                match e with
                | `Msg msg -> msg
                | #Caqti_error.t as caqti_error -> Caqti_error.show caqti_error
              in
              log "%s: failed with %s" session.public_id message);
          Dream.respond ~status:`Bad_Request "Invalid payload."
    in
    with_json_body request register_keys_payload_of_string inner
  in
  with_session request inner
