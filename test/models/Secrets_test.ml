open Models
open Utils

module Secrets = struct
  include Secrets

  let pp =
    let open Fmt in
    let open Secrets in
    record
      [
        field "user_id" (fun k -> k.user_id) int32;
        field "encrypted_master_key" (fun k -> k.encrypted_master_key) string;
        field "encrypted_protection_key"
          (fun k -> k.encrypted_protection_key)
          string;
        field "exported_protection_key"
          (fun k -> k.exported_protection_key)
          string;
        field "encrypted_verification_key"
          (fun k -> k.encrypted_verification_key)
          string;
        field "exported_verification_key"
          (fun k -> k.exported_verification_key)
          string;
      ]

  let equal x y =
    List.for_all Fun.id
      [
        Int32.equal x.user_id y.user_id;
        String.equal x.encrypted_master_key y.encrypted_master_key;
        String.equal x.encrypted_protection_key y.encrypted_protection_key;
        String.equal x.exported_protection_key y.exported_protection_key;
        String.equal x.encrypted_verification_key y.encrypted_verification_key;
        String.equal x.exported_verification_key y.exported_verification_key;
      ]
end

let username = "purefunctor"
let auth_token = String.make 128 ' '
let client_random = String.make 16 ' ' |> Base64.encode_exn
let encrypted_master_key = String.make 512 ' ' |> Base64.encode_exn
let encrypted_protection_key = String.make 512 ' ' |> Base64.encode_exn
let exported_protection_key = String.make 512 ' ' |> Base64.encode_exn
let encrypted_verification_key = String.make 512 ' ' |> Base64.encode_exn
let exported_verification_key = String.make 512 ' ' |> Base64.encode_exn

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
    let* user_id, _ = User.insert ~username ~auth_token ~client_random db in
    let* _ =
      Secrets.insert ~user_id ~encrypted_master_key ~encrypted_protection_key
        ~exported_protection_key ~encrypted_verification_key
        ~exported_verification_key db
    in
    Lwt.return_ok ()
  in
  make_test_case "insert" inner

let insert_existing =
  let inner db =
    let open Lwt_result.Syntax in
    let* _ = Initialize.initialize db in
    let* user_id, _ = User.insert ~username ~auth_token ~client_random db in
    let* _ =
      Secrets.insert ~user_id ~encrypted_master_key ~encrypted_protection_key
        ~exported_protection_key ~encrypted_verification_key
        ~exported_verification_key db
    in
    let errorful =
      Secrets.insert ~user_id ~encrypted_master_key ~encrypted_protection_key
        ~exported_protection_key ~encrypted_verification_key
        ~exported_verification_key db
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
    let* user_id, _ = User.insert ~username ~auth_token ~client_random db in
    let* _ =
      Secrets.insert ~user_id ~encrypted_master_key ~encrypted_protection_key
        ~exported_protection_key ~encrypted_verification_key
        ~exported_verification_key db
    in
    let expected =
      Secrets.
        {
          user_id;
          encrypted_master_key;
          encrypted_protection_key;
          exported_protection_key;
          encrypted_verification_key;
          exported_verification_key;
        }
    in
    let* actual = Secrets.get_by_user_id ~user_id db in
    let _ =
      Alcotest.(check @@ option (module Secrets))
        "private keys are equivalent" actual (Some expected)
    in
    Lwt.return_ok ()
  in
  make_test_case "get by user id" inner

let get_by_username =
  let inner db =
    let open Lwt_result.Syntax in
    let* _ = Initialize.initialize db in
    let* user_id, _ = User.insert ~username ~auth_token ~client_random db in
    let* _ =
      Secrets.insert ~user_id ~encrypted_master_key ~encrypted_protection_key
        ~exported_protection_key ~encrypted_verification_key
        ~exported_verification_key db
    in
    let expected =
      Secrets.
        {
          user_id;
          encrypted_master_key;
          encrypted_protection_key;
          exported_protection_key;
          encrypted_verification_key;
          exported_verification_key;
        }
    in
    let* actual = Secrets.get_by_username ~username db in
    let _ =
      Alcotest.(check @@ option (module Secrets))
        "private keys are equivalent" actual (Some expected)
    in
    Lwt.return_ok ()
  in
  make_test_case "get by username" inner

let get_missing =
  let inner db =
    let open Lwt_result.Syntax in
    let* _ = Initialize.initialize db in
    let* by_id = Secrets.get_by_user_id ~user_id:0l db in
    let* by_username = Secrets.get_by_username ~username:"purefunctor" db in
    let _ =
      [
        Alcotest.(check @@ option (module Secrets))
          "by user id is none" by_id None;
        Alcotest.(check @@ option (module Secrets))
          "by username is none" by_username None;
      ]
    in
    Lwt.return_ok ()
  in
  make_test_case "get missing" inner
