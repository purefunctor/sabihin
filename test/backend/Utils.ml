let make_test_case name ?(speed : Alcotest.speed_level = `Quick) action =
  let inner _ () =
    let stop_promise, stop_resolver = Lwt.wait () in
    let server = Backend_lib.Server.serve ~stop:stop_promise () in
    action ();%lwt
    Lwt.wakeup_later stop_resolver ();
    server
  in
  Alcotest_lwt.test_case name speed inner

let get_cookie_headers () =
  let open Cohttp in
  let open Cohttp_lwt_unix in
  let%lwt response, _ = Client.get (Uri.of_string "http://localhost:8080") in
  let code = Response.status response |> Code.code_of_status in
  let set_cookies =
    Response.headers response |> Cookie.Set_cookie_hdr.extract
  in
  let has_set_cookie key = List.mem_assoc key set_cookies in
  let _ =
    Alcotest.(check int) "status code is 200" code 200;
    Alcotest.(check bool)
      "dream.session exists"
      (has_set_cookie "dream.session")
      true;
    Alcotest.(check bool)
      "sabihin.csrf exists"
      (has_set_cookie "sabihin.csrf")
      true
  in
  let headers =
    List.map
      (fun (key, value) -> (key, Cookie.Set_cookie_hdr.value value))
      set_cookies
    |> Cookie.Cookie_hdr.serialize |> Fun.flip List.cons [] |> Header.of_list
  in
  Lwt.return headers

let post_json cookie_headers json url =
  let open Cohttp_lwt_unix in
  let headers =
    Cohttp.Header.add cookie_headers "Content-Type" "application/json"
  in
  let body = json |> Yojson.Safe.to_string |> Cohttp_lwt.Body.of_string in
  Client.post ~headers ~body (Uri.of_string url)
