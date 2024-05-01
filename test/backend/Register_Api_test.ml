open Cohttp
open Cohttp_lwt_unix
open Types_native.Definitions_j
open Types_native.Dynamic
open Utils

let username = "purefunctor"
let auth_token = String.make 128 ' '
let client_random = String.make 16 ' ' |> Base64.encode_string

let it_works prefix =
  let inner () =
    let%lwt cookie_headers = get_cookie_headers () in
    let%lwt response, body =
      let json =
        string_of_register_user_payload { username; auth_token; client_random }
      in
      post_json cookie_headers json "http://localhost:8080/api/register"
    in

    let code = Response.status response |> Code.code_of_status in
    let%lwt body = Cohttp_lwt.Body.to_string body in
    let%lwt parsed = is_parsed_by body register_response_of_string in

    let _ =
      Alcotest.(check int) "status code is 200" 200 code;
      Alcotest.(check bool) "response is parsed" true parsed
    in

    Lwt.return ()
  in
  make_test_case prefix "it works" inner

let it_fails prefix =
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
  make_test_case prefix "it fails" inner

let already_registered prefix =
  let inner () =
    let%lwt cookie_headers = get_cookie_headers () in
    let%lwt _, body =
      let json =
        string_of_register_user_payload { username; auth_token; client_random }
      in
      post_json cookie_headers json "http://localhost:8080/api/register"
    in
    Cohttp_lwt.Body.drain_body body;%lwt

    let%lwt response, body =
      let json =
        string_of_register_user_payload { username; auth_token; client_random }
      in
      post_json cookie_headers json "http://localhost:8080/api/register"
    in

    let code = response |> Response.status |> Code.code_of_status in

    let%lwt body = Cohttp_lwt.Body.to_string body in
    let%lwt parsed_by_yojson =
      is_parsed_by body yojson_error_response_of_string
    in
    let%lwt parsed_by_specific =
      is_parsed_by body register_error_response_of_string
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
  make_test_case prefix "already registered" inner

let creates_session prefix =
  let inner () =
    let%lwt cookie_headers = get_cookie_headers () in
    let original_session_cookie =
      Cookie.Cookie_hdr.extract cookie_headers |> List.assoc "dream.session"
    in

    let%lwt response, body =
      let json =
        string_of_register_user_payload { username; auth_token; client_random }
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
  make_test_case prefix "creates session" inner

let invalid_username prefix =
  let inner () =
    let%lwt cookie_headers = get_cookie_headers () in
    let%lwt response, body =
      let json =
        string_of_register_user_payload
          { username = "invalid&username"; auth_token; client_random }
      in
      post_json cookie_headers json "http://localhost:8080/api/register"
    in

    let code = Response.status response |> Code.code_of_status in
    let%lwt body = Cohttp_lwt.Body.to_string body in
    let%lwt parsed = is_parsed_by body yojson_error_response_of_string in

    let _ =
      Alcotest.(check int) "status code is 422" 422 code;
      Alcotest.(check bool) "response can be parsed" true parsed
    in

    Lwt.return ()
  in
  make_test_case prefix "invalid username" inner
