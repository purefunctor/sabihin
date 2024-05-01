open Models
open Utils

module User = struct
  include User

  let pp =
    let open Fmt in
    let open User in
    record
      [
        field "id" (fun user -> user.id) int32;
        field "username" (fun user -> user.username) string;
        field "auth_token" (fun user -> user.auth_token) string;
      ]

  let equal x y =
    let open User in
    Int32.equal x.id y.id
    && String.equal x.username y.username
    && String.equal x.auth_token y.auth_token
end

let username = "maho.akashi"
let auth_token = String.make 128 ' '
let client_random = String.make 16 ' ' |> Base64.encode_exn

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
    let* first_id, _ = User.insert ~username ~auth_token ~client_random db in
    let* second_id, _ =
      let username = "rinku.aimoto" in
      User.insert ~username ~auth_token ~client_random db
    in
    let _ = Alcotest.(check int32) "first_id = 1" first_id 1l in
    let _ = Alcotest.(check int32) "second_id = 2" second_id 2l in
    Lwt.return_ok ()
  in
  make_test_case "insert" inner

let insert_existing =
  let inner db =
    let open Lwt_result.Syntax in
    let* _ = Initialize.initialize db in
    let* _ = User.insert ~username ~auth_token ~client_random db in
    let errorful = User.insert ~username ~auth_token ~client_random db in
    Lwt.bind errorful @@ function
    | Ok _ -> Alcotest.fail "Expected an error."
    | Error _ -> Lwt.return_ok ()
  in
  make_test_case "insert existing" inner

let get_by_id =
  let inner db =
    let open Lwt_result.Syntax in
    let* _ = Initialize.initialize db in
    let* id, public_id = User.insert ~username ~auth_token ~client_random db in
    let expected =
      User.{ id; public_id; username; auth_token; client_random }
    in
    let* actual = User.get_by_id ~id db in
    let _ =
      Alcotest.(check @@ option (module User))
        "user is equivalent" actual (Some expected)
    in
    Lwt.return_ok ()
  in
  make_test_case "get by id" inner

let get_by_username =
  let inner db =
    let open Lwt_result.Syntax in
    let* _ = Initialize.initialize db in
    let* id, public_id = User.insert ~username ~auth_token ~client_random db in
    let expected =
      User.{ id; public_id; username; auth_token; client_random }
    in
    let* actual = User.get_by_username ~username db in
    let _ =
      Alcotest.(check @@ option (module User))
        "user is equivalent" actual (Some expected)
    in
    Lwt.return_ok ()
  in
  make_test_case "get by username" inner

let get_missing =
  let inner db =
    let open Lwt_result.Syntax in
    let* _ = Initialize.initialize db in
    let* by_id = User.get_by_id ~id:0l db in
    let* by_username = User.get_by_username ~username:"purefunctor" db in
    let _ =
      [
        Alcotest.(check @@ option (module User)) "by id is none" by_id None;
        Alcotest.(check @@ option (module User))
          "by username is none" by_username None;
      ]
    in
    Lwt.return_ok ()
  in
  make_test_case "get missing" inner
