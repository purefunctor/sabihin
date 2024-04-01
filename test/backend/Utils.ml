let options = Test_toolbox.Database.get_options_from_env ()
let default_db_url = Test_toolbox.Database.make_url ~options ""
let server_secret = "a_totally_legit_server_secret_you_should_use"

let perish action =
  Lwt.bind action (function
    | Ok o -> Lwt.return o
    | Error e -> raise (Caqti_error.Exn e))

let make_test_case name ?(speed : Alcotest.speed_level = `Quick) action =
  let database_name =
    Cstruct.of_string name |> Mirage_crypto.Hash.SHA256.digest
    |> Cstruct.to_hex_string
    |> Printf.sprintf "sabihin_backend_test_%s"
  in
  let database_url = Test_toolbox.Database.make_url ~options database_name in

  let initialize () =
    let open Lwt_result.Infix in
    Caqti_lwt.with_connection (Uri.of_string default_db_url) (fun connection ->
        Test_toolbox.Database.Q.initialize_database connection database_name)
    >>= fun _ ->
    Caqti_lwt.with_connection (Uri.of_string database_url) (fun connection ->
        Models.Initialize.initialize connection)
  in

  let inner _ () =
    perish @@ initialize ();%lwt
    let stop_promise, stop_resolver = Lwt.wait () in
    let wait_for_server =
      Backend_lib.Server.serve ~stop:stop_promise ~database_url ~server_secret
        ()
    in
    action ();%lwt
    Lwt.wakeup_later stop_resolver ();
    wait_for_server
  in

  Alcotest_lwt.test_case name speed inner

let get_cookie_headers () =
  let open Cohttp in
  let open Cohttp_lwt_unix in
  let%lwt response, body = Client.get (Uri.of_string "http://localhost:8080") in
  Cohttp_lwt.Body.drain_body body;%lwt
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
  let body = Cohttp_lwt.Body.of_string json in
  Client.post ~headers ~body (Uri.of_string url)

let is_parsed_by body parser =
  Logs.info (fun log -> log "Parsing: %s" body);
  try
    let _ = parser body in
    Lwt.return true
  with _ -> Lwt.return false
