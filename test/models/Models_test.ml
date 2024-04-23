open Lwt.Syntax

let () =
  Lwt_main.run
  @@
  let* pool = Utils.connect_pool () in
  let* () = Utils.initialize () in
  Alcotest_lwt.run "Models"
    [
      ( "User",
        List.map
          (fun f -> f pool)
          User_test.
            [
              initialize;
              insert;
              insert_existing;
              get_by_id;
              get_by_username;
              get_missing;
            ] );
      ( "Secrets",
        List.map
          (fun f -> f pool)
          Secrets_test.[ initialize; insert; insert_existing ] );
      ("Nanoid", [ Nanoid_test.simple pool ]);
    ]
