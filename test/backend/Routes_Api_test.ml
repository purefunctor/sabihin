open Cohttp
open Cohttp_lwt_unix
open Utils

let double_hmac data =
  let open Mirage_crypto.Hash in
  let key = Cstruct.of_string "an_obviously_fake_key" in
  data |> Cstruct.of_string |> SHA512.hmac ~key |> SHA512.hmac ~key
  |> Cstruct.to_hex_string

let it_works =
  let inner () =
    let%lwt cookie_headers = get_cookie_headers () in
    let%lwt response, _ =
      let headers =
        Header.add cookie_headers "Content-Type"
          "application/x-www-form-urlencoded"
      in
      let body =
        let username = "purefunctor" in
        let auth_token = double_hmac "auth_token" in
        Cohttp_lwt.Body.of_form
          [ ("username", [ username ]); ("auth_token", [ auth_token ]) ]
      in
      Client.post ~headers ~body
        (Uri.of_string "http://localhost:8080/api/register")
    in
    let code = Response.status response |> Code.code_of_status in
    let _ = Alcotest.(check int) "status code is 200" code 200 in
    Lwt.return ()
  in
  make_test_case "it works" inner

let it_fails =
  let inner () =
    let%lwt response, _ =
      Client.post (Uri.of_string "http://localhost:8080/api/register")
    in
    let code = Response.status response |> Code.code_of_status in
    let _ = Alcotest.(check int) "status code is 403" code 403 in
    Lwt.return ()
  in
  make_test_case "it fails" inner
