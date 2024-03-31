open Cohttp
open Cohttp_lwt_unix
open Types_native.Defs_j
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
    let%lwt parsed = is_parsed_by body raw_error_response_t_of_string in

    let _ =
      Alcotest.(check int) "status code is 403" 403 code;
      Alcotest.(check bool) "response is parsed" true parsed
    in

    Lwt.return ()
  in
  make_test_case "it fails" inner
