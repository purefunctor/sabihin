let () =
  (* We need to initialize Dream's log here since we're using Dream.serve
     to spin up the server for each test case. Alcotest also doesn't seem
     to use colors if this isn't called, so it's better overall to keep
     it here. *)
  Dream.initialize_log ();
  let cipher_secret = Dream.random 128 in
  let server_random = Dream.random 16 in

  Backend_lib.Cipher.set_cipher_secret cipher_secret;
  Backend_lib.Cipher.set_server_random server_random;
  Backend_lib.Vite.enable_dev ();

  Lwt_main.run
  @@ Alcotest_lwt.run "Backend"
       [
         ( "/api/register",
           List.map
             (fun f -> f "register")
             Register_Api_test.
               [
                 it_works;
                 it_fails;
                 already_registered;
                 creates_session;
                 invalid_username;
               ] );
         ( "/api/secrets",
           List.map
             (fun f -> f "secrets")
             Secrets_Api_test.
               [ it_works; it_fails; already_registered; invalid_payload ] );
       ]
