open Cohttp
open Cohttp_lwt_unix
open Utils

let it_works =
  let inner () =
    let%lwt headers = get_cookie_headers () in
    let%lwt response, _ =
      Client.post ~headers (Uri.of_string "http://localhost:8080/api/register")
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
