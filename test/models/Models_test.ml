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
          PrivateKeys_test.initialize pool;
          PrivateKeys_test.insert pool;
          PrivateKeys_test.insert_existing pool;
          PrivateKeys_test.get_by_user_id pool;
          PrivateKeys_test.get_by_username pool;
        ] );
    ]
