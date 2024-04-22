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
  let* exported_protection_key = base64_decode keys.exported_protection_key in
  let* protection_key_iv = base64_decode keys.protection_key_iv in
  let* encrypted_verification_key =
    base64_decode keys.encrypted_verification_key
  in
  let* exported_verification_key =
    base64_decode keys.exported_verification_key
  in
  let* verification_key_iv = base64_decode keys.verification_key_iv in

  Dream.sql request (fun db ->
      let (module Db) = db in
      Db.with_transaction (fun () ->
          let* _ =
            Models.PrivateKeys.insert ~user_id ~client_random_value
              ~encrypted_master_key ~master_key_iv ~encrypted_protection_key
              ~protection_key_iv ~encrypted_verification_key
              ~verification_key_iv db
          in
          let* () =
            Models.PublicKeys.insert ~user_id ~exported_protection_key
              ~exported_verification_key db
          in
          Lwt.return_ok ()))

let handler request =
  let inner (session : Session.t) =
    let inner (payload : register_keys_payload) =
      match%lwt insert request session.id payload with
      | Ok () -> Dream.empty `No_Content
      | Error _ -> Dream.respond ~status:`Bad_Request "Invalid payload."
    in
    with_json_body request register_keys_payload_of_string inner
  in
  with_session request inner
