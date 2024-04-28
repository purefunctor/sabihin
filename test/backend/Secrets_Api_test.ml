open Cohttp
open Cohttp_lwt
open Types_native.Definitions_j
open Utils

let register_fake_user () =
  let%lwt cookie_headers = get_cookie_headers () in
  let%lwt response, body =
    let json =
      string_of_register_user_payload
        { username = "purefunctor"; auth_token = "auth_token" }
    in
    post_json cookie_headers json "http://localhost:8080/api/register"
  in
  Cohttp_lwt.Body.drain_body body;%lwt
  let cookie_headers =
    let csrf_cookie =
      Cookie.Cookie_hdr.extract cookie_headers
      |> List.assoc_opt "sabihin.csrf"
      |> Option.get
    in
    let key, value =
      Response.headers response |> Cookie.Set_cookie_hdr.extract
      |> List.map (fun (k, v) -> (k, Cookie.Set_cookie_hdr.value v))
      |> List.cons ("sabihin.csrf", csrf_cookie)
      |> Cookie.Cookie_hdr.serialize
    in
    Header.init_with key value
  in
  Lwt.return cookie_headers

let fake_payload =
  let get_random_base64 () =
    Mirage_crypto_rng_unix.getrandom 16
    |> Cstruct.to_string |> Base64.encode_string
  in
  let client_random_value = get_random_base64 () in
  let encrypted_master_key = get_random_base64 () in
  let master_key_iv = get_random_base64 () in
  let encrypted_protection_key = get_random_base64 () in
  let protection_key_iv = get_random_base64 () in
  let encrypted_verification_key = get_random_base64 () in
  let verification_key_iv = get_random_base64 () in
  let exported_protection_key = get_random_base64 () in
  let exported_verification_key = get_random_base64 () in
  string_of_register_keys_payload
    {
      client_random_value;
      encrypted_master_key;
      master_key_iv;
      encrypted_protection_key;
      protection_key_iv;
      encrypted_verification_key;
      verification_key_iv;
      exported_protection_key;
      exported_verification_key;
    }

let it_works =
  let inner () =
    let%lwt cookie_headers = register_fake_user () in
    let%lwt response, body =
      post_json cookie_headers fake_payload "http://localhost:8080/api/secrets"
    in
    Cohttp_lwt.Body.drain_body body;%lwt

    let code = response |> Response.status |> Code.code_of_status in
    let _ = Alcotest.(check int) "status code is 204" 204 code in

    Lwt.return ()
  in
  make_test_case "it works" inner

let it_fails =
  let inner () =
    let%lwt cookie_headers = register_fake_user () in
    let%lwt response, body =
      post_json cookie_headers "{}" "http://localhost:8080/api/secrets"
    in
    Cohttp_lwt.Body.drain_body body;%lwt

    let code = response |> Response.status |> Code.code_of_status in
    let _ = Alcotest.(check int) "status code is 400" 400 code in

    Lwt.return ()
  in
  make_test_case "it fails" inner

let already_registered =
  let inner () =
    let%lwt cookie_headers = register_fake_user () in
    let%lwt first_response, body =
      post_json cookie_headers fake_payload "http://localhost:8080/api/secrets"
    in
    Cohttp_lwt.Body.drain_body body;%lwt
    let%lwt second_response, body =
      post_json cookie_headers fake_payload "http://localhost:8080/api/secrets"
    in
    Cohttp_lwt.Body.drain_body body;%lwt

    let first_code = first_response |> Response.status |> Code.code_of_status in
    let second_code =
      second_response |> Response.status |> Code.code_of_status
    in

    let _ =
      Alcotest.(check int) "first status code is 204" 204 first_code;
      Alcotest.(check int) "second status code is 400" 400 second_code
    in

    Lwt.return ()
  in
  make_test_case "already registered" inner
