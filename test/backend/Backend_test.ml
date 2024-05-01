let () =
  (* We need to initialize Dream's log here since we're using Dream.serve
     to spin up the server for each test case. Alcotest also doesn't seem
     to use colors if this isn't called, so it's better overall to keep
     it here. *)
  Dream.initialize_log ();

  Backend_lib.Cipher.set_cipher_secret Utils.cipher_secret;
  Backend_lib.Cipher.set_server_random Utils.server_random;
  Backend_lib.Vite.enable_dev ();

  Lwt_main.run
  @@ Alcotest_lwt.run "Backend"
       [
         ( "/api/login",
           List.map
             (fun f -> f "login")
             Login_Api_test.
               [ login_returns_client_salt; login_returns_server_salt ] );
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
