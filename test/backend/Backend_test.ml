let () =
  (* We need to initialize Dream's log here since we're using Dream.serve
     to spin up the server for each test case. Alcotest also doesn't seem
     to use colors if this isn't called, so it's better overall to keep
     it here. *)
  Dream.initialize_log ();
  Lwt_main.run
  @@ Alcotest_lwt.run "Backend"
       [
         ( "/api/register",
           Routes_Api_test.[ it_works; it_fails; already_registered ] );
       ]
