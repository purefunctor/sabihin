open Cohttp
open Cohttp_lwt_unix
open Types_native.Definitions_j
open Utils
open Vault_native

let username = "purefunctor"
let auth_token = String.make 128 'A'
let client_random_raw = String.make 16 'P'
let client_random = Base64.encode_string client_random_raw
let client_salt = Salt.compute_digest @@ Bytes.of_string client_random_raw
let server_salt = Salt.compute_digest @@ Bytes.of_string server_random

let default_register () =
  let%lwt cookie_headers = get_cookie_headers () in
  let%lwt _, body =
    let json =
      string_of_register_user_payload { username; auth_token; client_random }
    in
    post_json cookie_headers json "http://localhost:8080/api/register"
  in
  Cohttp_lwt.Body.drain_body body

let login_returns_client_salt prefix =
  let inner () =
    default_register ();%lwt

    let%lwt cookie_headers = get_cookie_headers () in
    let%lwt response, body =
      let json = string_of_login_payload { username; auth_token = None } in
      post_json cookie_headers json "http://localhost:8080/api/login"
    in

    let code = Response.status response |> Code.code_of_status in
    let%lwt body = Cohttp_lwt.Body.to_string body in
    let { salt } = login_salt_response_of_string body in

    let _ =
      Alcotest.(check int) "status code is 200" 200 code;
      Alcotest.(check string) "salt is from client" client_salt salt
    in

    Lwt.return ()
  in
  make_test_case prefix "it returns client salt" inner

let login_returns_server_salt prefix =
  let inner () =
    default_register ();%lwt

    let%lwt cookie_headers = get_cookie_headers () in
    let%lwt response, body =
      let json = string_of_login_payload { username; auth_token = None } in
      post_json cookie_headers json "http://localhost:8080/api/login"
    in

    let code = Response.status response |> Code.code_of_status in
    let%lwt body = Cohttp_lwt.Body.to_string body in
    let { salt } = login_salt_response_of_string body in

    let _ =
      Alcotest.(check int) "status code is 200" 200 code;
      Alcotest.(check string) "salt is from server" client_salt salt
    in

    Lwt.return ()
  in
  make_test_case prefix "it returns server salt" inner
