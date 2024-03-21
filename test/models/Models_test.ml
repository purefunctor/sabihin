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
          [
            User_test.initialize;
            User_test.insert;
            User_test.insert_existing;
            User_test.get_by_id;
            User_test.get_by_username;
            User_test.get_missing;
          ] );
      ( "PrivateKey",
        List.map
          (fun f -> f pool)
          [
            PrivateKeys_test.initialize;
            PrivateKeys_test.insert;
            PrivateKeys_test.insert_existing;
            PrivateKeys_test.get_by_user_id;
            PrivateKeys_test.get_by_username;
            PrivateKeys_test.get_missing;
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
    ]
