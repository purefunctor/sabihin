open Models
open Utils

module PublicKeys = struct
  include PublicKeys

  let pp =
    let open Fmt in
    let open PublicKeys in
    let bytes = of_to_string Bytes.to_string in
    record
      [
        field "user_id" (fun k -> k.user_id) int32;
        field "protection_key" (fun k -> k.exported_protection_key) bytes;
        field "verification_key" (fun k -> k.exported_verification_key) bytes;
      ]

  let equal x y =
    List.for_all Fun.id
      [
        Int32.equal x.user_id y.user_id;
        Bytes.equal x.exported_protection_key y.exported_protection_key;
        Bytes.equal x.exported_verification_key y.exported_verification_key;
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
    let* user_id, _ =
      let username = "insert.test" in
      let auth_token = String.make 128 ' ' in
      User.insert ~username ~auth_token db
    in
    let* _ =
      let exported_protection_key = Bytes.make 128 ' ' in
      let exported_verification_key = Bytes.make 128 ' ' in
      PublicKeys.insert ~user_id ~exported_protection_key
        ~exported_verification_key db
    in
    Lwt.return_ok ()
  in
  make_test_case "insert" inner

let insert_existing =
  let inner db =
    let open Lwt_result.Syntax in
    let* _ = Initialize.initialize db in
    let* user_id, _ =
      let username = "insert.test" in
      let auth_token = String.make 128 ' ' in
      User.insert ~username ~auth_token db
    in
    let exported_protection_key = Bytes.make 128 ' ' in
    let exported_verification_key = Bytes.make 128 ' ' in
    let* _ =
      PublicKeys.insert ~user_id ~exported_protection_key
        ~exported_verification_key db
    in
    let errorful =
      PublicKeys.insert ~user_id ~exported_protection_key
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
    let* user_id, _ =
      let username = "insert.test" in
      let auth_token = String.make 128 ' ' in
      User.insert ~username ~auth_token db
    in
    let exported_protection_key = Bytes.make 128 ' ' in
    let exported_verification_key = Bytes.make 128 ' ' in
    let* _ =
      PublicKeys.insert ~user_id ~exported_protection_key
        ~exported_verification_key db
    in
    let* actual = PublicKeys.get_by_user_id ~user_id db in
    let expected =
      PublicKeys.{ user_id; exported_protection_key; exported_verification_key }
    in
    let _ =
      Alcotest.(check @@ option (module PublicKeys))
        "public keys are equivalent" actual (Some expected)
    in
    Lwt.return_ok ()
  in
  make_test_case "get by user id" inner

let get_by_username =
  let inner db =
    let open Lwt_result.Syntax in
    let* _ = Initialize.initialize db in
    let username = "insert.test" in
    let* user_id, _ =
      let auth_token = String.make 128 ' ' in
      User.insert ~username ~auth_token db
    in
    let exported_protection_key = Bytes.make 128 ' ' in
    let exported_verification_key = Bytes.make 128 ' ' in
    let* _ =
      PublicKeys.insert ~user_id ~exported_protection_key
        ~exported_verification_key db
    in
    let* actual = PublicKeys.get_by_username ~username db in
    let expected =
      PublicKeys.{ user_id; exported_protection_key; exported_verification_key }
    in
    let _ =
      Alcotest.(check @@ option (module PublicKeys))
        "public keys are equivalent" actual (Some expected)
    in
    Lwt.return_ok ()
  in
  make_test_case "get by username" inner

let get_by_public_id =
  let inner db =
    let open Lwt_result.Syntax in
    let* _ = Initialize.initialize db in
    let username = "insert.test" in
    let* user_id, public_id =
      let auth_token = String.make 128 ' ' in
      User.insert ~username ~auth_token db
    in
    let exported_protection_key = Bytes.make 128 ' ' in
    let exported_verification_key = Bytes.make 128 ' ' in
    let* _ =
      PublicKeys.insert ~user_id ~exported_protection_key
        ~exported_verification_key db
    in
    let* actual = PublicKeys.get_by_public_id ~public_id db in
    let expected =
      PublicKeys.{ user_id; exported_protection_key; exported_verification_key }
    in
    let _ =
      Alcotest.(check @@ option (module PublicKeys))
        "public keys are equivalent" actual (Some expected)
    in
    Lwt.return_ok ()
  in
  make_test_case "get by public id" inner

let get_missing =
  let inner db =
    let open Lwt_result.Syntax in
    let* _ = Initialize.initialize db in
    let* by_id = PublicKeys.get_by_user_id ~user_id:0l db in
    let* by_username = PublicKeys.get_by_username ~username:"purefunctor" db in
    let* by_public_id =
      PublicKeys.get_by_public_id ~public_id:"abcdef012345" db
    in
    let _ =
      [
        Alcotest.(check @@ option (module PublicKeys))
          "by user id is none" by_id None;
        Alcotest.(check @@ option (module PublicKeys))
          "by username is none" by_username None;
        Alcotest.(check @@ option (module PublicKeys))
          "by public id is none" by_public_id None;
      ]
    in
    Lwt.return_ok ()
  in
  make_test_case "get missing" inner
