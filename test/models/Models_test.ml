open Lwt.Syntax

let () =
  Lwt_main.run
  @@
  let* pool = Utils.connect_pool () in
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
      ( "PrivateKeys",
        List.map
          (fun f -> f pool)
          PrivateKeys_test.
            [
              initialize;
              insert;
              insert_existing;
              get_by_user_id;
              get_by_username;
              get_missing;
            ] );
      ( "PublicKeys",
        List.map
          (fun f -> f pool)
          PublicKeys_test.
            [
              initialize;
              insert;
              insert_existing;
              get_by_user_id;
              get_by_username;
              get_missing;
            ] );
      ("Nanoid", [ Nanoid_test.simple pool ]);
    ]
