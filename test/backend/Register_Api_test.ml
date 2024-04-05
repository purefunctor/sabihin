open Cohttp
open Cohttp_lwt_unix
open Types_native.Defs_j
open Types_native.Dynamic
open Utils

let double_hmac data =
  let open Mirage_crypto.Hash in
  let key = Cstruct.of_string "an_obviously_fake_key" in
  data |> Cstruct.of_string |> SHA512.hmac ~key |> SHA512.hmac ~key
  |> Cstruct.to_hex_string

let it_works =
  let inner () =
    let%lwt cookie_headers = get_cookie_headers () in
    let%lwt response, body =
      let json =
        string_of_register_payload_t
          { username = "purefunctor"; auth_token = double_hmac "auth_token" }
      in
      post_json cookie_headers json "http://localhost:8080/api/register"
    in

    let code = Response.status response |> Code.code_of_status in
    let%lwt body = Cohttp_lwt.Body.to_string body in
    let%lwt parsed = is_parsed_by body register_response_t_of_string in

    let _ =
      Alcotest.(check int) "status code is 200" 200 code;
      Alcotest.(check bool) "response is parsed" true parsed
    in

    Lwt.return ()
  in
  make_test_case "it works" inner

let it_fails =
  let inner () =
    let%lwt response, body =
      Client.post (Uri.of_string "http://localhost:8080/api/register")
    in

    let code = Response.status response |> Code.code_of_status in
    let%lwt body = Cohttp_lwt.Body.to_string body in
    let%lwt parsed = is_parsed_by body yojson_error_response_of_string in

    let _ =
      Alcotest.(check int) "status code is 403" 403 code;
      Alcotest.(check bool) "response can be parsed" true parsed
    in

    Lwt.return ()
  in
  make_test_case "it fails" inner

let already_registered =
  let inner () =
    let%lwt cookie_headers = get_cookie_headers () in
    let%lwt _, body =
      let json =
        string_of_register_payload_t
          { username = "purefunctor"; auth_token = double_hmac "auth_token" }
      in
      post_json cookie_headers json "http://localhost:8080/api/register"
    in
    Cohttp_lwt.Body.drain_body body;%lwt

    let%lwt response, body =
      let json =
        string_of_register_payload_t
          { username = "purefunctor"; auth_token = double_hmac "auth_token" }
      in
      post_json cookie_headers json "http://localhost:8080/api/register"
    in

    let code = response |> Response.status |> Code.code_of_status in

    let%lwt body = Cohttp_lwt.Body.to_string body in
    let%lwt parsed_by_yojson =
      is_parsed_by body yojson_error_response_of_string
    in
    let%lwt parsed_by_specific =
      is_parsed_by body register_error_response_t_of_string
    in

    let _ =
      Alcotest.(check int) "status code is 422" 422 code;
      Alcotest.(check bool)
        "response can be parsed by yojson" true parsed_by_yojson;
      Alcotest.(check bool)
        "response can be parsed by specific" true parsed_by_specific
    in

    Lwt.return ()
  in
  make_test_case "already registered" inner

let creates_session =
  let inner () =
    let%lwt cookie_headers = get_cookie_headers () in
    let original_session_cookie =
      Cookie.Cookie_hdr.extract cookie_headers |> List.assoc "dream.session"
    in

    let%lwt response, body =
      let json =
        string_of_register_payload_t
          { username = "purefunctor"; auth_token = double_hmac "auth_token" }
      in
      post_json cookie_headers json "http://localhost:8080/api/register"
    in
    Cohttp_lwt.Body.drain_body body;%lwt

    let fresh_session_cookie =
      Response.headers response |> Cookie.Set_cookie_hdr.extract
      |> List.assoc "dream.session" |> Cookie.Set_cookie_hdr.value
    in

    let _ =
      Alcotest.(check bool)
        "fresh session cookie is sent" true
        (not @@ String.equal original_session_cookie fresh_session_cookie)
    in

    Lwt.return ()
  in
  make_test_case "creates session" inner

let register_secrets =
  let inner () =
    let%lwt cookie_headers = get_cookie_headers () in

    let%lwt response, body =
      let json =
        string_of_register_payload_t
          { username = "purefunctor"; auth_token = double_hmac "auth_token" }
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

    let%lwt response, body =
      let json =
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
        string_of_register_keys_payload_t
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
      in
      post_json cookie_headers json "http://localhost:8080/api/secrets"
    in
    Cohttp_lwt.Body.drain_body body;%lwt

    let code = response |> Response.status |> Code.code_of_status in

    let _ = Alcotest.(check int) "status code is 204" 204 code in

    Lwt.return ()
  in
  make_test_case "register secrets" inner
