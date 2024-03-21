open Lwt.Syntax

let () =
  Lwt_main.run
  @@
  let* pool = Utils.connect_pool () in
  Alcotest_lwt.run "Models"
    [
      ( "User",
        [
          User_test.initialize pool;
          User_test.insert pool;
          User_test.insert_existing pool;
          User_test.get_by_id pool;
          User_test.get_by_username pool;
          User_test.get_missing pool;
        ] );
      ( "PrivateKey",
        [
          PrivateKeys.initialize pool;
          PrivateKeys.insert pool;
          PrivateKeys.insert_existing pool;
          PrivateKeys.get_by_user_id pool;
          PrivateKeys.get_by_username pool;
        ] );
    ]
