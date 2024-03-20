module M = Models
open Utils

let initialize =
  let inner db =
    let open Lwt_result.Syntax in
    let* _ = M.Initialize.initialize db in
    Lwt.return_ok ()
  in
  make_test_case "initialize" inner

let insert =
  let inner db =
    let open Lwt_result.Syntax in
    let* _ = M.Initialize.initialize db in
    let* user_id =
      let username = "insert.test" in
      let auth_token = String.make 128 ' ' in
      M.User.insert ~username ~auth_token db
    in
    let* _ =
      let client_random_value = Bytes.make 16 ' ' in
      let encrypted_master_key = Bytes.make 512 ' ' in
      let master_key_iv = Bytes.make 12 ' ' in
      let encrypted_protection_key = Bytes.make 512 ' ' in
      let protection_key_iv = Bytes.make 12 ' ' in
      let encrypted_verification_key = Bytes.make 512 ' ' in
      let verification_key_iv = Bytes.make 12 ' ' in
      M.PrivateKeys.insert ~user_id ~client_random_value ~encrypted_master_key
        ~master_key_iv ~encrypted_protection_key ~protection_key_iv
        ~encrypted_verification_key ~verification_key_iv db
    in
    Lwt.return_ok ()
  in
  make_test_case "insert" inner

let insert_existing =
  let inner db =
    let open Lwt_result.Syntax in
    let* _ = M.Initialize.initialize db in
    let* user_id =
      let username = "insert.test" in
      let auth_token = String.make 128 ' ' in
      M.User.insert ~username ~auth_token db
    in
    let client_random_value = Bytes.make 16 ' ' in
    let encrypted_master_key = Bytes.make 512 ' ' in
    let master_key_iv = Bytes.make 12 ' ' in
    let encrypted_protection_key = Bytes.make 512 ' ' in
    let protection_key_iv = Bytes.make 12 ' ' in
    let encrypted_verification_key = Bytes.make 512 ' ' in
    let verification_key_iv = Bytes.make 12 ' ' in
    let* _ =
      M.PrivateKeys.insert ~user_id ~client_random_value ~encrypted_master_key
        ~master_key_iv ~encrypted_protection_key ~protection_key_iv
        ~encrypted_verification_key ~verification_key_iv db
    in
    let errorful =
      M.PrivateKeys.insert ~user_id ~client_random_value ~encrypted_master_key
        ~master_key_iv ~encrypted_protection_key ~protection_key_iv
        ~encrypted_verification_key ~verification_key_iv db
    in
    Lwt.bind errorful (function
      | Ok _ -> Alcotest.fail "Expected an error."
      | Error _ -> Lwt.return_ok ())
  in
  make_test_case "insert existing" inner

let get_by_user_id =
  let inner db =
    let open Lwt_result.Syntax in
    let* _ = M.Initialize.initialize db in
    let* user_id =
      let username = "insert.test" in
      let auth_token = String.make 128 ' ' in
      M.User.insert ~username ~auth_token db
    in
    let client_random_value = Bytes.make 16 ' ' in
    let encrypted_master_key = Bytes.make 512 ' ' in
    let master_key_iv = Bytes.make 12 ' ' in
    let encrypted_protection_key = Bytes.make 512 ' ' in
    let protection_key_iv = Bytes.make 12 ' ' in
    let encrypted_verification_key = Bytes.make 512 ' ' in
    let verification_key_iv = Bytes.make 12 ' ' in
    let* _ =
      M.PrivateKeys.insert ~user_id ~client_random_value ~encrypted_master_key
        ~master_key_iv ~encrypted_protection_key ~protection_key_iv
        ~encrypted_verification_key ~verification_key_iv db
    in
    let* private_keys = M.PrivateKeys.get_by_user_id ~user_id db in
    let _ =
      [
        Alcotest.(check int32)
          "user id is equivalent" private_keys.user_id user_id;
        Alcotest.(check bytes)
          "encrypted master key is equivalent" private_keys.encrypted_master_key
          encrypted_master_key;
        Alcotest.(check bytes)
          "master key iv is equivalent" private_keys.master_key_iv master_key_iv;
        Alcotest.(check bytes)
          "encrypted protection key is equivalent"
          private_keys.encrypted_protection_key encrypted_protection_key;
        Alcotest.(check bytes)
          "protection key iv is equivalent" private_keys.protection_key_iv
          protection_key_iv;
        Alcotest.(check bytes)
          "encrypted verification key is equivalent"
          private_keys.encrypted_verification_key encrypted_verification_key;
        Alcotest.(check bytes)
          "verification key iv is equivalent" private_keys.verification_key_iv
          verification_key_iv;
      ]
    in
    Lwt.return_ok ()
  in
  make_test_case "get by id" inner
