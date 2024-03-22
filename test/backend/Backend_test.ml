let () =
  Dream.initialize_log ();
  Lwt_main.run
  @@ Alcotest_lwt.run "Backend"
       [ ("/api/register", Routes_Api_test.[ it_works; it_fails ]) ]
