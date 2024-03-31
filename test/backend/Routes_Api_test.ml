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
    Cohttp_lwt.Body.drain_body body;%lwt
    let code = Response.status response |> Code.code_of_status in
    let _ = Alcotest.(check int) "status code is 200" code 200 in
    Lwt.return ()
  in
  make_test_case "it works" inner

let it_fails =
  let inner () =
    let%lwt response, body =
      Client.post (Uri.of_string "http://localhost:8080/api/register")
    in
    let%lwt parsed =
      let%lwt body = Cohttp_lwt.Body.to_string body in
      try
        let _ = error_response_t_of_string body in
        Lwt.return true
      with _ -> Lwt.return false
    in

    let code = Response.status response |> Code.code_of_status in
    let _ =
      Alcotest.(check int) "status code is 403" code 403;
      Alcotest.(check bool) "response is parsed" parsed true
    in

    Lwt.return ()
  in
  make_test_case "it fails" inner
