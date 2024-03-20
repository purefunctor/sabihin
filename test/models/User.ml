module M = Models
open Utils

let initialize pool =
  let inner db =
    let open Lwt_result.Syntax in
    let* _ = M.Initialize.initialize db in
    Lwt.return_ok ()
  in
  make_test_case inner pool

let insert pool =
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
  make_test_case inner pool
