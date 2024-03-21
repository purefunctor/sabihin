open Models
open Utils

module PrivateKeys = struct
  include PrivateKeys

  let pp =
    let open Fmt in
    let open PrivateKeys in
    let bytes = of_to_string Bytes.to_string in
    record
      [
        field "user_id" (fun k -> k.user_id) int32;
        field "client_random_value" (fun k -> k.client_random_value) bytes;
        field "encrypted_master_key" (fun k -> k.encrypted_master_key) bytes;
        field "master_key_iv" (fun k -> k.master_key_iv) bytes;
        field "encrypted_protection_key"
          (fun k -> k.encrypted_protection_key)
          bytes;
        field "protection_key_iv" (fun k -> k.protection_key_iv) bytes;
        field "encrypted_verification_key"
          (fun k -> k.encrypted_verification_key)
          bytes;
        field "verification_key_iv" (fun k -> k.verification_key_iv) bytes;
      ]

  let equal x y =
    List.for_all Fun.id
      [
        Int32.equal x.user_id y.user_id;
        Bytes.equal x.client_random_value y.client_random_value;
        Bytes.equal x.encrypted_master_key y.encrypted_master_key;
        Bytes.equal x.master_key_iv y.master_key_iv;
        Bytes.equal x.encrypted_protection_key y.encrypted_protection_key;
        Bytes.equal x.protection_key_iv y.protection_key_iv;
        Bytes.equal x.encrypted_verification_key y.encrypted_verification_key;
        Bytes.equal x.verification_key_iv y.verification_key_iv;
      ]
end

let initialize =
  let inner db =
    let open Lwt_result.Syntax in
    let* _ = Initialize.initialize db in
    Lwt.return_ok ()
  in
  make_test_case "initialize" inner

let insert =
  let inner db =
    let open Lwt_result.Syntax in
    let* _ = Initialize.initialize db in
    let* user_id =
      let username = "insert.test" in
      let auth_token = String.make 128 ' ' in
      User.insert ~username ~auth_token db
    in
    let* _ =
      let client_random_value = Bytes.make 16 ' ' in
      let encrypted_master_key = Bytes.make 512 ' ' in
      let master_key_iv = Bytes.make 12 ' ' in
      let encrypted_protection_key = Bytes.make 512 ' ' in
      let protection_key_iv = Bytes.make 12 ' ' in
      let encrypted_verification_key = Bytes.make 512 ' ' in
      let verification_key_iv = Bytes.make 12 ' ' in
      PrivateKeys.insert ~user_id ~client_random_value ~encrypted_master_key
        ~master_key_iv ~encrypted_protection_key ~protection_key_iv
        ~encrypted_verification_key ~verification_key_iv db
    in
    Lwt.return_ok ()
  in
  make_test_case "insert" inner

let insert_existing =
  let inner db =
    let open Lwt_result.Syntax in
    let* _ = Initialize.initialize db in
    let* user_id =
      let username = "insert.test" in
      let auth_token = String.make 128 ' ' in
      User.insert ~username ~auth_token db
    in
    let client_random_value = Bytes.make 16 ' ' in
    let encrypted_master_key = Bytes.make 512 ' ' in
    let master_key_iv = Bytes.make 12 ' ' in
    let encrypted_protection_key = Bytes.make 512 ' ' in
    let protection_key_iv = Bytes.make 12 ' ' in
    let encrypted_verification_key = Bytes.make 512 ' ' in
    let verification_key_iv = Bytes.make 12 ' ' in
    let* _ =
      PrivateKeys.insert ~user_id ~client_random_value ~encrypted_master_key
        ~master_key_iv ~encrypted_protection_key ~protection_key_iv
        ~encrypted_verification_key ~verification_key_iv db
    in
    let errorful =
      PrivateKeys.insert ~user_id ~client_random_value ~encrypted_master_key
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
    let* _ = Initialize.initialize db in
    let* user_id =
      let username = "insert.test" in
      let auth_token = String.make 128 ' ' in
      User.insert ~username ~auth_token db
    in
    let client_random_value = Bytes.make 16 ' ' in
    let encrypted_master_key = Bytes.make 512 ' ' in
    let master_key_iv = Bytes.make 12 ' ' in
    let encrypted_protection_key = Bytes.make 512 ' ' in
    let protection_key_iv = Bytes.make 12 ' ' in
    let encrypted_verification_key = Bytes.make 512 ' ' in
    let verification_key_iv = Bytes.make 12 ' ' in
    let* _ =
      PrivateKeys.insert ~user_id ~client_random_value ~encrypted_master_key
        ~master_key_iv ~encrypted_protection_key ~protection_key_iv
        ~encrypted_verification_key ~verification_key_iv db
    in
    let expected =
      PrivateKeys.
        {
          user_id;
          client_random_value;
          encrypted_master_key;
          master_key_iv;
          encrypted_protection_key;
          protection_key_iv;
          encrypted_verification_key;
          verification_key_iv;
        }
    in
    let* actual = PrivateKeys.get_by_user_id ~user_id db in
    let _ =
      Alcotest.(check @@ option (module PrivateKeys))
        "private keys are equivalent" actual (Some expected)
    in
    Lwt.return_ok ()
  in
  make_test_case "get by user id" inner

let get_by_username =
  let inner db =
    let open Lwt_result.Syntax in
    let* _ = Initialize.initialize db in
    let username = "insert.test" in
    let* user_id =
      let auth_token = String.make 128 ' ' in
      User.insert ~username ~auth_token db
    in
    let client_random_value = Bytes.make 16 ' ' in
    let encrypted_master_key = Bytes.make 512 ' ' in
    let master_key_iv = Bytes.make 12 ' ' in
    let encrypted_protection_key = Bytes.make 512 ' ' in
    let protection_key_iv = Bytes.make 12 ' ' in
    let encrypted_verification_key = Bytes.make 512 ' ' in
    let verification_key_iv = Bytes.make 12 ' ' in
    let* _ =
      PrivateKeys.insert ~user_id ~client_random_value ~encrypted_master_key
        ~master_key_iv ~encrypted_protection_key ~protection_key_iv
        ~encrypted_verification_key ~verification_key_iv db
    in
    let expected =
      PrivateKeys.
        {
          user_id;
          client_random_value;
          encrypted_master_key;
          master_key_iv;
          encrypted_protection_key;
          protection_key_iv;
          encrypted_verification_key;
          verification_key_iv;
        }
    in
    let* actual = PrivateKeys.get_by_username ~username db in
    let _ =
      Alcotest.(check @@ option (module PrivateKeys))
        "private keys are equivalent" actual (Some expected)
    in
    Lwt.return_ok ()
  in
  make_test_case "get by username" inner
