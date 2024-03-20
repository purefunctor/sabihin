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
    let* first_id =
      let username = "maho.akashi" in
      let auth_token = String.make 128 ' ' in
      M.User.insert ~username ~auth_token db
    in
    let* second_id =
      let username = "rinku.aimoto" in
      let auth_token = String.make 128 ' ' in
      M.User.insert ~username ~auth_token db
    in
    let _ = Alcotest.(check int32) "first_id = 1" first_id 1l in
    let _ = Alcotest.(check int32) "second_id = 2" second_id 2l in
    Lwt.return_ok ()
  in
  make_test_case "insert" inner

let insert_existing =
  let inner db =
    let open Lwt_result.Syntax in
    let* _ = M.Initialize.initialize db in
    let username = "maho.akashi" in
    let auth_token = String.make 128 ' ' in
    let* _ = M.User.insert ~username ~auth_token db in
    let errorful = M.User.insert ~username ~auth_token db in
    Lwt.bind errorful @@ function
    | Ok _ -> Alcotest.fail "Expected an error."
    | Error _ -> Lwt.return_ok ()
  in
  make_test_case "insert existing" inner

let get_by_id =
  let inner db =
    let open Lwt_result.Syntax in
    let* _ = M.Initialize.initialize db in
    let username = "maho.akashi" in
    let auth_token = String.make 128 ' ' in
    let* id = M.User.insert ~username ~auth_token db in
    let* user = M.User.get_by_id ~id db in
    let _ =
      Alcotest.(check string) "username is equivalent" user.username username
    in
    let _ =
      Alcotest.(check string)
        "auth_token is equivalent" user.auth_token auth_token
    in
    let _ = Alcotest.(check int32) "id is equivalent" user.id id in
    Lwt.return_ok ()
  in
  make_test_case "get by id" inner

let get_by_username =
  let inner db =
    let open Lwt_result.Syntax in
    let* _ = M.Initialize.initialize db in
    let username = "maho.akashi" in
    let auth_token = String.make 128 ' ' in
    let* id = M.User.insert ~username ~auth_token db in
    let* user = M.User.get_by_username ~username db in
    let _ =
      Alcotest.(check string) "username is equivalent" user.username username
    in
    let _ =
      Alcotest.(check string)
        "auth_token is equivalent" user.auth_token auth_token
    in
    let _ = Alcotest.(check int32) "id is equivalent" user.id id in
    Lwt.return_ok ()
  in
  make_test_case "get by username" inner
