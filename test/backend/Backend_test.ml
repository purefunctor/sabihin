let () =
  (* We need to initialize Dream's log here since we're using Dream.serve
     to spin up the server for each test case. Alcotest also doesn't seem
     to use colors if this isn't called, so it's better overall to keep
     it here. *)
  Dream.initialize_log ();
  let cipher_secret = Dream.to_base64url (Dream.random 128) in
  Backend_lib.Cipher.set_current_key cipher_secret;
  Lwt_main.run
  @@ Alcotest_lwt.run "Backend"
       [
         ( "/api/register",
           Register_Api_test.
             [ it_works; it_fails; already_registered; creates_session ] );
       ]
