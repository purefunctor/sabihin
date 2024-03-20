open Lwt.Syntax

let () =
  Lwt_main.run
  @@
  let* pool = Utils.connect_pool () in
  Alcotest_lwt.run "Models"
    [
      ( "User",
        [
          Alcotest_lwt.test_case "initialize" `Quick @@ User.initialize pool;
          Alcotest_lwt.test_case "insert" `Quick @@ User.insert pool;
        ] );
    ]
